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

bool KTCommandExecutor::createFrame(KTFrameResponse *response)
{
	response->setAction(KTProjectRequest::Add);
	
	int scenePosition = response->sceneIndex();
	int layerPosition = response->layerIndex();
	int position = response->frameIndex();
	QString name = response->arg().toString();
	
	QString state = response->state();
	
	KTScene *scene = m_project->scene(scenePosition);
	
	if ( !scene)
	{
		return false;
	}
	
	KTLayer *layer = scene->layer(layerPosition);
	
	if ( layer )
	{
		KTFrame *frame = layer->createFrame(position);
		
		if ( ! frame ) return false;
		
		if (!name.isEmpty())
		{
			dDebug("items") << name;
			frame->setFrameName( name );
		}
		else
		{
			response->setArg(frame->frameName());
		}
		
		emit responsed( response );
		
		if ( !state.isEmpty() )
		{
			frame->fromXml( state );
			response->setArg(frame->frameName());
		}
		
		return true;
	}
	
	return false;
}

bool KTCommandExecutor::removeFrame(KTFrameResponse *response)
{
	response->setAction(KTProjectRequest::Remove);
	
	int scenePos = response->sceneIndex();
	int layerPos = response->layerIndex();
	int position = response->frameIndex();
	
	KTScene *scene = m_project->scene(scenePos);
	
	if ( scene )
	{
		KTLayer *layer = scene->layer( layerPos );
		if ( layer )
		{
			KTFrame *frame = layer->frame( position );
			if ( frame )
			{
				QDomDocument doc;
				doc.appendChild(frame->toXml( doc ));
				response->setArg(frame->frameName());
				
				if ( layer->removeFrame(position) )
				{
					response->setState(doc.toString(0));
					emit responsed( response );
					
					return true;
				}
			}
		}
	}
	
	return false;
}


bool KTCommandExecutor::moveFrame(KTFrameResponse *response)
{
	response->setAction(KTProjectRequest::Move);
	
	int scenePos = response->sceneIndex();
	int layerPos = response->layerIndex();
	int position = response->frameIndex();
	int newPosition = response->arg().toInt();
	KTScene *scene = m_project->scene(scenePos);
	
	if ( !scene)
	{
		return false;
	}
	
	KTLayer *layer = scene->layer(layerPos);
	
	if ( layer )
	{
		if ( ! layer->moveFrame(position, newPosition) )
		{
			dWarning() << "Failed moving frame";
			return false;
		}
		else
		{
			emit responsed(response);
			return true;
		}
	}
	
	return false;
}


bool KTCommandExecutor::lockFrame(KTFrameResponse *response)
{
	response->setAction(KTProjectRequest::Lock);
	
	int scenePos = response->sceneIndex();
	int layerPos = response->layerIndex();
	int position = response->frameIndex();
	bool lock = response->arg().toBool();
	
	KTScene *scene = m_project->scene(scenePos);
	
	if ( !scene)
	{
		return false;
	}
	
	KTLayer *layer = scene->layer(layerPos);
	
	if ( layer )
	{
		KTFrame *frame = layer->frame(position);
		
		if ( ! frame ) return false;
		
		frame->setLocked( lock );
		emit responsed(response);
		return true;
	}
	
	return false;
}


bool KTCommandExecutor::renameFrame(KTFrameResponse *response)
{
	response->setAction(KTProjectRequest::Rename);
	
	int scenePos = response->sceneIndex();
	int layerPos = response->layerIndex();
	int position = response->frameIndex();
	QString newName= response->arg().toString();
	
	QString oldName;
	
	KTScene *scene = m_project->scene(scenePos);
	
	if ( !scene)
	{
		return false;
	}
	
	KTLayer *layer = scene->layer(layerPos);
	
	if ( layer )
	{
		KTFrame *frame = layer->frame(position);
		
		if ( ! frame ) return false;
		
		oldName = frame->frameName();
		
		frame->setFrameName( newName );
		
		emit responsed(response);
		return true;
	}
	
	return false;
}


bool KTCommandExecutor::selectFrame(KTFrameResponse *response)
{
	response->setAction(KTProjectRequest::Select);
	
	int scenePos = response->sceneIndex();
	int layerPos = response->layerIndex();
	int position = response->frameIndex();
	bool prioritary = response->arg().toBool();
	
	KTScene *scene = m_project->scene(scenePos);
	
	if ( !scene)
	{
		return false;
	}
	
	KTLayer *layer = scene->layer(layerPos);
	
	if ( layer )
	{
		KTFrame *frame = layer->frame(position);
		
		if ( ! frame ) return false;
		
		emit responsed(response);
		return true;
	}
	
	return false;
}


bool KTCommandExecutor::setFrameVisibility(KTFrameResponse *response)
{
	response->setAction(KTProjectRequest::View);
	int scenePos = response->sceneIndex();
	int layerPos = response->layerIndex();
	int position = response->frameIndex();
	bool view = response->arg().toBool();
	
	KTScene *scene = m_project->scene(scenePos);
	
	if ( !scene)
	{
		return false;
	}
	
	KTLayer *layer = scene->layer(layerPos);
	
	if ( layer )
	{
		KTFrame *frame = layer->frame(position);
		
		if ( ! frame ) return false;
		
		
		frame->setVisible(view);
		emit responsed(response);
		return true;
	}
	
	return false;
}






