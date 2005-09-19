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
#include <q3action.h>
#include <qwidget.h>
#include <q3accel.h>
#include <q3valuelist.h>
#include <q3dict.h>

typedef Q3ValueList<Q3Action *> QActionPtrList;
typedef Q3Dict<Q3Action> QActionDict;

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
		void insert(Q3Action *action);
		void remove( Q3Action* action );
		Q3Action *take( Q3Action* action );
		Q3Action *find(const QString &name) const;
		Q3Action *operator[](const QString &) const;

	private:
		Q3Accel *m_accel;
		QWidget *m_widget;
		QActionDict m_actionDict;
};

#endif
