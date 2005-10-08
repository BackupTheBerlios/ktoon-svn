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

APaintArea::APaintArea(QWidget *parent) : QWidget(parent), m_xpos(0), m_ypos(0), m_zero(0), m_drawGrid(true), m_tool(0), m_lastPosition(-1,-1)
{
	KTINIT;
	setAttribute(Qt::WA_StaticContents);
	
	m_paintDevice = QImage(520, 340, QImage::Format_RGB32);
	m_paintDevice.fill(qRgb(255, 255, 255));
	
	setMouseTracking(true);
	m_grid.createGrid(m_paintDevice);

	QPainter painter(&m_paintDevice);
	
	if(m_drawGrid)
	{
		painter.setPen(Qt::gray);
		painter.drawPath(m_grid.pathGrid());
	}
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

void APaintArea::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.drawImage(QPoint(m_xpos, m_ypos), m_paintDevice);
// 	painter.end();
}

void APaintArea::resizeEvent ( QResizeEvent * event )
{
	m_xpos = width()/2 -m_paintDevice.width()/2;
	m_ypos = height()/2-m_paintDevice.height()/2;
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

void APaintArea::mousePressEvent ( QMouseEvent * e )
{
	QMouseEvent *event = new QMouseEvent( e->type(), e->pos()-QPoint(m_zero/2, m_zero/2), mapToGlobal( e->pos()-QPoint(m_zero, m_zero) ), e->button(), e->buttons(), 0 );
	
	if (event->button() == Qt::LeftButton)
	{
		if ( ! m_path.isEmpty() )
		{
			QPainter painter(&m_paintDevice);
			setupPainter(painter);

			QRectF boundingRect = m_path.boundingRect();
			QLinearGradient gradient(boundingRect.topRight(), boundingRect.bottomLeft());
			gradient.setColorAt(0.0, QColor(m_color.red(), m_color.green(),
					    m_color.blue(), 63));
			gradient.setColorAt(1.0, QColor(m_color.red(), m_color.green(),
					    m_color.blue(), 191));
			painter.setBrush(gradient);
			
			painter.translate(event->pos() - boundingRect.center());
			painter.drawPath(m_path);

			m_path = QPainterPath();
			
			unsetCursor();
			update();
		} 
		else 
		{
			if (m_tool) 
			{
				QPainter painter(&m_paintDevice);
				setupPainter(painter);
				QRect rect = m_tool->press(m_brush, painter, event->pos());
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
		if (m_tool)
		{
			QPainter painter(&m_paintDevice);
			setupPainter(painter);
			QRect rect = m_tool->move(m_brush, painter, m_lastPosition, event->pos());
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
		if (m_tool)
		{
			QPainter painter(&m_paintDevice);
			setupPainter(painter);
			QRect rect = m_tool->release(m_brush, painter, event->pos());
			rect.translate(m_xpos, m_ypos);
			update(rect);
		}
		
		m_lastPosition = QPoint(-1, -1);
	}
}

void APaintArea::setBrush(ADrawingToolInterface *toolIface, const QString &brush)
{
	m_tool = toolIface;
	m_brush = brush;
}

void APaintArea::setupPainter(QPainter &painter)
{
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(QPen(m_color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

void APaintArea::setColor(const QColor& color)
{
	m_color = color;
}
