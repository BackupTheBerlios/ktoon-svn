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

AGraphicComponent::AGraphicComponent() : KTSerializableObject()
{
}

AGraphicComponent::AGraphicComponent(const AGraphicComponent &toCopy) : KTSerializableObject(toCopy.parent()), m_name(toCopy.m_name)
{
	foreach(AGraphic *graphic, toCopy.m_graphics)
	{
		m_graphics << new AGraphic(*graphic);
	}
	
	foreach(AGraphicComponent *child, toCopy.m_childs )
	{
		m_childs << new AGraphicComponent(*child);
	}
}

AGraphicComponent::~AGraphicComponent()
{
	foreach(AGraphic *graphic, m_graphics)
	{
		if ( graphic )
			delete graphic;
	}
}


QRectF AGraphicComponent::boundingRect() const
{
	QRectF r;
	
	foreach(AGraphic *graphic, m_graphics)
	{
		r = r | graphic->path.boundingRect();
	}
// 	SHOW_VAR(m_childs.count());
	if(m_childs.count() > 0)
	{
		foreach(AGraphicComponent *child, m_childs)
		{
			r = r | child->boundingRect();
		}
	}
	
	return r;
}

QPointF AGraphicComponent::position() const
{
	return boundingRect().topLeft();
}

bool AGraphicComponent::isValid()
{
	return (!m_graphics.isEmpty() || !m_childs.isEmpty());
}

// void AGraphicComponent::addGraphic(const AGraphic *graphic)
// {
// 	m_graphics << new AGraphic(*graphic);
// }

void AGraphicComponent::addGraphic(const QPainterPath &path, const QPen &pen, const QBrush &brush )
{
	AGraphic *graphic = new AGraphic;
	graphic->path = path;
	graphic->brush = brush;
	graphic->pen = pen;
	m_graphics << graphic;
}

void AGraphicComponent::addGraphic(const QList<QPolygonF> &polygons, const QPen &pen, const QBrush &brush )
{
	AGraphic *graphic = new AGraphic;
	
	QPainterPath path;
	
	foreach(QPolygonF pol, polygons)
	{
		path.addPolygon(pol);
	}
	
	graphic->path = path;
	graphic->brush = brush;
	graphic->pen = pen;
	m_graphics << graphic;
}

Graphics AGraphicComponent::graphics() const
{
	return m_graphics;
}

bool AGraphicComponent::intersects(const QRectF &rect)
{
	if ( isValid() )
	{
// 		QRectF r = m_graphics[0].path.boundingRect();
// 		
// 		for(int i = 1; i < m_graphics.count(); i++ )
// 		{
// 			r = r | m_graphics[i].path.boundingRect();
// 		}
		QRectF r = boundingRect();
		return r.intersects(rect);
	}
	
	return false;
}


void AGraphicComponent::scale(double sX, double sY) //FIXME
{
	
	if ( sX > 0 && sY > 0 )
	{
		QPointF pos = position();
		QMatrix mId(1,0,0,1, 0, 0);
		mId.scale(sX, sY);
		foreach(AGraphic *graphic, m_graphics)
		{
			graphic->path = mId.map(graphic->path);
		}
		if(m_childs.count() > 0)
		{
			foreach(AGraphicComponent *child, m_childs)
			{
				child->mapTo(mId);
// // 			
			}
		}
		translate( pos.x(), pos.y());
	}

}

void AGraphicComponent::shear(double sX, double sY)//FIXME
{
	
	QPointF pos = position();

	QMatrix mId(1,0,0,1, 0, 0);
	mId.shear(sX, sY);
	
	foreach(AGraphic *graphic, m_graphics)
	{
		graphic->path = mId.map(graphic->path);
	}
	if(m_childs.count() > 0)
	{
		foreach(AGraphicComponent *child, m_childs)
		{
			child->mapTo(mId);
		}
	}
	translate( pos.x(), pos.y());
}

void AGraphicComponent::translate(double sX, double sY)
{

	QPointF position = boundingRect().topLeft();
	QMatrix mId;
	mId.translate(sX - position.x(), sY - position.y());
	foreach(AGraphic *graphic, m_graphics)
	{
		
		QPointF position = graphic->path.currentPosition();
		graphic->path = mId.map(graphic->path);
	}
	if(m_childs.count() > 0)
	{
		foreach(AGraphicComponent *child, m_childs)
		{
			child->mapTo(mId);
		}
	}
}



void AGraphicComponent::rotate( double angle )//FIXME
{
	QPointF pos = position();
	QMatrix mId(1,0,0,1, position().x(), position().y());
	mId.rotate(angle);
	foreach(AGraphic *graphic, m_graphics)
	{
		graphic->path = mId.map(graphic->path);
	}
	if(m_childs.count() > 0)
	{
		foreach(AGraphicComponent *child, m_childs)
		{
			child->mapTo(mId);
		}
	}
	translate( pos.x(), pos.y());
}

void AGraphicComponent::mapTo(const QMatrix& matrix)
{
	foreach(AGraphic *graphic, m_graphics)
	{
		graphic->path = matrix.map(graphic->path);
	}
	if(m_childs.count() > 0)
	{
		foreach(AGraphicComponent *child, m_childs)
		{
			child->mapTo(matrix);
		}
	}
}

void AGraphicComponent::getPath(QPainterPath & path, const QMatrix& matrix)
{
	foreach(AGraphic *graphic, m_graphics)
	{
		graphic->path = matrix.map(graphic->path);
		path.addPath(graphic->path);
	}
	if(m_childs.count() > 0)
	{
		foreach(AGraphicComponent *child, m_childs)
		{
			child->getPath(path, matrix);
		}
	}
}

QDomElement AGraphicComponent::createXML( QDomDocument &doc )
{
	QDomElement item = doc.createElement("Component");
	
	if ( ! m_name.isNull() )
	{
		item.setAttribute("name", m_name);
	}
	
	foreach(AGraphic *graphic, m_graphics)
	{
		QDomElement graphicElement = doc.createElement("Graphic");
		
		QList<QPolygonF> polygons = graphic->path.toSubpathPolygons();
		
		QList<QPolygonF>::ConstIterator polygonIt = polygons.begin();
		
		
		QDomElement propertiesElement = doc.createElement("Properties");
		QDomElement penElement = doc.createElement("Pen");
		penElement.setAttribute("width", graphic->pen.widthF() );
		penElement.setAttribute("style", graphic->pen.style() );
		penElement.setAttribute("capstyle", graphic->pen.capStyle() );
		penElement.setAttribute("joinstyle", graphic->pen.joinStyle() );
		
		penElement.appendChild( brushToElement( graphic->pen.brush(), doc) );
		
		propertiesElement.appendChild(penElement);
		
		QDomElement brushElement = doc.createElement("Brush");
		brushElement.setAttribute("style", graphic->brush.style() );
		
		brushElement.appendChild(brushToElement( graphic->brush, doc) );
		
		propertiesElement.appendChild(brushElement);
		graphicElement.appendChild(propertiesElement);
		
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
			graphicElement.appendChild(polygonElement);
			
			++polygonIt;
		}
		
		item.appendChild(graphicElement);
	}
	
	foreach(AGraphicComponent *child, m_childs)
	{
		item.appendChild(child->createXML(doc));
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

void AGraphicComponent::addChild ( AGraphicComponent * child )
{
	m_childs << child;
	
}

QList<AGraphicComponent*> AGraphicComponent::childs() const
{
	return m_childs;
}

bool AGraphicComponent::hasChilds()
{
	return !m_childs.isEmpty();
}


QList<AGraphicComponent*> AGraphicComponent::allChilds() const
{
	QList<AGraphicComponent*> m_allChilds;
	foreach(AGraphicComponent *component, m_childs)
	{
		
		m_allChilds << component;
		if ( component->hasChilds() )
		{
			appendChilds(component, m_allChilds );
		}
	}
	
	return m_allChilds;
}

void AGraphicComponent::appendChilds(AGraphicComponent *component, QList<AGraphicComponent *> &childs) const
{	
	foreach(AGraphicComponent *child, component->childs())
	{
		childs << child;
		if ( child->hasChilds() )
		{
			appendChilds(child, childs );
		}
	}
}

QPolygonF AGraphicComponent::controlPoints() const
{
	return m_controlPoints;
}

void AGraphicComponent::setControlPoints(const QPolygonF& points) 
{
	m_controlPoints = points;
}

void AGraphicComponent::removeControlPoints()
{
	m_controlPoints.clear();
	if( m_childs.count() > 0  )
	{
		foreach(AGraphicComponent * child, m_childs)
		{
			if(!child->controlPoints().isEmpty())
			{
				child->removeControlPoints();
			}
		}
	}
}
