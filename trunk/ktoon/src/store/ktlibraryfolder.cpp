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

#include "ktlibraryfolder.h"
#include "ktlibraryobject.h"

#include <dcore/ddebug.h>

KTLibraryFolder::KTLibraryFolder(const QString &id, QObject *parent) : QObject(parent), m_id(id)
{
}


KTLibraryFolder::~KTLibraryFolder()
{
}

bool KTLibraryFolder::addObject(KTLibraryObject *object, const QString &id)
{
	if ( !m_objects.contains(id ) )
	{
		m_objects.insert(id, object);
		object->setParent(this);
		return true;
	}
	
	return false;
}

bool KTLibraryFolder::removeObject(const QString &id)
{
	int c = m_objects.remove(id);
	
	return c > 0;
}

bool KTLibraryFolder::moveObject(const QString &id, KTLibraryFolder *folder)
{
	if ( m_objects.contains(id) )
	{
		KTLibraryObject *object = m_objects[id];
		removeObject( id );
		
		folder->addObject( object, id);
		
		return true;
	}
	
	return false;
}

void KTLibraryFolder::setId(const QString &id)
{
	m_id = id;
}

QString KTLibraryFolder::id() const
{
	return m_id;
}

KTLibraryObject *KTLibraryFolder::findObject(const QString &id) const
{
	foreach ( QString oid, m_objects.keys())
	{
		if ( oid == id )
		{
			return m_objects[oid];
		}
	}
	
	foreach ( KTLibraryFolder *folder, m_folders )
	{
		KTLibraryObject *object = folder->findObject(id);
		
		if ( object )
		{
			return object;
		}
	}
	
	dDebug() << "Cannot find symbol with id: " << id;
	
	return 0;
}

int KTLibraryFolder::objectsCount() const
{
	return m_objects.count();
}

int KTLibraryFolder::foldersCount() const
{
	return m_folders.count();
}

