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

#include <ddebug.h>

KTCommandExecutor::KTCommandExecutor(KTProject *project) : QObject(project), m_project(project), m_state(None)
{
}

KTCommandExecutor::~KTCommandExecutor()
{
}


QString KTCommandExecutor::createScene(int position, const QString &xml)
{
	D_FUNCINFO;
	
	if ( position < 0 || position > m_project->scenes().count() )
	{
		return QString();
	}
	
	KTScene *scene = m_project->createScene( position );
	if ( !scene ) return QString();
	
	QDomDocument document;
	if ( document.setContent(xml) )
	{
		QDomElement root = document.documentElement();
	
		scene->setSceneName( root.attribute( "name", scene->sceneName()) );
		
		KTProjectRequest request = KTRequestBuilder::createSceneRequest( position, KTProjectRequest::Add, scene->sceneName() );
		emit commandExecuted(&request, m_state);
		
		QDomNode n = root.firstChild();
	
		while( !n.isNull() )
		{
			QDomElement e = n.toElement();
		
			if(!e.isNull())
			{
				if ( e.tagName() == "layer" )
				{
					int layerPos = scene->layers().count();
					QDomDocument newDoc;
					newDoc.appendChild(newDoc.importNode(n, true ));
					
					createLayer(position, layerPos, newDoc.toString(0) );
				}
			}
		
			n = n.nextSibling();
		}
	}
	
	
	return QString();
}

QString KTCommandExecutor::removeScene(int position)
{
	D_FUNCINFO;
	KTScene *toRemove = m_project->scene(position);
	
	if ( toRemove )
	{
		QDomDocument document;
		
		document.appendChild(toRemove->toXml(document));
		KTProjectRequest request = KTRequestBuilder::createSceneRequest( position, KTProjectRequest::Remove );
		
		emit commandExecuted(&request, m_state);
		
		m_project->removeScene( position );
		return document.toString( 0 );
	}
	
	return QString();
}



QString KTCommandExecutor::moveScene(int position, int newPosition)
{
	if ( m_project->moveScene( position, newPosition ) )
	{
		KTProjectRequest request = KTRequestBuilder::createSceneRequest( position, KTProjectRequest::Move, QString::number(newPosition) );
		emit commandExecuted(&request, m_state);
	}
	
	return QString();
}



QString KTCommandExecutor::lockScene(int position, bool lock)
{
	dWarning() << "Lock scene: " << lock;
	
	KTScene *scene = m_project->scene(position);
	
	if ( !scene)
	{
		return QString();
	}
	
	scene->setLocked(lock);
	
	KTProjectRequest request = KTRequestBuilder::createSceneRequest( position, KTProjectRequest::Lock, "1");
	emit commandExecuted(&request, m_state);
	
	return QString();
}


QString KTCommandExecutor::renameScene(int position, const QString &newName)
{
	QString oldName;
	KTScene *scene = m_project->scene(position);
	
	if ( !scene)
	{
		return oldName;
	}
	
	KTProjectRequest request = KTRequestBuilder::createSceneRequest( position, KTProjectRequest::Rename, newName);
	
	oldName = scene->sceneName();
	
	scene->setSceneName( newName );
	
	emit commandExecuted(&request, m_state);
	
	return oldName;
}


QString KTCommandExecutor::selectScene(int position, bool prioritary)
{
	KTProjectRequest request = KTRequestBuilder::createSceneRequest( position, KTProjectRequest::Select, prioritary ? "1" : "0" );
	emit commandExecuted(&request, m_state);
	
	return QString();
}


QString KTCommandExecutor::setSceneVisibility(int position, bool view)
{
	KTScene *scene = m_project->scene(position);
	
	if ( !scene)
	{
		return QString();
	}
	
	scene->setVisible(view);
	
	KTProjectRequest request = KTRequestBuilder::createSceneRequest( position, KTProjectRequest::View, view ? "1" : "0");
	emit commandExecuted(&request, m_state);
	
	return QString();
}


void KTCommandExecutor::reemitEvent(KTProjectRequest *event)
{
	emit commandExecuted(event, m_state);
}

void KTCommandExecutor::setState(State state)
{
	m_state = state;
}





