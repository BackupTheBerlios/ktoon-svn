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
 * @file ktgraphicelement.h
 * @brief Include this file if you need the class KTGraphicElement
 */

#include <QList>
#include <QPolygonF>
#include <QPen>
#include <QBrush>
#include <QPixmap>

#include <QPainterPath>

#include "dcore/dbrushadjuster.h"
#include "ktglobal.h"

/**
 * @if english
 * Class that represents a graphic
 * @elseif spanish
 * Clase que representa un grafico
 * @endif
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTOON_EXPORT KTGraphicElement
{
	public:
		/**
		 * Default Constructor
		 */
		KTGraphicElement();
		/**
		 * Copy Constructor
		 * @param toCopy 
		 */
		KTGraphicElement(const KTGraphicElement &toCopy);
		
		/**
		 * Destructor
		 * @return 
		 */
		~KTGraphicElement();
		
		/**
		 * Map the graphic path
		 */
		void mapTo(const QMatrix& matix);
		
		void mapPixmap(const QMatrix &matrix);
		
		void flip(Qt::Orientation o);
		void flip(Qt::Orientation o, const QPointF &pos );
		
		QPainterPath path;
		QBrush brush;
		QPen pen;
		
		void setPixmap(const QPixmap &pix, const QString &key = QString());
		
		QPixmap pixmap;
		
		QString pixmapHash() const;
		void savePixmap(const QString &path);
		
	private:
		QPixmap m_origPixmap;
		QString m_pixmapHash;
};

#endif
