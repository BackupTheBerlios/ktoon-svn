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
#include <QSvgRenderer>

#include <kcore/kdebug.h>

KTSvgItem::KTSvgItem(QGraphicsItem * parent)
    : QGraphicsSvgItem(parent)
{
}

KTSvgItem::KTSvgItem(QString &path)
    : QGraphicsSvgItem(path)
{
}

KTSvgItem::~KTSvgItem()
{
}

void KTSvgItem::setObjectName(QString &name)
{
    id = name;
}

QString KTSvgItem::objectName() const
{
    return id;
}

void KTSvgItem::setContent(QString &xml)
{
    data = xml;
}

QString KTSvgItem::content() const
{
    return data;
}

void KTSvgItem::rendering()
{
    QByteArray stream = data.toLocal8Bit(); 
    renderer()->load(stream);
}

void KTSvgItem::fromXml(const QString &xml)
{
}

QDomElement KTSvgItem::toXml(QDomDocument &doc) const
{
    QDomElement root = doc.createElement("svg");
    root.setAttribute("svgData", data);

    return root;
}
