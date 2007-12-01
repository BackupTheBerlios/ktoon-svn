/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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

#ifndef KTPALETTEDOCUMENT_H
#define KTPALETTEDOCUMENT_H

#include <QDomDocument>
#include <QBrush>
#include <QList>
#include "ktglobal_store.h"

/**
 * Esta clase se utiliza para guardar las paletas
 * @brief Esta clase representa un documento de paletta
 * @author David Cuadrado
*/

class STORE_EXPORT KTPaletteDocument : public QDomDocument
{
	public:
		/**
		 * Construye un documento de paleta con un nombre
		 */
		KTPaletteDocument(const QString &name, bool isEditable);
		
		/**
		 * Destructor
		 */
		~KTPaletteDocument();
		
		/**
		 * Añade un gradiente al documento
		 */
		void addGradient(const QGradient &gradient);
		
		/**
		 * Añade un color al documento
		 */
		void addColor(const QColor &color);
		
		/**
		 * Pone una lista de colores o gradientes en el documento
		 */
		void setElements(const QList<QBrush > &colors);
};

#endif
