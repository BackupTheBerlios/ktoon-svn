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

#include "animation.h"

Animation::Animation()
{
    frame_rate = 1;
    camera_length = 160;
    camera_width = 120;

    Scene *d_scene = new Scene();
    d_scene -> setNameScene( QObject::tr( "Scene" ) + QString( "1" ) );
    scenes.append( d_scene );
}

Animation::~Animation()
{
    scenes.setAutoDelete( true );
    scenes.clear();
    scenes.setAutoDelete( false );
}

void Animation::setFrameRate( const int & _rate )
{
 frame_rate = _rate;
}

void Animation::setCameraWidth ( const int & _width )
{
 camera_width = _width;
}

void Animation::setCameraLength ( const int & _length )
{
 camera_length = _length;
}

void Animation::setScenes( QPtrList<Scene> _scenes )
{
  scenes = _scenes;
}

int Animation::frameRate() const
{
 return frame_rate;
}

int Animation::cameraWidth() const
{
 return camera_width;
}

int Animation::cameraLength() const
{
 return camera_length;
}

QPtrList<Scene> Animation::getScenes() const
{
  return scenes;
}

QDomElement Animation::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Animation" );

    e.setAttribute( "FrameRate", frame_rate );
    e.setAttribute( "CameraWidth", camera_width );
    e.setAttribute( "CameraLength", camera_length );

    QDomElement s_tag = doc.createElement( "Scenes" );
    e.appendChild( s_tag );
    Scene *s_it;
    for ( s_it = scenes.first(); s_it; s_it = scenes.next() )
    {
	QDomElement scene_tag = s_it -> createXML( doc );
	s_tag.appendChild( scene_tag );
    }

    return e;
}
