/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
 *                                                                         *
 *   m_project program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   m_project program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with m_project program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "ktcommandexecutor.h"

#include <ktproject.h>
#include <ktscene.h>

#include "ktrequestbuilder.h"
#include "ktprojectrequest.h"

#include "ktprojectresponse.h"

#include <ddebug.h>

KTCommandExecutor::KTCommandExecutor(KTProject *project) : QObject(project), m_project(project), m_state(None)
{
}

KTCommandExecutor::~KTCommandExecutor()
{
}


bool KTCommandExecutor::createScene(KTSceneResponse *response)
{
	D_FUNCINFO;
	
	int position = response->sceneIndex();
	QString xml = response->arg().toString();
	
	if ( position < 0 || position > m_project->scenes().count() )
	{
		return false;
	}
	
	KTScene *scene = m_project->createScene( position );
	if ( !scene ) return false;
	
	response->setArg( scene->sceneName() );
	
	emit responsed( response, m_state );
	
// 	QDomDocument document;
// 	if ( document.setContent(xml) )
// 	{
// 		QDomElement root = document.documentElement();
// 	
// 		scene->setSceneName( root.attribute( "name", scene->sceneName()) );
// 		
// 		KTProjectRequest request = KTRequestBuilder::createSceneRequest( position, KTProjectRequest::Add, scene->sceneName() );
// 		emit commandExecuted(&request, m_state);
// 		
// 		QDomNode n = root.firstChild();
// 	
// 		while( !n.isNull() )
// 		{
// 			QDomElement e = n.toElement();
// 		
// 			if(!e.isNull())
// 			{
// 				if ( e.tagName() == "layer" )
// 				{
// 					int layerPos = scene->layers().count();
// 					QDomDocument newDoc;
// 					newDoc.appendChild(newDoc.importNode(n, true ));
// 					
// 					createLayer(position, layerPos, newDoc.toString(0) );
// 				}
// 			}
// 		
// 			n = n.nextSibling();
// 		}
// 	}
	
	
	return true;
}

bool KTCommandExecutor::removeScene(KTSceneResponse *response)
{
	D_FUNCINFO;
	
	int position = response->sceneIndex();
	
	KTScene *toRemove = m_project->scene(position);
	
	if ( toRemove )
	{
		QDomDocument document;
		
// 		document.appendChild(toRemove->toXml(document));
// 		KTProjectRequest request = KTRequestBuilder::createSceneRequest( position, KTProjectRequest::Remove );
		
// 		emit commandExecuted(&request, m_state);
		
		if(m_project->removeScene( position ))
		{
			emit responsed(response, m_state);
			return true;
		}
	}
	
	return false;
}



bool KTCommandExecutor::moveScene(KTSceneResponse *response)
{
	int position = response->sceneIndex();
	int newPosition = response->arg().toInt();
	if ( m_project->moveScene( position, newPosition ) )
	{
// 		KTProjectRequest request = KTRequestBuilder::createSceneRequest( position, KTProjectRequest::Move, QString::number(newPosition) );
// 		emit commandExecuted(&request, m_state);
		emit responsed(response, m_state);
		return true;
		
	}
	
	return false;
}



bool KTCommandExecutor::lockScene(KTSceneResponse *response)
{
	int position = response->sceneIndex();
	bool lock = response->arg().toBool();
	dWarning() << "Lock scene: " << lock;
	
	KTScene *scene = m_project->scene(position);
	
	if ( !scene)
	{
		return false;
	}
	
	scene->setLocked(lock);
	
// 	KTProjectRequest request = KTRequestBuilder::createSceneRequest( position, KTProjectRequest::Lock, "1");
// 	emit commandExecuted(&request, m_state);
	emit responsed(response, m_state);
	return true;
}


bool KTCommandExecutor::renameScene(KTSceneResponse *response)
{
	
	int position = response->sceneIndex();
	QString newName = response->arg().toString();
// 	QString oldName;
	KTScene *scene = m_project->scene(position);
	
	if ( !scene)
	{
// 		return oldName;
		return false;
	}
	
	KTProjectRequest request = KTRequestBuilder::createSceneRequest( position, KTProjectRequest::Rename, newName);
	
// 	oldName = scene->sceneName();
	
	scene->setSceneName( newName );
	
// 	emit commandExecuted(&request, m_state);
	emit responsed(response,m_state);
	return true;
}


void KTCommandExecutor::selectScene(KTSceneResponse *response)
{
	emit responsed(response, m_state);
}


bool KTCommandExecutor::setSceneVisibility(KTSceneResponse *response)
{
	int position = response->sceneIndex();
	bool view = response->arg().toBool();
	
	KTScene *scene = m_project->scene(position);
	
	if ( !scene)
	{
		return false;
	}
	
	scene->setVisible(view);
	
// 	KTProjectRequest request = KTRequestBuilder::createSceneRequest( position, KTProjectRequest::View, view ? "1" : "0");
	
	emit responsed(response, m_state);
	
	return true;
}


void KTCommandExecutor::setState(State state)
{
	m_state = state;
}

