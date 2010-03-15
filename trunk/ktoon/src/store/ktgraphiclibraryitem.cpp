/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   License:                                                              *
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

#include <kcore/kdebug.h>

struct KTGraphicLibraryItem::Private
{
	QString symbolName;
	QList<QGraphicsItem *> items;
};

KTGraphicLibraryItem::KTGraphicLibraryItem() : KTProxyItem(), k(new Private)
{
}

KTGraphicLibraryItem::KTGraphicLibraryItem(KTLibraryObject *object) : KTProxyItem(), k(new Private)
{
	setObject(object);
}

KTGraphicLibraryItem::~KTGraphicLibraryItem()
{
	qDeleteAll(k->items);
	delete k;
}

QDomElement KTGraphicLibraryItem::toXml(QDomDocument &doc) const
{
	QDomElement library = doc.createElement("symbol");
	library.setAttribute("id", k->symbolName);
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
		#ifdef K_DEBUG
			kWarning("library") << "Setting null library object";
		#endif
		return;
	}
	
	#ifdef K_DEBUG
		K_FUNCINFOX("library") << object->symbolName();
	#endif
	
	k->symbolName = object->symbolName();
	switch(object->type())
	{
		case KTLibraryObject::Item:
		case KTLibraryObject::Svg:
		case KTLibraryObject::Image:
		case KTLibraryObject::Text:
		{
			setItem( qvariant_cast<QGraphicsItem *>(object->data()) );
		}
		break;
		default: break;
	}
}

void KTGraphicLibraryItem::setSymbolName(const QString &name)
{
	k->symbolName = name;
}

QString KTGraphicLibraryItem::symbolName() const
{
	return k->symbolName;
}


