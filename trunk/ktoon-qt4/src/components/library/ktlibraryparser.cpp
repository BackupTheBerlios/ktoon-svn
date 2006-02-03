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
	: QXmlDefaultHandler(), m_gradient(0), m_tagCounter(0)
{
}


KTLibraryParser::~KTLibraryParser()
{
	if ( m_gradient ) delete m_gradient;
	
	foreach(AGraphic *graphic, m_graphics)
	{
		delete graphic;
	}
}

bool KTLibraryParser::startElement( const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
{
	if (m_root.isNull() )
	{
		m_root = qname;
	}
	else if ( m_root == "Library" )
	{
		if ( qname == "Component" )
		{
			if ( m_tagCounter == 0 )
			{
				AGraphicComponent *rootComponent = new AGraphicComponent;
				m_components << rootComponent;
			}
			
			qDeleteAll(m_graphics.begin(), m_graphics.end());
			m_graphics.clear();
			
			QString objName = atts.value("name");
			if ( !(objName.isEmpty() || objName.isNull()) )
			{
				m_objectName = objName;
			}
			m_tagCounter++;
		}
		else if ( qname == "Graphic" )
		{
			m_tmpPolygons.clear();
			
			AGraphic *graphic = new AGraphic;
			m_graphics << graphic;
		}
		else if ( qname == "Polygon")
		{
			QString points = atts.value("points");
			m_tmpPolygons << points;
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

bool KTLibraryParser::endElement(const QString&, const QString& , const QString& qname)
{
	if ( m_root == "Library" )
	{
		if ( qname == "Graphic" )
		{
			m_graphics[m_graphics.count()-1]->path = KTPathAdjuster::buildPath( m_tmpPolygons, ':');
			m_graphics[m_graphics.count()-1]->pen = m_pen;
			m_graphics[m_graphics.count()-1]->brush = m_brush;
		}
		else if ( qname == "Component" )
		{
			m_tagCounter--;
			
			if ( m_tagCounter != 0 )
			{
				AGraphicComponent *child = new AGraphicComponent;
				if ( ! m_objectName.isNull() )
				{
					child->setComponentName( m_objectName);
				}
				
				foreach(AGraphic *graphic, m_graphics)
				{
					child->addGraphic(graphic->path, graphic->pen, graphic->brush);
				}
				
				m_components.last()->addChild(child);
			}
			
			m_objectName = QString();
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
		else if ( m_qname == "Properties")
		{

		}
	}
	
	return true;
}

bool KTLibraryParser::error ( const QXmlParseException & exception )
{
	ktError() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	
	return true;
}


bool KTLibraryParser::fatalError ( const QXmlParseException & exception )
{
	ktFatal() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	
	return true;
}

QList<AGraphicComponent *> KTLibraryParser::components()
{
	return m_components;
}
