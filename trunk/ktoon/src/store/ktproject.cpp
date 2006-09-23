/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#include "ktproject.h"

#include <ddebug.h>

#include "ktscene.h"
#include "ktlayer.h"
#include "ktframe.h"

#include "ktscenerequest.h" //events

#include <QGraphicsView>

/**
 * Constructor por defecto
 */
KTProject::KTProject(QObject *parent) : QObject(parent), m_sceneCounter(0)
{
	DINIT;
}


/**
 * Destructor por defecto
 */
KTProject::~KTProject()
{
	DEND;
}

/**
 * Cierra el proyecto
 */
void KTProject::clear()
{
	qDeleteAll(m_scenes);
}

/**
 * Pone un nombre al proyecto
 */
void KTProject::setProjectName(const QString &name)
{
	m_name = name;
}

/**
 * @if spanish
 * Retorna el nombre del proyecto
 * @endif
 * 
 * @if english
 * Returns project name
 * @endif
 */
QString KTProject::projectName() const
{
	return m_name;
}


KTScene *KTProject::createScene(int position, const QString &xml)
{
	if ( position < 0 || position > m_scenes.count() )
	{
		return 0;
	}
	
	KTScene *scene = new KTScene(this);
	m_scenes.insert(position, scene);
	m_sceneCounter++;
	
	KTSceneRequest event(KTProjectRequest::Add, position);
	
	
	QDomDocument document;
	if ( document.setContent(xml) )
	{
		QDomElement root = document.documentElement();
	
		scene->setSceneName( root.attribute( "name", scene->sceneName()) );
		
		
		event.setPartName(scene->sceneName());
		emit commandExecuted(&event);
		
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
		scene->setSceneName(tr("Scene %1").arg(m_sceneCounter));
		
		event.setPartName(scene->sceneName());
		emit commandExecuted(&event);
	}
	
	
	
	return scene;
}

QString KTProject::removeScene(int position)
{
	D_FUNCINFO;
	KTScene *toRemove = scene(position);
	
	if ( toRemove )
	{
		QDomDocument document;
		
		document.appendChild(toRemove->toXml(document));
		
		m_scenes.removeAt(position);
		
		foreach(QGraphicsView *view, toRemove->views() )
		{
			view->setScene(0);
		}
		
		delete toRemove;
		toRemove = 0;
		
		KTSceneRequest event(KTProjectRequest::Remove, position);
		
		m_sceneCounter--;
		
		emit commandExecuted( &event );
		
		return document.toString( 0 );
	}
	
	return QString();
}

KTScene *KTProject::scene(int position)
{
	if ( position < 0 || position >= m_scenes.count() )
	{
		D_FUNCINFO << " FATAL ERROR: index out of bound " << position;
		return 0;
	}
	
	return m_scenes[position];
}

int KTProject::indexOf(KTScene *scene) const
{
	return m_scenes.indexOf(scene);
}

void KTProject::fromXml(const QString &xml )
{
	QDomDocument document;
	
	if (! document.setContent(xml) )
	{
		return;
	}
	
	QDomElement root = document.documentElement();
	
	setProjectName( root.attribute( "name", projectName() ) );
	
	QDomNode n = root.firstChild();
	
	while( !n.isNull() )
	{
		QDomElement e = n.toElement();
		
		if(!e.isNull())
		{
// 			dDebug() << "Scene??? " << e.tagName();
			if ( e.tagName() == "scene" )
			{
				KTScene *scene = createScene( m_scenes.count() );
				
				if ( scene )
				{
					QDomDocument newDoc;
					newDoc.appendChild( e );
					scene->fromXml( newDoc.toString(0) );
				}
			}
		}
		
		n = n.nextSibling();
	}
}

QDomElement KTProject::toXml(QDomDocument &doc)
{
	return QDomElement();
}

void KTProject::moveScene(int position, int newPosition)
{
// 	dDebug() << "Move scene from " << position << " to " << newPosition;
	
	if ( position < 0 || position >= m_scenes.count() || newPosition < 0 || newPosition >= m_scenes.count() )
	{
		dWarning() << "Failed moving scene!";
		return;
	}
	
	KTScene *scene = m_scenes.takeAt(position);
	
	m_scenes.insert(newPosition, scene);
	
	
	KTSceneRequest event(KTProjectRequest::Move, position, newPosition);
	emit commandExecuted( &event);
	
}



void KTProject::lockScene(int position, bool lock)
{
	dWarning() << "Lock scene: " << lock;
	KTScene *scene = this->scene(position);
	
	if ( !scene)
	{
		return;
	}
	
	scene->setLocked(lock);
	
	KTSceneRequest event(KTProjectRequest::Lock, position, lock);
	emit commandExecuted( &event);
	
	
}


QString KTProject::renameScene(int position, const QString &newName)
{
	QString oldName;
	KTScene *scene = this->scene(position);
	
	if ( !scene)
	{
		return oldName;
	}
	
	KTSceneRequest event(KTProjectRequest::Rename, position, newName);
	event.setPartName( scene->sceneName() );
	
	oldName = scene->sceneName();
	
	scene->setSceneName( newName);
	
	emit commandExecuted( &event );
	
	
	return oldName;
}


void KTProject::selectScene(int position, bool prioritary)
{
	KTSceneRequest event(KTProjectRequest::Select, position, prioritary);
	emit commandExecuted( &event );
}


void KTProject::setSceneVisibility(int position, bool view)
{
	KTScene *scene = this->scene(position);
	
	if ( !scene)
	{
		return;
	}
	
	scene->setVisible(view);
	
	KTSceneRequest event(KTProjectRequest::View, position, view);
	emit commandExecuted( &event );
}


void KTProject::reemitEvent(KTProjectRequest *event)
{
	emit commandExecuted( event );
}

