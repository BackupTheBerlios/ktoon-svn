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

#include "brush.h"

Brush::Brush()
{
    name = "";
    thickness_max = -1;
    thickness_min = -1;
    smoothness = -1;
    id_brush = -1;
}

Brush::Brush( int t_min, int t_max, int smooth ) : thickness_min( t_min ), thickness_max( t_max ), smoothness( smooth )
{
    name = "";
    id_brush = -1;
}

Brush::~Brush()
{

}

void Brush::setNameBrush( const QString & _name )
{
    name = _name;
}

void Brush::setThicknessMinBrush( const int & _thickness_min )
{
    thickness_min = _thickness_min;
}

void Brush::setThicknessMaxBrush( const int & _thickness_max )
{
    thickness_max = _thickness_max;
}

void Brush::setSmoothnessBrush( const int & _smoothness )
{
    smoothness = _smoothness;
}

QString Brush::nameBrush() const
{
    return name;
}

int Brush::thicknessMinBrush() const
{
    return thickness_min;
}

int Brush::thicknessMaxBrush() const
{
    return thickness_max;
}

int Brush::smoothnessBrush() const
{
    return smoothness;
}

int Brush::idBrush() const
{
    return id_brush;
}

QDomElement Brush::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Brush" );

    e.setAttribute( "Name", name );
    e.setAttribute( "MinThickness", thickness_min );
    e.setAttribute( "MaxThickness", thickness_max );
    e.setAttribute( "Smoothness", smoothness );
    e.setAttribute( "Id", id_brush );

    return e;
}
