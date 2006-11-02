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
		
		return true;
	}
	
	return false;
}

bool KTLibraryFolder::removeObject(const QString &id)
{
	int c = m_objects.remove(id);
	
	if (c == 0 )
	{
		return false;
	}
	
	return true;
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



