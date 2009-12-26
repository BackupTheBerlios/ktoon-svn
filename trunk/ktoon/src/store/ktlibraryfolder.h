/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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

#ifndef KTLIBRARYFOLDER_H
#define KTLIBRARYFOLDER_H

#include <QObject>
#include <QHash>
#include <QByteArray>

#include "ktabstractserializable.h"
#include "ktlibraryobject.h"
#include "ktglobal_store.h"

class KTProject;
class KTLibraryFolder;
class KTLibraryObject;

typedef QList<KTLibraryFolder *> Folders;
typedef QHash<QString, KTLibraryObject *> LibraryObjects;

/**
 * @author David Cuadrado \<krawek@gmail.com\>
*/
class STORE_EXPORT KTLibraryFolder : public QObject, public KTAbstractSerializable
{
    Q_OBJECT;
    
    public:
        KTLibraryFolder(const QString &id, KTProject *project, QObject *parent = 0);
        ~KTLibraryFolder();
        
        void setId(const QString &id);
        QString id() const;
        
        KTLibraryObject *createSymbol(KTLibraryObject::Type type, const QString &name, const QByteArray &data, bool loaded = false);
        
        virtual bool addObject(KTLibraryObject *object, const QString &id);
        virtual bool removeObject(const QString &id);
        
        virtual void addFolder(KTLibraryFolder *folder);
        
        bool moveObject(const QString &id, KTLibraryFolder *folder);
        
        KTLibraryObject *findObject(const QString &id) const;
        
        Folders folders() const;
        LibraryObjects objects() const;
        
        int objectsCount() const;
        int foldersCount() const;
        
        KTProject *project() const;
        
    public:
        virtual void fromXml(const QString &xml);
        virtual QDomElement toXml(QDomDocument &doc) const;
        
    private:
        struct Private;
        Private *const k;
};

#endif
