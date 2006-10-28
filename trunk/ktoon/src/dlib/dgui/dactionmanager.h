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
#include "dcore/dglobal.h"

typedef QList<QAction *> DActionList;
typedef QHash<QString, QAction *> DActionDict;
typedef QHash<QString, DActionDict> DActionContainer;

class QToolBar;
class QMenuBar;

/**
 * @short la clase DActionManager provee de un manejador de acciones, este manejador facilita el acceso y ordenamiento a las acciones contieniendo todas las acciones de la aplicacion.
 * @author David Cuadrado \<krawek@gmail.com\>
*/

class D_GUI_EXPORT DActionManager : public QObject
{
	Q_OBJECT

	public:
		
		DActionManager(QObject *parent = 0);
		~DActionManager();
		
		bool insert(QAction *action, const QString &id, const QString &container = "default" );
		void remove( QAction* action, const QString &container = QString()  );
		
		QAction *take( QAction* action, const QString &container = QString()  );
		QAction *find(const QString &id, const QString &container = QString() ) const;
		QAction *operator[](const QString &id) const;
		
		QMenuBar *setupMenuBar(QMenuBar *menu, const QStringList &containers, bool clear = true);
		QMenu *setupMenu(QMenu *menu, const QString &container, bool clear = true);
		QToolBar *setupToolBar(QToolBar *toolBar, const QString &container, bool clear = true );
		
	private:
		DActionContainer m_actionContainer;
};

#endif


