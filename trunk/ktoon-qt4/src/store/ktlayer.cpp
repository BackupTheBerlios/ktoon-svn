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

KTLayer::KTLayer(QObject *parent) : QObject(parent), m_isVisible(true), m_name(tr("Layer")), m_currentFrame(0), m_framesCount(0)
{
// 	m_currentFrame = createFrame();
}

KTLayer::KTLayer(const QString &layerName, QObject * parent)  : QObject(parent), m_isVisible(true), m_name(layerName), m_currentFrame(0), m_framesCount(0)
{
// 	m_currentFrame = createFrame();
}


KTLayer::~KTLayer()
{
}

Frames KTLayer::frames()
{
	return m_frames;
}

void KTLayer::setFrames(const Frames &frames)
{
	m_frames = frames;
}

KTKeyFrame *KTLayer::createFrame()
{
	QString name = tr("Drawing %1").arg(m_framesCount++);
	
	KTKeyFrame *keyFrame = new KTKeyFrame(this);
	keyFrame->setFrameName( name);
	
	m_currentFrame = keyFrame;
	
	m_frames << keyFrame;
	emit frameCreated( keyFrame->frameName() );
	
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

void KTLayer::setLayerName(const QString &name)
{
	m_name = name;
}

void KTLayer::setVisible(bool isVisible)
{
	m_isVisible = isVisible;
}

QString KTLayer::layerName() const
{
	return m_name;
}

bool KTLayer::isVisible()
{
	return m_isVisible;
}

