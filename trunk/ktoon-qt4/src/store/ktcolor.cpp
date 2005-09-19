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

#include "ktcolor.h"

//-------------- CONSTRUCTOR ---------------

KTColor::KTColor()
{
	name = "";
}

KTColor::KTColor( float r, float g, float b, float a ): red( r ), green( g ), blue ( b ), alpha( a )
{
	name = "";
}

//------------- DESTRUCTOR ------------------

KTColor::~KTColor()
{
}

//------------ PUBLIC MEMBERS ---------------

void KTColor::setNameColor( const QString & _name )
{
	name = _name;
}

void KTColor::setColorRed( const float & _red )
{
	red = _red;
}

void KTColor::setColorGreen( const float & _green )
{
	green = _green;
}

void KTColor::setColorBlue( const float & _blue )
{
	blue = _blue;
}

void KTColor::setColorAlpha( const float & _alpha )
{
	alpha = _alpha;
}

QString KTColor::nameColor() const
{
	return name;
}

float KTColor::colorRed() const
{
	return red;
}

float KTColor::colorGreen() const
{
	return green;
}

float KTColor::colorBlue() const
{
	return blue;
}

float KTColor::colorAlpha() const
{
	return alpha;
}

int KTColor::idColor() const
{
	return id_color;
}

QDomElement KTColor::createXML( QDomDocument &doc )
{
	QDomElement e = doc.createElement( "Color" );

	e.setAttribute( "Name", name );
	e.setAttribute( "Red", ( double )red );
	e.setAttribute( "Green", ( double )green );
	e.setAttribute( "Blue", ( double )blue );
	e.setAttribute( "Alpha", ( double )alpha );

	return e;
}
