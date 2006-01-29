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
#include <QTimer>
#include <cmath>
#include <QBoxLayout>
#include "ktgradientadjuster.h"

#define BEGIN_PAINTER(var) switch(m_renderType) { case Image: \
var.begin(IMAGE_DEVICE); break; \
	case Native: \
		var.begin(NATIVE_DEVICE); break; \
	case OpenGL: \
		var.begin(OPENGL_DEVICE); break; }; 

APaintArea::APaintArea(const QSize& size ,QWidget *parent) : QWidget(parent), m_xpos(0), m_ypos(0), m_zero(0,0), m_drawGrid(true), m_currentTool(0), m_lastPosition(-1,-1), m_currentFrame(0), m_layer(0), m_scene(0), m_previousFramesNumber(0), m_nextFramesNumber(0), m_currentGraphic(0)
{
	m_redrawAll = true;
	
	m_renderType = Image;
	
	KTINIT;
	setAttribute(Qt::WA_StaticContents);
// 	QBoxLayout *layout = new QBoxLayout( QBoxLayout::LeftToRight, this);
	switch(m_renderType)
	{
		case Image:
		{
			m_paintDevice = new AImageDeviceWidget(size, this);
		}
		break;
		case OpenGL:
		{
			m_paintDevice = new QGLWidget(this);
			m_paintDevice->resize(size);
		}
		break;
		case Native:
		{
			m_paintDevice = new QWidget(this);
			m_paintDevice->resize(size);
			m_paintDevice->setAttribute (Qt::WA_PaintOutsidePaintEvent);
		}
		break;
	}
	
	m_paintDevice->setAutoFillBackground(true);
// 	layout->addWidget(m_paintDevice);
	
	fillDevice( Qt::white );
	
	setMouseTracking(true);
	m_grid.createGrid(m_paintDevice->width(), m_paintDevice->height() );
	
	m_path = QPainterPath();
	
	m_currentBrush = new KTBrush;
	
	m_currentFrame = new KTKeyFrame;
	
	show();
}


APaintArea::~APaintArea()
{
	KTEND;
	if ( m_currentBrush ) delete m_currentBrush;
}

QSize APaintArea::sizeHint() const
{
	return m_paintDevice->size() + QSize(m_zero.x(),m_zero.y());
}

QSize APaintArea::minimumSizeHint () const
{
	return QSize(m_zero.x(),m_zero.y());
}

void APaintArea::paintEvent(QPaintEvent *e)
{
	QPainter painter;
	
	if ( m_redrawAll )
	{
		fillDevice( Qt::white );
		
		BEGIN_PAINTER(painter);
		painter.setRenderHint(QPainter::Antialiasing);
		
		if(m_drawGrid)
		{
			painter.save();
			painter.setPen(Qt::gray);
			painter.drawPath(m_grid.pathGrid());
			painter.restore();
		}
		
		painter.save();
		
// 		painter.setClipRect(e->rect());
		draw(&painter);
		
		painter.restore();
		
		painter.end();
		m_redrawAll = false;
	}
	m_paintDevice->update();
}

void APaintArea::setKeyFrame(int index)
{
	ktDebug() << "APaintArea::setKeyFrame(" << index << ")";
	if ( m_layer )
	{
		KTKeyFrame *frame = m_layer->frames()[index];
		
		if (frame && index < m_layer->frames().count())
		{
			m_currentFrame = frame;
			redrawAll();
		}
		else
		{
			ktFatal() << "Frame not exists!!!";
		}
	}
}

void APaintArea::setLayer(int index)
{
	ktDebug( ) << "APaintArea::setLayer(" << index << ")";
	if ( m_scene )
	{
		KTLayer *layer = m_scene->layers()[index];
		
		if (layer && index < m_scene->layers().count() )
		{
			m_layer = layer;
		}
		else
		{
			ktFatal() << "Layer not exists!!!";
		}
	}
}

void APaintArea::setScene(KTScene *scene)
{
	if (scene )
	{
		m_scene = scene;
		m_layer = m_scene->currentLayer();
		setKeyFrame( 0 );// FIXME
	}
	else
	{
		ktFatal() << "Layer not exists!!!";
	}
	
}

void APaintArea::draw(QPainter *painter)
{
	KT_FUNCINFO;
	Layers layers = m_scene->layers();
	Layers::iterator layerIterator = layers.begin();
	
	int index = m_layer->frames().indexOf(m_currentFrame);
	
	// draw visible layers
	while ( layerIterator != layers.end() )
	{
		if ( (*layerIterator) && (*layerIterator)->isVisible() )
		{
			// hasta indice - previews y indice + next
			
			float intensitive = 1;
			for (int frameIndex = index-1; frameIndex > index-m_previousFramesNumber-1; frameIndex-- )
			{
				intensitive -= 0.25f;

				if ( frameIndex < 0 || frameIndex > (*layerIterator)->frames().count()-1 )
				{
					break;
				}
				
				drawFrame((*layerIterator)->frames()[ frameIndex ], painter, intensitive );
			}
			
			intensitive = 1;
			for (int frameIndex = index+1; frameIndex < index+m_nextFramesNumber+1; frameIndex++ )
			{
				intensitive -= 0.25f;
				
				if ( frameIndex > (*layerIterator)->frames().count()-1 )
				{
					break;
				}
				drawFrame((*layerIterator)->frames()[ frameIndex ], painter, intensitive );
			}
			
			// Draw the current frame
			KTKeyFrame *frame = (*layerIterator)->frames()[ index ];
			
			if(frame && index < (*layerIterator)->frames().count() )
			{
				drawFrame(frame, painter);
			}
		}
		
		++layerIterator;
	}
	
	ktDebug() << "END " << __FUNCTION__;
}

void APaintArea::drawFrame(const KTKeyFrame *frame, QPainter *painter, float intensitive)
{
	SHOW_VAR(intensitive);
	
	if ( frame  )
	{
		QList<AGraphicComponent *> componentList = frame->components();
		
		if ( componentList.count() > 0)
		{
			QList<AGraphicComponent *>::iterator it = componentList.begin();
				
			while ( it != componentList.end() )
			{
				if ( *it )
				{
					drawGraphic( *it, painter);
				}
				++it;
			}
		}
	}
}

void APaintArea::drawGraphic(const AGraphicComponent *graphicComponent, QPainter *painter, float intensitive )
{
	painter->save();
	foreach(AGraphic *graphic, graphicComponent->graphics())
	{
		QPen pen = graphic->pen;
		QBrush brush = graphic->brush;
		if ( brush.gradient() )
		{
			brush = KTGradientAdjuster::adjustGradient(brush.gradient(), graphic->path.boundingRect().toRect());
		}
	
		if ( pen.brush().gradient() )
		{
			pen.setBrush( KTGradientAdjuster::adjustGradient( pen.brush().gradient(), graphic->path.boundingRect().toRect()) );
		}
	
		if ( intensitive < 1 && intensitive >= 0 )
		{
			QColor penColor = Qt::gray;
			QColor brushColor = Qt::gray;
						
			const float factor = (1-intensitive); // FIXME: fix this ;)
						
			penColor.setHsv(0,0, static_cast<int>(factor * pen.color().alpha()) % 360 );
						
			brushColor.setHsv(0,0, static_cast<int>( factor * brush.color().alpha() ) % 360 );
						
			penColor.setAlpha(pen.color().alpha());
			brushColor.setAlpha(brush.color().alpha());
						
			pen.setColor(penColor);
			brush.setColor(brushColor);
		}
		else if ( intensitive > 1 && intensitive < 0)
		{
			ktWarning() << "Intensitive must be lesser than 1 - No effect";
		}
		
		painter->setPen(pen);
		painter->setBrush(brush);
		
		
		
		QList<QPolygonF> poligons = graphic->path.toSubpathPolygons();
		
		if ( poligons.count() == 1 )
		{
			painter->drawPath(graphic->path);
		}
		else
		{
			QList<QPolygonF>::const_iterator it;
			for(it = poligons.begin(); it != poligons.end(); ++it)
			{
				painter->drawPolygon(*it);
			}
		}
	}
	
	painter->restore();
}

void APaintArea::redrawAll()
{
	m_redrawAll = true;
	QTimer::singleShot(0, this, SLOT(update()));;
}

void APaintArea::aUpdate(const QRectF &rect)
{
	QPainter painter;
	BEGIN_PAINTER(painter);
	
#if 1

	if ( !m_overBuffer.isNull())
	{
		painter.drawImage(m_overBufferRect, m_overBuffer);
		
		
// 		update(m_overBufferRect);
		QTimer::singleShot(0, this, SLOT(update())); // FIXME: calculate the rect
	}
	
	switch(m_renderType)
	{
		case Image:
		{
			m_overBuffer = IMAGE_DEVICE->copy( rect.toRect() );
		}
		break;
		case Native:
		{
		}
		break;
		case OpenGL:
		{
			m_overBuffer = OPENGL_DEVICE->grabFrameBuffer().copy( rect.toRect() );
		}
	}
// 	m_overBuffer = m_paintDevice->copy( rect.toRect() );
	m_overBufferRect = rect.toRect();
	
#else
	
	if(m_drawGrid)
	{
		painter.drawImage(rect.toRect(), m_grid.copy( rect.toRect() ));
	}
		
	QList<AGraphicComponent *> componentList = m_currentFrame->components();
	
	if ( componentList.count() > 0)
	{
		QList<AGraphicComponent *>::iterator it = componentList.begin();
		
		while ( it != componentList.end() )
		{
			if ( *it && (*it)->path().intersects(rect) )
			{
				drawGraphic( *it, &painter);
			}
			++it;
		}
	}
	QTimer::singleShot(0, this, SLOT(update()));
#endif
}

void APaintArea::drawGhostGraphic(const QPainterPath &path)
{
	aUpdate(path.boundingRect().toRect().normalized().adjusted(-60,-60,60,60) );

	QPainter painter;
	BEGIN_PAINTER(painter);
	
	painter.drawImage(m_overBufferRect, m_overBuffer);
	painter.setPen(QPen(Qt::black, 1, Qt::DashLine));
	painter.drawPath(path);
}

void APaintArea::resizeEvent( QResizeEvent * event )
{
	m_xpos = width() / 2 - m_paintDevice->width() / 2;
	m_ypos = height() / 2 - m_paintDevice->height() / 2;
// 	m_paintDevice->resize( size() );
	update();
	QWidget::resizeEvent(event);
}

QPoint APaintArea::paintDevicePosition() const
{
	return QPoint(m_xpos, m_ypos);
}

void APaintArea::setZeroAt(const QPoint & zero)
{
	m_zero = zero *2;
	m_paintDevice->move(zero);
// 	resize(m_paintDevice->sizeHint());
	update();
}

QWidget *APaintArea::paintDevice() const
{
	return m_paintDevice;
}

// void APaintArea::setPaintDevice(const QImage &image)
// {
// 	m_paintDevice = image.convertToFormat(QImage::Format_RGB32);
// 	update();
// 	updateGeometry();
// }

void APaintArea::mousePressEvent ( QMouseEvent * e )
{
	m_overBuffer = QImage();
	QMouseEvent *event = new QMouseEvent( e->type(), e->pos()-QPoint(m_zero.x()/2, m_zero.y()/2), mapToGlobal( e->pos()-QPoint(m_zero.x(), m_zero.y()) ), e->button(), e->buttons(), 0 );
	
	if ( m_currentFrame )
	{
		if (event->button() == Qt::LeftButton && !m_currentFrame->isLocked())
		{
			if (m_currentTool)
			{
				QPainter painter;
				BEGIN_PAINTER(painter);
				m_currentBrush->setupPainter(&painter);

				QList<AGraphicComponent *> components =  m_currentFrame->components();
				QList<AGraphicComponent *>::iterator it;
				AGraphicComponent *toSelect = 0;

				for(it = components.end()-1; it != components.begin()-1; it--)
				{
					if( (*it) && (*it)->intersects( QRectF(QPointF(static_cast<double>(event->pos().x()-1), static_cast<double>(event->pos().y()-1) ), QSizeF(2,2) ) ) )
					{
						if ( *it )
						{
							toSelect = (*it);
							break;
						}
					}
				}
				
				if(!toSelect)
				{
					m_currentFrame->clearSelections();
				}
				else if ( e->modifiers() & Qt::ControlModifier )
				{
					m_currentFrame->addSelectedComponent( toSelect );
				}
				else
				{
					m_currentFrame->clearSelections();
					m_currentFrame->addSelectedComponent( toSelect );
				}

				m_currentGraphic = new AGraphicComponent;
				
				QRect rect = m_currentTool->press(m_currentKeyTool, painter,translatePath(m_currentBrush->brushForm(),event->pos()), event->pos(), m_currentFrame);
				rect.translate(m_xpos, m_ypos);
				update(rect);
			}
			
			m_lastPosition = event->pos();
		}
	}
}

void APaintArea::mouseMoveEvent(QMouseEvent *e)
{
	QMouseEvent *event = new QMouseEvent( e->type(), e->pos()-m_zero/2 /*QPoint(m_zero/2, m_zero/2)*/, mapToGlobal( e->pos()-m_zero/2/*QPoint(m_zero, m_zero) */), e->button(), e->buttons(), 0 );
	
	emit mousePos(e->pos());
	
	if ( m_currentFrame )
	{
		if ((event->buttons() & Qt::LeftButton) && m_lastPosition != QPoint(-1, -1)  && !m_currentFrame->isLocked() )
		{
			if (m_currentTool)
			{
				QPainter painter;
				BEGIN_PAINTER(painter);
				m_currentBrush->setupPainter(&painter);
				
				QRect rect = m_currentTool->move(m_currentKeyTool, painter,translatePath(m_currentBrush->brushForm(),event->pos()), m_lastPosition, event->pos());
				rect.translate(m_xpos, m_ypos);
				update(rect);

			}
	
			m_lastPosition = event->pos();
		}
	}
}

void APaintArea::mouseReleaseEvent(QMouseEvent *e)
{
	QMouseEvent *event = new QMouseEvent( e->type(), e->pos()-m_zero/2, mapToGlobal( e->pos()-m_zero ), e->button(), e->buttons(), 0 );
	
	if ( m_currentFrame )
	{
		if (event->button() == Qt::LeftButton && m_lastPosition != QPoint(-1, -1)  && !m_currentFrame->isLocked())
		{
			if (m_currentTool)
			{
				QPainter painter;
				BEGIN_PAINTER(painter);
				m_currentBrush->setupPainter(&painter);
				QRect rect = m_currentTool->release(m_currentKeyTool, painter,translatePath(m_currentBrush->brushForm(), event->pos()), event->pos());
				
				rect.translate(m_xpos, m_ypos);
				update(rect);
				
				if ( !m_currentTool->path().isEmpty())
				{
					m_currentGraphic->addGraphic( m_currentTool->path(), painter.pen(), painter.brush());
				}
				
				if ( m_currentGraphic->isValid() && m_currentTool->isComplete() )
				{
					ktDebug() << "Adding component";
					
					m_currentFrame->addComponent(  m_currentGraphic );
					ktDebug() << "Components count: " << m_currentFrame->components().count();
					m_undoComponents.clear();
				}
			}
			
			m_lastPosition = QPoint(-1, -1);
		}
	}
}

void APaintArea::setTool( AToolInterface *toolIface, const QString &tool)
{
	if ( m_currentTool )
	{
		m_currentTool->aboutToChangeTool();
// 		if ( m_currentFrame && m_currentGraphic)
// 		m_currentFrame->addComponent(  m_currentGraphic );
	}
	
	m_currentTool = toolIface;
	m_currentKeyTool = tool;
}

void APaintArea::setBrush( const KTBrush *brush )
{
	m_currentBrush->setBrushForm( brush->brushForm() );
	m_currentBrush->setPenWidth( brush->penWidth() );
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

void APaintArea::undo()
{
	if ( m_currentFrame->components().count() > 0 )
	{
		AGraphicComponent *grafic = m_currentFrame->takeLastComponent();
		
// 		if ( grafic == m_selectedGraphic )
// 		{
// 			m_selectedGraphic = 0;
// 		}
		
		m_undoComponents << grafic;
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

void APaintArea::setPreviousFrames(int n)
{
	if ( n >= 0 )
	{
		m_previousFramesNumber = n;
		redrawAll();
	}
}

void APaintArea::setNextFrames(int n)
{
	if ( n >= 0)
	{
		m_nextFramesNumber = n;
		redrawAll();
	}
}

void APaintArea::removeSelectsGraphics()
{
	m_currentFrame->removeSelections();
	redrawAll();
}


AGraphicComponent *APaintArea::currentGraphic()
{
	return m_currentGraphic;
}

QList<AGraphicComponent *> APaintArea::selectedGraphic()
{
	if(m_currentFrame)
	{
		return m_currentFrame->selectedComponents();
	}
	
	return QList<AGraphicComponent *>();
}

KTBrush *APaintArea::currentBrush()
{
	return m_currentBrush;
}

void APaintArea::copy()
{
	if(m_currentFrame->selectedComponents().count() > 0)
	{
		foreach(AGraphicComponent *component, m_currentFrame->selectedComponents())
		{
			m_copiedGraphics << new AGraphicComponent(*component);
		}
	}
}

void APaintArea::paste()
{
	m_currentFrame->addComponents( m_copiedGraphics);
	redrawAll();
}

void APaintArea::cut()
{
	copy();
	m_currentFrame->removeSelections();
	redrawAll();
}

