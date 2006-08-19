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

#include "ktgraphicalgorithm.h"
#include "ktserializer.h"


KTItemFactory::KTItemFactory() : QXmlDefaultHandler(), m_item(0), m_readChar(false)
{
}


KTItemFactory::~KTItemFactory()
{
}

bool KTItemFactory::startElement( const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
{
	if (m_root.isNull() )
	{
		m_root = qname;
	}
	
	if ( qname == "path" )
	{
		m_item = new KTPathItem;
		
		QPainterPath path;
		KTSvg2Qt::svgpath2qtpath( atts.value("d"), path );
		
		qgraphicsitem_cast<KTPathItem *>(m_item)->setPath(path);
		
		
	}
	else if ( qname == "rect" )
	{
		m_item = new KTRectItem;
		
		QRectF rect(atts.value("x").toDouble(), atts.value("y").toDouble(), atts.value("width").toDouble(), atts.value("height").toDouble() );
		
		qgraphicsitem_cast<KTRectItem *>(m_item)->setRect(rect);
		
	}
	else if ( qname == "ellipse" )
	{
		m_item = new KTEllipseItem;
		
		QRectF rect(QPointF(0, 0), QSizeF(2 * atts.value("rx").toDouble(), 2 * atts.value("ry").toDouble() ));
		qgraphicsitem_cast<KTEllipseItem *>(m_item)->setRect(rect);
	}
	else if ( qname == "button" )
	{
		m_item = new KTButtonItem;
	}
	else if ( qname == "text" )
	{
		m_item = new KTTextItem;
		
		m_readChar = true;
		m_textReaded = "";
		
	}
	else if ( qname == "line" )
	{
		m_item = new KTLineItem;
		
		QLineF line(atts.value("x1").toDouble(), atts.value("y1").toDouble(), atts.value("x2").toDouble(), atts.value("y2").toDouble());
		
		qgraphicsitem_cast<KTLineItem *>(m_item)->setLine(line);
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
		
		if ( m_qname == "pen" )
		{
			QPen pen = itemPen();
			pen.setBrush(brush);
			setItemPen( pen );
		}
		else
		{
			setItemBrush( brush );
		}
	}
	else if ( qname == "pen" )
	{
		QPen pen;
		KTSerializer::loadPen( pen, atts);
		setItemPen( pen );
	}
	else if ( qname == "font" )
	{
		QFont font;
		
		KTSerializer::loadFont( font, atts);
		
		if ( m_root == "text" )
		{
			qgraphicsitem_cast<KTTextItem *>(m_item)->setFont(font);
		}
	}
	
	m_qname = qname;
	
	return true;
}

bool KTItemFactory::characters ( const QString & ch )
{
	if ( m_readChar )
	{
		m_textReaded += ch;
	}
	
	return true;
}

bool KTItemFactory::endElement(const QString&, const QString& , const QString& qname)
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
		m_readChar = false;
		qgraphicsitem_cast<KTTextItem *>(m_item)->setHtml(m_textReaded);
	}
	
	return true;
}



bool KTItemFactory::error ( const QXmlParseException & exception )
{
	dError() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	return true;
}


bool KTItemFactory::fatalError ( const QXmlParseException & exception )
{
	dFatal() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	
	return true;
}


void KTItemFactory::setItemPen(const QPen &pen)
{
	if ( m_root == "path" || m_root == "rect" || m_root == "ellipse" )
	{
		qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(m_item)->setPen(pen);
	}
}

void KTItemFactory::setItemBrush(const QBrush &brush)
{
	if ( m_root == "path" || m_root == "rect" || m_root == "ellipse" )
	{
		qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(m_item)->setBrush(brush);
	}
}

QPen KTItemFactory::itemPen() const
{
	if ( m_root == "path" || m_root == "rect" || m_root == "ellipse" )
	{
		return qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(m_item)->pen();
	}
	
	return QPen(Qt::transparent, 0);
}

QBrush KTItemFactory::itemBrush() const
{
	if ( m_root == "path" || m_root == "rect" || m_root == "ellipse" )
	{
		return qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(m_item)->brush();
	}
	
	return Qt::transparent;
}


QGraphicsItem *KTItemFactory::create(const QString &xml)
{
// 	dDebug() << "Creating item: " << xml;
	QXmlSimpleReader reader;
	reader.setContentHandler(this);
	reader.setErrorHandler(this);
	
	QXmlInputSource xmlsource;
	xmlsource.setData(xml);
	
	m_item = 0;
	
	if( reader.parse(&xmlsource) )
	{
		return m_item;
	}
	
	return 0;
}


