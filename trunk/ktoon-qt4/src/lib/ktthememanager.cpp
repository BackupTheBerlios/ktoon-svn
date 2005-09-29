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
 
#include "ktthememanager.h"
#include "ktapplication.h"
#include "ktthemedocument.h"

#include "ktdebug.h"

KTThemeManager::KTThemeManager() : QXmlDefaultHandler()
{
	m_colorGroup = QApplication::palette().active();
}


KTThemeManager::~KTThemeManager()
{
}

bool KTThemeManager::applyTheme(const QString &file)
{
	ktDebug() << "Applying theme" << endl;
	bool ok = false;
	QXmlSimpleReader reader;
	reader.setContentHandler(this);
	reader.setErrorHandler(this);
	QFile f(file);
	QXmlInputSource xmlsource(&f);
	if ( reader.parse(&xmlsource) )
	{
		ok = true;
	}
	else
	{
		ktError() <<  QObject::tr("I can't analize the theme file: %1").arg(file) << endl;
		ok = false;
	}
	
	return ok;
}

bool KTThemeManager::applyTheme(const KTThemeDocument &ktd)
{
	ktDebug() << "Applying theme" << endl;;
	bool ok = false;
	QXmlSimpleReader reader;
	reader.setContentHandler(this);
	reader.setErrorHandler(this);
	QXmlInputSource xmlsource;
	xmlsource.setData(ktd.toString());
	if ( reader.parse(&xmlsource) )
	{
		ok = true;
	}
	else
	{
		ktDebug() << QObject::tr("I can't analize the theme document") << endl;
		ok = false;
	}
	
	return ok;
}

bool KTThemeManager::startElement( const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
{
	m_qname = qname;

	if (m_root.isNull() )
	{
		m_root = qname;
	}
	else if ( m_root == "KTTheme" )
	{
		if ( qname == "Text" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_colorGroup.setColor( QColorGroup::Text, c);
			}
		}
		else if ( qname == "Base" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_colorGroup.setColor( QColorGroup::Base, c );
			}

		}
		else if ( qname == "Foreground" )
		{
			QColor c = getColor(atts);
			if ( ! c.isValid() )
			{
				c = 0xd7d7ef;
			}
			m_colorGroup.setColor( QColorGroup::Foreground, c );

		}
		else if ( qname == "Background" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_colorGroup.setColor( QColorGroup::Background, c );
			}
		}
		else if ( qname == "Button" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_colorGroup.setColor( QColorGroup::Button, c );
			}
		}
		else if ( qname == "ButtonText" )
		{
			QColor c = getColor(atts);
			if ( !c.isValid() )
			{
				c = 0xd7d7ef;
			}
			m_colorGroup.setColor( QColorGroup::ButtonText, c );
		
		}
		else if ( qname == "Light" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_colorGroup.setColor( QColorGroup::Light, c );
			}
		}
		else if ( qname == "Midlight" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_colorGroup.setColor( QColorGroup::Midlight, c );
			}
		}
		else if ( qname == "Dark" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_colorGroup.setColor( QColorGroup::Mid, c );
			}
		}
		else if ( qname == "Mid" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_colorGroup.setColor( QColorGroup::Mid, c );
			}
		}
		else if ( qname == "Highlight" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_colorGroup.setColor( QColorGroup::Highlight, c );
			}
		}
		else if ( qname == "HighlightedText" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_colorGroup.setColor( QColorGroup::HighlightedText, c );
			}
		}
		else if ( qname == "BrightText" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_colorGroup.setColor( QColorGroup::HighlightedText, c );
			}
		}
		else if ( qname == "Link" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_colorGroup.setColor( QColorGroup::Link, c );
			}
		}
		else if ( qname == "LinkVisited" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_colorGroup.setColor( QColorGroup::LinkVisited, c );
			}
		}
	}
	return true;
}

bool KTThemeManager::endElement(const QString&, const QString&, const QString& qname)
{
	if ( qname == "KTTheme" ) // Configuration document
	{
		// aplicar el theme
		QPalette p (m_colorGroup, m_colorGroup, m_colorGroup);
		ktapp->applyPalette(p);
	}
	
	return true;
}

bool KTThemeManager::characters(const QString &)
{
	return true;
}

bool KTThemeManager::error ( const QXmlParseException & exception )
{
	ktError() << "Error analizing theme: " << exception.message() << endl;
	return false;
}

bool KTThemeManager::fatalError ( const QXmlParseException & exception )
{
	ktError() << "FATAL Error analizing theme: " << endl;
	ktError() << "Line: " << exception.lineNumber() << " Column: " << exception.columnNumber() << " " << exception.message() << endl;
	return false;
}

QColor KTThemeManager::getColor(const QXmlAttributes& atts)
{
	QColor color(atts.value("color"));
	return color;
}


