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

KTFrameEvent::KTFrameEvent(Action action, int sceneIndex, int layerIndex, int frameIndex, const QVariant &data ) : KTLayerEvent(action, sceneIndex, layerIndex, data), m_frameIndex(frameIndex)
{
}


KTFrameEvent::~KTFrameEvent()
{
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
	KTFrameEvent *event = new KTFrameEvent(action(), sceneIndex(), layerIndex(), m_frameIndex, data());
	
	event->setPartName( partName() );
	
	return event;
}

