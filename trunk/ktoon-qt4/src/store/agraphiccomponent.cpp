/***************************************************************************
 *   Copyright (C) 2004 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                    *
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

#include "agraphiccomponent.h"
#include "ktdebug.h"

AGraphicComponent::AGraphicComponent() : KTSerializableObject(), m_pPath(), m_pColor(Qt::black), m_pPen(m_pColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
{
}

AGraphicComponent::AGraphicComponent(const AGraphicComponent &toCopy) : KTSerializableObject(toCopy.parent()), m_pPath(toCopy.m_pPath), m_pColor(toCopy.m_pColor), m_pBrush(toCopy.m_pBrush), m_pPen(toCopy.m_pPen), m_name(toCopy.m_name), m_previousPath(toCopy.m_previousPath)
{
}

AGraphicComponent::~AGraphicComponent()
{
}

 
QRectF AGraphicComponent::boundingRect() const
{
	return m_pPath.boundingRect();
}

QPainterPath AGraphicComponent::path() const
{
	return m_pPath;
}

QBrush AGraphicComponent::brush() const
{
	return m_pBrush;
}

QPen AGraphicComponent::pen() const
{
	return m_pPen;
}

QColor AGraphicComponent::color() const
{
	return m_pColor;
}

void AGraphicComponent::setPath(const QPainterPath &path )
{
	m_pPath = path;
	m_previousPath = path;
}

void AGraphicComponent::setBrush(const QBrush &brush)
{
	m_pBrush = brush;
}

void AGraphicComponent::setPen(const QPen &pen)
{
	m_pPen = pen;
}

void AGraphicComponent::setPen(const QColor &color)
{
	m_pColor = color;
	m_pPen = QPen(color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
}

void AGraphicComponent::setColor(const QColor &color)
{
	m_pColor = color;
}

void AGraphicComponent::scale(double sX, double sY)
{
	if ( sX > 0 && sY > 0 )
	{
		QPointF position = m_pPath.currentPosition();
		QMatrix mId(1,0,0,1, 0, 0);
		
		mId.scale(sX, sY);
		
		m_pPath = mId.map(m_previousPath);
		translate( position.x(), position.y());
	}
}

void AGraphicComponent::shear(double sX, double sY)
{
	QPointF position = m_pPath.currentPosition();
	QMatrix mId(1,0,0,1, 0, 0);
	
	mId.shear(sX, sY);
	m_pPath = mId.map(m_previousPath);
	translate( position.x(), position.y());
}

void AGraphicComponent::translate(double sX, double sY)
{
	QPointF position = m_pPath.currentPosition();
	QMatrix mId(1,0,0,1, 0, 0);
	
	mId.translate(sX-position.x(), sY-position.y());
	m_pPath = mId.map(m_pPath);	
}

void AGraphicComponent::rotate( double angle )
{
	QPointF position = m_pPath.currentPosition();
	QMatrix mId(1,0,0,1, position.x(), position.y());
	
	mId.rotate(angle);
	
	m_pPath = mId.map(m_previousPath);
	
	translate( position.x(), position.y());
}


QDomElement AGraphicComponent::createXML( QDomDocument &doc )
{
	QDomElement item = doc.createElement("Component");
	
	if ( ! m_name.isNull() )
	{
		item.setAttribute("name", m_name);
	}
	
	QList<QPolygonF> polygons = m_pPath.toSubpathPolygons ();
	
	QList<QPolygonF>::ConstIterator polygonIt = polygons.begin();
	
	
	QDomElement propertiesElement = doc.createElement("Properties");
	QDomElement penElement = doc.createElement("Pen");
	penElement.setAttribute("width", m_pPen.widthF() );
	penElement.setAttribute("style", m_pPen.style() );
	penElement.setAttribute("capstyle", m_pPen.capStyle() );
	penElement.setAttribute("joinstyle", m_pPen.joinStyle() );
	
	penElement.appendChild( brushToElement( m_pPen.brush(), doc) );
	
	propertiesElement.appendChild(penElement);
	
	QDomElement brushElement = doc.createElement("Brush");
	brushElement.setAttribute("style", m_pBrush.style() );
	
	brushElement.appendChild(brushToElement( m_pBrush, doc) );
	
	propertiesElement.appendChild(brushElement);
	item.appendChild(propertiesElement);
	
	while ( polygonIt != polygons.end() )
	{
		QDomElement polygonElement = doc.createElement("Polygon");
		
		QPolygonF::ConstIterator pointIt = (*polygonIt).begin();
		
		QString attribute = "";
		while (pointIt != (*polygonIt).end() )
		{
			attribute += QString("%1:%2 ").arg((*pointIt).x()).arg((*pointIt).y());
			++pointIt;
		}
		polygonElement.setAttribute("points", attribute.trimmed());
		item.appendChild(polygonElement);
		
		++polygonIt;
	}
	
	return item;
}

void AGraphicComponent::setComponentName(const QString &name)
{
	m_name = name;
}

QString AGraphicComponent::componentName() const
{
	return m_name;
}

QDomElement AGraphicComponent::brushToElement(const QBrush &brush, QDomDocument &doc)
{
	QDomElement element;
	
	if ( brush.gradient() )
	{
		const QGradient *gradient = brush.gradient();
		element = doc.createElement("Gradient");
		
		element.setAttribute("type", gradient->type() );
	
		QGradientStops stops = gradient->stops();
	
		foreach(QGradientStop stop, stops)
		{
			QDomElement stopElement = doc.createElement("Stop");
			stopElement.setAttribute("value", stop.first );
			stopElement.setAttribute("colorName", stop.second.name());
			stopElement.setAttribute("alpha", stop.second.alpha());
		
			element.appendChild(stopElement);
		}
	}
	else
	{
		element = doc.createElement("Color");
		element.setAttribute("colorName", brush.color().name());
		element.setAttribute("alpha", QString::number(brush.color().alpha()) );
	}
	
	return element;
}




