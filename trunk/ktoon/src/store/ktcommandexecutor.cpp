/********************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                       *
 *   krawek@toonka.com                                                          *
 *                                                                              *
 *   m_project program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by       *
 *   the Free Software Foundation; either version 2 of the License, or          *
 *   (at your option) any later version.                                        *
 *                                                                              *
 *   m_project program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 *   GNU General Public License for more details.                               *
 *                                                                              *
 *   You should have received a copy of the GNU General Public License          *
 *   along with m_project program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                            *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                  *
 ********************************************************************************/

#include "ktcommandexecutor.h"

#include <ktproject.h>
#include <ktscene.h>

#include "ktrequestbuilder.h"
#include "ktprojectrequest.h"

#include "ktprojectresponse.h"

#include <ddebug.h>

KTCommandExecutor::KTCommandExecutor(KTProject *project) : QObject(project), m_project(project)
{
}

KTCommandExecutor::~KTCommandExecutor()
{
}


bool KTCommandExecutor::createScene(KTSceneResponse *response)
{
	D_FUNCINFO;
	
	response->setAction(KTProjectRequest::Add);
	
	int position = response->sceneIndex();
	QString name = response->arg().toString();
	
	if ( position < 0 || position > m_project->scenes().count() )
	{
		return false;
	}
	
	KTScene *scene = m_project->createScene( position );
	if ( !scene ) return false;
	
	if ( !name.isEmpty() )
	{
		scene->setSceneName(name);
	}
	else
	{
		response->setArg(scene->sceneName());
	}
	
	emit responsed( response );
	
	QString state =  response->state();
	
	if ( ! state.isEmpty() )
	{
		scene->fromXml(state);
		response->setArg( scene->sceneName() );
	}
	
	return true;
}

bool KTCommandExecutor::removeScene(KTSceneResponse *response)
{
	D_FUNCINFO;
	
	response->setAction(KTProjectRequest::Remove);
	
	int position = response->sceneIndex();
	
	KTScene *toRemove = m_project->scene(position);
	
	if ( toRemove )
	{
		QDomDocument document;
		document.appendChild(toRemove->toXml(document));
		
		response->setState(document.toString());
		response->setArg(toRemove->sceneName());
		
		
		if(m_project->removeScene( position ))
		{
			emit responsed(response);
			return true;
		}
	}
	
	return false;
}



bool KTCommandExecutor::moveScene(KTSceneResponse *response)
{
	response->setAction(KTProjectRequest::Move);
	
	int position = response->sceneIndex();
	int newPosition = response->arg().toInt();
	if ( m_project->moveScene( position, newPosition ) )
	{
		emit responsed(response);
		return true;
		
	}
	
	return false;
}



bool KTCommandExecutor::lockScene(KTSceneResponse *response)
{
	response->setAction(KTProjectRequest::Lock);
	int position = response->sceneIndex();
	bool lock = response->arg().toBool();
	dWarning() << "Lock scene: " << lock;
	
	KTScene *scene = m_project->scene(position);
	
	if ( !scene)
	{
		return false;
	}
	
	scene->setLocked(lock);
	
	emit responsed(response);
	return true;
}


bool KTCommandExecutor::renameScene(KTSceneResponse *response)
{
	response->setAction(KTProjectRequest::Rename);
	int position = response->sceneIndex();
	QString newName = response->arg().toString();
	KTScene *scene = m_project->scene(position);
	
	if ( !scene)
	{
		return false;
	}
	
	KTProjectRequest request = KTRequestBuilder::createSceneRequest( position, KTProjectRequest::Rename, newName);
	
	scene->setSceneName( newName );
	
	emit responsed(response);
	return true;
}


void KTCommandExecutor::selectScene(KTSceneResponse *response)
{
	response->setAction(KTProjectRequest::Select);
	emit responsed(response);
}


bool KTCommandExecutor::setSceneVisibility(KTSceneResponse *response)
{
	response->setAction(KTProjectRequest::View);
	int position = response->sceneIndex();
	bool view = response->arg().toBool();
	
	KTScene *scene = m_project->scene(position);
	
	if ( !scene)
	{
		return false;
	}
	
	scene->setVisible(view);
	
// 	KTProjectRequest request = KTRequestBuilder::createSceneRequest( position, KTProjectRequest::View, view ? "1" : "0");
	
	emit responsed(response);
	
	return true;
}


