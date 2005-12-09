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
#include "ktdebug.h"

KTPaletteParser::KTPaletteParser()
	: QXmlDefaultHandler(), m_paletteName(""), m_isEditable(false), m_tmpGradient(0)
{
	KTINIT;
}


KTPaletteParser::~KTPaletteParser()
{
	KTEND;
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
				ktError() << "Invalid Color";
		}
		else if ( qname == "Gradient")
		{
			QString type = atts.value("type");
			QString spread = atts.value("spread");
			
			if ( type == "linear" )
			{
				m_tmpGradient = new QLinearGradient(0, 0, 0, 0 );
			}
			else if ( type == "conical" )
			{
				m_tmpGradient = new QConicalGradient(0, 0, atts.value("angle").toDouble() );
			}
			else if ( type == "radial" )
			{
				m_tmpGradient = new QRadialGradient(0, 0, 0, 0, 0 );
			}
			
			if ( spread == "PadSpread")
			{
				m_tmpGradient->setSpread(QGradient::PadSpread);
			}
			else if ( spread == "RepeatSpread" )
			{
				m_tmpGradient->setSpread(QGradient::RepeatSpread);
			}
			else if ( spread == "ReflectSpread" )
			{
				m_tmpGradient->setSpread(QGradient::ReflectSpread);
			}
		}
		else if ( qname == "Stop" )
		{
			QColor c = QColor(atts.value("colorName"));
			c.setAlpha( atts.value("alpha").toInt() );
			
			if ( c.isValid() )
			{
				m_tmpStops << qMakePair(atts.value("value").toDouble() ,c);
			}
		}
	}
	return true;
}

bool KTPaletteParser::endElement(const QString&, const QString& , const QString& qname)
{
	if ( m_root == "Palette" )
	{
		if ( qname == "Gradient" && m_tmpGradient )
		{
			m_tmpGradient->setStops(m_tmpStops);
			m_brushes << *m_tmpGradient;
			m_tmpStops.clear();
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
	ktError() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
}


bool KTPaletteParser::fatalError ( const QXmlParseException & exception )
{
	ktFatal() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
}
