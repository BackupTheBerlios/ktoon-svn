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

#ifndef DACTIONMANAGER_H
#define DACTIONMANAGER_H

#include <QObject>
#include <QWidget>
#include <QList>
#include <QHash>

#include "daction.h"

typedef QList<DAction *> QActionList;
typedef QHash<QString, DAction *> QActionDict;

/**
 * @short la clase DActionManager provee de un manejador de acciones, este manejador facilita el acceso y ordenamiento a las acciones contieniendo todas las acciones de la aplicacion.
 * @author David Cuadrado <krawek@toonka.com>
*/

class DActionManager : public QObject
{
	Q_OBJECT

	public:
		/**
		 * Construye un manejador de acciones.
		 * @param parent widget que contine el manejador de acciones
		 */
		DActionManager(QWidget *parent = 0L);
		/**
		 * Destructor
		 */
		~DActionManager();

		/**
		 * Inserta una accion al manejador
		 * @param action accion para añadir
		 * @return 
		 */
		bool insert(DAction *action);
		/**
		 * Remueve una accion del manejador
		 * @param action para remover
		 */
		void remove( DAction* action );
		/**
		 * Remuve una accion del manejador retornando dicha accion.
		 * @param action para remover
		 * @return la accion removida o cero si esta no estaba en el manejador
		 */
		QAction *take( DAction* action );
		/**
		 * Busca una accion en el manejardor.
		 * @param id asociado a la accion
		 * @return la accion requeriada
		 */
		QAction *find(const QString &id) const;
		/**
		 * Retorna la accion asociada a id
		 * @param id 
		 */
		QAction *operator[](const QString &id) const;

	private:
		QActionDict m_actionDict;
};

#endif
