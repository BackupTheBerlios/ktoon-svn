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

#include "point3d.h"

//-------------- CONSTRUCTOR ---------------

Point3D::Point3D() : pos_x( 0 ), pos_y( 0 ), pos_z( 0 )
{

}

Point3D::Point3D( int _x, int _y , int _z ) : pos_x( _x ), pos_y( _y ), pos_z( _z )
{

}

//------------- DESTRUCTOR ------------------

Point3D::~Point3D()
{

}

//------------ PUBLIC MEMBERS ---------------

void Point3D::setX( const int & x )
{
 pos_x = x;
}

void Point3D::setY( const int & y )
{
 pos_y = y;
}

void Point3D::setZ( const int & z )
{
 pos_z = z;
}

int Point3D::x() const
{
 return pos_x;
}

int Point3D::y() const
{
 return pos_y;
}

int Point3D::z() const
{
 return pos_z;
}

QDomElement Point3D::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( QObject::tr( "Point" ) + QString( "3D" ) );

    e.setAttribute( "X", pos_x );
    e.setAttribute( "Y", pos_y );
    e.setAttribute( "Z", pos_z );

    return e;
}
