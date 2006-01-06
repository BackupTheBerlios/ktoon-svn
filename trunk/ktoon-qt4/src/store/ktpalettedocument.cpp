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

#include "ktpalettedocument.h"

KTPaletteDocument::KTPaletteDocument(const QString &name, bool isEditable) : QDomDocument()
{
	QDomProcessingInstruction header = this->createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
	this->appendChild(header);
	
	QDomElement root = createElement( "Palette" );
	root.setAttribute("name", name);
	
	if ( isEditable )
	{
		root.setAttribute("editable", "true" );
	}
	else
	{
		root.setAttribute("editable", "false" );
	}
	
	appendChild( root );	
}

KTPaletteDocument::~KTPaletteDocument()
{
}

void KTPaletteDocument::addColor(const QColor &color)
{
	QDomElement element = createElement("Color");
	
	element.setAttribute("colorName", color.name());
	element.setAttribute("alpha", QString::number(color.alpha()) );
	
	documentElement().appendChild(element);
	
}

void KTPaletteDocument::addGradient(const QGradient &gradient)
{
	QDomElement element = createElement("Gradient");
	
	element.setAttribute("type", gradient.type() );
	
	QGradientStops stops = gradient.stops();
	
	foreach(QGradientStop stop, stops)
	{
		QDomElement stopElement = createElement("Stop");
		stopElement.setAttribute("value", stop.first );
		stopElement.setAttribute("colorName", stop.second.name());
		stopElement.setAttribute("alpha", stop.second.alpha());
		
		element.appendChild(stopElement);
	}
	
	documentElement().appendChild(element);
}

void KTPaletteDocument::setElements(const QList<QBrush > &brushes)
{
	foreach( QBrush brush, brushes)
	{
		if ( brush.gradient() )
		{
			addGradient( *brush.gradient() );
		}
		else
		{
			addColor(brush.color());
		}
	}
}



