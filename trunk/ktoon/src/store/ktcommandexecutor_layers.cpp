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

#include "ktprojectrequest.h"
#include "ktrequestbuilder.h"

#include <dcore/ddebug.h>

QString KTCommandExecutor::createLayer(int scenePosition, int position, const QString &xml)
{
	KTScene *scene = m_project->scene(scenePosition);
	
	if ( scene )
	{
		KTLayer *layer = scene->createLayer(position);
		
		if ( ! layer ) return 0;
		
		QDomDocument document;
		if ( document.setContent(xml) )
		{
			QDomElement root = document.documentElement();
	
			layer->setLayerName( root.attribute( "name", layer->layerName() ) );
			
			
			KTProjectRequest request = KTRequestBuilder::createLayerRequest( scenePosition, position, KTProjectRequest::Add, layer->layerName() );
			
			QDomNode n = root.firstChild();
			
			emit commandExecuted(&request, m_state);
	
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
		
// 		dDebug() << "Añadiendo layer en escena: " << scene->sceneName();
		
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
				KTProjectRequest request = KTRequestBuilder::createLayerRequest( scenePos, position, KTProjectRequest::Remove );
				
				emit commandExecuted(&request, m_state);
				
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
		KTProjectRequest request = KTRequestBuilder::createLayerRequest( scenePosition, position, KTProjectRequest::Move, QString::number(newPosition)  );
		emit commandExecuted(&request, m_state);
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
		
		KTProjectRequest request = KTRequestBuilder::createLayerRequest( scenePosition, position, KTProjectRequest::Lock, lock ? "1" : "0" );
		emit commandExecuted(&request, m_state);
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
		
		KTProjectRequest request = KTRequestBuilder::createLayerRequest( scenePosition, position, KTProjectRequest::Rename, newName);
		
		oldName = layer->layerName();
		
		layer->setLayerName( newName );
		
		emit commandExecuted(&request, m_state);
	}
	
	return oldName;
}



QString KTCommandExecutor::selectLayer(int scene, int position, bool prioritary)
{
	KTProjectRequest request = KTRequestBuilder::createLayerRequest( scene, position, KTProjectRequest::Select, prioritary ? "1" : "0" );
	emit commandExecuted(&request, m_state);
	
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
		
		KTProjectRequest request = KTRequestBuilder::createLayerRequest( scenePos, position, KTProjectRequest::View, view ? "1" : "0" );
		emit commandExecuted(&request, m_state);
	}
	
	return QString();
}


