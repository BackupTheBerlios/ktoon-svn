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

#ifndef KTBRUSH_H
#define KTBRUSH_H

#include <QObject>
#include <QPainter>
#include <QBrush>
#include <QPen>

#include "ktserializableobject.h"

/**
 * Esta clase abstrae los componentes de una brocha
 * 
 * @brief Contiene los datos para utilizar una brocha con forma
 * @author David Cuadrado <krawek@toonka.com>
*/
class Q_DECL_EXPORT KTBrush : public KTSerializableObject
{
	public:
		/**
		 * Constructor por defecto
		 */
		KTBrush();
		
		/**
		 * Constructor de copia
		 */
		KTBrush(const KTBrush &toCopy);
		
		/**
		 * Destructor
		 */
		~KTBrush();
		
		/**
		 * Pone una brocha de Qt
		 */
		void setBrush(const QBrush &brush);
		
		/**
		 * Pone un lapicero de Qt
		 */
		void setPen(const QPen &pen);
				
		/**
		 * Pone una brocha de Qt al lapicero
		 */
		void setPenBrush(const QBrush &brush);
		
		/**
		 * Pone un ancho al lapicero
		 */
		void setPenWidth(double width);
		
		/**
		 * Retorna el ancho del lapicero
		 */
		double penWidth() const;
		
		/**
		 * Configura el painter con la brocha
		 */
		void setupPainter(QPainter *painter );
		
		/**
		 * Reimplementado de KTSerializableObject
		 */
		QDomElement createXML( QDomDocument &doc );
		
		
		QPen pen() const;
		QBrush brush() const;
		
	private:
		void setup();
		
	private:
		int m_thickness;
		QString m_brushName; // TODO: save me!! ;)
		
		QBrush m_brush;
		QPen m_pen;
		
		bool m_hasGradient;
};

#endif
