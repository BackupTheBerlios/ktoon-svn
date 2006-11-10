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
#include "ddebug.h"

#include "dpathadjuster.h"


KTProjectParser::KTProjectParser() : QXmlDefaultHandler(), m_gradient(0), m_currentComponent(0), m_rootComponent(0), m_tagCounter(0)
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
			m_projectDir.cd("../../");
			m_partName = atts.value("name");
			if ( m_currentComponent ) delete m_currentComponent;
			if ( m_rootComponent ) delete m_rootComponent;
			
			m_rootComponent = m_currentComponent = 0;
		}
		else if ( qname == "Layer" )
		{
			m_components.clear();
			emit createLayer(atts.value("name"));
		}
		else if ( qname == "Frame")
		{
			bool ok = false;
			int clones = atts.value("nClones").toInt(&ok);
			
			if(!ok) clones = 0;
			
			emit createFrame(atts.value("name"), clones);
		}
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
				m_partName = objName;
			}
			m_tagCounter++;
		}
		else if ( qname == "Graphic" )
		{
			m_polygons.clear();
			
			AGraphic *graphic = new AGraphic;
			m_graphics << graphic;
		}
		else if ( qname == "Image" )
		{
			QPixmap pix(m_projectDir.path()+"/resources/"+atts.value("path"));
			m_graphics.last()->setPixmap(pix, atts.value("path"));
			
			QMatrix rotate;
			rotate.rotate(atts.value("rotate").toDouble());

			QMatrix shear;
			shear.shear(atts.value("shearX").toDouble(), atts.value("shearY").toDouble());

			QMatrix scale;
			scale.scale(atts.value("scaleX").toDouble(), atts.value("scaleY").toDouble());

			QMatrix translate;
			translate.translate(0,0);
			
			m_graphics.last()->mapPixmap(rotate*shear*scale*translate);
			
// 			m_components.last()->rotate(-atts.value("rotate").toDouble());
// 			m_components.last()->scale(atts.value("scaleX").toDouble(), atts.value("scaleY").toDouble());
// 			m_components.last()->shear(atts.value("shearX").toDouble(), atts.value("shearY").toDouble());
			
		}
		else if ( qname == "Polygon")
		{
			m_polygons << atts.value("points");
		}
		else if ( qname == "Pen" )
		{
			m_pen = QPen( Qt::PenStyle(atts.value("style").toInt()) );
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
			QGradient::Spread spread = QGradient::Spread(atts.value("spread").toInt());
			
			switch( type )
			{
				
				case QGradient::LinearGradient:
				{
					m_gradient = new QLinearGradient(atts.value("startX").toDouble(),atts.value("startY").toDouble(),atts.value("finalX").toDouble(), atts.value("finalY").toDouble());
				}
				break;
				case QGradient::RadialGradient:
				{
					m_gradient = new QRadialGradient(atts.value("centerX").toDouble(),atts.value("centerY").toDouble(), atts.value("radius").toDouble(),atts.value("focalX").toDouble(),atts.value("focalY").toDouble() );
				}
				break;
				case QGradient::ConicalGradient:
				{
					m_gradient = new QConicalGradient(atts.value("centerX").toDouble(),atts.value("centerY").toDouble(),atts.value("angle").toDouble());
				}
				break;
				default:
				{
					dFatal() << "No gradient type: " << type;
				}
				break;
			}
			
			m_gradient->setSpread(spread);
			
			
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
			while ( m_components.count() )
			{
				emit createComponent( m_components.takeAt(0));
			}
		}
		else if ( qname == "Graphic" )
		{
			m_graphics.last()->path = DPathAdjuster::buildPath( m_polygons, ':');
			m_graphics.last()->pen = m_pen;
			m_graphics.last()->brush = m_brush;
		}
		else if ( qname == "Component" )
		{
			m_tagCounter--;
			
			AGraphicComponent *root = m_components.last();
			if ( m_tagCounter == 0 )
			{
				foreach(AGraphic *graphic, m_graphics)
				{
					root->addGraphic(graphic->path, graphic->pen, graphic->brush, graphic->pixmap);
				}
			}
			
			if ( m_tagCounter != 0 )
			{
				{
					AGraphicComponent *child = new AGraphicComponent;
					if ( ! m_partName.isNull() )
					{
						child->setComponentName( m_partName);
					}
					
					foreach(AGraphic *graphic, m_graphics)
					{
						child->addGraphic(graphic->path, graphic->pen, graphic->brush);
					}
					
					root->addChild(child);
				}
			}
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
	dError() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	return true;
}


bool KTProjectParser::fatalError ( const QXmlParseException & exception )
{
	dFatal() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	
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

bool KTProjectParser::parse(const QString &filePath)
{
	QXmlSimpleReader reader;
	reader.setContentHandler(this);
	reader.setErrorHandler(this);
		
	QFile source(filePath);
	QXmlInputSource xmlsource(&source);
	
	QFileInfo finfo(filePath);
	
	m_projectDir = finfo.absolutePath();
		
	return reader.parse(&xmlsource);
}


