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

#include "color.h"

//-------------- CONSTRUCTOR ---------------

Color::Color()
{
    name = "";
}

Color::Color( float r, float g, float b, float a ): red( r ), green( g ), blue ( b ), alpha( a )
{
    name = "";
}

//------------- DESTRUCTOR ------------------

Color::~Color()
{

}

//------------ PUBLIC MEMBERS ---------------

void Color::setNameColor( const QString & _name )
{
    name = _name;
}

void Color::setColorRed( const float & _red )
{
    red = _red;
}

void Color::setColorGreen( const float & _green )
{
    green = _green;
}

void Color::setColorBlue( const float & _blue )
{
    blue = _blue;
}

void Color::setColorAlpha( const float & _alpha )
{
    alpha = _alpha;
}

QString Color::nameColor() const
{
    return name;
}

float Color::colorRed() const
{
    return red;
}

float Color::colorGreen() const
{
    return green;
}

float Color::colorBlue() const
{
    return blue;
}

float Color::colorAlpha() const
{
    return alpha;
}

int Color::idColor() const
{
    return id_color;
}

QDomElement Color::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Color" );

    e.setAttribute( "Name", name );
    e.setAttribute( "Red", ( double )red );
    e.setAttribute( "Green", ( double )green );
    e.setAttribute( "Blue", ( double )blue );
    e.setAttribute( "Alpha", ( double )alpha );

    return e;
}
