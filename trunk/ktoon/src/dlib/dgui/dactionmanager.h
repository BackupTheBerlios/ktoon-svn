/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#ifndef DACTIONMANAGER_H
#define DACTIONMANAGER_H

#include <QObject>
#include <QWidget>
#include <QList>
#include <QHash>

#include "dgui/daction.h"

typedef QList<DAction *> DActionList;
typedef QHash<QString, DAction *> DActionDict;
typedef QHash<QString, DActionDict> DActionContainer;

class QToolBar;
class QMenuBar;

/**
 * @short la clase DActionManager provee de un manejador de acciones, este manejador facilita el acceso y ordenamiento a las acciones contieniendo todas las acciones de la aplicacion.
 * @author David Cuadrado <krawek@gmail.com>
*/

class DActionManager : public QObject
{
	Q_OBJECT

	public:
		
		DActionManager(QWidget *parent = 0L);
		
		~DActionManager();
		
		bool insert(DAction *action, const QString &container = "default" );
		void remove( DAction* action, const QString &container = "default" );
		
		QAction *take( DAction* action, const QString &container = "default" );
		QAction *find(const QString &id, const QString &container = "default") const;
		QAction *operator[](const QString &id) const;
		
		QMenuBar *setupMenuBar(QMenuBar *menu, const QStringList &containers);
		QToolBar *setupToolBar(QToolBar *toolBar, const QString &container = "default");
		
	private:
		DActionContainer m_actionContainer;
};

#endif
