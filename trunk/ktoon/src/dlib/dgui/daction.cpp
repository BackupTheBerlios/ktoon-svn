/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@gmail.com   *
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

#include "daction.h"
#include "dactionmanager.h"


DAction::DAction( QObject * parent, const QString &id ) : QAction(parent)
{
	if ( DActionManager *m = dynamic_cast<DActionManager *>(parent) )
	{
		initWithManager( m, id );
	}
}

DAction::DAction ( const QString & text, QObject * parent, const QString &id) : QAction(text, parent)
{
	if ( DActionManager *m = dynamic_cast<DActionManager *>(parent) )
	{
		initWithManager( m, id );
	}
}

DAction::DAction ( const QIcon & icon, const QString & text, QObject * parent, const QString &id) : QAction(icon, text, parent)
{
	if ( DActionManager *m = dynamic_cast<DActionManager *>(parent) )
	{
		initWithManager( m, id );
	}
}

DAction::DAction ( const QIcon & icon, QObject *parent, const QString &id ) : QAction(parent)
{
	setIcon(icon);
	if ( DActionManager *m = dynamic_cast<DActionManager *>(parent) )
	{
		initWithManager( m , id);
	}
}

DAction::DAction ( const QIcon & icon, const QString & text, const QString &key, QObject * parent, const QString &id) : QAction(icon,text,parent)
{
	setShortcut(QKeySequence(key));
	if ( DActionManager *m = dynamic_cast<DActionManager *>(parent) )
	{
		initWithManager( m , id);
	}
}

DAction::DAction ( const QIcon & icon, const QKeySequence &key, QObject * parent, const QString &id) : QAction(parent)
{
	setIcon(icon);
	setShortcut (key);
	
	if ( DActionManager *m = dynamic_cast<DActionManager *>(parent) )
	{
		initWithManager( m, id );
	}
}

DAction::DAction(const QIcon & icon, const QString &text, const QKeySequence &key, QObject *reciever, const char *slot, QObject * parent, const QString &id) : QAction(icon, text, parent)
{
	setShortcut(key);
	connect(this, SIGNAL(triggered()), reciever, slot);
	
	if ( DActionManager *m = dynamic_cast<DActionManager *>(parent) )
	{
		initWithManager( m, id );
	}
}

DAction::DAction(const QString &text, const QKeySequence &key, QObject *reciever, const char *slot, QObject * parent, const QString &id) : QAction(text, parent)
{
	setShortcut(key);
	connect(this, SIGNAL(triggered()), reciever, slot);
	
	if ( DActionManager *m = dynamic_cast<DActionManager *>(parent) )
	{
		initWithManager( m, id );
	}
}



DAction::~DAction()
{
}

void DAction::initWithManager(DActionManager * parent, const QString &id)
{
	if ( !id.isEmpty() )
		parent->insert(this, id);
}

void DAction::init()
{
}

void DAction::setCursor(const QCursor &cursor)
{
	m_cursor = cursor;
}

QCursor DAction::cursor() const
{
	return m_cursor;
}

