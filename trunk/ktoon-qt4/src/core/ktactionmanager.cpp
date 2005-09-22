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

KTActionManager::KTActionManager(QWidget *parent, const char *name) : QObject(parent, name), m_widget(0)
{
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

void KTActionManager::insert(QAction *action)
{
	char uname[100];
	const char *name = action->name();
	if( !qstrcmp( name, "unnamed" ) )
	{
		sprintf(uname, "unnamed-%p", (void *)action);
		name = uname;
	}
	
	QAction *a = m_actionDict[ name ];
	if ( a == action )
	{
		return;
	}

	m_actionDict.insert( name, action );
}

void KTActionManager::remove( QAction* action )
{
	delete take( action );
}

QAction *KTActionManager::take( QAction* action )
{
	char unnamed_name[100];
	const char *name = action->name();
	
	if( !qstrcmp( name, "unnamed" ) )
	{
		sprintf(unnamed_name, "unnamed-%p", (void *) action);
		name = unnamed_name;
	}
	
	QAction *a = m_actionDict.take( name );
	if ( !a || a != action )
	{
		return 0;
	}
	
	return a;
}

QAction *KTActionManager::find(const QString &name) const
{
	return m_actionDict[name];
}

QAction *KTActionManager::operator[](const QString &name) const
{
	return find(name);
}
