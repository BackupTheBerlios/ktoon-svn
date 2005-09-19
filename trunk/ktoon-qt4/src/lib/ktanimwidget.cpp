/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   				   *
 *   krawek@toonka.com    						   *
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

#define DEBUG_KTANIMWIDGET 0

#include "ktanimwidget.h"
#include "ktapplication.h"
//Added by qt3to4:
#include <QPixmap>
#include <QHideEvent>
#include <QShowEvent>

KTAnimWidget::KTAnimWidget(const QPixmap &px, const QString &text, QWidget *parent) : Q3CanvasView(parent, "KTAnimWidgetText"), m_type(AnimText)
{
#if DEBUG_KTANIMWIDGET
	qDebug("[Initializing KTAnimWidget]");
#endif
	setVScrollBarMode(AlwaysOff);
	setHScrollBarMode(AlwaysOff);
	
	m_canvas = new Q3Canvas(px.width(), px.height());
	m_canvas->setAdvancePeriod(40);
	m_canvas->setBackgroundPixmap(px);
	m_canvas->setDoubleBuffering(true);

	m_item = new Q3CanvasText(text, QApplication::font(), m_canvas);
	static_cast<Q3CanvasText*>(m_item)->setTextFlags (Qt::AlignLeft );
	
	setCanvas(m_canvas);
	resize(px.width()/2, px.height());
	show();
}

KTAnimWidget::KTAnimWidget(ListOfPixmaps lop, QWidget *parent) : Q3CanvasView(parent, "KTAnimWidgetPixmap"), m_type(AnimPixmap)
{
#if DEBUG_KTANIMWIDGET
	qDebug("[Initializing KTAnimWidget]");
#endif
	Q_ASSERT(lop.count() > 1);
	setVScrollBarMode(AlwaysOff);
	setHScrollBarMode(AlwaysOff);

	m_canvas = new Q3Canvas(lop[0].width(), lop[0].height());
	m_canvas->setAdvancePeriod(5000);
	m_canvas->setDoubleBuffering(true);
	
	Q3CanvasPixmapArray *pa = new Q3CanvasPixmapArray(lop);
	
	m_item = new Q3CanvasSprite(pa, m_canvas);

	setCanvas(m_canvas);
	resize(m_canvas->width()/2, m_canvas->height());

	show();
}

KTAnimWidget::~ KTAnimWidget()
{
}

void KTAnimWidget::setBackgroundPixmap(const QPixmap &px)
{
	m_canvas->setBackgroundPixmap(px);
}

void KTAnimWidget::stop()
{
	m_item->setAnimated(false);
}

void KTAnimWidget::start()
{
	switch ( m_type )
	{
		case AnimText:
		{
			m_item->move( 20, m_canvas->height());
			m_item->show();
			m_item->setAnimated(true);
			m_item->setYVelocity(-0.5);
		}
		break;
		case AnimPixmap:
		{
			// QCanvasPixmap *px = static_cast<QCanvasSprite*>(m_item)->image(0);
			// setBackgroundPixmap(*px);
	
			static_cast<Q3CanvasSprite*>(m_item)->setFrameAnimation(Q3CanvasSprite::Cycle);
			static_cast<Q3CanvasSprite*>(m_item)->setAnimated(true);
			static_cast<Q3CanvasSprite*>(m_item)->show();
			static_cast<Q3CanvasSprite*>(m_item)->advance(1);
			//m_item->setYVelocity(-0.5);
		}
		break;
	}
}

void KTAnimWidget::follow()
{
	m_item->setAnimated(true);
}

bool KTAnimWidget::isItemVisible()
{
	bool incanvas = false;
	QRect r = m_item->boundingRect();
	
	if ( r.y() < - r.height() || r.x() < - r.width() )
		incanvas = false;
	else
		incanvas = true;
	
	return m_item->isVisible() && incanvas;
}

void KTAnimWidget::showEvent ( QShowEvent * e )
{
	if ( isItemVisible() )
	{
		follow();
	}
	else
	{
		start();
	}
	Q3CanvasView::showEvent ( e);
}

void KTAnimWidget::hideEvent ( QHideEvent * e)
{
	stop();
	Q3CanvasView::hideEvent ( e);
}
