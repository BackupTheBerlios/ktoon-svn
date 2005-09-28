//
// C++ Interface: ktaction
//
// Description: 
//
//
// Author: David Cuadrado <krawek@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KTACTION_H
#define KTACTION_H

#include <QAction>
#include <QKeySequence>

class KTActionManager;

/**
	@author David Cuadrado <krawek@toonka.com>
*/
class KTAction : public QAction
{
	public:
		KTAction ( QObject * parent, const QString &id);
		KTAction ( const QString & text, QObject * parent, const QString &id );
		KTAction ( const QIcon & icon, const QString & text, QObject * parent, const QString &id );
		KTAction ( const QIcon & icon, const QString & text, const QString &key, QObject * parent, const QString &id );
		KTAction ( const QIcon & icon, QObject * parent, const QString &id );
		KTAction ( const QIcon & icon,  const QKeySequence &key, QObject * parent, const QString &id );
		KTAction (const QIcon & icon, const QString &text, const QKeySequence &key, QObject *reciever, const char *slot, QObject * parent, const QString &id);
		
		
		/////////
		
		KTAction ( KTActionManager * parent, const QString &id );
		KTAction ( const QString & text, KTActionManager * parent , const QString &id);
		KTAction ( const QIcon & icon, const QString & text, KTActionManager * parent , const QString &id);
		KTAction ( const QIcon & icon, const QString & text, const QString &key, KTActionManager * parent , const QString &id);
		KTAction ( const QIcon & icon, KTActionManager * parent , const QString &id);
		KTAction ( const QIcon & icon, const QKeySequence &key, KTActionManager * parent , const QString &id);
		
		KTAction(const QIcon & icon, const QString &text, const QKeySequence &key, QObject *reciever, const char *slot, KTActionManager * parent, const QString &id);
		
		~KTAction();
		
		QString id() { return m_id; };
		
	private:
		void initWithManager(KTActionManager * parent);
		void init();
		
	private:
		QString m_id;
};

#endif
