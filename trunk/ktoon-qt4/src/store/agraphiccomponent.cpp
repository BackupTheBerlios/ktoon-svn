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
#include "ddebug.h"
#include <cmath> // fabs

#include <QPainter>

AGraphicComponent::AGraphicComponent() : KTSerializableObject(), m_scale(1,1), m_shear(0,0), m_angle(0)
{
	
}

AGraphicComponent::AGraphicComponent(const AGraphicComponent &toCopy) : KTSerializableObject(toCopy.parent()), m_name(toCopy.m_name), m_scale( toCopy.m_scale), m_shear(toCopy.m_shear), m_angle(toCopy.m_angle), m_controlPoints(toCopy.m_controlPoints)
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
	qDeleteAll(m_graphics.begin(), m_graphics.end());
	qDeleteAll(m_childs.begin(), m_childs.end());
}


QRectF AGraphicComponent::boundingRect() const
{
	QRectF r(0,0,0,0);
	
	foreach(AGraphic *graphic, m_graphics)
	{
		r = r | graphic->path.boundingRect();
	}
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
		QPainterPath path;
		getPath( path );
		
// 		QRectF r = boundingRect();
		return path.intersects(rect);
	}
	
	return false;
}

bool AGraphicComponent::contains(const QRectF &rect)
{
	if ( isValid() )
	{
		QPainterPath path;
		getPath( path );
		
		return path.contains(rect);
	}
	
	return false;
}

void AGraphicComponent::scale(double sX, double sY)
{
	QPointF delta( sX/m_scale.x(), sY/m_scale.y());
	if ( delta.x() > 0 && delta.y() > 0 )
	{
		QPointF pos = position();
		QMatrix mId(1,0,0,1, 0, 0);
		mId = mId.scale( delta.x(),  delta.y());
		mapTo(mId);
		
		m_scale.setX(sX);
		m_scale.setY(sY);
		
		translate( pos.x(), pos.y());
	}
}

void AGraphicComponent::shear(double sX, double sY)
{
	QPointF delta( sX-m_shear.x(), sY-m_shear.y());
	QPointF pos = position();
	
	
	QMatrix mId(1,0,0,1, 0, 0);
	mId.shear(delta.x()  , delta.y() );
	mapTo(mId);
	
	m_shear.setX(sX);
	m_shear.setY(sY);
	
	
	translate( pos.x(), pos.y());
}

void AGraphicComponent::translate(double sX, double sY)
{
	QPointF position = boundingRect().topLeft();
	QMatrix mId;
	mId.translate(sX - position.x(), sY - position.y());
	mapTo(mId);
}

void AGraphicComponent::rotate( double angle )
{
	QPointF pos = boundingRect().center();
	QMatrix mId;
	QMatrix origin;
	origin.translate(-pos.x(), -pos.y());
	QMatrix rotate;
	rotate.rotate(-(m_angle-angle));
	QMatrix position;
	position.translate(pos.x(), pos.y());
	mId = origin * rotate * position;
	mapTo( mId );
	m_angle = (int)angle;
}

void AGraphicComponent::adjustToRect(QRect rect, float offset)
{
	QRectF br = boundingRect();
	QMatrix matrix;
	
	float sx = 1, sy = 1;
// 	if ( rect.width() < br.width() )
// 	{
		sx = static_cast<float>(rect.width()-offset) / static_cast<float>(br.width());
// 	}
// 	if ( rect.height() < br.height() )
// 	{
		sy = static_cast<float>(rect.height()-offset) / static_cast<float>(br.height());
// 	}
	
	float factor = qMin(sx, sy);
	matrix.scale(factor, factor);
	mapTo(matrix);
	matrix.reset();
	
	QPointF pos = boundingRect().topLeft();
	
	float tx = offset/2-pos.x(), ty = offset/2-pos.y();
	
	matrix.translate(tx, ty);
	mapTo(matrix);
}

void AGraphicComponent::mapTo(const QMatrix& matrix)
{
	foreach(AGraphic *graphic, m_graphics)
	{
		graphic->mapTo(matrix);
	}
	
	if(m_childs.count() > 0)
	{
		foreach(AGraphicComponent *child, m_childs)
		{
			child->mapTo(matrix);
		}
	}
	if(!m_controlPoints.isEmpty())
	{
		m_controlPoints = matrix.map(m_controlPoints);
	}
}

void AGraphicComponent::getPath(QPainterPath & path, const QMatrix& matrix)
{
	foreach(AGraphic *graphic, m_graphics)
	{
		graphic->mapTo(matrix);
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

void AGraphicComponent::flip(Qt::Orientation o)
{
	foreach(AGraphic *graphic, m_graphics)
	{
		graphic->flip(o);
	}
	if(m_childs.count() > 0)
	{
		foreach(AGraphicComponent *child, m_childs)
		{
			child->flip(o);
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
		
		element.setAttribute("spread", gradient->spread() );
		
		switch(gradient->type() )
		{
			case QGradient::LinearGradient:
			{
				element.setAttribute("startX", static_cast<const QLinearGradient *>(gradient)->start().x() );
				element.setAttribute("startY", static_cast<const QLinearGradient *>(gradient)->start().y() );
				
				element.setAttribute("finalX", static_cast<const QLinearGradient *>(gradient)->finalStop().x() );
				element.setAttribute("finalY", static_cast<const QLinearGradient *>(gradient)->finalStop().y() );
			}
			break;
			case QGradient::RadialGradient:
			{
				element.setAttribute("centerX", static_cast<const QRadialGradient *>(gradient)->center().x() );
				element.setAttribute("centerY", static_cast<const QRadialGradient *>(gradient)->center().y() );
				
				element.setAttribute("focalX", static_cast<const QRadialGradient *>(gradient)->focalPoint().x() );
				element.setAttribute("focalY", static_cast<const QRadialGradient *>(gradient)->focalPoint().y() );
				
				element.setAttribute("radius", static_cast<const QRadialGradient *>(gradient)->radius() );
			}
			break;
			case QGradient::ConicalGradient:
			{
				element.setAttribute("centerX", static_cast<const QRadialGradient *>(gradient)->center().x() );
				element.setAttribute("centerY", static_cast<const QRadialGradient *>(gradient)->center().y() );
				
				element.setAttribute("angle", static_cast<const QConicalGradient *>(gradient)->angle() );
			}
			break;
			case QGradient::NoGradient:
			{
				
			}
			break;
		}
	
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

void AGraphicComponent::removeChild( AGraphicComponent * child )
{
	m_childs.removeAll(child);
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

void AGraphicComponent::copyAttributes(const AGraphicComponent *other)
{
	m_name = other->componentName();
	m_scale = other->scaleFactor();
	m_shear = other->shearFactor();
	m_angle = other->angleFactor();
	
	m_controlPoints = other->controlPoints();
	
	qDeleteAll(m_childs.begin(), m_childs.end());
	qDeleteAll(m_graphics.begin(), m_graphics.end());
	
	m_graphics.clear();
	m_childs.clear();
	
	foreach(AGraphic *graphic, other->graphics() )
	{
		m_graphics << new AGraphic(*graphic);
	}
	
	foreach(AGraphicComponent *child, other->childs() )
	{
		m_childs << new AGraphicComponent(*child);
	}
}

QPointF AGraphicComponent::scaleFactor() const
{
	return m_scale;
}

QPointF AGraphicComponent::shearFactor() const
{
	return m_shear;
}

int AGraphicComponent::angleFactor() const
{
	return m_angle;
}

void AGraphicComponent::draw(QPainter *painter)
{
	painter->save();

	foreach(AGraphic *graphic, graphics())
	{
		QPen pen = graphic->pen;
		QBrush brush = graphic->brush;
		
		painter->setPen(pen);
		painter->setBrush(brush);
		
		QList<QPolygonF> poligons = graphic->path.toSubpathPolygons();
		
		if ( poligons.count() == 1 )
		{
			painter->drawPath(graphic->path);
		}
		else
		{
			QList<QPolygonF>::const_iterator it;
			for(it = poligons.begin(); it != poligons.end(); ++it)
			{
				painter->drawPolygon(*it);
			}
		}
	}
	
	foreach(AGraphicComponent *child, childs())
	{
		child->draw(painter);
	}
	
	painter->restore();
}

QPointF AGraphicComponent::currentPosition() const
{
	return boundingRect().topLeft();
}
