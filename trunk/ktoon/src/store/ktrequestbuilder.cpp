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

#include "ktrequestbuilder.h"
#include <QDomDocument>

#include "ktprojectrequest.h"

KTRequestBuilder::KTRequestBuilder()
{
}


KTRequestBuilder::~KTRequestBuilder()
{
}

KTProjectRequest KTRequestBuilder::createItemRequest(int sceneIndex, int layerIndex, int frameIndex, int itemIndex, int actionId, const QVariant &arg, const QByteArray &data)
{
	QDomDocument doc;
	
	QDomElement root = doc.createElement("request");
	
	QDomElement scene = doc.createElement("scene");
	scene.setAttribute( "index", sceneIndex);
	
	QDomElement layer = doc.createElement("layer");
	layer.setAttribute( "index", layerIndex);
	
	QDomElement frame = doc.createElement("frame");
	frame.setAttribute( "index", frameIndex);
	
	QDomElement item = doc.createElement("item");
	item.setAttribute( "index", itemIndex);
	
	QDomElement action = doc.createElement("action");
	action.setAttribute( "id", actionId);
	action.setAttribute( "arg", arg.toString());
	action.setAttribute( "part", KTProjectRequest::Item );
	
	
	KTRequestBuilder::appendData( doc, action, data);
	root.appendChild( action );
	frame.appendChild(item);
	layer.appendChild(frame);
	scene.appendChild(layer);
	root.appendChild(scene);
	
	doc.appendChild(root);
	
	return KTProjectRequest(doc.toString(0));
}

KTProjectRequest KTRequestBuilder::createFrameRequest(int sceneIndex, int layerIndex, int frameIndex, int actionId, const QVariant &arg, const QByteArray &data)
{
	QDomDocument doc;
	
	QDomElement root = doc.createElement("request");
	
	QDomElement scene = doc.createElement("scene");
	scene.setAttribute( "index", sceneIndex);
	
	QDomElement layer = doc.createElement("layer");
	layer.setAttribute( "index", layerIndex);
	
	QDomElement frame = doc.createElement("frame");
	frame.setAttribute( "index", frameIndex);
	
	QDomElement action = doc.createElement("action");
	action.setAttribute( "id", actionId);
	action.setAttribute( "arg", arg.toString());
	action.setAttribute( "part", KTProjectRequest::Frame );
	
	KTRequestBuilder::appendData( doc, action, data);
	
	root.appendChild( action );
	layer.appendChild(frame);
	scene.appendChild(layer);
	root.appendChild(scene);
	
	doc.appendChild(root);
	
	return KTProjectRequest(doc.toString(0));
}


KTProjectRequest KTRequestBuilder::createLayerRequest(int sceneIndex, int layerIndex, int actionId, const QVariant &arg, const QByteArray &data)
{
	QDomDocument doc;
	
	QDomElement root = doc.createElement("request");
	
	QDomElement scene = doc.createElement("scene");
	scene.setAttribute( "index", sceneIndex);
	
	QDomElement layer = doc.createElement("layer");
	layer.setAttribute( "index", layerIndex);
	
	QDomElement action = doc.createElement("action");
	action.setAttribute( "id", actionId);
	action.setAttribute( "arg", arg.toString());
	action.setAttribute( "part", KTProjectRequest::Layer );
	
	KTRequestBuilder::appendData( doc, action, data);
	
	root.appendChild( action );
	scene.appendChild(layer);
	root.appendChild(scene);
	
	doc.appendChild(root);
	
	return KTProjectRequest(doc.toString(0));
}


KTProjectRequest KTRequestBuilder::createSceneRequest(int sceneIndex, int actionId, const QVariant &arg, const QByteArray &data)
{
	QDomDocument doc;
	
	QDomElement root = doc.createElement("request");
	
	QDomElement scene = doc.createElement("scene");
	scene.setAttribute( "index", sceneIndex);
	
	QDomElement action = doc.createElement("action");
	action.setAttribute( "id", actionId);
	action.setAttribute( "arg", arg.toString());
	action.setAttribute( "part", KTProjectRequest::Scene );
	
	KTRequestBuilder::appendData( doc, action, data);
	
	root.appendChild(action);
	root.appendChild(scene);
	
	doc.appendChild(root);
	
	return KTProjectRequest(doc.toString(0));
}

KTProjectRequest KTRequestBuilder::createLibraryRequest(int type, int actionId, const QVariant &arg, const QByteArray &data)
{
	QDomDocument doc;
	
	QDomElement root = doc.createElement("request");
	
	QDomElement library = doc.createElement("library");
	
	QDomElement symbol = doc.createElement("symbol");
	symbol.setAttribute("type", type);
	
	QDomElement action = doc.createElement("action");
	action.setAttribute( "id", actionId);
	action.setAttribute( "arg", arg.toString());
	action.setAttribute( "part", KTProjectRequest::Library );
	
	KTRequestBuilder::appendData( doc, action, data);
	
	library.appendChild(action);
	
	library.appendChild(symbol);
	root.appendChild(library);
	
	doc.appendChild(root);
	
	return KTProjectRequest(doc.toString(0));
}

void KTRequestBuilder::appendData(QDomDocument &doc, QDomElement &element, const QByteArray &data)
{
	if ( !data.isNull() && !data.isEmpty())
	{
		QDomCDATASection cdata = doc.createCDATASection( "data" );
		cdata.setData( QString(data.toBase64()) );
		
		element.appendChild( cdata );
	}
}

