/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@toonka.com   *
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

#include "ktpaletteparser.h"
#include "ddebug.h"

KTPaletteParser::KTPaletteParser()
	: QXmlDefaultHandler(), m_paletteName(""), m_isEditable(false), m_gradient(0)
{
// 	DINIT;
}


KTPaletteParser::~KTPaletteParser()
{
// 	DEND;
}


bool KTPaletteParser::startElement( const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
{
	m_qname = qname;

	if (m_root.isNull() )
	{
		m_root = qname;
		m_paletteName = atts.value("name");
		if ( atts.value("editable") == "true" )
		{
			m_isEditable = true;
		}
		else
		{
			m_isEditable = false;
		}
	}
	else if ( m_root == "Palette" )
	{
		if ( qname == "Color" )
		{
			QColor c = QColor(atts.value("colorName"));
			c.setAlpha( atts.value("alpha").toInt() );
			
			if ( c.isValid() )
			{
				m_brushes << c;
			}
			else
				dError() << "Invalid Color";
		}
		if ( qname == "Gradient" )
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
	return true;
}

bool KTPaletteParser::endElement(const QString&, const QString& , const QString& qname)
{
	if ( m_root == "Palette" )
	{
		if ( qname == "Gradient" && m_gradient )
		{
			m_gradient->setStops(m_gradientStops);
			m_brushes << *m_gradient;
			m_gradientStops.clear(); 
		}
	}
	
	return true;
}

QList<QBrush> KTPaletteParser::brushes() const
{
	return m_brushes;
}

QString KTPaletteParser::paletteName() const
{
	return m_paletteName;
}

bool KTPaletteParser::paletteIsEditable() const
{
	return m_isEditable;
}

bool KTPaletteParser::error ( const QXmlParseException & exception )
{
	dError() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	return true;
}


bool KTPaletteParser::fatalError ( const QXmlParseException & exception )
{
	dFatal() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	return true;
}
