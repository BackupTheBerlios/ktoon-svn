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
#include <ktscenerequest.h>

#include <ddebug.h>

KTCommandExecutor::KTCommandExecutor(KTProject *project) : QObject(project), m_project(project), m_isRedo(true)
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
		return QString();;
	}
	
	KTScene *scene = m_project->createScene( position );
	if ( !scene ) return QString();
	
	
	KTSceneRequest event(KTProjectRequest::Add, position);
	
	QDomDocument document;
	if ( document.setContent(xml) )
	{
		QDomElement root = document.documentElement();
	
		scene->setSceneName( root.attribute( "name", scene->sceneName()) );
		event.setPartName(scene->sceneName());
		
		emit commandExecuted(&event, m_isRedo);
		
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
	else
	{
		event.setPartName(scene->sceneName());
		
		emit commandExecuted(&event, m_isRedo);
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
		KTSceneRequest event(KTProjectRequest::Remove, position);
		
		emit commandExecuted(&event, m_isRedo);
		
		m_project->removeScene( position );
		return document.toString( 0 );
	}
	
	return QString();
}



QString KTCommandExecutor::moveScene(int position, int newPosition)
{
	if ( m_project->moveScene( position, newPosition ) )
	{
		KTSceneRequest event(KTProjectRequest::Move, position, newPosition);
		emit commandExecuted(&event, m_isRedo);
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
	
	KTSceneRequest event(KTProjectRequest::Lock, position, lock);
	emit commandExecuted(&event, m_isRedo);
	
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
	
	KTSceneRequest event(KTProjectRequest::Rename, position, newName);
	event.setPartName( scene->sceneName() );
	
	oldName = scene->sceneName();
	
	scene->setSceneName( newName);
	
	emit commandExecuted(&event, m_isRedo);
	
	return oldName;
}


QString KTCommandExecutor::selectScene(int position, bool prioritary)
{
	KTSceneRequest event(KTProjectRequest::Select, position, prioritary);
	emit commandExecuted(&event, m_isRedo);
	
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
	
	KTSceneRequest event(KTProjectRequest::View, position, view);
	emit commandExecuted(&event, m_isRedo);
	
	return QString();
}


void KTCommandExecutor::reemitEvent(KTProjectRequest *event)
{
	emit commandExecuted(event, m_isRedo);
}

void KTCommandExecutor::setIsRedo(bool isRedo)
{
	m_isRedo = isRedo;
}





