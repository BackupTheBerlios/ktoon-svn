//
// C++ Implementation: ktaction
//
// Description: 
//
//
// Author: David Cuadrado <krawek@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

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



