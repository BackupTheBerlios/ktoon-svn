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

#include "ktitemrequest.h"
#include <ddebug.h>

KTItemRequest::KTItemRequest(Action action, int sceneIndex, int layerIndex, int frameIndex, int position, const QVariant &data) : KTFrameRequest(action, sceneIndex, layerIndex, frameIndex, data), m_position(position)
{
}


KTItemRequest::~KTItemRequest()
{
}


int KTItemRequest::id() const
{
	return KTProjectRequest::Item;
}


bool KTItemRequest::isValid() const
{
	return KTSceneRequest::isValid() && KTLayerRequest::isValid() && KTFrameRequest::isValid() && (m_position >= -1 );
}


KTProjectRequest *KTItemRequest::clone() const
{
	KTItemRequest *event = new KTItemRequest(action(), sceneIndex(), layerIndex(), frameIndex(), m_position, data() );
	
	event->setPartName( partName() );
	
	return event;
}

int KTItemRequest::itemIndex() const
{
	return m_position;
}

