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

#include "ktserializableobject.h"

#include "agraphic.h"

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/

typedef QList<AGraphic *> Graphics;

class AGraphicComponent;
class AGraphicComponent : public KTSerializableObject
{
	Q_OBJECT
	public:
		AGraphicComponent();
		AGraphicComponent(const AGraphicComponent &);
		virtual ~AGraphicComponent();
		
		QDomElement createXML( QDomDocument &doc );
		
		QRectF boundingRect() const;
		QPointF position() const;
		
		bool isValid();
		
// 		void addGraphic(const AGraphic *graphic);
		void addGraphic(const QPainterPath &path, const QPen &pen, const QBrush &brush );
		void addGraphic(const QList<QPolygonF> &polygons, const QPen &pen, const QBrush &brush );
		
		Graphics graphics() const;
		
		bool intersects(const QRectF &rect);
		
		void scale(double sX, double sY);
		void shear(double sX, double sY);
		void translate(double sX, double sY);
		void rotate( double angle );
		
		void mapTo(const QMatrix& matrix);
		void getPath(QPainterPath & path, const QMatrix& matrix = QMatrix() );
		
		void setComponentName(const QString &name);
		QString componentName() const;
		
		void addChild ( AGraphicComponent * child );
		QList<AGraphicComponent*> childs() const ;
		bool hasChilds();
		
		QList<AGraphicComponent*> allChilds() const;
		
		
	private:
		QDomElement brushToElement(const QBrush &brush, QDomDocument &doc);
		QList<AGraphicComponent*> m_childs;
		
	protected:
		QString m_name;
		Graphics m_graphics;
		
	private: // AUX FUNCTIONS
		void appendChilds(AGraphicComponent *component, QList<AGraphicComponent *> &childs) const;
		
};

#endif
