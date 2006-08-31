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
#ifndef KTITEMCONVERTER_H
#define KTITEMCONVERTER_H

#include <QAbstractGraphicsShapeItem>

class KTPathItem;
class KTEllipseItem;
class KTRectItem;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class KTItemConverter
{
	private:
		KTItemConverter();
		
	public:
		~KTItemConverter();
		
		static void copyProperties(QGraphicsItem *src, QGraphicsItem *dest);
		
		static KTPathItem *convertToPath(QGraphicsItem *item);
		static KTEllipseItem *convertToEllipse(QGraphicsItem *item);
		static KTRectItem *convertToRect(QGraphicsItem *item);
		
};

#endif
