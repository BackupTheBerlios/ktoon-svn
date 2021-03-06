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
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "ktgraphiclibraryitem.h"
#include "ktlibraryobject.h"
#include "ktserializer.h"

#include <QGraphicsTextItem>

#include <kcore/kdebug.h>

struct KTGraphicLibraryItem::Private
{
    QString symbolName;
    QString svgContent;
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
    library.appendChild(KTSerializer::properties(this, doc));
    
    return library;
}

void KTGraphicLibraryItem::fromXml(const QString &xml)
{
    Q_UNUSED(xml);
}

void KTGraphicLibraryItem::setObject(KTLibraryObject *object)
{
    if (!object) {
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
        case KTLibraryObject::Text:
        case KTLibraryObject::Image:
        {
             setItem(qvariant_cast<QGraphicsItem *>(object->data()));
        }
        break;
        case KTLibraryObject::Svg:
        {
             setSvgContent(object->dataPath());
        }
        break;
        default: 
        break;
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

void KTGraphicLibraryItem::setSvgContent(const QString &path)
{
    k->svgContent = path;
}

QString KTGraphicLibraryItem::svgContent()
{
   return k->svgContent;
}
