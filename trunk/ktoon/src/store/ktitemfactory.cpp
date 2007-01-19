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

#include "ktitemfactory.h"
#include <QGraphicsItem>

#include <ddebug.h>
#include <ktsvg2qt.h>

#include "ktpathitem.h"
#include "ktpixmapitem.h"
#include "kttextitem.h"
#include "ktbuttonitem.h"
#include "ktrectitem.h"
#include "ktellipseitem.h"
#include "ktlineitem.h"
#include "ktitemgroup.h"

#include "ktgraphicalgorithm.h"
#include "ktserializer.h"



KTItemFactory::KTItemFactory() : KTXmlParserBase(), m_item(0), m_addToGroup(false)
{
}


KTItemFactory::~KTItemFactory()
{
}

QGraphicsItem* KTItemFactory::createItem(const QString &root)
{
	QGraphicsItem* item = 0;
	if ( root == "path" )
	{
		item = new KTPathItem;
	}
	else if ( root == "rect" )
	{
		item = new KTRectItem;
	}
	else if ( root == "ellipse" )
	{
		item = new KTEllipseItem;
	}
	else if ( root == "button" )
	{
		item = new KTButtonItem;
	}
	else if ( root == "text" )
	{
		item = new KTTextItem;
	}
	else if ( root == "line" )
	{
		item = new KTLineItem;
	}
	else if ( root == "g" )
	{
		item = new KTItemGroup;
	}
	
	return item;
}

bool KTItemFactory::startTag( const QString& qname, const QXmlAttributes& atts)
{
	if ( qname == "path" )
	{
		QPainterPath path;
		KTSvg2Qt::svgpath2qtpath( atts.value("d"), path );
		
		if ( !m_item )
		{
			m_item = createItem( qname );
		}
// 		else if ( m_addToGroup )
// 		{
// 			QGraphicsItem *item = createItem(qname);
// 			qgraphicsitem_cast<KTPathItem *>(item)->setPath(path);
// 			
// 			qgraphicsitem_cast<QGraphicsItemGroup *>(m_item)->addToGroup(item );
// 		}
		
		qgraphicsitem_cast<KTPathItem *>(m_item)->setPath(path);
	}
	else if ( qname == "rect" )
	{
		if ( !m_item )
		{
			m_item = createItem( qname );
		}
		
		QRectF rect(atts.value("x").toDouble(), atts.value("y").toDouble(), atts.value("width").toDouble(), atts.value("height").toDouble() );
		
		qgraphicsitem_cast<KTRectItem *>(m_item)->setRect(rect);
		
	}
	else if ( qname == "ellipse" )
	{
		if ( !m_item )
		{
			m_item = createItem( qname );
		}
		
		QRectF rect(QPointF(0, 0), QSizeF(2 * atts.value("rx").toDouble(), 2 * atts.value("ry").toDouble() ));
		qgraphicsitem_cast<KTEllipseItem *>(m_item)->setRect(rect);
	}
	else if ( qname == "button" )
	{
		if ( !m_item )
		{
			m_item = createItem( qname );
		}
	}
	else if ( qname == "text" )
	{
		if ( !m_item )
		{
			m_item = createItem( qname );
		}
		
		setReadText(true);
		m_textReaded = "";
		
	}
	else if ( qname == "line" )
	{
		if ( !m_item )
		{
			m_item = createItem( qname );
		}
		
		QLineF line(atts.value("x1").toDouble(), atts.value("y1").toDouble(), atts.value("x2").toDouble(), atts.value("y2").toDouble());
		
		qgraphicsitem_cast<KTLineItem *>(m_item)->setLine(line);
	}
	else if ( qname == "g" )
	{
		if ( !m_item )
		{
			m_item = createItem( qname );
		}
		
		m_addToGroup = true;
	}
	
	//////////
	
	if ( qname == "properties" && m_item )
	{
		KTSerializer::loadProperties( m_item, atts);
	}
	else if ( qname == "brush" )
	{
		QBrush brush;
		KTSerializer::loadBrush( brush, atts);
		
		
		if ( currentTag() == "pen" )
		{
			m_loading = "pen";
			QPen pen = itemPen();
			pen.setBrush(brush);
			setItemPen( pen );
		}
		else
		{
			m_loading = qname;
			setItemBrush( brush );
		}
	}
	else if ( qname == "pen" )
	{
		QPen pen;
		m_loading = qname;
		KTSerializer::loadPen( pen, atts);
		setItemPen( pen );
	}
	else if ( qname == "font" )
	{
		QFont font;
		
		KTSerializer::loadFont( font, atts);
		
		if ( root() == "text" )
		{
			qgraphicsitem_cast<KTTextItem *>(m_item)->setFont(font);
		}
	}
	else if(qname == "stop")
	{
		if(m_gradient)
		{
			QColor c(atts.value("colorName"));
			c.setAlpha(atts.value("alpha").toInt());
			m_gradient->setColorAt ( atts.value("value").toDouble(), c);
		}
	}
	else if(qname == "gradient")
	{
		m_gradient = KTSerializer::createGradient( atts);
	}
	
	return true;
}

void KTItemFactory::text ( const QString & ch )
{
	m_textReaded += ch;
}

bool KTItemFactory::endTag(const QString& qname)
{
	if ( qname == "path" )
	{
		
	}
	else if ( qname == "rect" )
	{
		
	}
	else if ( qname == "button" )
	{
		
	}
	else if ( qname == "text" )
	{
		qgraphicsitem_cast<KTTextItem *>(m_item)->setHtml(m_textReaded);
	}
	else if ( qname == "g" )
	{
		m_addToGroup = false;
	}
	else if( qname == "gradient")
	{
		if(m_loading == "brush")
		{
			setItemGradient(*m_gradient, true);
		}
		else
		{
			setItemGradient(*m_gradient, false);
		}
	}
	
	return true;
}

void KTItemFactory::setItemPen(const QPen &pen)
{
	if ( root() == "path" || root() == "rect" || root() == "ellipse" )
	{
		qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(m_item)->setPen(pen);
	}
	else if(root() == "line")
	{
		qgraphicsitem_cast<QGraphicsLineItem *>(m_item)->setPen(pen);
	}
}

void KTItemFactory::setItemBrush(const QBrush &brush)
{
	if ( root() == "path" || root() == "rect" || root() == "ellipse" )
	{
		qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(m_item)->setBrush(brush);
	}

}

void  KTItemFactory::setItemGradient(const QGradient& gradient, bool brush)
{
	
	QBrush gBrush(gradient);
	if ( root() == "path" || root() == "rect" || root() == "ellipse" )
	{
		QAbstractGraphicsShapeItem * tmp = qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(m_item);
		
		if(brush)
		{
			gBrush.setMatrix(tmp->brush().matrix());
			tmp->setBrush( gBrush );
		}
		else
		{
			gBrush.setMatrix(tmp->pen().brush().matrix());
			QPen pen = tmp->pen();
			pen.setBrush(gBrush);
			tmp->setPen(pen);
		}
	}
	else if(root() == "line")
	{
		QGraphicsLineItem * tmp = qgraphicsitem_cast<QGraphicsLineItem *>(m_item);
		gBrush.setMatrix(tmp->pen().brush().matrix());
		QPen pen = tmp->pen();
		pen.setBrush(gBrush);
		tmp->setPen(pen);
	}
}

QPen KTItemFactory::itemPen() const
{
	if ( root() == "path" || root() == "rect" || root() == "ellipse" )
	{
		return qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(m_item)->pen();
	}
	else if(root() == "line")
	{
		return qgraphicsitem_cast<QGraphicsLineItem *>(m_item)->pen();
	}
	return QPen(Qt::transparent, 0);
}

QBrush KTItemFactory::itemBrush() const
{
	if ( root() == "path" || root() == "rect" || root() == "ellipse" )
	{
		return qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(m_item)->brush();
	}
	
	return Qt::transparent;
}

bool KTItemFactory::loadItem(QGraphicsItem *item, const QString &xml)
{
	m_item = item;
	bool ok = parse(xml);
// 	QAbstractGraphicsShapeItem *tmp =  qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(m_item);
	return ok;
}

QGraphicsItem *KTItemFactory::create(const QString &xml)
{
	if( loadItem(0, xml) )
	{
		return m_item;
	}
	
	return 0;
}


