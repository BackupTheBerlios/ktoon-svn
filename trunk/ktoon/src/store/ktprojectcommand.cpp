/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#include "ktprojectcommand.h"

#include <kcore/kdebug.h>

#include "ktcommandexecutor.h"

#include "ktprojectrequest.h"
#include "ktpaintareaevent.h"


#include "ktrequestparser.h"
#include "ktprojectresponse.h"

#include "ktsvg2qt.h"

struct KTProjectCommand::Private 
{
	KTCommandExecutor *executor;
	KTProjectResponse *response;
	bool executed;
};

KTProjectCommand::KTProjectCommand(KTCommandExecutor *executor, const KTProjectRequest *request) : QUndoCommand(), k(new Private())
{
	k->executor = executor;
	k->executed = false;
	
	KTRequestParser parser;
	if ( ! parser.parse( request->xml() ) )
	{
		qFatal("==> KTProjectCommand::KTProjectCommand()");
	}
	k->response = parser.response();
	
	k->response->setExternal(request->isExternal());
	
	if ( !k->response ) qFatal("Unparsed response!");
	
	initText();
}

KTProjectCommand::KTProjectCommand(KTCommandExecutor *executor, KTProjectResponse *response) : QUndoCommand(), k(new Private)
{
	k->executor = executor;
	k->response = response;
	k->executed = false;
	initText();
}


void KTProjectCommand::initText()
{
	switch( k->response->part() )
	{
		case KTProjectRequest::Frame:
		{
			setText(actionString( k->response->action() ) +" frame" );
		}
		break;
		case KTProjectRequest::Layer:
		{
			setText(actionString( k->response->action() )+" layer");
		}
		break;
		case KTProjectRequest::Scene:
		{
			setText(actionString( k->response->action() )+" scene");
		}
		break;
		case KTProjectRequest::Item:
		{
			setText(actionString( k->response->action() )+" item");
		}
		break;
		
		case KTProjectRequest::Library:
		{
			setText(actionString( k->response->action() )+" symbol");
		}
		break;
		
		default:
		{
			kfDebug << "CAN'T HANDLE ID: " << k->response->part();
		}
		break;
	}
}

QString KTProjectCommand::actionString(int action)
{
	switch(action)
	{
		case KTProjectRequest::Add:
		{
			return QObject::tr("add");
		}
		break;
		case KTProjectRequest::Remove:
		{
			return QObject::tr("remove");
		}
		break;
		case KTProjectRequest::Move:
		{
			return QObject::tr("move");
		}
		break;
		case KTProjectRequest::Lock:
		{
			return QObject::tr("lock");
		}
		break;
		case KTProjectRequest::Rename:
		{
			return QObject::tr("rename");
		}
		break;
		case KTProjectRequest::Select:
		{
			return QObject::tr("select");
		}
		break;
		case KTProjectRequest::EditNodes:
		{
			return QObject::tr("edit node");
		}
		break;
		case KTProjectRequest::View:
		{
			return QObject::tr("view");
		}
		break;
		case KTProjectRequest::Transform:
		{
			return QObject::tr("transform");
		}
		break;
		case KTProjectRequest::Convert:
		{
			return QObject::tr("convert");
		}
		break;
	}
	
	return QString("Unknown");
}


KTProjectCommand::~KTProjectCommand()
{
	delete k->response;
	delete k;
}


void KTProjectCommand::redo()
{
	K_FUNCINFO << k->response->part();
	
	if ( k->executed )
	{
		k->response->setMode( KTProjectResponse::Redo );
	}
	else
	{
		k->response->setMode( KTProjectResponse::Do );
		k->executed = true;
	}
	
	switch( k->response->part() )
	{
		case KTProjectRequest::Project:
		{
			kDebug() << "Project response isn't handle";
		}
		break;
		case KTProjectRequest::Frame:
		{
			frameCommand();
		}
		break;
		case KTProjectRequest::Layer:
		{
			layerCommand();
		}
		break;
		case KTProjectRequest::Scene:
		{
			sceneCommand();
		}
		break;
		case KTProjectRequest::Item:
		{
			itemCommand();
		}
		break;
		case KTProjectRequest::Library:
		{
			libraryCommand();
		}
		break;
		default:
		{
			K_FUNCINFO << ("Unknown project response!");
		}
		break;
	}
}

void KTProjectCommand::undo()
{
	k->response->setMode( KTProjectResponse::Undo );
	
	switch(k->response->part() )
	{
		case KTProjectRequest::Project:
		{
			kDebug() << "Project response isn't handle";
		}
		break;
		case KTProjectRequest::Frame:
		{
			frameCommand();
		}
		break;
		case KTProjectRequest::Layer:
		{
			layerCommand();
		}
		break;
		case KTProjectRequest::Scene:
		{
			sceneCommand();
		}
		break;
		case KTProjectRequest::Item:
		{
			itemCommand();
		}
		break;
		
		case KTProjectRequest::Library:
		{
			libraryCommand();
		}
		break;
		default:
		{
			K_FUNCINFO << ("Unknown project response!");
		}
		break;
	}
}

void KTProjectCommand::frameCommand()
{
	K_FUNCINFO;
	KTFrameResponse *response = static_cast<KTFrameResponse *>(k->response);
	switch(response->action())
	{
		case KTProjectRequest::Add:
		{
			k->executor->createFrame( response );
		}
		break;
		case KTProjectRequest::Remove:
		{
			k->executor->removeFrame( response);
		}
		break;
		case KTProjectRequest::Move:
		{
			k->executor->moveFrame( response);
		}
		break;
		case KTProjectRequest::Lock:
		{
			k->executor->lockFrame( response );
		}
		break;
		case KTProjectRequest::Rename:
		{
			k->executor->renameFrame( response );
		}
		break;
		case KTProjectRequest::Select:
		{
			k->executor->selectFrame(response );
		}
		break;
		case KTProjectRequest::View:
		{
			k->executor->setFrameVisibility(response);
		}
		break;
		case KTProjectRequest::Expand:
		{
			k->executor->expandFrame(response);
		}
		break;
		case KTProjectRequest::Paste:
		{
			k->executor->pasteFrame(response);
		}
		break;
		
		
		default: break;
	}
}

void KTProjectCommand::layerCommand()
{
	KTLayerResponse *response = static_cast<KTLayerResponse *>(k->response);
	
	switch(response->action())
	{
		case KTProjectRequest::Add:
		{
			k->executor->createLayer( response );
		}
		break;
		case KTProjectRequest::Remove:
		{
			k->executor->removeLayer( response);
		}
		break;
		case KTProjectRequest::Move:
		{
			k->executor->moveLayer( response );
		}
		break;
		case KTProjectRequest::Lock:
		{
			k->executor->lockLayer( response );
		}
		break;
		case KTProjectRequest::Rename:
		{
			k->executor->renameLayer( response);
		}
		break;
		case KTProjectRequest::Select:
		{
			k->executor->selectLayer(response);
		}
		break;
		case KTProjectRequest::View:
		{
			k->executor->setLayerVisibility(response);
		}
		break;
		default: break;
	}
}

void KTProjectCommand::sceneCommand()
{
	KTSceneResponse *response = static_cast<KTSceneResponse *>(k->response);
	switch(response->action())
	{
		case KTProjectRequest::Add:
		{
			k->executor->createScene( response );
		}
		break;
		case KTProjectRequest::Remove:
		{
			k->executor->removeScene( response );
		}
		break;
		case KTProjectRequest::Move:
		{
			k->executor->moveScene( response);
		}
		break;
		case KTProjectRequest::Lock:
		{
			k->executor->lockScene( response );
		}
		break;
		case KTProjectRequest::Rename:
		{
			k->executor->renameScene( response);
		}
		break;
		case KTProjectRequest::Select:
		{
			k->executor->selectScene(response );
		}
		break;
		case KTProjectRequest::View:
		{
			k->executor->setSceneVisibility(response);
		}
		break;
		default: break;
	}
}


void KTProjectCommand::itemCommand()
{
	K_FUNCINFO;
	
	KTItemResponse *response = static_cast<KTItemResponse *>(k->response);
	switch(response->action())
	{
		case KTProjectRequest::Add:
		{
			k->executor->createItem(response);
		}
		break;
		case KTProjectRequest::Remove:
		{
			k->executor->removeItem( response );
		}
		break;
		case KTProjectRequest::Move:
		{
			k->executor->moveItem( response );
		}
		break;
		case KTProjectRequest::Lock:
		{
		}
		break;
		case KTProjectRequest::Rename:
		{
			
		}
		break;
		case KTProjectRequest::Convert:
		{
			k->executor->convertItem(response);
		}
		break;
		case KTProjectRequest::EditNodes:
		{
			k->executor->setPathItem( response);
		}
		break;
		case KTProjectRequest::Select:
		{
		}
		break;
		case KTProjectRequest::View:
		{
		}
		break;
		case KTProjectRequest::Transform:
		{
			k->executor->transformItem(response);
		}
		break;
		case KTProjectRequest::Group:
		{
			k->executor->groupItems(response);
		}
		break;
		case KTProjectRequest::Ungroup:
		{
			
			k->executor->ungroupItems(response);
		}
		break;
		case KTProjectRequest::Tweening:
		{
			k->executor->createTweening(response);
		}
		break;
		default: break;
	}
}

void KTProjectCommand::libraryCommand()
{
	K_FUNCINFO;
	
	KTLibraryResponse *response = static_cast<KTLibraryResponse *>(k->response);
	switch(response->action())
	{
		case KTProjectRequest::Add:
		{
			k->executor->createSymbol(response);
		}
		break;
		case KTProjectRequest::Remove:
		{
			k->executor->removeSymbol(response);
		}
		break;
		case KTProjectRequest::AddSymbolToProject:
		{
			k->executor->addSymbolToProject(response);
		}
		break;
	}
}

void KTProjectCommand::paintAreaCommand()
{
	qFatal("FIX ME in ktprojectcommand.cpp ");
// 	if ( redo )
// 	{
// 		d->executor->reemitEvent( response );
// 	}
// 	else
// 	{
// 		d->executor->reemitEvent( response );
// 	}
}


