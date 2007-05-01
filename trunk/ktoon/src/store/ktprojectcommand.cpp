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

#include "ktprojectcommand.h"

#include <dcore/ddebug.h>

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

KTProjectCommand::KTProjectCommand(KTCommandExecutor *executor, const KTProjectRequest *request) : QUndoCommand(), d(new Private())
{
	d->executor = executor;
	d->executed = false;
	
	KTRequestParser parser;
	if ( ! parser.parse( request->xml() ) )
	{
		qFatal("==> KTProjectCommand::KTProjectCommand()");
	}
	d->response = parser.response();
	
	d->response->setExternal(request->isExternal());
	
	if ( !d->response ) qFatal("Unparsed response!");
	
	initText();
}

KTProjectCommand::KTProjectCommand(KTCommandExecutor *executor, KTProjectResponse *response) : QUndoCommand(), d(new Private)
{
	d->executor = executor;
	d->response = response;
	d->executed = false;
	initText();
}


void KTProjectCommand::initText()
{
	switch( d->response->part() )
	{
		case KTProjectRequest::Frame:
		{
			setText(actionString( d->response->action() ) +" frame" );
		}
		break;
		case KTProjectRequest::Layer:
		{
			setText(actionString( d->response->action() )+" layer");
		}
		break;
		case KTProjectRequest::Scene:
		{
			setText(actionString( d->response->action() )+" scene");
		}
		break;
		case KTProjectRequest::Item:
		{
			setText(actionString( d->response->action() )+" item");
		}
		break;
		
		case KTProjectRequest::Library:
		{
			setText(actionString( d->response->action() )+" symbol");
		}
		break;
		
		default:
		{
			dfDebug << "CAN'T HANDLE ID: " << d->response->part();
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
	delete d->response;
	delete d;
}


void KTProjectCommand::redo()
{
	D_FUNCINFO << d->response->part();
	
	if ( d->executed )
	{
		d->response->setMode( KTProjectResponse::Redo );
	}
	else
	{
		d->response->setMode( KTProjectResponse::Do );
		d->executed = true;
	}
	
	switch( d->response->part() )
	{
		case KTProjectRequest::Project:
		{
			dDebug() << "Project response isn't handle";
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
			D_FUNCINFO << ("Unknown project response!");
		}
		break;
	}
}

void KTProjectCommand::undo()
{
	d->response->setMode( KTProjectResponse::Undo );
	
	switch(d->response->part() )
	{
		case KTProjectRequest::Project:
		{
			dDebug() << "Project response isn't handle";
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
			D_FUNCINFO << ("Unknown project response!");
		}
		break;
	}
}

void KTProjectCommand::frameCommand()
{
	D_FUNCINFO;
	KTFrameResponse *response = static_cast<KTFrameResponse *>(d->response);
	switch(response->action())
	{
		case KTProjectRequest::Add:
		{
			d->executor->createFrame( response );
		}
		break;
		case KTProjectRequest::Remove:
		{
			d->executor->removeFrame( response);
		}
		break;
		case KTProjectRequest::Move:
		{
			d->executor->moveFrame( response);
		}
		break;
		case KTProjectRequest::Lock:
		{
			d->executor->lockFrame( response );
		}
		break;
		case KTProjectRequest::Rename:
		{
			d->executor->renameFrame( response );
		}
		break;
		case KTProjectRequest::Select:
		{
			d->executor->selectFrame(response );
		}
		break;
		case KTProjectRequest::View:
		{
			d->executor->setFrameVisibility(response);
		}
		break;
		case KTProjectRequest::Expand:
		{
			d->executor->expandFrame(response);
		}
		break;
		case KTProjectRequest::Paste:
		{
			d->executor->pasteFrame(response);
		}
		break;
		
		
		default: break;
	}
}

void KTProjectCommand::layerCommand()
{
	KTLayerResponse *response = static_cast<KTLayerResponse *>(d->response);
	
	switch(response->action())
	{
		case KTProjectRequest::Add:
		{
			d->executor->createLayer( response );
		}
		break;
		case KTProjectRequest::Remove:
		{
			d->executor->removeLayer( response);
		}
		break;
		case KTProjectRequest::Move:
		{
			d->executor->moveLayer( response );
		}
		break;
		case KTProjectRequest::Lock:
		{
			d->executor->lockLayer( response );
		}
		break;
		case KTProjectRequest::Rename:
		{
			d->executor->renameLayer( response);
		}
		break;
		case KTProjectRequest::Select:
		{
			d->executor->selectLayer(response);
		}
		break;
		case KTProjectRequest::View:
		{
			d->executor->setLayerVisibility(response);
		}
		break;
		default: break;
	}
}

void KTProjectCommand::sceneCommand()
{
	KTSceneResponse *response = static_cast<KTSceneResponse *>(d->response);
	switch(response->action())
	{
		case KTProjectRequest::Add:
		{
			d->executor->createScene( response );
		}
		break;
		case KTProjectRequest::Remove:
		{
			d->executor->removeScene( response );
		}
		break;
		case KTProjectRequest::Move:
		{
			d->executor->moveScene( response);
		}
		break;
		case KTProjectRequest::Lock:
		{
			d->executor->lockScene( response );
		}
		break;
		case KTProjectRequest::Rename:
		{
			d->executor->renameScene( response);
		}
		break;
		case KTProjectRequest::Select:
		{
			d->executor->selectScene(response );
		}
		break;
		case KTProjectRequest::View:
		{
			d->executor->setSceneVisibility(response);
		}
		break;
		default: break;
	}
}


void KTProjectCommand::itemCommand()
{
	D_FUNCINFO;
	
	KTItemResponse *response = static_cast<KTItemResponse *>(d->response);
	switch(response->action())
	{
		case KTProjectRequest::Add:
		{
			d->executor->createItem(response);
		}
		break;
		case KTProjectRequest::Remove:
		{
			d->executor->removeItem( response );
		}
		break;
		case KTProjectRequest::Move:
		{
			d->executor->moveItem( response );
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
				d->executor->convertItem(response);
		}
		break;
		case KTProjectRequest::EditNodes:
		{
			d->executor->setPathItem( response);
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
			d->executor->transformItem(response);
		}
		break;
		case KTProjectRequest::Group:
		{
			d->executor->groupItems(response);
		}
		break;
		case KTProjectRequest::Ungroup:
		{
			
			d->executor->ungroupItems(response);
		}
		break;
		case KTProjectRequest::Tweening:
		{
			d->executor->createTweening(response);
		}
		break;
		default: break;
	}
}

void KTProjectCommand::libraryCommand()
{
	D_FUNCINFO;
	
	KTLibraryResponse *response = static_cast<KTLibraryResponse *>(d->response);
	switch(response->action())
	{
		case KTProjectRequest::Add:
		{
			d->executor->createSymbol(response);
		}
		break;
		case KTProjectRequest::Remove:
		{
			d->executor->removeSymbol(response);
		}
		break;
		case KTProjectRequest::AddSymbolToProject:
		{
			d->executor->addSymbolToProject(response);
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


