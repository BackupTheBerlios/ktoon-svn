/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#include "ktitempreview.h"

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

KTItemPreview::KTItemPreview(QWidget *parent) : QWidget(parent), m_item(0)
{
}


KTItemPreview::~KTItemPreview()
{
}

QSize KTItemPreview::sizeHint() const
{
	if ( m_item )
	{
		return m_item->boundingRect().size().toSize();
	}
	
	return QWidget::sizeHint();
}


void KTItemPreview::render(QGraphicsItem *item)
{
	m_item = item;
	update();
}

void KTItemPreview::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	
	if ( m_item )
	{
		QStyleOptionGraphicsItem opt;
		opt.state = QStyle::State_None;
		
		if (m_item->isEnabled())
			opt.state |= QStyle::State_Enabled;
		
		opt.exposedRect = QRectF(QPointF(0,0), m_item->boundingRect().size());
		opt.levelOfDetail = 1;
		
		QMatrix matrix;
		
		opt.matrix = matrix;
		
		// TODO: poner una matrix al 'opt' para que el item se escale lo suficiente en el widget y transladarlo al origen.
		
		opt.palette = palette();
		
		m_item->paint ( &p, &opt, this ); // paint isn't const...
	}
}




