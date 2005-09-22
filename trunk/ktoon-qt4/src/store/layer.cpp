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
//Added by qt3to4:
#include <QList>
 
//-------------- CONSTRUCTOR ---------------

Layer::Layer()
{
	m_index = -1;
	m_name = "";
//     KeyFrame *d_keyframe = new KeyFrame();
//     try {
//         d_keyframe -> setLengthKeyFrame( 1 );
//         d_keyframe -> setOffsetKeyFrame( 1 );
//         d_keyframe -> setNameKeyFrame( QObject::tr( "Drawing " ) + QString( "1-1" ) );
//         keyframes.append( d_keyframe );
//         }
//     catch(...)
//         {
// 	  delete d_keyframe;
// 	  throw;
// 	  }
}

//------------- DESTRUCTOR ------------------

Layer::~Layer()
{
	qDeleteAll(m_keyframes);
//     keyframes.setAutoDelete( true );
//     keyframes.clear();
//     keyframes.setAutoDelete( false );
}

//------------ PUBLIC MEMBERS ---------------

void Layer::setIndexLayer( const int & index )
{
	m_index = index;
}

void Layer::setNameLayer( const QString & name )
{
	m_name = name;
}

void Layer::setKeyFrames( QList<KeyFrame*> frames )
{
	m_keyframes = frames;
}

int Layer::indexLayer() const
{
	return m_index;
}

QString Layer::nameLayer() const
{
	return m_name;
}

QList<KeyFrame*> Layer::keyFrames() const
{
	return m_keyframes;
}

KeyFrame *Layer::keyFrameAt( int start, int end )
{
	for( int i = 0; i < m_keyframes.count(); i++)
	{
		KeyFrame *kfIt = m_keyframes[i];
		int kfOffset = kfIt->offsetKeyFrame();
		int kfLength = kfIt->lengthKeyFrame();
		int limit = kfOffset + kfLength - 1;
		if ( limit >= start && kfOffset <= end )
			return kfIt;
	}
	return NULL;
}

QDomElement Layer::createXML( QDomDocument &doc )
{
	QDomElement e = doc.createElement( "Layer" );
	e.setAttribute( "Id", m_index );
	e.setAttribute( "Name", m_name );
	QDomElement kTag = doc.createElement( "Keyframes" );
	e.appendChild( kTag );
	for( int i = 0; i < m_keyframes.count(); i++)
	{
		KeyFrame *kIt = m_keyframes [i];
		QDomElement keyframeTag = kIt -> createXML( doc );
		kTag.appendChild( keyframeTag );
	}
	return e;
}
