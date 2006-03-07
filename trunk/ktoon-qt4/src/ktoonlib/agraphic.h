/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#ifndef AGRAPHIC_H
#define AGRAPHIC_H

/**
 * @file agraphic.h
 * @brief Include this file if you need the class AGraphic
 */

#include <QList>
#include <QPolygonF>
#include <QPen>
#include <QBrush>

#include <QPainterPath>

#include "dbrushadjuster.h"

/**
 * @if english
 * Class that represents a graphic
 * @elseif spanish
 * Clase que representa un grafico
 * @endif
 * @author David Cuadrado <krawek@toonka.com>
*/
class AGraphic
{
	public:
		/**
		 * Default Constructor
		 */
		AGraphic();
		/**
		 * Copy Constructor
		 * @param toCopy 
		 */
		AGraphic(const AGraphic &toCopy);
		
		/**
		 * Destructor
		 * @return 
		 */
		~AGraphic();
		
		/**
		 * Map the graphic path
		 */
		void mapTo(const QMatrix& matix);
		
		QPainterPath path;
		QBrush brush;
		QPen pen;
};

#endif
