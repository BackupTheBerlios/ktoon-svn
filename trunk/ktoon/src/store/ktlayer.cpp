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
#include "ddebug.h"

KTLayer::KTLayer(QObject *parent) : QObject(parent), m_isVisible(true), m_name(tr("Layer")), m_framesCount(0), m_currentFrameIndex(-1)
{
}

KTLayer::KTLayer(const QString &layerName, QObject * parent)  : QObject(parent), m_isVisible(true), m_name(layerName), m_framesCount(0), m_currentFrameIndex(-1)
{
}

KTLayer::~KTLayer()
{
	qDeleteAll(m_frames.begin(), m_frames.end());
}

Frames KTLayer::frames()
{
	return m_frames;
}

void KTLayer::setFrames(const Frames &frames)
{
	m_frames = frames;
	m_framesCount = frames.count();
}

void KTLayer::setLayerName(const QString &name)
{
	m_name = name;
}

void KTLayer::setVisible(bool isVisible)
{
	m_isVisible = isVisible;
// 	emit visibilityChanged(isVisible);
}

QString KTLayer::layerName() const
{
	return m_name;
}

bool KTLayer::isVisible() const
{
	return m_isVisible;
}

KTFrame *KTLayer::createFrame(bool addToEnd)
{
	KTFrame *keyFrame = new KTFrame(this);
	
	m_framesCount++;
	
	keyFrame->setFrameName(tr("Drawing %1").arg(m_framesCount));
	
	if ( addToEnd )
	{
		m_currentFrameIndex = m_frames.count();
		m_frames << keyFrame;
	}
	else
	{
		m_currentFrameIndex++;
		m_frames.insert(m_currentFrameIndex, keyFrame);
	}
	
	emit frameCreated( keyFrame->frameName(), addToEnd );
	
	return keyFrame;
}

KTFrame *KTLayer::currentFrame()
{
	if ( m_currentFrameIndex >= 0 && m_currentFrameIndex < m_frames.count() )
	{
		return m_frames[m_currentFrameIndex];
	}
	
	return 0;
}

