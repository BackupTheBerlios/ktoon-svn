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

#include "ktlibrary.h"

#include "ktproject.h"

#include <dcore/ddebug.h>

KTLibrary::KTLibrary(const QString &id, KTProject *parent) : KTLibraryFolder(id, parent)
{
}


KTLibrary::~KTLibrary()
{
}

void KTLibrary::fromXml(const QString &xml )
{
	QDomDocument document;
	
	if(! document.setContent(xml) )
	{
		return;
	}
	
	QDomElement root = document.documentElement();
	QDomNode n = root.firstChild();
	
	while( !n.isNull() )
	{
		QDomElement e = n.toElement();
		
		if(!e.isNull())
		{
			if( e.tagName() == "library" )
			{
				
			}
			else if( e.tagName() == "folder" )
			{
				QString doc;
				{
					QTextStream ts(&doc);
					ts << n;
				}
				
				KTLibraryFolder::fromXml(doc);
			}
		}
		
		n = n.nextSibling();
	}
}


QDomElement KTLibrary::toXml(QDomDocument &doc) const
{
	QDomElement root = doc.createElement("library");
	
	root.appendChild(KTLibraryFolder::toXml(doc));
	
	return root;
}


