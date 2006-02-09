/***************************************************************************
 *   Copyright (C) 2004 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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

#ifndef AGRAPHICCOMPONENT_H
#define AGRAPHICCOMPONENT_H

#include <QObject>
#include <QPainterPath>
#include <QBrush>
#include <QPen>

#include <QDomElement>
#include <QDomDocument>

#include <QPointF>
#include "ktserializableobject.h"

#include "agraphic.h"

/**
 * @short Abstraction of graphic component
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/

typedef QList<AGraphic *> Graphics;

class AGraphicComponent;
class AGraphicComponent : public KTSerializableObject
{
	Q_OBJECT
	public:
		/**
		 * Default constructor
		 */
		AGraphicComponent();
		
		/**
		 * Copy constructor
		 */
		AGraphicComponent(const AGraphicComponent &);
		
		/**
		 * Destructor
		 */
		virtual ~AGraphicComponent();
		
		/**
		 * Reimplemented from KTSerializableObject
		 */
		QDomElement createXML( QDomDocument &doc );
		
		/**
		 * returns the bounding rect
		 */
		QRectF boundingRect() const;
		
		/**
		 * Returns the current position
		 */
		QPointF position() const;
		
		/**
		 * Return true when graphic is valid
		 */
		bool isValid();
		
		/**
		 * Add a new graphic
		 */
		void addGraphic(const QPainterPath &path, const QPen &pen, const QBrush &brush );
		
		/**
		 * Add a new graphic
		 */
		void addGraphic(const QList<QPolygonF> &polygons, const QPen &pen, const QBrush &brush );
		
		/**
		 * return all graphics in component
		 */
		Graphics graphics() const;
		
		/**
		 * Returns true when component intersects with rect
		 */
		bool intersects(const QRectF &rect);
		
		/**
		 * Scale the component
		 */
		void scale(double sX, double sY);
		
		/**
		 * Shears the component
		 */
		void shear(double sX, double sY);
		
		/**
		 * Translate the component
		 */
		void translate(double sX, double sY);
		
		/**
		 * Rotate the component
		 */
		void rotate( double angle );
		
		/**
		 * adjust the component to rect 
		 */
		void adjustToRect(QRect rect, float offset);
		
		/**
		 * map the component to matrix
		 */
		void mapTo(const QMatrix& matrix);
		
		/**
		 * Returns all component path
		 */
		void getPath(QPainterPath & path, const QMatrix& matrix = QMatrix() );
		
		/**
		 * set the name to the component
		 */
		void setComponentName(const QString &name);
		
		/**
		 * Returns the component name
		 */
		QString componentName() const;
		
		
		/**
		 * Add a child
		 */
		void addChild ( AGraphicComponent * child );
		
		/**
		 * Return the childs
		 */
		QList<AGraphicComponent*> childs() const ;
		
		/**
		 * Returns true if component has childs.
		 */
		bool hasChilds();
		
		/**
		 * Returns the childs and child of childs...
		 */
		QList<AGraphicComponent*> allChilds() const;
		
		/**
		 * Returns graphic component control points
		 */
		QPolygonF controlPoints() const;
		
		/**
		 * set the control points
		 */
		void setControlPoints(const QPolygonF& points) ;
		
		/**
		 * Remove the control points
		 */
		void removeControlPoints();
		
		
		
	private:
		QDomElement brushToElement(const QBrush &brush, QDomDocument &doc);
		QList<AGraphicComponent*> m_childs;
	protected:
		QString m_name;
		Graphics m_graphics;
		QPolygonF m_controlPoints;
		QPointF m_scale, m_shear;
		int m_angle;
	private: // AUX FUNCTIONS
		void appendChilds(AGraphicComponent *component, QList<AGraphicComponent *> &childs) const;
		
};

#endif
