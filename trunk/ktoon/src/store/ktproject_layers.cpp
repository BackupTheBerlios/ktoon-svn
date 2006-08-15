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

#include "ktproject.h"
#include "ktscene.h"

#include "ktlayerevent.h"
#include <dcore/ddebug.h>

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
		
		dDebug() << "Añadiendo layer en escena: " << scene->sceneName();
		
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


void KTProject::moveLayer(int scenePosition, int position, int newPosition)
{
	dDebug() << "Move layer from " << position << " to " << newPosition;
	KTScene *scene = this->scene(scenePosition);
	
	if ( !scene)
	{
		return;
	}
	
	if ( ! scene->moveLayer(position, newPosition) )
	{
		dWarning() << "Failed moving layer";
	}
	else
	{
		KTLayerEvent event(KTProjectEvent::Move,scenePosition, position, newPosition);
		emit commandExecuted( &event);
	}
	
}

void KTProject::lockLayer(int scenePosition, int position, bool lock)
{
	dWarning() << "Lock layer: " << lock;
	KTScene *scene = this->scene(scenePosition);
	
	if ( !scene)
	{
		return;
	}
	
	KTLayer *layer = scene->layer(position);
	
	if ( layer )
	{
		layer->setLocked(lock);
		
		KTLayerEvent event(KTProjectEvent::Lock, scenePosition, position, lock);
		emit commandExecuted( &event);
	}
}



QString KTProject::renameLayer(int scenePosition, int position, const QString &newName)
{
	dWarning() << "Renombrando layer: " << newName;
	
	QString oldName;
	
	KTScene *scene = this->scene(scenePosition);
	
	if ( !scene)
	{
		return oldName;
	}
	
	KTLayer *layer = scene->layer(position);
	
	if ( layer )
	{
		
		KTLayerEvent event(KTProjectEvent::Rename, scenePosition, position, newName);
		event.setPartName( layer->layerName());
		
		oldName = layer->layerName();
		
		layer->setLayerName( newName );
		
		emit commandExecuted( &event);
	}
	
	return oldName;
}



void KTProject::selectLayer(int scene, int position, bool prioritary)
{
	KTLayerEvent event(KTProjectEvent::Select, scene, position, prioritary);
	emit commandExecuted( &event );
}


void KTProject::setLayerVisibility(int scenePos, int position, bool view)
{
	KTScene *scene = this->scene(scenePos);
	
	if ( !scene)
	{
		return;
	}
	
	KTLayer *layer = scene->layer(position);
	
	if ( layer )
	{
		layer->setVisible(view);
		
		KTLayerEvent event(KTProjectEvent::View, scenePos, position, view);
		emit commandExecuted( &event );
	}
}


