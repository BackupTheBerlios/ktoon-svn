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

#include <dcore/ddebug.h>
#include <ktsvg2qt.h>

#include "ktpathitem.h"
#include "ktpixmapitem.h"
#include "kttextitem.h"
#include "ktbuttonitem.h"
#include "ktrectitem.h"
#include "ktellipseitem.h"
#include "ktlineitem.h"
#include "ktitemgroup.h"
#include "ktgraphiclibraryitem.h"

#include "ktgraphicalgorithm.h"
#include "ktserializer.h"

struct KTItemFactory::Private
{
	QGraphicsItem *item;
	QGradient *gradient;
	QString loading;//brush or pen
	
	QStack<QGraphicsItem *> childs;
	
	bool addToGroup;
	
	QString textReaded;
};


KTItemFactory::KTItemFactory() : DXmlParserBase(), d(new Private)
{
	d->item = 0;
	d->addToGroup = false;
}


KTItemFactory::~KTItemFactory()
{
	delete d;
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
	else if( root == "symbol")
	{
		item = new KTGraphicLibraryItem;
	}
	
	return item;
}

bool KTItemFactory::startTag( const QString& qname, const QXmlAttributes& atts)
{
	if ( qname == "path" )
	{
		QPainterPath path;
		KTSvg2Qt::svgpath2qtpath( atts.value("d"), path );
		
		if ( !d->item )
		{
			d->item = createItem( qname );
		}
// 		else if ( d->addToGroup )
// 		{
// 			QGraphicsItem *item = createItem(qname);
// 			qgraphicsitem_cast<KTPathItem *>(item)->setPath(path);
// 			
// 			qgraphicsitem_cast<QGraphicsItemGroup *>(d->item)->addToGroup(item );
// 		}
		
		qgraphicsitem_cast<KTPathItem *>(d->item)->setPath(path);
	}
	else if ( qname == "rect" )
	{
		if ( !d->item )
		{
			d->item = createItem( qname );
		}
		
		QRectF rect(atts.value("x").toDouble(), atts.value("y").toDouble(), atts.value("width").toDouble(), atts.value("height").toDouble() );
		
		qgraphicsitem_cast<KTRectItem *>(d->item)->setRect(rect);
		
	}
	else if ( qname == "ellipse" )
	{
		if ( !d->item )
		{
			d->item = createItem( qname );
		}
		
		QRectF rect(QPointF(0, 0), QSizeF(2 * atts.value("rx").toDouble(), 2 * atts.value("ry").toDouble() ));
		qgraphicsitem_cast<KTEllipseItem *>(d->item)->setRect(rect);
	}
	else if ( qname == "button" )
	{
		if ( !d->item )
		{
			d->item = createItem( qname );
		}
	}
	else if ( qname == "text" )
	{
		if ( !d->item )
		{
			d->item = createItem( qname );
		}
		
		setReadText(true);
		d->textReaded = "";
		
	}
	else if ( qname == "line" )
	{
		if ( !d->item )
		{
			d->item = createItem( qname );
		}
		
		QLineF line(atts.value("x1").toDouble(), atts.value("y1").toDouble(), atts.value("x2").toDouble(), atts.value("y2").toDouble());
		
		qgraphicsitem_cast<KTLineItem *>(d->item)->setLine(line);
	}
	else if ( qname == "g" )
	{
		if ( !d->item )
		{
			d->item = createItem( qname );
		}
		
		d->addToGroup = true;
	}
	else if ( qname == "symbol" )
	{
		if( !d->item)
		{
			d->item = createItem(qname);
		}
		
		qgraphicsitem_cast<KTGraphicLibraryItem *>(d->item)->setSymbolName(atts.value("id"));
	}
	
	//////////
	
	if ( qname == "properties" && d->item )
	{
		KTSerializer::loadProperties( d->item, atts);
	}
	else if ( qname == "brush" )
	{
		QBrush brush;
		KTSerializer::loadBrush( brush, atts);
		
		
		if ( currentTag() == "pen" )
		{
			d->loading = "pen";
			QPen pen = itemPen();
			pen.setBrush(brush);
			setItemPen( pen );
		}
		else
		{
			d->loading = qname;
			setItemBrush( brush );
		}
	}
	else if ( qname == "pen" )
	{
		QPen pen;
		d->loading = qname;
		KTSerializer::loadPen( pen, atts);
		setItemPen( pen );
	}
	else if ( qname == "font" )
	{
		QFont font;
		
		KTSerializer::loadFont( font, atts);
		
		if ( root() == "text" )
		{
			qgraphicsitem_cast<KTTextItem *>(d->item)->setFont(font);
		}
	}
	else if(qname == "stop")
	{
		if(d->gradient)
		{
			QColor c(atts.value("colorName"));
			c.setAlpha(atts.value("alpha").toInt());
			d->gradient->setColorAt ( atts.value("value").toDouble(), c);
		}
	}
	else if(qname == "gradient")
	{
		d->gradient = KTSerializer::createGradient( atts);
	}
	
	return true;
}

void KTItemFactory::text ( const QString & ch )
{
	d->textReaded += ch;
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
		qgraphicsitem_cast<KTTextItem *>(d->item)->setHtml(d->textReaded);
	}
	else if ( qname == "g" )
	{
		d->addToGroup = false;
	}
	else if( qname == "gradient")
	{
		if(d->loading == "brush")
		{
			setItemGradient(*d->gradient, true);
		}
		else
		{
			setItemGradient(*d->gradient, false);
		}
	}
	
	return true;
}

void KTItemFactory::setItemPen(const QPen &pen)
{
	if ( root() == "path" || root() == "rect" || root() == "ellipse" )
	{
		qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(d->item)->setPen(pen);
	}
	else if(root() == "line")
	{
		qgraphicsitem_cast<QGraphicsLineItem *>(d->item)->setPen(pen);
	}
}

void KTItemFactory::setItemBrush(const QBrush &brush)
{
	if ( root() == "path" || root() == "rect" || root() == "ellipse" )
	{
		qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(d->item)->setBrush(brush);
	}

}

void  KTItemFactory::setItemGradient(const QGradient& gradient, bool brush)
{
	
	QBrush gBrush(gradient);
	if ( root() == "path" || root() == "rect" || root() == "ellipse" )
	{
		QAbstractGraphicsShapeItem * tmp = qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(d->item);
		
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
		QGraphicsLineItem * tmp = qgraphicsitem_cast<QGraphicsLineItem *>(d->item);
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
		return qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(d->item)->pen();
	}
	else if(root() == "line")
	{
		return qgraphicsitem_cast<QGraphicsLineItem *>(d->item)->pen();
	}
	return QPen(Qt::transparent, 0);
}

QBrush KTItemFactory::itemBrush() const
{
	if ( root() == "path" || root() == "rect" || root() == "ellipse" )
	{
		return qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(d->item)->brush();
	}
	
	return Qt::transparent;
}

bool KTItemFactory::loadItem(QGraphicsItem *item, const QString &xml)
{
	d->item = item;
	bool ok = parse(xml);
// 	QAbstractGraphicsShapeItem *tmp =  qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(d->item);
	return ok;
}

QGraphicsItem *KTItemFactory::create(const QString &xml)
{
	if( loadItem(0, xml) )
	{
		return d->item;
	}
	
	return 0;
}


