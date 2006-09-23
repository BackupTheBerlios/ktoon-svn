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

#include "ktlayerrequest.h"
#include <ddebug.h>

KTLayerRequest::KTLayerRequest(Action action, int sceneIndex, int layerIndex, const QVariant &data) : KTSceneRequest(action, sceneIndex, data), m_layerIndex(layerIndex)
{
}


KTLayerRequest::~KTLayerRequest()
{
}

int KTLayerRequest::id() const
{
	return KTProjectRequest::Layer;
}

int KTLayerRequest::layerIndex() const
{
	return m_layerIndex;
}

bool KTLayerRequest::isValid() const
{
	return KTSceneRequest::isValid() && (m_layerIndex >= 0 );
}

KTProjectRequest *KTLayerRequest::clone() const
{
	KTLayerRequest *event = new KTLayerRequest(action(), sceneIndex(), m_layerIndex, data());
	
	event->setPartName( partName());
	
	return event;
}

