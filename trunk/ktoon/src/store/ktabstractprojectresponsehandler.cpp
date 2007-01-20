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

#include "ktabstractprojectresponsehandler.h"


#include "ktprojectresponse.h"
#include "ktpaintareaevent.h"

KTAbstractProjectResponseHandler::KTAbstractProjectResponseHandler()
{
}


KTAbstractProjectResponseHandler::~KTAbstractProjectResponseHandler()
{
}

bool KTAbstractProjectResponseHandler::handleResponse(KTProjectResponse *response)
{
	switch ( response->part())
	{
		case KTProjectRequest::Item:
		{
			itemResponse( static_cast<KTItemResponse *>(response) );
		}
		break;
		case KTProjectRequest::Project:
		{
			projectResponse( response );
		}
		break;
		case KTProjectRequest::Frame:
		{
			frameResponse( static_cast<KTFrameResponse *>(response) );
		}
		break;
		case KTProjectRequest::Layer:
		{
			layerResponse(static_cast<KTLayerResponse *>(response));
		}
		break;
		case KTProjectRequest::Scene:
		{
			sceneResponse( static_cast<KTSceneResponse *>(response) );
		}
		break;
		case KTProjectRequest::Library:
		{
// 			libraryResponse( response );
		}
		break;
		default:
		{
			qWarning("Unknown project response!");
			return false;
		}
		break;
	}
	
	return true;
}
