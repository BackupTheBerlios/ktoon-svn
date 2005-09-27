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

#ifndef KTACTIONMANAGER_H
#define KTACTIONMANAGER_H

#include <qobject.h>
#include <qaction.h>
#include <qwidget.h>
#include <qaccel.h>
#include <qvaluelist.h>
#include <qdict.h>

typedef QValueList<QAction *> QActionPtrList;
typedef QDict<QAction> QActionDict;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTActionManager : public QObject
{
	Q_OBJECT

	public:
		KTActionManager(QWidget *parent = 0L, const char *name = 0L);
		~KTActionManager();
		void setWidget(QWidget *w);
		void insert(QAction *action);
		void remove( QAction* action );
		QAction *take( QAction* action );
		QAction *find(const QString &name) const;
		QAction *operator[](const QString &) const;

	private:
		QAccel *m_accel;
		QWidget *m_widget;
		QActionDict m_actionDict;
};

#endif