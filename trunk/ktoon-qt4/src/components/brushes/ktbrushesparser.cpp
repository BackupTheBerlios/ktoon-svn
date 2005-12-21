/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#include "ktbrushesparser.h"
#include "ktdebug.h"
#include "ktpathadjuster.h"

KTBrushesParser::KTBrushesParser() : QXmlDefaultHandler()
{
}


KTBrushesParser::~KTBrushesParser()
{
}

bool KTBrushesParser::startElement( const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
{
	m_qname = qname;

	if (m_root.isNull() )
	{
		m_root = qname;
	}
	else if ( m_root == "Brushes" )
	{
		if ( qname == "Item" )
		{
			m_tmpPolygons.clear();
		}
		else if ( qname == "Polygon")
		{
			QString points = atts.value("points");
			m_tmpPolygons << points;
		}
	}
	return true;
}

bool KTBrushesParser::endElement(const QString&, const QString& , const QString& qname)
{
	if ( m_root == "Brushes" )
	{
		if ( qname == "Item" )
		{
			m_brushes << KTPathAdjuster::buildPath( m_tmpPolygons, ':');
		}
	}
	
	return true;
}

bool KTBrushesParser::error ( const QXmlParseException & exception )
{
	ktError() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
}


bool KTBrushesParser::fatalError ( const QXmlParseException & exception )
{
	ktFatal() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
}

QList<QPainterPath> KTBrushesParser::brushes()
{
	return m_brushes;
}
