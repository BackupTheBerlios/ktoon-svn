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

#include <QObject>
#include <QWidget>
#include <QList>
#include <QHash>

#include "ktaction.h"

typedef QList<KTAction *> QActionList;
typedef QHash<QString, KTAction *> QActionDict;

/**
 * @short la clase KTActionManager provee de un manejador de acciones, este manejador facilita el acceso y ordenamiento a las acciones contieniendo todas las acciones de la aplicacion.
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTActionManager : public QObject
{
	Q_OBJECT

	public:
		/**
		 * Construye un manejador de acciones.
		 * @param parent widget que contine el manejador de acciones
		 */
		KTActionManager(QWidget *parent = 0L);
		/**
		 * Destructor
		 */
		~KTActionManager();
		/**
		 * 
		 * @param w 
		 */
		void setWidget(QWidget *w);
		/**
		 * Inserta una accion al manejador
		 * @param action accion para añadir
		 * @return 
		 */
		bool insert(KTAction *action);
		/**
		 * Remueve una accion del manejador
		 * @param action para remover
		 */
		void remove( KTAction* action );
		/**
		 * Remuve una accion del manejador retornando dicha accion.
		 * @param action para remover
		 * @return la accion removida o cero si esta no estaba en el manejador
		 */
		QAction *take( KTAction* action );
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
		QWidget *m_widget;
		QActionDict m_actionDict;
};

#endif
