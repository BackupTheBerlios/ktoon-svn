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
 
#include "ktlibraryparser.h"
#include "ktdebug.h"
#include "ktpathadjuster.h"

KTLibraryParser::KTLibraryParser()
 : QXmlDefaultHandler()
{
}


KTLibraryParser::~KTLibraryParser()
{
}

bool KTLibraryParser::startElement( const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
{
	m_qname = qname;

	if (m_root.isNull() )
	{
		m_root = qname;
	}
	else if ( m_root == "Library" )
	{
		if ( qname == "Component" )
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

bool KTLibraryParser::endElement(const QString&, const QString& , const QString& qname)
{
	if ( m_root == "Library" )
	{
		if ( qname == "Component" )
		{
			AGraphicComponent *graphic = new AGraphicComponent;
			
			graphic->setPath( KTPathAdjuster::buildPath( m_tmpPolygons, ':') );
			
			m_components << graphic;
		}
	}
	
	return true;
}

bool KTLibraryParser::error ( const QXmlParseException & exception )
{
	ktError() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
}


bool KTLibraryParser::fatalError ( const QXmlParseException & exception )
{
	ktFatal() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
}

QList<AGraphicComponent *> KTLibraryParser::components()
{
	return m_components;
}
