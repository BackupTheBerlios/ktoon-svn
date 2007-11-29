/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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
 
#include "kthememanager.h"
#include "qapplication.h"
#include "kthemedocument.h"

#include <kglobal.h>
#include <kapplication.h>

#include "kdebug.h"

KThemeManager::KThemeManager() : QXmlDefaultHandler()
{
	m_palette = QApplication::palette();
}


KThemeManager::~KThemeManager()
{
}

bool KThemeManager::applyTheme(const QString &file)
{
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
		kError() <<  QObject::tr("I can't analize the theme file: %1").arg(file) << endl;
		ok = false;
	}
	
	return ok;
}

bool KThemeManager::applyTheme(const KThemeDocument &kd)
{
// 	dDebug() << "Applying theme" << endl;;
	bool ok = false;
	QXmlSimpleReader reader;
	reader.setContentHandler(this);
	reader.setErrorHandler(this);
	QXmlInputSource xmlsource;
	xmlsource.setData(kd.toString());
	if ( reader.parse(&xmlsource) )
	{
		ok = true;
	}
	else
	{
		kDebug() << QObject::tr("I can't analize the theme document") << endl;
		ok = false;
	}
	
	return ok;
}

bool KThemeManager::startElement( const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
{
	m_qname = qname;

	if (m_root.isNull() )
	{
		m_root = qname;
	}
	else if ( m_root == "KTheme" )
	{
		if ( qname == "Text" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_palette.setColor( QPalette::Text, c);
			}
		}
		else if ( qname == "Base" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_palette.setColor( QPalette::Base, c );
			}
		}
		else if ( qname == "Foreground" )
		{
			QColor c = getColor(atts);
			if ( ! c.isValid() )
			{
				c = 0xd7d7ef;
			}
			m_palette.setColor( QPalette::Foreground, c );

		}
		else if ( qname == "Background" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_palette.setColor( QPalette::Background, c );
			}
		}
		else if ( qname == "Button" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_palette.setColor( QPalette::Button, c );
			}
		}
		else if ( qname == "ButtonText" )
		{
			QColor c = getColor(atts);
			if ( !c.isValid() )
			{
				c = 0xd7d7ef;
			}
			m_palette.setColor( QPalette::ButtonText, c );
		
		}
		else if ( qname == "Light" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_palette.setColor( QPalette::Light, c );
			}
		}
		else if ( qname == "Midlight" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_palette.setColor( QPalette::Midlight, c );
			}
		}
		else if ( qname == "Dark" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_palette.setColor( QPalette::Dark, c );
			}
		}
		else if ( qname == "Mid" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_palette.setColor( QPalette::Mid, c );
			}
		}
		else if ( qname == "Highlight" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_palette.setColor( QPalette::Highlight, c );
			}
		}
		else if ( qname == "HighlightedText" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_palette.setColor( QPalette::HighlightedText, c );
			}
		}
		else if ( qname == "BrightText" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_palette.setColor( QPalette::HighlightedText, c );
			}
		}
		else if ( qname == "Link" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_palette.setColor( QPalette::Link, c );
			}
		}
		else if ( qname == "LinkVisited" )
		{
			QColor c = getColor(atts);
			if ( c.isValid() )
			{
				m_palette.setColor( QPalette::LinkVisited, c );
			}
		}
	}
	return true;
}

bool KThemeManager::endElement(const QString&, const QString&, const QString& qname)
{
	if ( qname == "KTheme" )
	{
		kApp->applyPalette(m_palette);
	}
	
	return true;
}

bool KThemeManager::characters(const QString &)
{
	return true;
}

bool KThemeManager::error ( const QXmlParseException & exception )
{
	kError() << "Error analizing theme: " << exception.message() << endl;
	return false;
}

bool KThemeManager::fatalError ( const QXmlParseException & exception )
{
	kError() << "FATAL Error analizing theme: " << endl;
	kError() << "Line: " << exception.lineNumber() << " Column: " << exception.columnNumber() << " " << exception.message() << endl;
	return false;
}

QColor KThemeManager::getColor(const QXmlAttributes& atts)
{
	QColor color(atts.value("color"));
	return color;
}

