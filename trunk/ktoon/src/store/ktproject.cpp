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
#include "ktlibrary.h"
#include "ktlibraryobject.h"
#include "ktitemfactory.h"

#include "ktprojectresponse.h"

#include "ktprojectloader.h"


#include <QGraphicsView>

/**
 * Constructor por defecto
 */
KTProject::KTProject(QObject *parent) : QObject(parent), m_sceneCounter(0)
{
	DINIT;
	
	m_library = new KTLibrary("library", this);
	
	loadLibrary();
}


/**
 * Destructor por defecto
 */
KTProject::~KTProject()
{
	DEND;
}

/**
 * Esta funcion carga la libreria local
 */
void KTProject::loadLibrary()
{
}

/**
 * Cierra el proyecto
 */
void KTProject::clear()
{
	qDeleteAll(m_scenes);
	m_scenes.clear();
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


KTScene *KTProject::createScene(int position, bool loaded )
{
	dDebug("project") << "Creating scene " << position;
	if ( position < 0 || position > m_scenes.count() )
	{
		return 0;
	}
	
	KTScene *scene = new KTScene(this);
	m_scenes.insert(position, scene);
	m_sceneCounter++;
	
	scene->setSceneName(tr("Scene %1").arg(m_sceneCounter));
	
	if ( loaded )
	{
		KTProjectLoader::createScene(position, scene->sceneName(), this);
	}
	
	return scene;
}



bool KTProject::removeScene(int position)
{
	D_FUNCINFO;
	KTScene *toRemove = scene(position);
	
	if ( toRemove )
	{
		m_scenes.removeAt(position);
		
		foreach(QGraphicsView *view, toRemove->views() )
		{
			view->setScene(0);
		}
		
		delete toRemove;
		toRemove = 0;
		
		m_sceneCounter--;
		
		return true;
	}
	
	return false;
}


bool KTProject::moveScene(int position, int newPosition)
{
	if ( position < 0 || position >= m_scenes.count() || newPosition < 0 || newPosition >= m_scenes.count() )
	{
		dWarning() << "Failed moving scene!";
		return false;
	}
	
	KTScene *scene = m_scenes.takeAt(position);
	
	m_scenes.insert(newPosition, scene);
	
	return true;
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
	
	QDomNode n = root.firstChild();
	
	while( !n.isNull() )
	{
		QDomElement e = n.toElement();
		
		if(!e.isNull())
		{
			if ( e.tagName() == "Project" )
			{
				setProjectName( e.attribute( "name", projectName() ) );
			}
		}
		
		n = n.nextSibling();
	}
}

QDomElement KTProject::toXml(QDomDocument &doc) const
{
	QDomElement ktoon = doc.createElement("KToon");
	ktoon.setAttribute("version", "1");
	
	QDomElement project = doc.createElement("Project");
	project.setAttribute("name", m_name);
	
	QDomElement meta = doc.createElement("meta");
	
	QDomElement author = doc.createElement("author");
	author.setAttribute("value", "");
	
	
	meta.appendChild(author);
	project.appendChild(meta);
	ktoon.appendChild(project);
	
	return ktoon;
}

Scenes KTProject::scenes() const
{
	return m_scenes;
}


bool KTProject::createSymbol(const QString &xml)
{
	QDomDocument doc;
	if ( !doc.setContent(xml) )
	{
		dfDebug << "Cannot set content!";
		return false;
	}
	
	QDomElement root = doc.documentElement();
	
	if ( root.tagName() == "library" )
	{
		QDomNode n = root.firstChild();
		while(!n.isNull())
		{
			QDomElement e = n.toElement();
			if(!e.isNull())
			{
				if ( e.tagName() == "symbol" )
				{
					KTLibraryObject *object = new KTLibraryObject(m_library);
					object->setType( e.attribute( "type" ).toInt() );
					
					QDomDocument buildDoc;
					buildDoc.appendChild(buildDoc.importNode(n.firstChild(), true));
					
					switch(object->type())
					{
						case KTLibraryObject::Item:
						{
							KTItemFactory factory;
							QGraphicsItem *item = factory.create(buildDoc.toString(0));
							
							object->setData( QVariant::fromValue(item) );
						}
						break;
					}
					
					m_library->addObject( object, e.attribute( "name"));
				}
			}
			n = n.nextSibling();
		}
	}
	
	return true;
}

bool KTProject::removeSymbol(const QString &xml)
{
	qFatal("Implement me");
	return false;
}

KTLibrary *KTProject::library() const
{
	return m_library;
}

void KTProject::emitResponse(KTProjectResponse *response)
{
	emit responsed(response);
}

