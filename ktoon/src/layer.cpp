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

 #include "layer.h"
 
Layer::Layer()
{
    index = -1;
    name = "";
    KeyFrame *d_keyframe = new KeyFrame();
    d_keyframe -> setLengthKeyFrame( 1 );
    d_keyframe -> setOffsetKeyFrame( 1 );
    d_keyframe -> setNameKeyFrame( QObject::tr( "Drawing " ) + QString( "1-1" ) );
    keyframes.append( d_keyframe );
}

Layer::~Layer()
{
    keyframes.setAutoDelete( true );
    keyframes.clear();
    keyframes.setAutoDelete( false );
}

void Layer::setIndexLayer( const int & _index )
{
 index = _index;
}

void Layer::setNameLayer( const QString & _name )
{
 name = _name;
}

void Layer::setKeyFrames( QPtrList<KeyFrame> frames )
{
  keyframes = frames;
}

int Layer::indexLayer() const
{
 return index;
}

QString Layer::nameLayer() const
{
 return name;
}

QPtrList<KeyFrame> Layer::keyFrames() const
{
 return keyframes;
}

KeyFrame *Layer::keyFrameAt( int start, int end )
{
    KeyFrame *kf_it;
    for ( kf_it = keyframes.first(); kf_it; kf_it = keyframes.next() )
    {
	int kf_offset = kf_it -> offsetKeyFrame();
	int kf_length = kf_it -> lengthKeyFrame();
	int limit = kf_offset + kf_length - 1;
	if ( limit >= start && kf_offset <= end )
	    return kf_it;
    }

    return NULL;
}

QDomElement Layer::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Layer" );

    e.setAttribute( "Id", index );
    e.setAttribute( "Name", name );

    QDomElement k_tag = doc.createElement( "Keyframes" );
    e.appendChild( k_tag );
    KeyFrame *k_it;
    for ( k_it = keyframes.first(); k_it; k_it = keyframes.next() )
    {
	QDomElement keyframe_tag = k_it -> createXML( doc );
	k_tag.appendChild( keyframe_tag );
    }

    return e;
}
