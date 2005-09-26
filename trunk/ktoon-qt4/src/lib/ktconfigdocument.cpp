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

#include "ktconfigdocument.h"

#include <qfile.h>
//Added by qt3to4:
#include <QTextStream>
#include <iostream>
#include <qobject.h>
#include <q3stylesheet.h>

KTConfigDocument::KTConfigDocument(const QString &path) : QDomDocument(), m_path(path), MAXRECENTS(6)
{
	qDebug("[Initializing KTConfigDocument]");
	setup();

	if ( !m_isOk )
	{
		QDomProcessingInstruction header = this->createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
		this->appendChild(header);
		
		QDomElement root = createElement( "KTConfig" );
		appendChild( root );
	}
}

void KTConfigDocument::setup()
{
	QFile config( m_path );
	m_isOk = false;
	
	if ( config.exists() )
	{
		QString errorMsg = "";
		int errorLine; 
		int errorColumn;
		if ( setContent(&config, &errorMsg, &errorLine, &errorColumn) )
		{
			m_isOk = true;
		}
		else
		{
			qDebug(QObject::tr("Configuration file is corrupted %1:%2: %3").arg(errorLine).arg(errorColumn).arg(errorMsg) );
			m_isOk = false;
		}
		config.close();
	}
}


KTConfigDocument::~KTConfigDocument()
{
}

void KTConfigDocument::setHome(const QString &home)
{
	QDomElement kthome = createElement("KTHome");
	kthome.setAttribute("path", home);
	
	if ( ! exists("KTHome" ) )
	{
		documentElement().appendChild(kthome);
	}
	else
	{
		QDomNodeList list = documentElement().elementsByTagName("KTHome");
		documentElement().replaceChild(kthome, list.item(0) );
	}
}

void KTConfigDocument::setLang(const QString &lang)
{
	QDomElement ktlang = createElement("Lang");
	ktlang.setAttribute("id", lang);
	
	if ( ! exists("Lang" ) )
	{
		documentElement().appendChild(ktlang);
	}
	else
	{
		QDomNodeList list = documentElement().elementsByTagName("Lang");
		documentElement().replaceChild(ktlang, list.item(0) );
	}
}

void KTConfigDocument::setRepository(const QString &repository)
{
	QDomElement ktrepository = createElement("Repository");
	ktrepository.setAttribute("path", repository);
	
	if ( ! exists("Repository" ) )
	{
		documentElement().appendChild(ktrepository);
	}
	else
	{
		QDomNodeList list = documentElement().elementsByTagName("Repository");
		documentElement().replaceChild(ktrepository, list.item(0) );
	}
}

void KTConfigDocument::setThemePath(const QString &theme)
{
	QDomElement kttheme = createElement("KTTheme");
	kttheme.setAttribute("path", theme);
	
	if ( ! exists("KTTheme" ) )
	{
		documentElement().appendChild(kttheme);
	}
	else
	{
		QDomNodeList list = documentElement().elementsByTagName("KTTheme");
		documentElement().replaceChild(kttheme, list.item(0) );
	}
}

void KTConfigDocument::addRecentFiles(const QStringList &names)
{
	QDomElement root = documentElement();

	QDomElement recent;
	QDomNode n = root.firstChild();
	bool exists = false;
	while( !n.isNull() )
	{
		QDomElement e = n.toElement();
		if( !e.isNull() ) 
		{
			if ( e.tagName() == "Recent" )
			{
				recent = e; // Find Recent element
				exists = true;
				break;
			}
		}
		n = n.nextSibling();
	}
	
	if ( exists )
	{
		QDomNodeList list = recent.elementsByTagName("file");

		Q_ASSERT(list.count() == names.count());
		
		for (uint i = 0; i < names.count(); i++)
		{
			QDomElement e = list.item(i).toElement();
			if ( names[i] == "" )
			{
				e.setAttribute("name", /*QStyleSheet::escape(*/QObject::tr("<empty>")/*)*/);
			}
			else
			{
				e.setAttribute("name", names[i]);
			}
			recent.replaceChild(e, list.item(i));
		}
	}
	else
	{
		recent = createElement("Recent");
		for(uint i = 0; i < names.count(); i++)
		{
			QDomElement e = createElement("file");
			e.setAttribute("name", /*QStyleSheet::escape(*/QObject::tr("<empty>"))/*)*/;
			recent.appendChild(e);
		}
		
		root.appendChild(recent);
	}
}

QString KTConfigDocument::path()
{
	return m_path;
}

bool KTConfigDocument::exists(const QString &key)
{
	QDomElement root = documentElement();

	QDomElement recent;
	QDomNode n = root.firstChild();
	
	bool exists = false;
	while( !n.isNull() )
	{
		QDomElement e = n.toElement();
		if( !e.isNull() ) 
		{
			if ( e.tagName() == key )
			{
				recent = e; // Find Recent element
				exists = true;
				break;
			}
		}
		n = n.nextSibling();
	}
	
	return exists;
	
}

void KTConfigDocument::saveConfig(const QString &file)
{
	QFile *f = 0;
	if ( file.isNull() || file.isEmpty() )
	{
		f = new QFile(path());
	}
	else
	{
		f = new QFile(file);
	}
		
	if ( f->open(QIODevice::WriteOnly) )
	{
		QTextStream st( f );
		st << toString() << endl;
		m_isOk = true;
		f->close();
	}
	else
	{
		m_isOk = false;
	}
}

bool KTConfigDocument::isOk()
{
	return m_isOk;
}
