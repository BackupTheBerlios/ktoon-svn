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
 
#include "kthemedocument.h"

KThemeDocument::KThemeDocument() : QDomDocument()
{
	QDomProcessingInstruction header = this->createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
	this->appendChild(header);
	
	QDomElement root = createElement( "KTheme" );
	
	appendChild( root );
}

KThemeDocument::KThemeDocument(const QString &name, const QString &version) : QDomDocument()
{
	QDomProcessingInstruction header = this->createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
	this->appendChild(header);
	
	QDomElement root = createElement( "KTheme" );
	root.setAttribute("name", name);
	root.setAttribute("version", version);
	
	appendChild( root );
}


KThemeDocument::~KThemeDocument()
{
}

void KThemeDocument::addGeneralSection(ThemeKey tk)
{
	QDomElement general = createElement("General");
	QStringList keys = tk.keys();
	QStringList values = tk.values();
	for(int i = 0; i < keys.count(); i++)
	{
		QDomElement e = createElement(keys[i]);
		e.setAttribute("color", values[i]);
		general.appendChild(e);
	}
	documentElement().appendChild(general);
}

void KThemeDocument::addEffectsSection(ThemeKey tk)		
{
	QDomElement general = createElement("Effects");
	QStringList keys = tk.keys();
	QStringList values = tk.values();
	for(int i = 0; i < keys.count(); i++)
	{
		QDomElement e = createElement(keys[i]);
		e.setAttribute("color", values[i]);
		general.appendChild(e);
	}
	documentElement().appendChild(general);
}

void KThemeDocument::addSelections(ThemeKey tk)
{
	QDomElement general = createElement("Selections");
	QStringList keys = tk.keys();
	QStringList values = tk.values();
	for(int i = 0; i < keys.count(); i++)
	{
		QDomElement e = createElement(keys[i]);
		e.setAttribute("color", values[i]);
		general.appendChild(e);
	}
	documentElement().appendChild(general);
}

void KThemeDocument::addTextEffect(ThemeKey tk)
{
	QDomElement general = createElement("TextEffects");
	QStringList keys = tk.keys();
	QStringList values = tk.values();
	for(int i = 0; i < keys.count(); i++)
	{
		QDomElement e = createElement(keys[i]);
		e.setAttribute("color", values[i]);
		general.appendChild(e);
	}
	documentElement().appendChild(general);
}

