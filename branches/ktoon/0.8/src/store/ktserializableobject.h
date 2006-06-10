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

#ifndef KTSERIALIZABLEOBJECT_H
#define KTSERIALIZABLEOBJECT_H

#include <QObject>
#include <QDomDocument>
#include <QVariant>

/**
 * @brief Esta clase es una abstraccion de objetos serializables
 * @author David Cuadrado <krawek@toonka.com>
*/
class Q_DECL_EXPORT KTSerializableObject : public QObject
{
	public:
		/**
		 * Constructor por defecto
		 */
		KTSerializableObject(QObject *parent = 0);
		
		/**
		 * Destructor
		 */
		~KTSerializableObject();
		
		/**
		 * Funcion que debe ser reimplementada con el codigo para guardar el objeto
		 */
		virtual QDomElement createXML( QDomDocument &doc );
		
		virtual void saveResources(const QString &resourcesDir);
};

#endif
