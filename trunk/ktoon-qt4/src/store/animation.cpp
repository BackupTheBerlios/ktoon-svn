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
//Added by qt3to4:
#include <Q3PtrList>

#include "animation.h"

#include <memory>


//-------------- CONSTRUCTOR ---------------

Animation::Animation()
{
    frame_rate = 1;
    camera_length = 160;
    camera_width = 120;

    std::auto_ptr<Scene> ap_d_scene(new Scene);
    
    ap_d_scene.get() -> setNameScene( QObject::tr( "Scene" ) + QString( "1" ) );
    scenes.append( ap_d_scene.get() );
    
    ap_d_scene.release();
        
}

//------------- DESTRUCTOR ------------------

Animation::~Animation()
{
    scenes.setAutoDelete( true );
    scenes.clear();
    scenes.setAutoDelete( false );
}

//------------ PUBLIC MEMBERS ---------------

void Animation::setFrameRate( const int & _rate )
{
 Q_ASSERT( _rate > 0 );
 frame_rate = _rate;
}

void Animation::setCameraWidth ( const int & _width )
{
 Q_ASSERT( _width > 0 );
 camera_width = _width;
}

void Animation::setCameraLength ( const int & _length )
{
 Q_ASSERT( _length > 0 );
 camera_length = _length;
}

void Animation::setScenes( Q3PtrList<Scene> _scenes )
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

Q3PtrList<Scene> Animation::getScenes() const
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
