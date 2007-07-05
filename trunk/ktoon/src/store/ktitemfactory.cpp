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

#include <kcore/kdebug.h>
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
#include "ktlibrary.h"

#include "ktgraphicalgorithm.h"
#include "ktserializer.h"

struct KTItemFactory::Private
{
	QGraphicsItem *item;
	QGradient *gradient;
	QString loading;//brush or pen
	
	QStack<KTItemGroup *> groups;
	QStack<QGraphicsItem *> objects;
	
	bool addToGroup, isLoading;
	
	QString textReaded;
	
	const KTLibrary *library;
};


KTItemFactory::KTItemFactory() : DXmlParserBase(), d(new Private)
{
	d->item = 0;
	d->addToGroup = false;
	d->isLoading = false;
	d->library = 0;
}


KTItemFactory::~KTItemFactory()
{
	delete d;
}

void KTItemFactory::setLibrary(const KTLibrary *library)
{
	d->library = library;
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
	else if ( root == "group" )
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
	D_FUNCINFOX("items") << qname;
	if ( qname == "path" )
	{
		QPainterPath path;
		KTSvg2Qt::svgpath2qtpath( atts.value("d"), path );
		
		if ( d->addToGroup )
		{
			QGraphicsItem *item = createItem(qname);
			qgraphicsitem_cast<KTPathItem *>(item)->setPath(path);
			
			d->objects.push(item);
		}
		else
		{
			if ( !d->item )
				d->item = createItem( qname );
			
			qgraphicsitem_cast<KTPathItem *>(d->item)->setPath(path);
			
			d->objects.push(d->item);
		}
	}
	else if ( qname == "rect" )
	{
		QRectF rect(atts.value("x").toDouble(), atts.value("y").toDouble(), atts.value("width").toDouble(), atts.value("height").toDouble() );
		
		if( d->addToGroup )
		{
			KTRectItem *item = qgraphicsitem_cast<KTRectItem *>(createItem(qname));
			item->setRect(rect);
			
			
			d->objects.push(item);
		}
		else
		{
			if ( !d->item )
				d->item = createItem( qname );
			qgraphicsitem_cast<KTRectItem *>(d->item)->setRect(rect);
			
			d->objects.push(d->item);
		}
	}
	else if ( qname == "ellipse" )
	{
		QRectF rect(QPointF(0, 0), QSizeF(2 * atts.value("rx").toDouble(), 2 * atts.value("ry").toDouble() ));
		
		if( d->addToGroup )
		{
			KTEllipseItem *item = qgraphicsitem_cast<KTEllipseItem *>(createItem(qname));
			item->setRect(rect);
			
			d->objects.push(item);
		}
		else
		{
			if ( !d->item )
				d->item = createItem( qname );
			
			qgraphicsitem_cast<KTEllipseItem *>(d->item)->setRect(rect);
			
			d->objects.push(d->item);
		}
		
	}
	else if ( qname == "button" )
	{
		if ( !d->item )
		{
			d->item = createItem( qname );
			d->objects.push(d->item);
		}
		
		if( d->addToGroup )
		{
// 			d->groups.last()->addToGroup( createItem( qname ) );
		}
	}
	else if ( qname == "text" )
	{
		if( d->addToGroup )
		{
			KTTextItem *item = qgraphicsitem_cast<KTTextItem *>(createItem( qname ));
			d->objects.push(item);
		}
		else
		{
			if ( !d->item )
				d->item = createItem( qname );
			
			d->objects.push(d->item);
		}
		
		
		setReadText(true);
		d->textReaded = "";
	}
	else if ( qname == "line" )
	{
		QLineF line(atts.value("x1").toDouble(), atts.value("y1").toDouble(), atts.value("x2").toDouble(), atts.value("y2").toDouble());
		
		if( d->addToGroup )
		{
			KTLineItem *item = qgraphicsitem_cast<KTLineItem *>(createItem(qname));
			item->setLine(line);
			
			d->objects.push(item);
		}
		else
		{
			if ( !d->item )
				d->item = createItem( qname );
			
			qgraphicsitem_cast<KTLineItem *>(d->item)->setLine(line);
			d->objects.push(d->item);
		}
	}
	else if ( qname == "group" )
	{
		if( d->addToGroup )
		{
			KTItemGroup *group = qgraphicsitem_cast<KTItemGroup *>( createItem(qname) );
			
			d->groups.push(group);
			d->objects.push(group);
		}
		else
		{
			if ( !d->item )
				d->item = createItem( qname );
			d->groups.push(qgraphicsitem_cast<KTItemGroup *>(d->item));
			d->objects.push(d->item);
		}
		
		d->addToGroup = true;
	}
	else if ( qname == "symbol" )
	{
		if( d->addToGroup )
		{
			KTGraphicLibraryItem *item = qgraphicsitem_cast<KTGraphicLibraryItem *>(createItem( qname ));
			
			QString id = atts.value("id");
			
			item->setSymbolName(id);
			
			if( d->library )
				item->setObject(d->library->findObject(id));
			
			d->objects.push(item);
		}
		else
		{
			if( !d->item)
				d->item = createItem(qname);
			
			QString id = atts.value("id");
			
			qgraphicsitem_cast<KTGraphicLibraryItem *>(d->item)->setSymbolName(id);
			
			if( d->library )
				qgraphicsitem_cast<KTGraphicLibraryItem *>(d->item)->setObject(d->library->findObject(id));
			
			d->objects.push(d->item);
		}
	}
	
	//////////
	
	if ( qname == "properties" && !d->objects.isEmpty() )
	{
		KTSerializer::loadProperties( d->objects.last(), atts);
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
		
		if ( KTTextItem *text = qgraphicsitem_cast<KTTextItem *>(d->objects.last()) )
		{
			text->setFont(font);
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
	D_FUNCINFOX("items") << qname;
	if ( qname == "path" )
	{
		if(d->addToGroup)
		{
			d->groups.last()->addToGroup(d->objects.last());
		}
		d->objects.pop();
	}
	else if ( qname == "rect" )
	{
		if(d->addToGroup)
		{
			d->groups.last()->addToGroup(d->objects.last());
		}
		d->objects.pop();
	}
	else if ( qname == "ellipse" )
	{
		if(d->addToGroup)
		{
			d->groups.last()->addToGroup(d->objects.last());
		}
		d->objects.pop();
	}
	else if( qname == "symbol" )
	{
		if(d->addToGroup)
		{
			d->groups.last()->addToGroup(d->objects.last());
		}
		d->objects.pop();
	}
	else if( qname == "line" )
	{
		if(d->addToGroup)
		{
			d->groups.last()->addToGroup(d->objects.last());
		}
		d->objects.pop();
	}
	else if ( qname == "button" )
	{
		if(d->addToGroup)
		{
			d->groups.last()->addToGroup(d->objects.last());
		}
		d->objects.pop();
	}
	else if ( qname == "text" )
	{
		if(d->addToGroup)
		{
			d->groups.last()->addToGroup(d->objects.last());
		}
		
		if(KTTextItem *text = qgraphicsitem_cast<KTTextItem *>(d->objects.last()) )
		{
			text->setHtml(d->textReaded);
		}
		d->objects.pop();
	}
	else if ( qname == "group" )
	{
		d->groups.pop();
		d->addToGroup = !d->groups.isEmpty();
		
		if(d->addToGroup)
		{
			d->groups.last()->addToGroup(d->objects.last());
		}
		
		d->objects.pop();
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
	else
	{
		dWarning("items") << "Unhandled: " << qname;
	}
	
	return true;
}

void KTItemFactory::setItemPen(const QPen &pen)
{
	if(d->objects.isEmpty() ) return;
	
	if(QGraphicsLineItem *line = qgraphicsitem_cast<QGraphicsLineItem *>(d->objects.last()) )
	{
		line->setPen(pen);
	}
	else if ( QAbstractGraphicsShapeItem *shape = qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(d->objects.last() ) )
	{
		shape->setPen(pen);
	}
}

void KTItemFactory::setItemBrush(const QBrush &brush)
{
	if(d->objects.isEmpty() ) return;
	
	if ( QAbstractGraphicsShapeItem *shape = qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(d->objects.last() ) )
	{
		shape->setBrush(brush);
	}

}

void  KTItemFactory::setItemGradient(const QGradient& gradient, bool brush)
{
	if(d->objects.isEmpty() ) return;
	
	QBrush gBrush(gradient);
	if ( QAbstractGraphicsShapeItem *shape = qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(d->objects.last() ) )
	{
		if(brush)
		{
			gBrush.setMatrix(shape->brush().matrix());
			shape->setBrush( gBrush );
		}
		else
		{
			gBrush.setMatrix(shape->pen().brush().matrix());
			QPen pen = shape->pen();
			pen.setBrush(gBrush);
			shape->setPen(pen);
		}
	}
	else if(QGraphicsLineItem *line = qgraphicsitem_cast<QGraphicsLineItem *>(d->objects.last()) )
	{
		gBrush.setMatrix(line->pen().brush().matrix());
		QPen pen = line->pen();
		pen.setBrush(gBrush);
		line->setPen(pen);
	}
}

QPen KTItemFactory::itemPen() const
{
	if( ! d->objects.isEmpty() )
	{
		if(QGraphicsLineItem *line = qgraphicsitem_cast<QGraphicsLineItem *>(d->objects.last()) )
		{
			return line->pen();
		}
		else if ( QAbstractGraphicsShapeItem *shape = qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(d->objects.last() ) )
		{
			return shape->pen();
		}
	}
	return QPen(Qt::transparent, 1);
}

QBrush KTItemFactory::itemBrush() const
{
	if( ! d->objects.isEmpty() )
	{
		if ( QAbstractGraphicsShapeItem *shape = qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(d->objects.last() ) )
		{
			return shape->brush();
		}
	}
	return Qt::transparent;
}

bool KTItemFactory::loadItem(QGraphicsItem *item, const QString &xml)
{
	d->item = item;
	
	d->isLoading = true;
	
	bool ok = parse(xml);
	
	d->isLoading = false;
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


