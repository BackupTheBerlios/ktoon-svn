/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#include "ktframeevent.h"
#include <ddebug.h>

KTFrameEvent::KTFrameEvent(Action action, int sceneIndex, int layerIndex, int frameIndex, QObject *parent ) : KTLayerEvent(action, sceneIndex, layerIndex, parent), m_frameIndex(frameIndex)
{
	DINIT;
}


KTFrameEvent::~KTFrameEvent()
{
	DEND;
}

int KTFrameEvent::id() const
{
	return KTProjectEvent::Frame;
}

int KTFrameEvent::frameIndex() const
{
	return m_frameIndex;
}


bool KTFrameEvent::isValid() const
{
	return KTSceneEvent::isValid() && KTLayerEvent::isValid() && (m_frameIndex >= 0);
}


KTProjectEvent *KTFrameEvent::clone() const
{
	KTFrameEvent *event = new KTFrameEvent(action(), sceneIndex(), layerIndex(), m_frameIndex, parent());
	
	event->setPartName( event->partName() );
	
	return event;
}


/////////// Move Frame Event

KTMoveFrameEvent::KTMoveFrameEvent(int sceneIndex, int layerIndex, int frameIndex, int newFrameIndex, QObject *parent) : KTFrameEvent(KTProjectEvent::Move, sceneIndex, layerIndex, frameIndex, parent), m_newFrameIndex(newFrameIndex)
{
}


KTMoveFrameEvent::~KTMoveFrameEvent()
{
}


bool KTMoveFrameEvent::isValid() const
{
	return KTFrameEvent::isValid() && ( m_newFrameIndex >= 0 );
}

KTProjectEvent *KTMoveFrameEvent::clone() const
{
	KTMoveFrameEvent *event = new KTMoveFrameEvent( sceneIndex(), layerIndex(), frameIndex(), m_newFrameIndex );
	
	return event;
}


int KTMoveFrameEvent::newFrameIndex() const
{
	return m_newFrameIndex;
}



/////////// Lock Frame Event

KTLockFrameEvent::KTLockFrameEvent(int sceneIndex, int layerIndex, int frameIndex, bool lock, QObject *parent) : KTFrameEvent(KTProjectEvent::Lock, sceneIndex, layerIndex, frameIndex, parent), m_isLocked(lock)
{
}


KTLockFrameEvent::~KTLockFrameEvent()
{
}

bool KTLockFrameEvent::isLocked() const
{
	return m_isLocked;
}

KTProjectEvent *KTLockFrameEvent::clone() const
{
	KTLockFrameEvent *event = new KTLockFrameEvent( sceneIndex(), layerIndex(), frameIndex(), m_isLocked );
	
	return event;
}


/////////// Rename Frame Event

KTRenameFrameEvent::KTRenameFrameEvent(int sceneIndex, int layerIndex, int frameIndex, const QString &newName, QObject *parent) : KTFrameEvent(KTProjectEvent::Rename, sceneIndex, layerIndex, frameIndex, parent), m_newName(newName)
{
}


KTRenameFrameEvent::~KTRenameFrameEvent()
{
}


QString KTRenameFrameEvent::newName() const
{
	return m_newName;
}


KTProjectEvent *KTRenameFrameEvent::clone() const
{
	KTRenameFrameEvent *event = new KTRenameFrameEvent( sceneIndex(), layerIndex(), frameIndex(), m_newName, parent() );
	
	event->setPartName( partName() );
	
	return event;
}



