//
// C++ Implementation: daction
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

#include "daction.h"
#include "dactionmanager.h"

DAction::DAction( QObject * parent, const QString &id  ) : QAction(parent), m_id(id.toLower())
{
}

DAction::DAction ( const QString & text, QObject * parent, const QString &id  ) : QAction(text, parent), m_id(id.toLower())
{
}

DAction::DAction ( const QIcon & icon, const QString & text, QObject * parent, const QString &id  ) : QAction(icon, text, parent), m_id(id.toLower())
{
}

DAction::DAction ( const QIcon & icon, QObject * parent, const QString &id ) : QAction(parent), m_id(id.toLower())
{
	setIcon(icon);
}

DAction::DAction ( const QIcon & icon, const QString & text, const QString &key, QObject * parent, const QString &id ) : QAction(icon,text,parent), m_id(id.toLower())
{
	setShortcut(QKeySequence(key));
}

DAction::DAction ( const QIcon & icon, const QKeySequence &key, QObject * parent, const QString &id ) : QAction(parent), m_id(id.toLower())
{
	setIcon(icon);
	setShortcut (key);
}

DAction::DAction(const QIcon & icon, const QString &text, const QKeySequence &key, QObject *reciever, const char *slot, QObject * parent, const QString &id) : QAction(icon, text, parent), m_id(id.toLower())
{
	setShortcut(key);
	connect(this, SIGNAL(triggered()), reciever, slot);
}

//////////

DAction::DAction( DActionManager * parent, const QString &id  ) : QAction(parent), m_id(id.toLower())
{
	initWithManager(parent);
}

DAction::DAction ( const QString & text, DActionManager * parent, const QString &id  ) : QAction(text, parent), m_id(id.toLower())
{
	initWithManager(parent);
}

DAction::DAction ( const QIcon & icon, const QString & text, DActionManager * parent, const QString &id  ) : QAction(icon, text, parent), m_id(id.toLower())
{
	initWithManager(parent);
}

DAction::DAction ( const QIcon & icon, const QString & text, const QString &key, DActionManager * parent , const QString &id) : QAction(icon, text, parent), m_id(id.toLower())
{
	setShortcut(QKeySequence(key));
	initWithManager(parent);
}
				
DAction::DAction ( const QIcon & icon, DActionManager * parent, const QString &id) : QAction(parent), m_id(id.toLower())
{
	setIcon(icon);
	initWithManager(parent);
}

DAction::DAction ( const QIcon & icon, const QKeySequence &key, DActionManager * parent , const QString &id) : QAction(parent), m_id(id.toLower())
{
	setIcon(icon);
	setShortcut (key);
	initWithManager(parent);
}

DAction::DAction(const QIcon & icon, const QString &text, const QKeySequence &key, QObject *reciever, const char *slot, DActionManager * parent, const QString &id) : QAction(icon, text, parent), m_id(id.toLower())
{
	setShortcut(key);
	connect(this, SIGNAL(triggered()), reciever, slot);
	initWithManager(parent);
}

DAction::DAction(const QString &text, const QKeySequence &key, QObject *reciever, const char *slot, DActionManager * parent, const QString &id) : QAction(text, parent), m_id(id.toLower())
{
	setShortcut(key);
	connect(this, SIGNAL(triggered()), reciever, slot);
	initWithManager(parent);
}

DAction::~DAction()
{
}

void DAction::initWithManager(DActionManager * parent)
{
	parent->insert(this);	
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

