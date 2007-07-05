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
	Private() : realItem(0) {}
	QGraphicsItem *realItem;
};

KTProxyItem::KTProxyItem(QGraphicsItem *item) : QGraphicsItem(), d(new Private)
{
	setItem(item);
	setPos(0,0);
}


KTProxyItem::~KTProxyItem()
{
}

void KTProxyItem::setItem(QGraphicsItem *item)
{
	if( d->realItem )
	{
		this->removeSceneEventFilter(d->realItem);
	}
	
	d->realItem = item;
	
	if( d->realItem )
	{
		d->realItem->installSceneEventFilter(this);
		this->setFlags(d->realItem->flags());
	}
}

QGraphicsItem *KTProxyItem::item() const
{
	return d->realItem;
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
		d->realItem->paint(painter, option, widget);
		
	}
}

QPainterPath KTProxyItem::shape() const
{
	if( d->realItem)
		return d->realItem->shape();
	
	return QGraphicsItem::shape();
}

bool KTProxyItem::collidesWithItem( const QGraphicsItem * other, Qt::ItemSelectionMode mode) const
{
	if( d->realItem)
		return d->realItem->collidesWithItem(other, mode);
	
	return QGraphicsItem::collidesWithItem(other, mode);
}


bool KTProxyItem::collidesWithPath ( const QPainterPath & path, Qt::ItemSelectionMode mode) const
{
	if( d->realItem)
		return d->realItem->collidesWithPath(path, mode);
	
	return QGraphicsItem::collidesWithPath(path, mode);
}


bool KTProxyItem::contains ( const QPointF & point ) const
{
	if( d->realItem)
		return d->realItem->contains(point);
	
	return QGraphicsItem::contains(point);
}


bool KTProxyItem::isObscuredBy ( const QGraphicsItem * item ) const
{
	if( d->realItem)
		return d->realItem->isObscuredBy(item);
	
	return QGraphicsItem::isObscuredBy(item);
}


QPainterPath KTProxyItem::opaqueArea () const
{
	if( d->realItem)
		return d->realItem->opaqueArea();
	
	return QGraphicsItem::opaqueArea();
}


		

