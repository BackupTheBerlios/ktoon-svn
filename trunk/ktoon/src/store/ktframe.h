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

#ifndef KTFRAME_H
#define KTFRAME_H

#include <QGraphicsScene>

class KTFrame;

/**
 * @brief Esta clase representa un marco o frame de la animacion
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTFrame : public QGraphicsScene
{
	public:
		/**
		 * Constructor por defecto
		 */
		KTFrame(QObject *parent = 0);
		
		/**
		 * Construye un frame con un nombre
		 */
		KTFrame(const QString &frameName, QObject * parent = 0);
		
		/**
		 * Destructor
		 */
		~KTFrame();
		
		/**
		 * Pone el nombre del frame
		 */
		void setFrameName(const QString &name);
		
		/**
		 * Bloquea el frame
		 */
		void setLocked(bool isLocked);
		
		/**
		 * Retorna el nombre del frame
		 */
		QString frameName() const;
		
		/**
		 * Returna verdadero cuando el frame esta bloqueado
		 */
		bool isLocked();
		
	private:
		void init();
		QString m_name;
		bool m_isLocked;
};

#endif
