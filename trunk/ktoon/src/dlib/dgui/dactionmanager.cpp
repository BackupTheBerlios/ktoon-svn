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

#include "dactionmanager.h"
#include "ddebug.h"

#include <QMenuBar>
#include <QToolBar>

/**
 * Construye un manejador de acciones.
 * @param parent widget que contine el manejador de acciones
 */
DActionManager::DActionManager(QWidget *parent) : QObject(parent)
{
	setObjectName( "DActionManager"+parent->objectName() );
}

/**
 * Destructor
 */
DActionManager::~DActionManager()
{
}

/**
 * Inserta una accion al manejador
 * @param action accion para añadir
 * @return 
 */
bool DActionManager::insert(DAction *action, const QString &container )
{
	QString id = action->id();
	
	DAction *a = (m_actionContainer[container])[ id ];
	if ( a == action )
	{
		return false;
	}

	return m_actionContainer[container].insert( id, action );
}

/**
 * Remueve una accion del manejador
 * @param action para remover
 */
void DActionManager::remove( DAction* action, const QString &container )
{
	delete take( action, container );
}

/**
 * Remuve una accion del manejador retornando dicha accion.
 * @param action para remover
 * @return la accion removida o cero si esta no estaba en el manejador
 */
QAction *DActionManager::take( DAction* action, const QString &container  )
{
	QString id = action->id();
	
	QAction *a = m_actionContainer[container].take( id );
	if ( !a || a != action )
	{
		return 0;
	}
	
	return a;
}

/**
 * Busca una accion en el manejardor.
 * @param id asociado a la accion
 * @return la accion requeriada
 */
QAction *DActionManager::find(const QString &id, const QString &container) const
{
	QAction *action = m_actionContainer[container][id.toLower()];
	
	if ( action == 0)
	{
		dError() << "DActionManager::find(): Returning NULL action: " << id << " in " << container;
	}
	
	return action;
}

/**
 * Retorna la accion asociada a id
 * @param id 
 */
QAction *DActionManager::operator[](const QString &id) const
{
	return find(id, "default");
}


QMenuBar *DActionManager::setupMenuBar(QMenuBar *menuBar, const QStringList &containers)
{
	if (menuBar)
		menuBar->clear();
	else
		menuBar = new QMenuBar( 0 );

	foreach(QString container, containers)
	{
		foreach( QAction *action, m_actionContainer[container].values() )
		{
			QMenu *menu = new QMenu(menuBar);
			if ( action )
			{
				menu->addMenu(menu);
			}
		}
	}

	return menuBar;
}

QToolBar *DActionManager::setupToolBar(QToolBar *toolBar, const QString &container)
{
	if (!toolBar)
		toolBar = new QToolBar();

	toolBar->clear();
	int i = 0;
	
	foreach( QAction *a, m_actionContainer[container] )
	{
		if (a)
		{
			toolBar->addAction( a );
		}
		i++;
	}

	if (m_actionContainer.count() == 0)
	{
		toolBar->hide();
	}
	else
	{
		toolBar->show();
	}
	
	return toolBar;
}


