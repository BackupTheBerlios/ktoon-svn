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

KTAnimWidget::KTAnimWidget(const QPixmap &px, const QString &text, QWidget *parent) : QCanvasView(parent, "KTAnimWidgetText"), m_type(AnimText)
{
#if DEBUG_KTANIMWIDGET
	qDebug("[Initializing KTAnimWidget]");
#endif
	setVScrollBarMode(AlwaysOff);
	setHScrollBarMode(AlwaysOff);
	
	m_canvas = new QCanvas(px.width(), px.height());
	m_canvas->setAdvancePeriod(40);
	m_canvas->setBackgroundPixmap(px);
	m_canvas->setDoubleBuffering(true);

	m_item = new QCanvasText(text, QFont( "helvetica", 12 ), m_canvas);
	static_cast<QCanvasText*>(m_item)->setTextFlags (Qt::AlignLeft );
	
	setCanvas(m_canvas);
	resize(px.width()/2, px.height());
	show();
	start();
}

KTAnimWidget::KTAnimWidget(ListOfPixmaps lop, QWidget *parent) : QCanvasView(parent, "KTAnimWidgetPixmap"), m_type(AnimPixmap)
{
#if DEBUG_KTANIMWIDGET
	qDebug("[Initializing KTAnimWidget]");
#endif
	Q_ASSERT(lop.count() > 1);
	setVScrollBarMode(AlwaysOff);
	setHScrollBarMode(AlwaysOff);
	/*px.width(), px.height()*/
	m_canvas = new QCanvas(lop[0].width(), lop[0].height());
	m_canvas->setAdvancePeriod(5000);
	m_canvas->setDoubleBuffering(true);
	
	QCanvasPixmapArray *pa = new QCanvasPixmapArray(lop);
	
	m_item = new QCanvasSprite(pa, m_canvas);

	setCanvas(m_canvas);
	start();
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
			m_item->move( width() / 2, canvas()->height());
			m_item->show();
			m_item->setAnimated(true);
			m_item->setYVelocity(-0.5);
		}
		break;
		case AnimPixmap:
		{
			// QCanvasPixmap *px = static_cast<QCanvasSprite*>(m_item)->image(0);
			// setBackgroundPixmap(*px);
	
			static_cast<QCanvasSprite*>(m_item)->setFrameAnimation(QCanvasSprite::Cycle);
			static_cast<QCanvasSprite*>(m_item)->setAnimated(true);
			static_cast<QCanvasSprite*>(m_item)->show();
			static_cast<QCanvasSprite*>(m_item)->advance(1);
			//m_item->setYVelocity(-0.5);
		}
		break;
	}
}

void KTAnimWidget::follow()
{
	m_item->setAnimated(true);
}

void KTAnimWidget::showEvent ( QShowEvent * e)
{
	follow();
	QCanvasView::showEvent ( e);
}

void KTAnimWidget::hideEvent ( QHideEvent * e)
{
	stop();
	QCanvasView::hideEvent ( e);
}

