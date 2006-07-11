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


#include "ktframeevent.h" //events

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
	
	KTSceneEvent event(KTProjectEvent::Add, position);
	
	
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
		delete toRemove;
		
		KTSceneEvent event(KTProjectEvent::Remove, position);
		
		m_sceneCounter--;
		
		emit commandExecuted( &event );
		
		return document.toString( 0 );
	}
	
	return QString();
}

KTLayer *KTProject::createLayer(int scenePosition, int position, const QString &xml)
{
	KTScene *scene = this->scene(scenePosition);
	
	if ( scene )
	{
		KTLayer *layer = scene->createLayer(position);
		
		if ( ! layer ) return 0;
		
		KTLayerEvent event(KTProjectEvent::Add, scenePosition, position );
		QDomDocument document;
		if ( document.setContent(xml) )
		{
			QDomElement root = document.documentElement();
	
			layer->setLayerName( root.attribute( "name", layer->layerName() ) );
	
			QDomNode n = root.firstChild();
			
			event.setPartName(layer->layerName());
			emit commandExecuted(&event);
	
			while( !n.isNull() )
			{
				QDomElement e = n.toElement();
		
				if(!e.isNull())
				{
					if ( e.tagName() == "frame" )
					{
						int framePos = layer->frames().count();
						QDomDocument newDoc;
						newDoc.appendChild(newDoc.importNode(n, true ));
						createFrame(scenePosition, position, framePos, newDoc.toString(0) );
					}
				}
		
				n = n.nextSibling();
			}
		}
		else
		{
			event.setPartName(layer->layerName());
			emit commandExecuted(&event);
		}
		
		return layer;
	}
	else
	{
		D_CHECKPTR(scene);
	}
	
	return 0;
}


QString KTProject::removeLayer(int scenePos, int position)
{
	KTScene *scene = this->scene(scenePos);
	
	if ( scene )
	{
		KTLayer *layer = scene->layer( position );
		if ( layer )
		{
			QDomDocument document;
			
			document.appendChild(layer->toXml(document));
			
			if ( scene->removeLayer(position) )
			{
				KTLayerEvent event(KTProjectEvent::Remove, scenePos, position);
				
				emit commandExecuted( &event );
				
				return document.toString(0);
			}
		}
	}
	
	return QString();
}

KTFrame *KTProject::createFrame(int scenePosition, int layerPosition, int position, const QString &xml)
{
	KTScene *scene = this->scene(scenePosition);
	
	if ( !scene)
	{
		return 0;
	}
	
	KTLayer *layer = scene->layer(layerPosition);
	
	if ( layer )
	{
		KTFrame *frame = layer->createFrame(position);
		
		if ( ! frame ) return 0;
		
		KTFrameEvent event(KTProjectEvent::Add, scenePosition, layerPosition, position );
		
		
		QDomDocument document;
		if ( document.setContent(xml) )
		{
			QDomElement root = document.documentElement();
	
			frame->setFrameName( root.attribute( "name", frame->frameName() ) );
			
			event.setPartName(frame->frameName());
			emit commandExecuted(&event);
			
			QDomNode n = root.firstChild();
	
			while( !n.isNull() )
			{
				QDomElement e = n.toElement();
		
				if(!e.isNull())
				{
					dDebug() << "Item??? " << e.tagName();
#if 0
					if ( e.tagName() == "frame" )
					{
						KTFrame *frame = createFrame( m_frames.count() );
						
						if ( frame )
						{
							QDomDocument newDoc;
							newDoc.appendChild(newDoc.importNode(n, true ));
							frame->fromXml( newDoc.toString(0) );
						}
					}
#endif
				}
		
				n = n.nextSibling();
			}
		}
		else
		{
			event.setPartName(frame->frameName());
			emit commandExecuted(&event);
		}
		
		return frame;
	}
	else
	{
		D_CHECKPTR(layer);
	}
	
	return 0;
}


QString KTProject::removeFrame(int scenePos, int layerPos, int position)
{
	KTScene *scene = this->scene(scenePos);
	
	if ( scene )
	{
		KTLayer *layer = scene->layer( layerPos );
		if ( layer )
		{
			KTFrame *frame = layer->frame( position );
			if ( frame )
			{
				QDomDocument document;
				
				document.appendChild(frame->toXml(document));
				
				if ( layer->removeFrame(position) )
				{
					KTFrameEvent event(KTProjectEvent::Remove, scenePos, layerPos, position);
					
					emit commandExecuted( &event );
					
					return document.toString(0);
				}
			}
		}
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
			dDebug() << "Scene??? " << e.tagName();
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

