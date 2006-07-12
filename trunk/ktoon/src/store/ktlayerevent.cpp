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

#include "ktlayerevent.h"
#include <ddebug.h>

KTLayerEvent::KTLayerEvent(Action action, int sceneIndex, int layerIndex, QObject *parent) : KTSceneEvent(action, sceneIndex, parent), m_layerIndex(layerIndex)
{
	DINIT;
}


KTLayerEvent::~KTLayerEvent()
{
	DEND;
}

int KTLayerEvent::id() const
{
	return KTProjectEvent::Layer;
}

int KTLayerEvent::layerIndex() const
{
	return m_layerIndex;
}

bool KTLayerEvent::isValid() const
{
	return KTSceneEvent::isValid() && (m_layerIndex >= 0 );
}

KTProjectEvent *KTLayerEvent::clone() const
{
	KTLayerEvent *event = new KTLayerEvent(action(), sceneIndex(), m_layerIndex, parent());
	
	event->setPartName( event->partName());
	return event;
}

/////////// Move Layer Event

KTMoveLayerEvent::KTMoveLayerEvent(int sceneIndex, int layerIndex, int newIndex, QObject *parent) : KTLayerEvent(KTProjectEvent::Move, sceneIndex, layerIndex, parent), m_newLayerIndex(newIndex)
{
}


KTMoveLayerEvent::~KTMoveLayerEvent()
{
}


bool KTMoveLayerEvent::isValid() const
{
	return KTLayerEvent::isValid() && ( m_newLayerIndex >= 0 );
}

KTProjectEvent *KTMoveLayerEvent::clone() const
{
	KTMoveLayerEvent *event = new KTMoveLayerEvent( sceneIndex(), layerIndex(), m_newLayerIndex );
	
	return event;
}


int KTMoveLayerEvent::newLayerIndex() const
{
	return m_newLayerIndex;
}



/////////// Lock Layer Event

KTLockLayerEvent::KTLockLayerEvent(int sceneIndex, int layerIndex, bool lock, QObject *parent) : KTLayerEvent(KTProjectEvent::Lock, sceneIndex, layerIndex, parent), m_isLocked(lock)
{
}


KTLockLayerEvent::~KTLockLayerEvent()
{
}

bool KTLockLayerEvent::isLocked() const
{
	return m_isLocked;
}

KTProjectEvent *KTLockLayerEvent::clone() const
{
	KTLockLayerEvent *event = new KTLockLayerEvent( sceneIndex(), layerIndex(), m_isLocked );
	
	return event;
}


/////////// Rename Layer Event

KTRenameLayerEvent::KTRenameLayerEvent(int sceneIndex, int layerIndex, const QString &newName, QObject *parent) : KTLayerEvent(KTProjectEvent::Rename, sceneIndex, layerIndex, parent), m_newName(newName)
{
}


KTRenameLayerEvent::~KTRenameLayerEvent()
{
}


QString KTRenameLayerEvent::newName() const
{
	return m_newName;
}


KTProjectEvent *KTRenameLayerEvent::clone() const
{
	KTRenameLayerEvent *event = new KTRenameLayerEvent( sceneIndex(), layerIndex(), m_newName, parent() );
	
	event->setPartName( partName() );
	
	return event;
}



