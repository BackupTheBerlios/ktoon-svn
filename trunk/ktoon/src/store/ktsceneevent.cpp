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

#include "ktsceneevent.h"
#include <ddebug.h>

KTSceneEvent::KTSceneEvent(Action action, int sceneIndex, QObject *parent) : KTProjectEvent(action, parent), m_sceneIndex(sceneIndex)
{
	DINIT;
}


KTSceneEvent::~KTSceneEvent()
{
	DEND;
}

int KTSceneEvent::id() const
{
	return KTProjectEvent::Scene;
}


int KTSceneEvent::sceneIndex() const
{
	return m_sceneIndex;
}

bool KTSceneEvent::isValid() const
{
	if ( m_sceneIndex < 0 )
		return false;
	
	return true;
}

KTProjectEvent *KTSceneEvent::clone() const
{
	KTSceneEvent *event = new KTSceneEvent(action(), sceneIndex(), parent());
	
	event->setPartName( partName() );
	
	return event;
}


/////////// Move Scene Event

KTMoveSceneEvent::KTMoveSceneEvent(int sceneIndex, int newIndex, QObject *parent) : KTSceneEvent(KTProjectEvent::Move, sceneIndex, parent), m_newSceneIndex(newIndex)
{
}


KTMoveSceneEvent::~KTMoveSceneEvent()
{
}


bool KTMoveSceneEvent::isValid() const
{
	return KTSceneEvent::isValid() && ( m_newSceneIndex >= 0 );
}

KTProjectEvent *KTMoveSceneEvent::clone() const
{
	KTMoveSceneEvent *event = new KTMoveSceneEvent( sceneIndex(), m_newSceneIndex );
	
	return event;
}


int KTMoveSceneEvent::newSceneIndex() const
{
	return m_newSceneIndex;
}



/////////// Lock Scene Event

KTLockSceneEvent::KTLockSceneEvent(int sceneIndex, bool lock, QObject *parent) : KTSceneEvent(KTProjectEvent::Lock, sceneIndex, parent), m_isLocked(lock)
{
}


KTLockSceneEvent::~KTLockSceneEvent()
{
}

bool KTLockSceneEvent::isLocked() const
{
	return m_isLocked;
}

KTProjectEvent *KTLockSceneEvent::clone() const
{
	KTLockSceneEvent *event = new KTLockSceneEvent( sceneIndex(), m_isLocked );
	
	return event;
}


/////////// Rename Scene Event

KTRenameSceneEvent::KTRenameSceneEvent(int sceneIndex, const QString &newName, QObject *parent) : KTSceneEvent(KTProjectEvent::Rename, sceneIndex, parent), m_newName(newName)
{
}


KTRenameSceneEvent::~KTRenameSceneEvent()
{
}


QString KTRenameSceneEvent::newName() const
{
	return m_newName;
}


KTProjectEvent *KTRenameSceneEvent::clone() const
{
	KTRenameSceneEvent *event = new KTRenameSceneEvent( sceneIndex(), m_newName, parent() );
	
	event->setPartName( partName() );
	
	return event;
}

