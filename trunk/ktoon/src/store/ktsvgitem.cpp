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

#include "ktsvgitem.h"
#include "ktserializer.h"

#include <QSvgRenderer>

#include <kcore/kdebug.h>

KTSvgItem::KTSvgItem(QGraphicsItem * parent)
    : QGraphicsSvgItem(parent)
{
}

KTSvgItem::KTSvgItem(QString &file)
    : QGraphicsSvgItem(file)
{
    path = file;
}

KTSvgItem::~KTSvgItem()
{
}

QString KTSvgItem::itemPath() const
{
    return path;
}

void KTSvgItem::rendering()
{
    QByteArray stream = data.toLocal8Bit(); 
    renderer()->load(stream);
}

void KTSvgItem::fromXml(const QString &xml)
{
    Q_UNUSED(xml);
}

QDomElement KTSvgItem::toXml(QDomDocument &doc) const
{
    QDomElement root = doc.createElement("svg");
    root.setAttribute("itemPath", path); 
    root.appendChild(KTSerializer::properties(this, doc));
 
    return root;
}
