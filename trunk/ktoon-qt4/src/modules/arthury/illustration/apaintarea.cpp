/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "apaintarea.h"
#include "ktdebug.h"

#include <QPalette>
#include <QPainter>
#include <cmath>

APaintArea::APaintArea(QWidget *parent) : QWidget(parent), m_xpos(0), m_ypos(0), m_zero(0), m_drawGrid(true), m_currentTool(0), m_lastPosition(-1,-1), m_brushColor(Qt::transparent)
{
	m_redrawAll = true;
	
	KTINIT;
	setAttribute(Qt::WA_StaticContents);
	
	m_paintDevice = QImage(520, 340, QImage::Format_RGB32);
	m_paintDevice.fill(qRgb(255, 255, 255));
	
	setMouseTracking(true);
	m_grid.createGrid(m_paintDevice);
	
	m_path = QPainterPath();
	
	m_currentBrush = new KTBrush;
	
	m_currentFrame = new KTKeyFrame;
	
	show();
}


APaintArea::~APaintArea()
{
	KTEND;
}

QSize APaintArea::sizeHint() const
{
	return m_paintDevice.size() + QSize(m_zero,m_zero);
}

QSize APaintArea::minimumSizeHint () const
{
	return QSize(m_zero,m_zero);
}

void APaintArea::paintEvent(QPaintEvent *e)
{
	QPainter painter;
	
	if ( m_redrawAll )
	{
		m_paintDevice.fill(qRgb(255, 255, 255));
		
		painter.begin(&m_paintDevice);
	// 	painter.setClipRect(e->rect());
		painter.setRenderHint(QPainter::Antialiasing);
		
		if(m_drawGrid)
		{
			painter.save();
			painter.setPen(Qt::gray);
			painter.drawPath(m_grid.pathGrid());
			painter.restore();
		}
		
		painter.save();
		
		draw(&painter);
		
		painter.restore();
		painter.end();
		
		m_redrawAll = false;
	}
	
	painter.begin(this);
	painter.drawImage(QPoint(m_xpos, m_ypos), m_paintDevice);
}

void APaintArea::setKeyFrame(int index)
{
	ktDebug( ) << "APaintArea::setKeyFrame(int " << index << ")" << endl;
	KTKeyFrame *frame = m_layer->frames()[index];
	if (frame )
	{
		m_currentFrame = frame;
		redrawAll();
	}
	else
	{
		ktFatal() << "Frame not exists!!!" << endl;
	}
}

void APaintArea::setLayer(KTLayer *layer)
{
	m_layer = layer;
	setKeyFrame( 0 );// FIXME
}

void APaintArea::draw(QPainter *painter)
{
	QList<AGraphicComponent *> componentList = m_currentFrame->components();
	
	if ( componentList.count() > 0)
	{
		QList<AGraphicComponent *>::iterator it = componentList.begin();
		
		while ( it != componentList.end() )
		{
			if ( *it )
			{
				painter->save();
				
				painter->setPen((*it)->pen());
				painter->setBrush((*it)->brush());
				
				painter->drawPath((*it)->path());
				
				painter->restore();
			}
			++it;
		}
	}
}

void APaintArea::redrawAll()
{
	m_redrawAll = true;
	update();
}

void APaintArea::resizeEvent( QResizeEvent * event )
{
	m_xpos = width() / 2 - m_paintDevice.width() / 2;
	m_ypos = height() / 2 - m_paintDevice.height() / 2;
	repaint();
	QWidget::resizeEvent(event);
}

QPoint APaintArea::paintDevicePosition() const
{
	return QPoint(m_xpos, m_ypos);
}

void APaintArea::setZeroAt(int zero)
{
	m_zero = zero*2;
	resize(sizeHint());
	repaint();
}

QImage APaintArea::paintDevice() const
{
	return m_paintDevice;
}

void APaintArea::setPaintDevice(const QImage &image)
{
	m_paintDevice = image.convertToFormat(QImage::Format_RGB32);
	update();
	updateGeometry();
}

void APaintArea::mousePressEvent ( QMouseEvent * e )
{
	QMouseEvent *event = new QMouseEvent( e->type(), e->pos()-QPoint(m_zero/2, m_zero/2), mapToGlobal( e->pos()-QPoint(m_zero, m_zero) ), e->button(), e->buttons(), 0 );
	
	if (event->button() == Qt::LeftButton)
	{
		m_currentGraphic = new AGraphicComponent;

		if ( ! m_path.isEmpty() )
		{
			QPainter painter(&m_paintDevice);
			setupPainter(painter);

			QRectF boundingRect = m_path.boundingRect();
			QLinearGradient gradient(boundingRect.topRight(), boundingRect.bottomLeft());
			gradient.setColorAt(0.0, QColor(m_brushColor.red(), m_brushColor.green(),m_brushColor.blue(), 63));
			gradient.setColorAt(1.0, QColor(m_brushColor.red(), m_brushColor.green(),m_brushColor.blue(), 191));
			painter.setBrush(gradient);
			
			painter.translate(event->pos() - boundingRect.center());
			painter.drawPath(m_path);

			m_path = QPainterPath();
			
			unsetCursor();
			update();
		}
		else 
		{
			if (m_currentTool)
			{
				QPainter painter(&m_paintDevice);
				setupPainter(painter);
				QRect rect = m_currentTool->press(m_currentKeyTool, painter,translatePath(m_currentBrush->brushForm(),event->pos()), event->pos());
				rect.translate(m_xpos, m_ypos);
				update(rect);
			}

			m_lastPosition = event->pos();
		}
	}
}

void APaintArea::mouseMoveEvent(QMouseEvent *e)
{
	emit mousePos(e->pos());
	QMouseEvent *event = new QMouseEvent( e->type(), e->pos()-QPoint(m_zero/2, m_zero/2), mapToGlobal( e->pos()-QPoint(m_zero, m_zero) ), e->button(), e->buttons(), 0 );
	
	if ((event->buttons() & Qt::LeftButton) && m_lastPosition != QPoint(-1, -1))
	{
		if (m_currentTool)
		{
			QPainter painter(&m_paintDevice);
			setupPainter(painter);
			QRect rect = m_currentTool->move(m_currentKeyTool, painter,translatePath(m_currentBrush->brushForm(),event->pos()), m_lastPosition, event->pos());
			rect.translate(m_xpos, m_ypos);
			update(rect);
		}

		m_lastPosition = event->pos();
	}
}

void APaintArea::mouseReleaseEvent(QMouseEvent *e)
{
	QMouseEvent *event = new QMouseEvent( e->type(), e->pos()-QPoint(m_zero/2, m_zero/2), mapToGlobal( e->pos()-QPoint(m_zero, m_zero) ), e->button(), e->buttons(), 0 );
	
	if (event->button() == Qt::LeftButton && m_lastPosition != QPoint(-1, -1)) 
	{
		if (m_currentTool)
		{
			QPainter painter(&m_paintDevice);
			setupPainter(painter);
			QRect rect = m_currentTool->release(m_currentKeyTool, painter,translatePath(m_currentBrush->brushForm(), event->pos()), event->pos());
			rect.translate(m_xpos, m_ypos);
			update(rect);
			
			ktDebug() << "Adding component" << endl;
			
			m_currentGraphic->setPath(m_currentTool->path());
			m_currentGraphic->setPen(painter.pen());
			m_currentGraphic->setBrush(painter.brush());
			
			m_currentFrame->addComponent(  m_currentGraphic );
			
			ktDebug() << "Components count: " << m_currentFrame->components().count() << endl;
			
			m_undoComponents.clear();
#if 0
			redrawAll();
#endif
		}
		
		m_lastPosition = QPoint(-1, -1);
	}
}

void APaintArea::setTool( AToolInterface *toolIface, const QString &tool)
{
	m_currentTool = toolIface;
	m_currentKeyTool = tool;
}

void APaintArea::setBrush( KTBrush *brush )
{
	m_currentBrush = brush;
}

QPainterPath APaintArea::translatePath(const QPainterPath &path, const QPoint &pos)
{
	QPainterPath result;
	QList<QPolygonF> polygons = path.toSubpathPolygons();

	for(int i = 0; i < polygons.count(); i++)
	{
		polygons[i].translate( pos );
		result.addPolygon(polygons[i]);
	}
	
	return result;
}

void APaintArea::setupPainter(QPainter &painter)
{
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(QPen(m_penColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	painter.setBrush(QBrush(m_brushColor));
}

void APaintArea::setPenColor(const QColor& color)
{
	m_penColor = color;
}

void APaintArea::setBrushColor(const QColor& color)
{
	m_brushColor = color;
}

void APaintArea::undo()
{
	if ( m_currentFrame->components().count() > 0 )
	{
		m_undoComponents << m_currentFrame->takeLastComponent();
		redrawAll();
	}
}

void APaintArea::redo()
{
	if ( m_undoComponents.count() > 0 )
	{
		m_currentFrame->addComponent(  m_undoComponents.takeLast() );
		redrawAll();
	}
}




