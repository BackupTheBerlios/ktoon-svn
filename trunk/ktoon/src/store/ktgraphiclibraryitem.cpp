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

#include "ktgraphiclibraryitem.h"
#include "ktlibraryobject.h"

#include "ktserializer.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>

#include <dcore/ddebug.h>

struct KTGraphicLibraryItem::Private
{
	QString symbolName;
	QList<QGraphicsItem *> items;
};

KTGraphicLibraryItem::KTGraphicLibraryItem() : KTProxyItem(), d(new Private)
{
}

KTGraphicLibraryItem::KTGraphicLibraryItem(KTLibraryObject *object) : KTProxyItem(), d(new Private)
{
	setObject(object);
}

KTGraphicLibraryItem::~KTGraphicLibraryItem()
{
	qDeleteAll(d->items);
	delete d;
}

QDomElement KTGraphicLibraryItem::toXml(QDomDocument &doc) const
{
	QDomElement library = doc.createElement("symbol");
	library.setAttribute("id", d->symbolName);
	library.appendChild( KTSerializer::properties( this, doc));
	
	return library;
}

void KTGraphicLibraryItem::fromXml(const QString &xml)
{
}

void KTGraphicLibraryItem::setObject(KTLibraryObject *object)
{
	if( !object)
	{
		dWarning("library") << "Setting null library object";
		return;
	}
	
	D_FUNCINFOX("library") << object->symbolName();
	
	d->symbolName = object->symbolName();
	switch(object->type())
	{
		case KTLibraryObject::Item:
		case KTLibraryObject::Svg:
		case KTLibraryObject::Image:
		{
			setItem( qvariant_cast<QGraphicsItem *>(object->data()) );
		}
		break;
		case KTLibraryObject::Text:
		{
			QGraphicsTextItem *item = new QGraphicsTextItem(object->data().toString());
			setItem(item);
			
			d->items << item;
		}
		break;
		default: break;
	}
}

void KTGraphicLibraryItem::setSymbolName(const QString &name)
{
	d->symbolName = name;
}

QString KTGraphicLibraryItem::symbolName() const
{
	return d->symbolName;
}


