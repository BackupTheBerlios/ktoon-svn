/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado                                  *
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

#include "ktproxyitem.h"
#include <QPainter>

struct KTProxyItem::Private 
{
	QGraphicsItem *realItem;
};

KTProxyItem::KTProxyItem(QGraphicsItem *item) : QGraphicsItem(), d(new Private)
{
	d->realItem = item;
	
	setPos(0,0);
}


KTProxyItem::~KTProxyItem()
{
}

void KTProxyItem::setItem(QGraphicsItem *item)
{
	d->realItem = item;
}

QRectF KTProxyItem::boundingRect() const
{
	if ( d->realItem )
		return d->realItem->boundingRect();
	
	return QRectF(0,0, 0,0);
}

void KTProxyItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	if( d->realItem )
	{
		if( QGraphicsPathItem *path = qgraphicsitem_cast<QGraphicsPathItem *>(d->realItem) )
		{
			painter->translate(-path->path().boundingRect().topLeft().x(), -path->path().boundingRect().topLeft().y());
		}
		d->realItem->paint(painter, option, widget);
		
	}
}


