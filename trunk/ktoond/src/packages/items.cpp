/***************************************************************************
 *   Project KTOOND: KToon Collaboration Daemon                            *
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
 *                                                                         *
 *  License:                                                               *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *    This program is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#include "items.h"
#include <QStringList>

/*
<!-- Answer to request <list> -->
<items version="0">
        <item>project1.ktn</item>
        <item>project2.ktn</item>
        <item>project3.ktn</item>
        <item>project4.ktn</item>
</items>
*/

namespace Packages
{
    
Items::Items()
 : Package()
{
    m_root = createElement("items");
    m_root.setAttribute("version", "0");
    appendChild(m_root);
}

Items::~Items()
{
    
}

void Items::addItem(const QString& item)
{
    m_root.appendChild(createElement("item")).appendChild(createTextNode(item));
}

void Items::addItems(const QStringList& items)
{
    foreach (const QString item, items)
             addItem(item);
}

void Items::setItems( const QStringList& items)
{
    removeChild (m_root);
    m_root = createElement("items");
    m_root.setAttribute("version",  "0");
    appendChild(m_root);
    
    addItems(items);
}

}
