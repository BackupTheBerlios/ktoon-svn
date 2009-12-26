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

#ifndef KTLIBRARY_H
#define KTLIBRARY_H

#include <QObject>

#include "ktglobal_store.h"
#include "ktlibraryfolder.h"

class KTProject;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class STORE_EXPORT KTLibrary : public KTLibraryFolder
{
    Q_OBJECT;
    
    public:
        enum Class
        {
            Symbol = 0x01,
            Folder
        };
        KTLibrary(const QString &id, KTProject *parent = 0);
        ~KTLibrary();
        
    public:
        virtual void fromXml(const QString &xml);
        virtual QDomElement toXml(QDomDocument &doc) const;
};

#endif
