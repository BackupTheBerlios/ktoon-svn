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

#include "ktactionmanager.h"

KTActionManager::KTActionManager(QWidget *parent) : QObject(parent), m_widget(0)
{
	setObjectName( "KTActionManager"+parent->objectName() );
	setWidget(parent);
}

KTActionManager::~KTActionManager()
{
}

void KTActionManager::setWidget(QWidget *w)
{
	if ( ! m_widget )
	{
		m_widget = w;
	}
}

bool KTActionManager::insert(KTAction *action)
{
	QString id = action->id();
	
	KTAction *a = m_actionDict[ id ];
	if ( a == action )
	{
		return false;
	}

	return m_actionDict.insert( id, action );
}

void KTActionManager::remove( KTAction* action )
{
	delete take( action );
}

QAction *KTActionManager::take( KTAction* action )
{
	QString id = action->id();
	
	QAction *a = m_actionDict.take( id );
	if ( !a || a != action )
	{
		return 0;
	}
	
	return a;
}

QAction *KTActionManager::find(const QString &id) const
{
	return m_actionDict[id.toLower()];
}

QAction *KTActionManager::operator[](const QString &id) const
{
	return find(id);
}
