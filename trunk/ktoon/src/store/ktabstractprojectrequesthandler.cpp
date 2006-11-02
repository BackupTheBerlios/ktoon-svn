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

#include "ktabstractprojectrequesthandler.h"


#include "ktitemrequest.h"
#include "ktprojectrequest.h"
#include "ktframerequest.h"
#include "ktlayerrequest.h"
#include "ktscenerequest.h"
#include "ktpaintareaevent.h"

KTAbstractProjectRequestHandler::KTAbstractProjectRequestHandler()
{
}


KTAbstractProjectRequestHandler::~KTAbstractProjectRequestHandler()
{
}

bool KTAbstractProjectRequestHandler::handleRequest(KTProjectRequest *event)
{
	switch ( event->id())
	{
		case KTProjectRequest::Item:
		{
			itemRequest( static_cast<KTItemRequest *>(event) );
		}
		break;
		case KTProjectRequest::Project:
		{
			projectRequest( event );
		}
		break;
		case KTProjectRequest::Frame:
		{
			frameRequest( static_cast<KTFrameRequest *>(event) );
		}
		break;
		case KTProjectRequest::Layer:
		{
			layerRequest( static_cast<KTLayerRequest *>(event) );
		}
		break;
		case KTProjectRequest::Scene:
		{
			sceneRequest( static_cast<KTSceneRequest *>(event) );
		}
		break;
		case KTProjectRequest::Library:
		{
			libraryRequest( event );
		}
		break;
		default:
		{
			qWarning("Unknown project event!");
			return false;
		}
		break;
	}
	
	return true;
}

// void KTAbstractProjectRequestHandler::frameRequest(KTFrameRequest *frameRequest)
// {
// }
// 
// void KTAbstractProjectRequestHandler::layerRequest(KTLayerRequest *layerRequest)
// {
// }
// 
// void KTAbstractProjectRequestHandler::sceneRequest(KTSceneRequest *sceneRequest)
// {
// }
// 
// void KTAbstractProjectRequestHandler::projectRequest(KTProjectRequest *projectRequest)
// {
// }

