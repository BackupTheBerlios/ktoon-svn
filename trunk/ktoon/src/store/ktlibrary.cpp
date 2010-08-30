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

#include "ktlibrary.h"
#include "ktproject.h"

#include <kcore/kdebug.h>

KTLibrary::KTLibrary(const QString &id, KTProject *parent) : KTLibraryFolder(id, parent)
{
}

KTLibrary::~KTLibrary()
{
}

void KTLibrary::fromXml(const QString &xml)
{
    QDomDocument document;
    
    if (! document.setContent(xml))
        return;
    
    QDomElement root = document.documentElement();
    QDomNode n = root.firstChild();
    
    while (!n.isNull()) {
           QDomElement e = n.toElement();
        
           if (!e.isNull()) {
               if (e.tagName() == "library") {
                
               } else if (e.tagName() == "folder") {
                          QString doc;
                          {
                             QTextStream ts(&doc);
                             ts << n;
                          }
                
                          KTLibraryFolder::fromXml(doc);
              }
           }
           n = n.nextSibling();
    }
}

QDomElement KTLibrary::toXml(QDomDocument &doc) const
{
    QDomElement root = doc.createElement("library");
    
    root.appendChild(KTLibraryFolder::toXml(doc));
    
    return root;
}
