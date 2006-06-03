/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#include "dactionmanager.h"
#include "ddebug.h"

DActionManager::DActionManager(QWidget *parent) : QObject(parent)
{
	setObjectName( "DActionManager"+parent->objectName() );
}

DActionManager::~DActionManager()
{
}

bool DActionManager::insert(DAction *action)
{
	QString id = action->id();
	
	DAction *a = m_actionDict[ id ];
	if ( a == action )
	{
		return false;
	}

	return m_actionDict.insert( id, action );
}

void DActionManager::remove( DAction* action )
{
	delete take( action );
}

QAction *DActionManager::take( DAction* action )
{
	QString id = action->id();
	
	QAction *a = m_actionDict.take( id );
	if ( !a || a != action )
	{
		return 0;
	}
	
	return a;
}

QAction *DActionManager::find(const QString &id) const
{
	QAction *action = m_actionDict[id.toLower()];
	
	if ( action == 0)
	{
		dError() << "DActionManager::find(): Returning NULL action: " << id;
	}
	
	return action;
}

QAction *DActionManager::operator[](const QString &id) const
{
	return find(id);
}
