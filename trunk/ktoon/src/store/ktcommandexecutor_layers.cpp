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
#include "ktprojectresponse.h"

#include <dcore/ddebug.h>

bool KTCommandExecutor::createLayer(KTLayerResponse *response)
{
	int scenePosition = response->sceneIndex();
	int position = response->layerIndex();
	QString name = response->arg().toString();
	QString state = response->state();
	
	KTScene *scene = m_project->scene(scenePosition);
	
	if ( scene )
	{
		KTLayer *layer = scene->createLayer(position);
		
		if ( ! layer ) return false;
		
		if (!name.isEmpty())
		{
			layer->setLayerName( name );
		}
		
		layer->fromXml( state );
		
		response->setArg(layer->layerName());
		emit responsed(response, m_state);
		return true;
	}
	return false;
}


bool KTCommandExecutor::removeLayer(KTLayerResponse *response)
{
	int scenePos = response->sceneIndex();
	int position = response->layerIndex();
	
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
				
				emit responsed(response, m_state);
				
				return true;
			}
		}
	}
	
	return false;
}


bool KTCommandExecutor::moveLayer(KTLayerResponse *response)
{
	int scenePos = response->sceneIndex();
	int position = response->layerIndex();
	int newPosition = response->arg().toInt();
	
	
	KTScene *scene = m_project->scene(scenePos);
	
	if ( !scene)
	{
		return false;
	}
	
	if ( ! scene->moveLayer(position, newPosition) )
	{
		dWarning() << "Failed moving layer";
		return false;
	}
	else
	{
		emit responsed(response, m_state);
		return true;
		
	}
	return false;
}


bool KTCommandExecutor::lockLayer(KTLayerResponse *response)
{
	int scenePos = response->sceneIndex();
	int position = response->layerIndex();
	bool lock = response->arg().toBool();
	
	KTScene *scene = m_project->scene(scenePos);
	
	if ( !scene)
	{
		return false;
	}
	
	KTLayer *layer = scene->layer(position);
	
	if ( layer )
	{
		layer->setLocked(lock);
		emit responsed(response, m_state);
		return true;
	}
	
	return false;
}



bool KTCommandExecutor::renameLayer(KTLayerResponse *response)
{
	int scenePos = response->sceneIndex();
	int position = response->layerIndex();
	QString newName = response->arg().toString();
	
	dWarning() << "Renombrando layer: " << newName;
	
	QString oldName;
	
	KTScene *scene = m_project->scene(scenePos);
	
	if ( !scene)
	{
		return false;
	}
	
	KTLayer *layer = scene->layer(position);
	
	if ( layer )
	{
		layer->setLayerName( newName );
		emit responsed(response, m_state);
		return true;
	}
	
	return false;
}



bool KTCommandExecutor::selectLayer(KTLayerResponse *response)
{
	emit responsed(response, m_state);
	return true;
}


bool KTCommandExecutor::setLayerVisibility(KTLayerResponse *response)
{
	int scenePos = response->sceneIndex();
	int position = response->layerIndex();
	bool view = response->arg().toBool();
	
	
	KTScene *scene = m_project->scene(scenePos);
	
	if ( !scene)
	{
		return false;
	}
	
	KTLayer *layer = scene->layer(position);
	
	if ( layer )
	{
		layer->setVisible(view);
		responsed(response, m_state);
		return true;
	}
	
	return false;
}


