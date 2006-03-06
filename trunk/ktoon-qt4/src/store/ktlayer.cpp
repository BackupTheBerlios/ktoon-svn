/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include "ktlayer.h"
#include "ktdebug.h"

KTLayer::KTLayer(QObject *parent) : KTSerializableObject(parent), m_isVisible(true), m_name(tr("Layer")), m_currentFrame(0), m_framesCount(0)/*, m_copyFrame(0)*/
{
// 	m_currentFrame = createFrame();
}

KTLayer::KTLayer(const QString &layerName, QObject * parent)  : KTSerializableObject(parent), m_isVisible(true), m_name(layerName), m_currentFrame(0), m_framesCount(0)/*, m_copyFrame(0)*/
{
// 	m_currentFrame = createFrame();
}

KTLayer::~KTLayer()
{
	KTEND;
	for(int i = 0; i < m_frames.count(); i++ )
	{
		delete m_frames.takeAt(i);
	}
}

Frames KTLayer::frames()
{
	return m_frames;
}

void KTLayer::setFrames(const Frames &frames)
{
	m_frames = frames;
}

KTKeyFrame *KTLayer::createFrame(const QString& name, bool addToEnd)
{
	ktDebug() << "createFrame(const QString&" <<  name.isNull() << "," << " bool" <<  addToEnd << ")" ;
	
	KTKeyFrame *keyFrame = new KTKeyFrame(this);
	if(name.isNull())
	{
		keyFrame->setFrameName(tr("Drawing %1").arg(m_framesCount++));
	}
	else
	{
		keyFrame->setFrameName( name);
	}
	if ( addToEnd )
	{
		m_frames << keyFrame;
	}
	else
	{
		m_frames.insert(m_frames.indexOf(m_currentFrame)+1, keyFrame);
	}
	
	m_currentFrame = keyFrame;

	emit frameCreated( keyFrame->frameName(), addToEnd );
	
	return keyFrame;
}

KTKeyFrame *KTLayer::currentFrame()
{
	return m_currentFrame;
}

void KTLayer::setCurrentFrame(int index)
{
	KTKeyFrame *frame = m_frames[index]; 
	
	if ( frame )
	{
		m_currentFrame = frame;
	}
}

int KTLayer::indexCurrentFrame()
{
	return m_frames.indexOf(m_currentFrame);
}


void KTLayer::setLayerName(const QString &name)
{
	m_name = name;
}

void KTLayer::pasteFrame(const int& index, KTKeyFrame* copy)
{
	if ( index > m_frames.count() )
	{
		m_frames << copy;
	}
	else
	{
		m_frames.replace(index, copy);
	}
}


void KTLayer::moveCurrentFrame( bool up)
{
	if(m_currentFrame)
	{
		if(up )
		{
			if(m_frames.indexOf(m_currentFrame) > 0)
			{
				m_frames.swap ( indexCurrentFrame(), indexCurrentFrame()-1);
				emit frameMoved(up);
			}
		}
		else if(m_frames.indexOf(m_currentFrame) < m_frames.count()-1)
		{
			m_frames.swap ( indexCurrentFrame(), indexCurrentFrame()+1);
			emit frameMoved(up);
		}
	}
}

void KTLayer::removeCurrentFrame()
{
// 	ktDebug() << "emit KTLayer::removeCurrentFrame()";
	if(m_currentFrame)
	{
		m_frames.removeAt( indexCurrentFrame() );
		emit frameRemoved();
	}
}

void KTLayer::lockCurrentFrame()
{
	if ( m_currentFrame)
	{
		m_currentFrame->setLocked(!m_currentFrame->isLocked());
		emit frameLocked(/*m_currentFrame->isLocked()*/);
	}
}

void KTLayer::setVisible(bool isVisible)
{
// 	ktDebug() << "KTLayer::setVisible(" << isVisible << ")" << endl;
	m_isVisible = isVisible;
	emit visibilityChanged(isVisible);
}

QString KTLayer::layerName() const
{
	return m_name;
}

bool KTLayer::isVisible()
{
	return m_isVisible;
}


QDomElement KTLayer::createXML( QDomDocument &doc )
{
	QDomElement layer = doc.createElement("Layer");
	layer.setAttribute ( "name", m_name);
	Frames::ConstIterator iterator = m_frames.begin();
	
	while( iterator != m_frames.end() )
	{
		layer.appendChild((*iterator)->createXML(doc));
		++iterator;
	}
	
	return layer;
}



