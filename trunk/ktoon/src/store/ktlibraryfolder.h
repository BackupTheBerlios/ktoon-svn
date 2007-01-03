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

#ifndef KTLIBRARYFOLDER_H
#define KTLIBRARYFOLDER_H

#include <QObject>
#include <QHash>
#include "ktlibraryobject.h"

class KTLibraryFolder;

typedef QList<KTLibraryFolder *> Folders;
typedef QHash<QString, KTLibraryObject *> LibraryObjects;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class KTLibraryFolder : public QObject
{
	Q_OBJECT;
	
	public:
		KTLibraryFolder(const QString &id, QObject *parent = 0);
		~KTLibraryFolder();
		
		void setId(const QString &id);
		QString id() const;
		
		virtual bool addObject(KTLibraryObject *object, const QString &id);
		virtual bool removeObject(const QString &id);
		
		bool moveObject(const QString &id, KTLibraryFolder *folder);
		
		KTLibraryObject *findObject(const QString &id) const;
		
		
		int objectsCount() const;
		int foldersCount() const;
		
	private:
		QString m_id;
		Folders m_folders;
		LibraryObjects m_objects;
};


#endif




