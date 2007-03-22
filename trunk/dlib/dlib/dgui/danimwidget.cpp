/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   				   *
 *   krawek@gmail.com    						   *
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

#include "danimwidget.h"
#include "dapplication.h"

#include <QPixmap>
#include <QHideEvent>
#include <QShowEvent>
#include <QPainter>
#include <QFontMetricsF>

#include "ddebug.h"

class DAnimWidget::Controller
{
	public:
		Controller(DAnimWidget *area) : m_area(area), m_timerId(-1)
		{
		}
		~Controller()
		{
		}
		void start(int ms)
		{
			if ( m_timerId != -1 )
			{
				stop();
			}
			
			m_timerId = m_area->startTimer(ms);
		}
		void stop()
		{
			m_area->killTimer(m_timerId);
			m_timerId = -1;
		}
		
	private:
		DAnimWidget *m_area;
		int m_timerId;
};

DAnimWidget::DAnimWidget(const QPixmap &px, const QString &text, QWidget *parent) : QWidget(parent), m_type(AnimText), m_controller(new Controller(this)), m_background(px), m_text(text)
{
	resize(px.width()/2, px.height());
	
	QPoint position = QPoint(50, px.height());
	
	QFontMetricsF fontMetrics(font());
	m_textRect = QRectF(QPointF(40, height()), fontMetrics.size(Qt::TextWordWrap, m_text).expandedTo( QSizeF(px.width(), 0)) );
}

DAnimWidget::DAnimWidget(ListOfPixmaps lop, QWidget *parent) : QWidget(parent), m_type(AnimPixmap), m_controller(new Controller(this)), m_pixmaps(lop), m_pixmapIndex(0)
{
	m_background = lop[0];
}

DAnimWidget::~ DAnimWidget()
{
	delete m_controller;
}

void DAnimWidget::setBackgroundPixmap(const QPixmap &px)
{
	m_background = px;
}

void DAnimWidget::showEvent ( QShowEvent * e )
{
	switch(m_type)
	{
		case AnimText:
		{
			m_controller->start(50);
		}
		break;
		case AnimPixmap:
		{
			m_controller->start(5000);
		}
		break;
	}
	QWidget::showEvent (e);
}

void DAnimWidget::hideEvent ( QHideEvent *e)
{
	m_controller->stop();
	QWidget::hideEvent ( e);
}

void DAnimWidget::timerEvent(QTimerEvent *e)
{
	switch(m_type)
	{
		case AnimText:
		{
			int yPos = m_textRect.y() - 1;
			m_textRect.setY( yPos );
		}
		break;
		case AnimPixmap:
		{
			m_pixmapIndex = (m_pixmapIndex + 1) % m_pixmaps.count();
			m_background = m_pixmaps[m_pixmapIndex];
		}
		break;
	}
	update();
}

void DAnimWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	
	painter.drawPixmap(0,0, m_background);
	
	switch(m_type)
	{
		case AnimText:
		{
			painter.setRenderHint(QPainter::TextAntialiasing, true);
			painter.drawText( m_textRect, m_text);
		}
		break;
		case AnimPixmap:
		{
		}
		break;
	}
}

