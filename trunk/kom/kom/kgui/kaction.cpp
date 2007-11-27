/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#include "kaction.h"
#include "kactionmanager.h"


KAction::KAction( QObject * parent, const QString &id ) : QAction(parent)
{
	if ( KActionManager *m = dynamic_cast<KActionManager *>(parent) )
	{
		initWithManager( m, id );
	}
}

KAction::KAction ( const QString & text, QObject * parent, const QString &id) : QAction(text, parent)
{
	if ( KActionManager *m = dynamic_cast<KActionManager *>(parent) )
	{
		initWithManager( m, id );
	}
}

KAction::KAction ( const QIcon & icon, const QString & text, QObject * parent, const QString &id) : QAction(icon, text, parent)
{
	if ( KActionManager *m = dynamic_cast<KActionManager *>(parent) )
	{
		initWithManager( m, id );
	}
}

KAction::KAction ( const QIcon & icon, QObject *parent, const QString &id ) : QAction(parent)
{
	setIcon(icon);
	if ( KActionManager *m = dynamic_cast<KActionManager *>(parent) )
	{
		initWithManager( m , id);
	}
}

KAction::KAction ( const QIcon & icon, const QString & text, const QString &key, QObject * parent, const QString &id) : QAction(icon,text,parent)
{
	setShortcut(QKeySequence(key));
	if ( KActionManager *m = dynamic_cast<KActionManager *>(parent) )
	{
		initWithManager( m , id);
	}
}

KAction::KAction ( const QIcon & icon, const QKeySequence &key, QObject * parent, const QString &id) : QAction(parent)
{
	setIcon(icon);
	setShortcut (key);
	
	if ( KActionManager *m = dynamic_cast<KActionManager *>(parent) )
	{
		initWithManager( m, id );
	}
}

KAction::KAction(const QIcon & icon, const QString &text, const QKeySequence &key, QObject *reciever, const char *slot, QObject * parent, const QString &id) : QAction(icon, text, parent)
{
	setShortcut(key);
	connect(this, SIGNAL(triggered()), reciever, slot);
	
	if ( KActionManager *m = dynamic_cast<KActionManager *>(parent) )
	{
		initWithManager( m, id );
	}
}

KAction::KAction(const QString &text, const QKeySequence &key, QObject *reciever, const char *slot, QObject * parent, const QString &id) : QAction(text, parent)
{
	setShortcut(key);
	connect(this, SIGNAL(triggered()), reciever, slot);
	
	if ( KActionManager *m = dynamic_cast<KActionManager *>(parent) )
	{
		initWithManager( m, id );
	}
}

KAction::~KAction()
{
}

void KAction::initWithManager(KActionManager * parent, const QString &id)
{
	if ( !id.isEmpty() )
		parent->insert(this, id);
}

void KAction::init()
{
}

void KAction::setCursor(const QCursor &cursor)
{
	m_cursor = cursor;
}

QCursor KAction::cursor() const
{
	return m_cursor;
}
