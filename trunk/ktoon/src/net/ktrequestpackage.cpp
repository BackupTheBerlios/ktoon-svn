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

#include "ktrequestpackage.h"
#include "ktitemrequest.h"

KTRequestPackage::KTRequestPackage(KTProjectRequest *request)
 : QDomDocument()
{
	QDomElement root = createElement( "request" );
	root.setAttribute( "action", request->action());
	root.setAttribute( "id", request->id() );
	root.setAttribute( "name", request->partName() );
	
	appendChild( root );
	
	handleRequest( request );
}


KTRequestPackage::~KTRequestPackage()
{
}


void KTRequestPackage::itemRequest(KTItemRequest *itemRequest)
{
	QDomElement scene = createElement("scene");
	scene.setAttribute("index", itemRequest->sceneIndex() );
	
	
	QDomElement layer = createElement("layer");
	layer.setAttribute("index", itemRequest->layerIndex() );
	
	
	scene.appendChild( layer );
	
	QDomElement frame = createElement("frame");
	frame.setAttribute("index", itemRequest->frameIndex() );
	
	
	layer.appendChild( frame );
	
	QDomElement item = createElement("item");
	item.setAttribute("index", itemRequest->itemIndex() );
	
	
	frame.appendChild( item );
	
	appendData( item, itemRequest->data() );
	
	documentElement().appendChild( scene );
}

void KTRequestPackage::frameRequest(KTFrameRequest *frameRequest)
{
	QDomElement scene = createElement("scene");
	scene.setAttribute("index", frameRequest->sceneIndex() );
	
	
	QDomElement layer = createElement("layer");
	layer.setAttribute("index", frameRequest->layerIndex() );
	
	
	scene.appendChild( layer );
	
	
	
	QDomElement frame = createElement("frame");
	frame.setAttribute("index", frameRequest->frameIndex() );
	
	
	layer.appendChild( frame );
	
	appendData( frame, frameRequest->data() );
	documentElement().appendChild( scene );
}

void KTRequestPackage::layerRequest(KTLayerRequest *layerRequest)
{
	QDomElement scene = createElement("scene");
	scene.setAttribute("index", layerRequest->sceneIndex() );
	
	
	QDomElement layer = createElement("layer");
	layer.setAttribute("index", layerRequest->layerIndex() );
	
	appendData( layer, layerRequest->data() );
	
	scene.appendChild( layer );
	
	documentElement().appendChild( scene );
}

void KTRequestPackage::sceneRequest(KTSceneRequest *sceneRequest)
{
	QDomElement scene = createElement("scene");
	scene.setAttribute("index", sceneRequest->sceneIndex() );
	
	
	appendData( scene, sceneRequest->data() );
	
	documentElement().appendChild( scene );
}

void KTRequestPackage::projectRequest(KTProjectRequest *projectRequest)
{
	Q_UNUSED(projectRequest);
}


void KTRequestPackage::appendData(QDomElement &element, const QVariant &data)
{
	QDomElement dataElement = createElement( "data" );
	
	
	QDomCDATASection dataSection = createCDATASection( "data" );
	dataSection.setData( data.toString() );
	
	dataElement.appendChild( dataSection );
	
	element.appendChild( dataElement );
}



