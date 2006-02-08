//
// C++ Implementation: ktaction
/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@toonka.com   *
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

#include "ktaction.h"
#include "ktactionmanager.h"

KTAction::KTAction( QObject * parent, const QString &id  ) : QAction(parent), m_id(id.toLower())
{
}

KTAction::KTAction ( const QString & text, QObject * parent, const QString &id  ) : QAction(text, parent), m_id(id.toLower())
{
}

KTAction::KTAction ( const QIcon & icon, const QString & text, QObject * parent, const QString &id  ) : QAction(icon, text, parent), m_id(id.toLower())
{
}

KTAction::KTAction ( const QIcon & icon, QObject * parent, const QString &id ) : QAction(parent), m_id(id.toLower())
{
	setIcon(icon);
}

KTAction::KTAction ( const QIcon & icon, const QString & text, const QString &key, QObject * parent, const QString &id ) : QAction(icon,text,parent), m_id(id.toLower())
{
	setShortcut(QKeySequence(key));
}

KTAction::KTAction ( const QIcon & icon, const QKeySequence &key, QObject * parent, const QString &id ) : QAction(parent), m_id(id.toLower())
{
	setIcon(icon);
	setShortcut (key);
}

KTAction::KTAction(const QIcon & icon, const QString &text, const QKeySequence &key, QObject *reciever, const char *slot, QObject * parent, const QString &id) : QAction(icon, text, parent), m_id(id.toLower())
{
	setShortcut(key);
	connect(this, SIGNAL(triggered()), reciever, slot);
}

//////////

KTAction::KTAction( KTActionManager * parent, const QString &id  ) : QAction(parent), m_id(id.toLower())
{
	initWithManager(parent);
}

KTAction::KTAction ( const QString & text, KTActionManager * parent, const QString &id  ) : QAction(text, parent), m_id(id.toLower())
{
	initWithManager(parent);
}

KTAction::KTAction ( const QIcon & icon, const QString & text, KTActionManager * parent, const QString &id  ) : QAction(icon, text, parent), m_id(id.toLower())
{
	initWithManager(parent);
}

KTAction::KTAction ( const QIcon & icon, const QString & text, const QString &key, KTActionManager * parent , const QString &id) : QAction(icon, text, parent), m_id(id.toLower())
{
	setShortcut(QKeySequence(key));
	initWithManager(parent);
}
				
KTAction::KTAction ( const QIcon & icon, KTActionManager * parent, const QString &id) : QAction(parent), m_id(id.toLower())
{
	setIcon(icon);
	initWithManager(parent);
}

KTAction::KTAction ( const QIcon & icon, const QKeySequence &key, KTActionManager * parent , const QString &id) : QAction(parent), m_id(id.toLower())
{
	setIcon(icon);
	setShortcut (key);
	initWithManager(parent);
}

KTAction::KTAction(const QIcon & icon, const QString &text, const QKeySequence &key, QObject *reciever, const char *slot, KTActionManager * parent, const QString &id) : QAction(icon, text, parent), m_id(id.toLower())
{
	setShortcut(key);
	connect(this, SIGNAL(triggered()), reciever, slot);
	initWithManager(parent);
}

KTAction::KTAction(const QString &text, const QKeySequence &key, QObject *reciever, const char *slot, KTActionManager * parent, const QString &id) : QAction(text, parent), m_id(id.toLower())
{
	setShortcut(key);
	connect(this, SIGNAL(triggered()), reciever, slot);
	initWithManager(parent);
}

KTAction::~KTAction()
{
}

void KTAction::initWithManager(KTActionManager * parent)
{
	parent->insert(this);	
}

void KTAction::init()
{
}

void KTAction::setCursor(const QCursor &cursor)
{
	m_cursor = cursor;
}

QCursor KTAction::cursor() const
{
	return m_cursor;
}

