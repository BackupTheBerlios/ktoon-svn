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
#include "ktgraphicalgorithm.h"


KTItemFactory::KTItemFactory() : QXmlDefaultHandler(), m_item(0)
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
		
		QMatrix matrix;
		KTSvg2Qt::svgmatrix2qtmatrix( atts.value("transform"), matrix );
		
		qgraphicsitem_cast<KTPathItem *>(m_item)->setMatrix(matrix);
		
		QPointF pos;
		KTSvg2Qt::parsePointF(atts.value("pos"), pos );
		
		qgraphicsitem_cast<KTPathItem *>(m_item)->setPos( pos );
		
	}
	else if ( qname == "rect" )
	{
		m_item = new KTRectItem;
	}
	else if ( qname == "ellipse" )
	{
		m_item = new KTEllipseItem;
		
		QRectF rect(QPointF(0, 0), QSizeF(2 * atts.value("rx").toDouble(), 2 * atts.value("ry").toDouble() ));
		SHOW_VAR(rect);
		qgraphicsitem_cast<KTEllipseItem *>(m_item)->setRect(rect);
		
		QPointF pos;
		KTSvg2Qt::parsePointF(atts.value("pos"), pos );
		qgraphicsitem_cast<KTEllipseItem *>(m_item)->setPos(pos);
		
		QMatrix matrix;
		KTSvg2Qt::svgmatrix2qtmatrix( atts.value("transform"), matrix );
		qgraphicsitem_cast<KTPathItem *>(m_item)->setMatrix(matrix);
	}
	else if ( qname == "button" )
	{
		m_item = new KTButtonItem;
	}
	else if ( qname == "text" )
	{
		m_item = new KTTextItem;
	}
	
	m_qname = qname;
	
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


QGraphicsItem *KTItemFactory::create(const QString &xml)
{
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

