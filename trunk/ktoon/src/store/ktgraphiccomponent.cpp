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

#include "ktgraphiccomponent.h"
#include "ddebug.h"
#include <cmath> // fabs

#include <QPainter>

KTGraphicComponent::KTGraphicComponent() : KTSerializableObject(), m_scale(1,1), m_shear(0,0), m_angle(0), m_selected(false)
{
	
}

KTGraphicComponent::KTGraphicComponent(const KTGraphicComponent &toCopy) : KTSerializableObject(toCopy.parent()), m_name(toCopy.m_name), m_scale( toCopy.m_scale), m_shear(toCopy.m_shear), m_angle(toCopy.m_angle), m_selectPoints(toCopy.m_selectPoints), m_selected(toCopy.m_selected)
{
	foreach(KTGraphicElement *graphic, toCopy.m_graphics)
	{
		m_graphics << new KTGraphicElement(*graphic);
	}
	
	foreach(KTGraphicComponent *child, toCopy.m_childs )
	{
		m_childs << new KTGraphicComponent(*child);
	}
}

KTGraphicComponent::~KTGraphicComponent()
{
	qDeleteAll(m_graphics.begin(), m_graphics.end());
	qDeleteAll(m_childs.begin(), m_childs.end());
}


QRectF KTGraphicComponent::boundingRect() const
{
	QRectF r(0,0,0,0);
	
	foreach(KTGraphicElement *graphic, m_graphics)
	{
		r = r | graphic->path.boundingRect();
	}
	if(m_childs.count() > 0)
	{
		foreach(KTGraphicComponent *child, m_childs)
		{
			r = r | child->boundingRect();
		}
	}
	
	return r;
}

QPointF KTGraphicComponent::position() const
{
	return boundingRect().topLeft();
}

bool KTGraphicComponent::isValid()
{
	return (!m_graphics.isEmpty() || !m_childs.isEmpty());
}

void KTGraphicComponent::addGraphic(const QPainterPath &path, const QPen &pen, const QBrush &brush, const QPixmap &pix )
{
	KTGraphicElement *graphic = new KTGraphicElement;
	graphic->path = path;
	graphic->brush = brush;
	graphic->pen = pen;
// 	graphic->pixmap = pix;
// 	graphic->m_origPixmap = pix;
	graphic->setPixmap( pix );
	m_graphics << graphic;
}

void KTGraphicComponent::addGraphic(const QList<QPolygonF> &polygons, const QPen &pen, const QBrush &brush, const QPixmap &pix )
{
	QPainterPath path;
	
	foreach(QPolygonF pol, polygons)
	{
		path.addPolygon(pol);
	}
	
	addGraphic( path, pen, brush);
}

Graphics KTGraphicComponent::graphics() const
{
	return m_graphics;
}

bool KTGraphicComponent::intersects(const QRectF &rect)
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

bool KTGraphicComponent::contains(const QRectF &rect)
{
	if ( isValid() )
	{
		QPainterPath path;
		getPath( path );
		
		return path.contains(rect);
	}
	
	return false;
}

void KTGraphicComponent::scale(double sX, double sY)
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

void KTGraphicComponent::shear(double sX, double sY)
{
	QPointF delta( sX-m_shear.x(), sY-m_shear.y());
	QPointF pos = position();
	
	QMatrix mId(1,0,0,1, 0, 0);
	mId.shear(delta.x(), delta.y() );
	mapTo(mId);
	
	m_shear.setX(sX);
	m_shear.setY(sY);
	
	
	translate( pos.x(), pos.y());
}

void KTGraphicComponent::translate(double sX, double sY)
{
	QPointF position = boundingRect().topLeft();
	QMatrix mId;
	mId.translate(sX - position.x(), sY - position.y());
	mapTo(mId);
}

void KTGraphicComponent::rotate( double angle )
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

void KTGraphicComponent::adjustToRect(QRect rect, float offset)
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
	
	m_scale = QPointF(factor, factor);
	mapTo(matrix);
	matrix.reset();
	
	QPointF pos = boundingRect().topLeft();
	
	float tx = offset/2-pos.x(), ty = offset/2-pos.y();
	
	matrix.translate(tx, ty);
	mapTo(matrix);
}

void KTGraphicComponent::mapTo(const QMatrix& matrix)
{
	QMatrix orig;
	
	orig.scale(m_scale.x(), m_scale.y());
	orig.shear(m_shear.x(), m_shear.y());
	orig.rotate(m_angle);
	
	foreach(KTGraphicElement *graphic, m_graphics)
	{
		graphic->mapTo(matrix);
		graphic->mapPixmap(orig);
	}
	
	if(m_childs.count() > 0)
	{
		foreach(KTGraphicComponent *child, m_childs)
		{
			child->mapTo(matrix);
		}
	}
	if(!m_selectPoints.isEmpty())
	{
		m_selectPoints = matrix.map(m_selectPoints);
	}
}

void KTGraphicComponent::getPath(QPainterPath & path, const QMatrix& matrix)
{
	foreach(KTGraphicElement *graphic, m_graphics)
	{
		graphic->mapTo(matrix);
		path.addPath(graphic->path);
	}
	if(m_childs.count() > 0)
	{
		foreach(KTGraphicComponent *child, m_childs)
		{
			child->getPath(path, matrix);
		}
	}
}

void KTGraphicComponent::flip(Qt::Orientation o, const QPointF & pos)
{
	foreach(KTGraphicElement *graphic, m_graphics)
	{
		graphic->flip(o, pos);
	}
	if(m_childs.count() > 0)
	{
		foreach(KTGraphicComponent *child, m_childs)
		{
			child->flip(o, pos);
		}
	}
}

QDomElement KTGraphicComponent::createXML( QDomDocument &doc )
{
	QDomElement item = doc.createElement("Component");
	
	if ( ! m_name.isNull() )
	{
		item.setAttribute("name", m_name);
	}
	
	foreach(KTGraphicElement *graphic, m_graphics)
	{
		QDomElement graphicElement = doc.createElement("Graphic");
		
		QList<QPolygonF> polygons = graphic->path.toSubpathPolygons();
		
		QList<QPolygonF>::ConstIterator polygonIt = polygons.begin();
		
		QDomElement resourcesElement = doc.createElement("Resources");
		
		if ( !graphic->pixmap.isNull() )
		{
			QDomElement imageRes = doc.createElement("Image");
			imageRes.setAttribute("path", graphic->pixmapHash() );

			imageRes.setAttribute("scaleX", m_scale.x() );
			imageRes.setAttribute("scaleY", m_scale.y() );

			imageRes.setAttribute("shearX", m_shear.x() );
			imageRes.setAttribute("shearY", m_shear.y() );
			
			imageRes.setAttribute("rotate", m_angle );
			
			resourcesElement.appendChild(imageRes);
		}
		
		graphicElement.appendChild(resourcesElement);
		
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
	
	foreach(KTGraphicComponent *child, m_childs)
	{
		item.appendChild(child->createXML(doc));
	}
	
	return item;
}

void KTGraphicComponent::setComponentName(const QString &name)
{
	m_name = name;
}

QString KTGraphicComponent::componentName() const
{
	return m_name;
}

QDomElement KTGraphicComponent::brushToElement(const QBrush &brush, QDomDocument &doc)
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

void KTGraphicComponent::addChild ( KTGraphicComponent * child )
{
	m_childs << child;
	
}

void KTGraphicComponent::removeChild( KTGraphicComponent * child )
{
	m_childs.removeAll(child);
}

QList<KTGraphicComponent*> KTGraphicComponent::childs() const
{
	return m_childs;
}

bool KTGraphicComponent::hasChilds()
{
	return !m_childs.isEmpty();
}


QList<KTGraphicComponent*> KTGraphicComponent::allChilds() const
{
	QList<KTGraphicComponent*> m_allChilds;
	foreach(KTGraphicComponent *component, m_childs)
	{
		m_allChilds << component;
		if ( component->hasChilds() )
		{
			appendChilds(component, m_allChilds );
		}
	}
	
	return m_allChilds;
}

void KTGraphicComponent::appendChilds(KTGraphicComponent *component, QList<KTGraphicComponent *> &childs) const
{
	foreach(KTGraphicComponent *child, component->childs())
	{
		childs << child;
		if ( child->hasChilds() )
		{
			appendChilds(child, childs );
		}
	}
}

QPolygonF KTGraphicComponent::controlPoints() const
{
	return m_selectPoints;
}

void KTGraphicComponent::setControlPoints(const QPolygonF& points) 
{
	m_selectPoints = points;
}

void KTGraphicComponent::removeControlPoints()
{
	m_selectPoints.clear();
	if( m_childs.count() > 0  )
	{
		foreach(KTGraphicComponent * child, m_childs)
		{
			if(!child->controlPoints().isEmpty())
			{
				child->removeControlPoints();
			}
		}
	}
}

void KTGraphicComponent::copyAttributes(const KTGraphicComponent *other)
{
	m_name = other->componentName();
	m_scale = other->scaleFactor();
	m_shear = other->shearFactor();
	m_angle = other->angleFactor();
	
	m_selectPoints = other->controlPoints();
	
	qDeleteAll(m_childs.begin(), m_childs.end());
	qDeleteAll(m_graphics.begin(), m_graphics.end());
	
	m_graphics.clear();
	m_childs.clear();
	
	foreach(KTGraphicElement *graphic, other->graphics() )
	{
		m_graphics << new KTGraphicElement(*graphic);
	}
	
	foreach(KTGraphicComponent *child, other->childs() )
	{
		m_childs << new KTGraphicComponent(*child);
	}
}

QPointF KTGraphicComponent::scaleFactor() const
{
	return m_scale;
}

QPointF KTGraphicComponent::shearFactor() const
{
	return m_shear;
}

int KTGraphicComponent::angleFactor() const
{
	return m_angle;
}

void KTGraphicComponent::draw(QPainter *painter)
{
	painter->save();
	
	foreach(KTGraphicElement *graphic, graphics())
	{
		QPen pen = graphic->pen;
		QBrush brush = graphic->brush;
		
		painter->setPen(pen);
		
		painter->setBrush(brush);
		
		if ( !graphic->pixmap.isNull() )
		{
			painter->drawPixmap(graphic->path.boundingRect().topLeft(), graphic->pixmap);
		}
		
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
	
	foreach(KTGraphicComponent *child, childs())
	{
		child->draw(painter);
	}
	
	painter->restore();
}

QPointF KTGraphicComponent::currentPosition() const
{
	return boundingRect().topLeft();
}

//FIXME
void KTGraphicComponent::setSelected(bool select)
{
	m_selected = select;
// 	if(!m_selected)
// 	{
		removeControlPoints();
// 	}
// 	else if(m_selectPoints.isEmpty())
// 	{
	QPolygonF points;
	QRectF rect = boundingRect();
	points  << rect.topLeft() 
		<< QPointF(rect.x()+rect.width()/2, rect.y()) 
		<< rect.topRight() 
		<< QPointF(rect.x()+rect.width(), rect.y()+rect.height()/2) 
		<< rect.bottomRight() 
		<< QPointF(rect.x()+rect.width()/2, rect.y()+rect.height() ) 
		<< rect.bottomLeft() 
		<< QPointF(rect.x(), rect.y()+rect.height()/2 ) 
		<< rect.center();
	setControlPoints(points);
// 	}
}
bool KTGraphicComponent::selected() const
{
	return m_selected;
}

void KTGraphicComponent::saveResources(const QString &path)
{
	foreach(KTGraphicElement *graphic, graphics())
	{
		graphic->savePixmap(path);
	}
}






