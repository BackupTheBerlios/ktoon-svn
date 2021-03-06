/***************************************************************************
 *   Copyright (C) 2004 by Simena Dinas                                    *
 *   dsimena@toonka.com                                                    *
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

#include <qobject.h>

#include "palette.h"

//-------------- CONSTRUCTOR ---------------

Palette::Palette()
{

}

//------------- DESTRUCTOR ------------------

Palette::~Palette()
{

}

void Palette::setColors( QPtrList<Color> _colors )
{
    colors = _colors;
}


QPtrList<Color> Palette::getColors() const
{
    return colors;
}


QDomElement Palette::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Palette" );

    Color *c_it;
    for ( c_it = colors.first(); c_it; c_it = colors.next() )
    {
	QDomElement color_tag = c_it -> createXML( doc );
	e.appendChild( color_tag );
    }

    return e;
}
