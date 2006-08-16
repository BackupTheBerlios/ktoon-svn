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

#include "ktserializer.h"

#include <QGraphicsItem>

#include "ktsvg2qt.h"

KTSerializer::KTSerializer()
{
}


KTSerializer::~KTSerializer()
{
}

QDomElement KTSerializer::properties(const QGraphicsItem *item, QDomDocument &doc)
{
	QDomElement properties = doc.createElement("properties");
	
	QString strMatrix = "matrix(";
	QMatrix m = item->matrix();
	qreal a = m.m11();
	qreal b = m.m12();
	qreal c = m.m21();
	qreal d = m.m22();
	qreal e = m.dx();
	qreal f = m.dy();
	
	strMatrix += QString::number(a) + "," +QString::number(b) + "," + QString::number(c) + "," + QString::number(d) + "," + QString::number(e) + "," + QString::number(f) + ")" ; 
	
	properties.setAttribute( "transform", strMatrix);
	
	
	properties.setAttribute( "pos", "(" + QString::number(item->pos().x()) + "," + QString::number(item->pos().y()) + ")"  );
	
	properties.setAttribute( "enabled", item->isEnabled());
	
	properties.setAttribute( "flags", item->flags());
	
	properties.setAttribute( "selected", item->isSelected() );
	
	return properties;
}


void KTSerializer::loadProperties(QGraphicsItem *item, const QXmlAttributes &atts)
{
	QMatrix matrix;
	KTSvg2Qt::svgmatrix2qtmatrix( atts.value("transform"), matrix );
	item->setMatrix(matrix);
		
	QPointF pos;
	KTSvg2Qt::parsePointF(atts.value("pos"), pos );
	item->setPos( pos );
		
	item->setEnabled(atts.value("pos") != "0"); // default true
		
	item->setFlags( QGraphicsItem::GraphicsItemFlags(atts.value("flags").toInt()) );
	
	item->setSelected( atts.value("selected") == "1" ); // default false
}

void KTSerializer::loadProperties(QGraphicsItem *item, const QDomElement &e)
{
	if ( e.tagName() == "properties" )
	{
		QMatrix matrix;
		KTSvg2Qt::svgmatrix2qtmatrix( e.attribute( "transform"), matrix );
		item->setMatrix(matrix);
		
		QPointF pos;
		KTSvg2Qt::parsePointF(e.attribute("pos"), pos );
		item->setPos( pos );
		
		item->setEnabled(e.attribute("pos") != "0");
		
		item->setFlags( QGraphicsItem::GraphicsItemFlags(e.attribute("flags").toInt()) );
		
		item->setSelected( e.attribute("selected") == "1" ); // default false
	}
}


QDomElement KTSerializer::brush(const QBrush *brush, QDomDocument &doc)
{
	QDomElement brushElement = doc.createElement("brush");
	
// 	matrix () const
	
	brushElement.setAttribute( "style", brush->style());
	brushElement.setAttribute( "color", brush->color().name() );
	brushElement.setAttribute( "alpha", brush->color().alpha());
	
	if ( brush->gradient() )
	{
		qDebug("NOT IMPLEMENTED YET!!!");
	}
	
	QString strMatrix = "matrix(";
	QMatrix m = brush->matrix();
	qreal a = m.m11();
	qreal b = m.m12();
	qreal c = m.m21();
	qreal d = m.m22();
	qreal e = m.dx();
	qreal f = m.dy();
	
	strMatrix += QString::number(a) + "," +QString::number(b) + "," + QString::number(c) + "," + QString::number(d) + "," + QString::number(e) + "," + QString::number(f) + ")" ; 
	
	brushElement.setAttribute( "transform", strMatrix);
	
	return brushElement;
}

void KTSerializer::loadBrush(QBrush &brush, const QXmlAttributes &atts)
{
	brush.setStyle(Qt::BrushStyle(atts.value("style").toInt()) );
	
	QColor color(atts.value("color"));
	color.setAlpha(atts.value("alpha").toInt());
	
	brush.setColor(color);
	
	QMatrix matrix;
	KTSvg2Qt::svgmatrix2qtmatrix( atts.value("transform"), matrix );
	brush.setMatrix(matrix);
}

void KTSerializer::loadBrush(QBrush &brush, const QDomElement &e)
{
	brush.setStyle(Qt::BrushStyle(e.attribute("style").toInt()) );
	
	brush.setColor(QColor(e.attribute("color")));
	
	QMatrix matrix;
	KTSvg2Qt::svgmatrix2qtmatrix( e.attribute("transform"), matrix );
	brush.setMatrix(matrix);
}


QDomElement KTSerializer::pen(const QPen *pen, QDomDocument &doc)
{
	QDomElement penElement = doc.createElement("pen");
	
	penElement.setAttribute( "style", pen->style());
	penElement.setAttribute( "color", pen->color().name() );
	penElement.setAttribute( "alpha", pen->color().alpha() );
	penElement.setAttribute( "capStyle", pen->capStyle() );
	penElement.setAttribute( "joinStyle", pen->joinStyle() );
	penElement.setAttribute( "width", pen->width() );
	penElement.setAttribute( "miterLimit", pen->miterLimit() );
	
	QBrush brush = pen->brush();
	penElement.appendChild(KTSerializer::brush(&brush, doc));
	
	return penElement;
}

void KTSerializer::loadPen(QPen &pen, const QXmlAttributes &atts)
{
	pen.setCapStyle( Qt::PenCapStyle(atts.value("capStyle").toInt() ));
	pen.setStyle( Qt::PenStyle(atts.value("style").toInt() ));
	pen.setJoinStyle( Qt::PenJoinStyle(atts.value("joinStyle").toInt() ));
	pen.setWidth( atts.value("width").toInt() );
	pen.setMiterLimit( atts.value("miterLimit").toInt() );
	
	QColor color(atts.value("color") );
	color.setAlpha(atts.value("alpha").toInt() );
	
	
}

void KTSerializer::loadPen(QPen &pen, const QDomElement &e)
{
	
}



