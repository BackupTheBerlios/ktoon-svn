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

KTProjectParser::KTProjectParser() : QXmlDefaultHandler(), m_gradient(0)
{
}


KTProjectParser::~KTProjectParser()
{
	if ( m_gradient ) delete m_gradient;
}

bool KTProjectParser::startElement( const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
{
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
		else if ( qname == "Pen" )
		{
			m_pen = QPen( atts.value("style").toInt() );
			m_pen.setWidthF( atts.value("width").toDouble() );
			m_pen.setCapStyle( Qt::PenCapStyle(atts.value("capstyle").toInt()) );
			m_pen.setJoinStyle( Qt::PenJoinStyle(atts.value("joinstyle").toInt()) );
		}
		else if ( qname == "Brush")
		{
			m_brush = QBrush(Qt::BrushStyle(atts.value("style").toInt()));
		}
		else if ( qname == "Color" )
		{
			QColor c( atts.value("colorName") );
			c.setAlpha( atts.value("alpha").toInt());
			
			if ( m_qname == "Brush" )
			{
				m_brush.setColor(c);
			}
			else if ( m_qname == "Pen")
			{
				m_pen.setColor(c);
			}
		}
		else if ( qname == "Gradient" )
		{
			if ( m_gradient ) delete m_gradient;
			m_gradientStops.clear();
			
			QGradient::Type type = QGradient::Type(atts.value("type").toInt());
			
			switch( type )
			{
				
				case QGradient::LinearGradient:
				{
					m_gradient = new QLinearGradient(0,0,0,0);
				}
				break;
				case QGradient::RadialGradient:
				{
					m_gradient = new QRadialGradient(0,0,0);
				}
				break;
				case QGradient::ConicalGradient:
				{
					m_gradient = new QConicalGradient(0,0,0);
				}
				break;
				default:
				{
					ktFatal() << "No gradient type: " << type;
				}
				break;
			}
		}
		else if ( qname == "Stop" )
		{
			QColor c(atts.value("colorName") );
			c.setAlpha(atts.value("alpha").toInt() );
			
			m_gradientStops << qMakePair(atts.value("value").toDouble(), c);	
		}
	}
	
	m_qname = qname;
	
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
			emit createComponent( m_polygons, m_pen, m_brush );
			
			m_polygons.clear();
		}
		else if ( qname == "Brush")
		{
			if ( m_qname == "Stop"  && m_gradient )
			{
				Qt::BrushStyle style = m_brush.style();
				m_gradient->setStops(m_gradientStops);
				
				m_brush = QBrush( *m_gradient );
				m_brush.setStyle(style);
			}
		}
		else if ( m_qname == "Pen")
		{
			if ( m_qname == "Stop" && m_gradient )
			{
				m_gradient->setStops(m_gradientStops);
				m_pen.setBrush( *m_gradient );
			}
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
