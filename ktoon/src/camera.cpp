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

#include "camera.h"

Camera::Camera()
{
    position = Point3D();
    center = Point3D();
    up = Point3D();
}

Camera::~Camera()
{

}

void Camera::setPositionCamera( const Point3D & _position )
{
position = _position;
}

void Camera::setCenterCamera( const Point3D & _center )
{
center = _center;
}

void Camera::setUpCamera( const Point3D & _up )
{
up = _up;
}

Point3D Camera::positionCamera( ) const
{
 return position;
}

Point3D Camera::centerCamera( ) const
{
 return center;
}

Point3D Camera::upCamera( ) const
{
 return up;
}

QDomElement Camera::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Camera" );

    QDomElement p_tag = doc.createElement( "Position" );
    e.appendChild( p_tag );
    QDomElement position_tag = position.createXML( doc );
    p_tag.appendChild( position_tag );

    QDomElement c_tag = doc.createElement( "Center" );
    e.appendChild( c_tag );
    QDomElement center_tag = center.createXML( doc );
    c_tag.appendChild( center_tag );

    QDomElement u_tag = doc.createElement( "Up" );
    e.appendChild( u_tag );
    QDomElement up_tag = up.createXML( doc );
    u_tag.appendChild( up_tag );

    return e;
}
