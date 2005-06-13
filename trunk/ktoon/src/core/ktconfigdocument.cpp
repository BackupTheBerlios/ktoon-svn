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

#include "ktconfigdocument.h"

#include <qfile.h>

#include <iostream>

KTConfigDocument::KTConfigDocument(const QString &path) : QDomDocument(), m_path(path), MAXRECENTS(6)
{
	qDebug("[Initializing KTConfigDocument]");
	QDomProcessingInstruction header = this->createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
	this->appendChild(header);
	
	QDomElement root = createElement( "KTConfig" );
	appendChild( root );
}


KTConfigDocument::~KTConfigDocument()
{
}

void KTConfigDocument::setHome(const QString &home)
{
	QDomElement kthome = createElement("KTHome");
	kthome.setAttribute("path", home);
	documentElement().appendChild(kthome);
}

void KTConfigDocument::setLang(const QString &lang)
{
	QDomElement ktlang = createElement("Lang");
	ktlang.setAttribute("id", lang);
	documentElement().appendChild(ktlang);
}

void KTConfigDocument::setRepository(const QString &repository)
{
	QDomElement ktrepository = createElement("Repository");
	ktrepository.setAttribute("path", repository);
	documentElement ().appendChild(ktrepository);
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
				e.setAttribute("name", "<empty>");
			}
			else
				e.setAttribute("name", names[i]);
			
			recent.replaceChild(e, list.item(i));
		}
	}
	else
	{
		recent = createElement("Recent");
		for(uint i = 0; i < names.count(); i++)
		{
			QDomElement e = createElement("file");
			e.setAttribute("name", "<empty>");
			recent.appendChild(e);
		}
		
		root.appendChild(recent);
	}
}

QString KTConfigDocument::path()
{
	return m_path;
}

void KTConfigDocument::saveConfig(const QString &file)
{
	QFile *f;
	if ( file.isNull() )
	{
		f = new QFile(path());
	}
	else
	{
		f = new QFile(file);
	}			
	if ( f->open(IO_WriteOnly) )
	{
		QTextStream st( f );
		st << toString() << endl;
	}
}



