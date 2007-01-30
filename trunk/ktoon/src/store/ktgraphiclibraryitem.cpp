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

#include "ktgraphiclibraryitem.h"

#include "ktlibraryobject.h"

struct KTGraphicLibraryItem::Private 
{
	KTLibraryObject *object;
	QGraphicsItem *realItem;
};

KTGraphicLibraryItem::KTGraphicLibraryItem(KTLibraryObject *object) : QGraphicsItem(), d(new Private)
{
	d->object = object;
	
	if ( d->object->type() == KTLibraryObject::Item || d->object->type() == KTLibraryObject::Svg )
	{
		d->realItem = qvariant_cast<QGraphicsItem *>(d->object->data());
	}
	else
	{
		qFatal("UNKNOWN TYPE: IMPLEMENT ME: KTGraphicLibraryItem");
	}
}


KTGraphicLibraryItem::~KTGraphicLibraryItem()
{
}


QRectF KTGraphicLibraryItem::boundingRect() const
{
	return d->realItem->boundingRect();
}

void KTGraphicLibraryItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	d->realItem->paint(painter, option, widget);
}

void KTGraphicLibraryItem::fromXml(const QString &xml )
{
}

QDomElement KTGraphicLibraryItem::toXml(QDomDocument &doc) const
{
	return QDomElement();
}


