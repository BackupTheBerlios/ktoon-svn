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

 #include "keyframe.h"

KeyFrame::KeyFrame()
{
    offset = 0;
    length = 1;
    motion = false;
    name = "";
    camera = new Camera();
    drawing = new GLDrawing();
}

KeyFrame::~KeyFrame()
{
    delete camera;
    delete drawing;
}

void KeyFrame::setOffsetKeyFrame( const int & _offset )
{
    offset = _offset;
}

void KeyFrame::setLengthKeyFrame( const int & _length )
{
    length = _length;
}

void KeyFrame::setMotionKeyFrame( bool _motion )
{
    motion = _motion;
}

void KeyFrame::setNameKeyFrame( const QString & _name )
{
    name = _name;
}

void KeyFrame::setDrawing( GLDrawing *_drawing )
{
    drawing = _drawing;
}

void KeyFrame::setCamera( Camera *_camera )
{
    camera = _camera;
}

int KeyFrame::offsetKeyFrame( ) const
{
    return offset;
}

int KeyFrame::lengthKeyFrame( ) const
{
    return length;
}

bool KeyFrame::motionKeyFrame() const
{
    return motion;
}

QString KeyFrame::nameKeyFrame( ) const
{
    return name;
}

GLDrawing *KeyFrame::getDrawing( ) const
{
    return drawing;
}

Camera *KeyFrame::getCamera() const
{
    return camera;
}

QDomElement KeyFrame::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Keyframe" );

    QString bool_str;
    if ( motion )
        bool_str = "true";
    else
        bool_str = "false";

    e.setAttribute( "Name", name );
    e.setAttribute( "Offset", offset );
    e.setAttribute( "Length", length );
    e.setAttribute( "Motion", bool_str );

    QDomElement camera_tag = camera -> createXML( doc );
    e.appendChild( camera_tag );

    QDomElement drawing_tag = drawing -> createXML( doc );
    e.appendChild( drawing_tag );

    return e;
}
