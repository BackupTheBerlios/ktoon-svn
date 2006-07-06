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

#include "ktabstractprojecteventhandler.h"

#include "ktprojectevent.h"
#include "ktframeevent.h"
#include "ktlayerevent.h"
#include "ktsceneevent.h"

KTAbstractProjectEventHandler::KTAbstractProjectEventHandler()
{
}


KTAbstractProjectEventHandler::~KTAbstractProjectEventHandler()
{
}

bool KTAbstractProjectEventHandler::handleEvent(KTProjectEvent *event)
{
	switch ( event->id())
	{
		case KTProjectEvent::Project:
		{
			projectEvent( event );
		}
		break;
		case KTProjectEvent::Frame:
		{
			frameEvent( static_cast<KTFrameEvent *>(event) );
		}
		break;
		case KTProjectEvent::Layer:
		{
			layerEvent( static_cast<KTLayerEvent *>(event) );
		}
		break;
		case KTProjectEvent::Scene:
		{
			sceneEvent( static_cast<KTSceneEvent *>(event) );
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

// void KTAbstractProjectEventHandler::frameEvent(KTFrameEvent *frameEvent)
// {
// }
// 
// void KTAbstractProjectEventHandler::layerEvent(KTLayerEvent *layerEvent)
// {
// }
// 
// void KTAbstractProjectEventHandler::sceneEvent(KTSceneEvent *sceneEvent)
// {
// }
// 
// void KTAbstractProjectEventHandler::projectEvent(KTProjectEvent *projectEvent)
// {
// }

