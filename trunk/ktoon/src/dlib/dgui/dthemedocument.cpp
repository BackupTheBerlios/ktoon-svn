/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@gmail.com   *
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
 
#include "dthemedocument.h"


DThemeDocument::DThemeDocument() : QDomDocument()
{
	QDomProcessingInstruction header = this->createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
	this->appendChild(header);
	
	QDomElement root = createElement( "DTheme" );
	
	appendChild( root );
}

DThemeDocument::DThemeDocument(const QString &name, const QString &version) : QDomDocument()
{
	QDomProcessingInstruction header = this->createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
	this->appendChild(header);
	
	QDomElement root = createElement( "DTheme" );
	root.setAttribute("name", name);
	root.setAttribute("version", version);
	
	appendChild( root );
}


DThemeDocument::~DThemeDocument()
{
}

void DThemeDocument::addGeneralSection(ThemeKey tk)
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

void DThemeDocument::addEffectsSection(ThemeKey tk)		
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

void DThemeDocument::addSelections(ThemeKey tk)
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

void DThemeDocument::addTextEffect(ThemeKey tk)
{
	QDomElement general = createElement("TextEffects");
	QStringList keys = tk.keys();
	QStringList values = tk.values();
	for(uint i = 0; i < keys.count(); i++)
	{
		QDomElement e = createElement(keys[i]);
		e.setAttribute("color", values[i]);
		general.appendChild(e);
	}
	documentElement().appendChild(general);
}

