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

#include "ktscene.h"

#include "ktlayerrequest.h"
#include <dcore/ddebug.h>

QString KTCommandExecutor::createLayer(int scenePosition, int position, const QString &xml)
{
	KTScene *scene = m_project->scene(scenePosition);
	
	if ( scene )
	{
		KTLayer *layer = scene->createLayer(position);
		
		if ( ! layer ) return 0;
		
		KTLayerRequest event(KTProjectRequest::Add, scenePosition, position );
		QDomDocument document;
		if ( document.setContent(xml) )
		{
			QDomElement root = document.documentElement();
	
			layer->setLayerName( root.attribute( "name", layer->layerName() ) );
	
			QDomNode n = root.firstChild();
			
			event.setPartName(layer->layerName());
			emit commandExecuted(&event, m_state);
	
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
			emit commandExecuted(&event, m_state);
		}
		
// 		dDebug() << "A�adiendo layer en escena: " << scene->sceneName();
		
		return document.toString(0);
	}
	else
	{
		D_CHECKPTR(scene);
	}
	
	return 0;
}


QString KTCommandExecutor::removeLayer(int scenePos, int position)
{
	KTScene *scene = m_project->scene(scenePos);
	
	if ( scene )
	{
		KTLayer *layer = scene->layer( position );
		if ( layer )
		{
			QDomDocument document;
			
			document.appendChild(layer->toXml(document));
			
			if ( scene->removeLayer(position) )
			{
				KTLayerRequest event(KTProjectRequest::Remove, scenePos, position);
				
				emit commandExecuted(&event, m_state);
				
				return document.toString(0);
			}
		}
	}
	
	return QString();
}


QString KTCommandExecutor::moveLayer(int scenePosition, int position, int newPosition)
{
// 	dDebug() << "Move layer from " << position << " to " << newPosition;
	KTScene *scene = m_project->scene(scenePosition);
	
	if ( !scene)
	{
		return QString();
	}
	
	if ( ! scene->moveLayer(position, newPosition) )
	{
		dWarning() << "Failed moving layer";
	}
	else
	{
		KTLayerRequest event(KTProjectRequest::Move,scenePosition, position, newPosition);
		emit commandExecuted(&event, m_state);
	}
	
	
	return QString();

}

QString KTCommandExecutor::lockLayer(int scenePosition, int position, bool lock)
{
	dWarning() << "Lock layer: " << lock;
	KTScene *scene = m_project->scene(scenePosition);
	
	if ( !scene)
	{
		return QString();
	}
	
	KTLayer *layer = scene->layer(position);
	
	if ( layer )
	{
		layer->setLocked(lock);
		
		KTLayerRequest event(KTProjectRequest::Lock, scenePosition, position, lock);
		emit commandExecuted(&event, m_state);
	}
	
	return QString();
}



QString KTCommandExecutor::renameLayer(int scenePosition, int position, const QString &newName)
{
	dWarning() << "Renombrando layer: " << newName;
	
	QString oldName;
	
	KTScene *scene = m_project->scene(scenePosition);
	
	if ( !scene)
	{
		return oldName;
	}
	
	KTLayer *layer = scene->layer(position);
	
	if ( layer )
	{
		
		KTLayerRequest event(KTProjectRequest::Rename, scenePosition, position, newName);
		event.setPartName( layer->layerName());
		
		oldName = layer->layerName();
		
		layer->setLayerName( newName );
		
		emit commandExecuted(&event, m_state);
	}
	
	return oldName;
}



QString KTCommandExecutor::selectLayer(int scene, int position, bool prioritary)
{
	KTLayerRequest event(KTProjectRequest::Select, scene, position, prioritary);
	emit commandExecuted(&event, m_state);
	
	return QString();
}


QString KTCommandExecutor::setLayerVisibility(int scenePos, int position, bool view)
{
	KTScene *scene = m_project->scene(scenePos);
	
	if ( !scene)
	{
		return QString();;
	}
	
	KTLayer *layer = scene->layer(position);
	
	if ( layer )
	{
		layer->setVisible(view);
		
		KTLayerRequest event(KTProjectRequest::View, scenePos, position, view);
		emit commandExecuted(&event, m_state);
	}
	
	return QString();
}


