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

void KTLibraryFolder::addObject(KTLibraryObject *object)
{
	m_objects << object;
}

bool KTLibraryFolder::removeObject(KTLibraryObject *object)
{
	int c = m_objects.removeAll(object);
	
	if (c == 0 )
	{
		return false;
	}
	
	return true;
}

bool KTLibraryFolder::moveObject(KTLibraryObject *object, KTLibraryFolder *folder)
{
	LibraryObjects::iterator obit = m_objects.begin();
	
	while ( obit != m_objects.end())
	{
		if ( *obit == object )
		{
			m_objects.erase(obit);
			
			folder->addObject( object );
			
			return true;
			
			break;
		}
		
		++obit;
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



