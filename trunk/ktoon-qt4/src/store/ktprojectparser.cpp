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

#include "ktprojectparser.h"
#include "ktdebug.h"

KTProjectParser::KTProjectParser() : QXmlDefaultHandler()
{
}


KTProjectParser::~KTProjectParser()
{
}

bool KTProjectParser::startElement( const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
{
	m_qname = qname;

	if (m_root.isNull() )
	{
		m_root = qname;
	}
	
	if ( m_root == "KToon" )
	{
		if ( qname == "Project" )
		{
			m_partName = atts.value("name");
			int width = atts.value( "width" ).toInt();
			int height = atts.value ("height").toInt();
			
			m_documentSize = QSize(width, height);
		}
		else if ( qname == "Document")
		{
			m_locations << atts.value("location");
		}
	}
	else if ( m_root == "Document" )
	{
		if ( qname == "Document" )
		{
			m_partName = atts.value("name");
		}
		else if ( qname == "Scene")
		{
			m_locations << atts.value("location");
		}
	}
	else if ( m_root == "Scene")
	{
		if ( qname == "Scene" )
		{
			m_partName = atts.value("name");
		}
		else if ( qname == "Layer" )
		{
			emit createLayer();
		}
		else if ( qname == "Frame")
		{
			emit createFrame();
		}
		else if ( qname == "Component")
		{
		}
		else if ( qname == "Polygon")
		{
			m_polygons << atts.value("points");
		}
	}
	
	return true;
}

bool KTProjectParser::endElement(const QString&, const QString& , const QString& qname)
{
	if ( m_root == "Scene")
	{
		if ( qname == "Layer" )
		{
		}
		else if ( qname == "Frame")
		{
		}
		else if ( qname == "Component")
		{
			emit createComponent( m_polygons );
			
			m_polygons.clear();
		}
		else if ( qname == "Polygon")
		{
		}
	}
	
	return true;
}

bool KTProjectParser::error ( const QXmlParseException & exception )
{
	ktError() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	return true;
}


bool KTProjectParser::fatalError ( const QXmlParseException & exception )
{
	ktFatal() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	
	return true;
}


QString KTProjectParser::partName() const
{
	return m_partName;
}

QStringList KTProjectParser::locations() const
{
	return m_locations;
}

QSize KTProjectParser::documentSize() const
{
	return m_documentSize;
}
