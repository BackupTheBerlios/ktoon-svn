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

bool KTCommandExecutor::createFrame(KTProjectResponse *response)
{
	KTFrameResponse *frameResponse = static_cast<KTFrameResponse *>(response);
	
	int scenePosition = frameResponse->sceneIndex();
	int layerPosition = frameResponse->layerIndex();
	int position = frameResponse->frameIndex();
	QString name = frameResponse->arg().toString();
	QString state = frameResponse->state();
	
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
			frame->setFrameName( name );
		}
		frame->fromXml( state );
		
		emit responsed( response, m_state );
		
		return true;
	}
	
	return false;
}

bool KTCommandExecutor::removeFrame(KTProjectResponse *response)
{
	KTFrameResponse *frameResponse = static_cast<KTFrameResponse *>(response);
	
	int scenePos = frameResponse->sceneIndex();
	int layerPos = frameResponse->layerIndex();
	int position = frameResponse->frameIndex();
	
	
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
				
				if ( layer->removeFrame(position) )
				{
					frameResponse->setState(doc.toString(0));
					emit responsed( response, m_state );
					
					return true;
				}
			}
		}
	}
	
	return false;
}


QString KTCommandExecutor::moveFrame(int scenePosition, int layerPosition, int position, int newPosition)
{
// 	dDebug() << "Move frame from " << position << " to " << newPosition;
	KTScene *scene = m_project->scene(scenePosition);
	
	if ( !scene)
	{
		return QString();
	}
	
	KTLayer *layer = scene->layer(layerPosition);
	
	if ( layer )
	{
		if ( ! layer->moveFrame(position, newPosition) )
		{
			dWarning() << "Failed moving frame";
			return QString();
		}
		else
		{
			KTProjectRequest request = KTRequestBuilder::createFrameRequest( scenePosition, layerPosition, position, KTProjectRequest::Move, QString::number( newPosition) );
			emit commandExecuted(&request, m_state);
		}
	}
	
	return QString();
}


QString KTCommandExecutor::lockFrame(int scenePosition, int layerPosition, int position, bool lock)
{
	dWarning() << "Lock frame: " << lock;
	KTScene *scene = m_project->scene(scenePosition);
	
	if ( !scene)
	{
		return QString();
	}
	
	KTLayer *layer = scene->layer(layerPosition);
	
	if ( layer )
	{
		KTFrame *frame = layer->frame(position);
		
		if ( ! frame ) return QString();
		
		frame->setLocked( lock );
		
		KTProjectRequest request = KTRequestBuilder::createFrameRequest( scenePosition, layerPosition, position, KTProjectRequest::Lock, lock ? "1" : "0" );
		emit commandExecuted(&request, m_state);
	}
	
	return QString();
}


QString KTCommandExecutor::renameFrame(int scenePosition, int layerPosition, int position, const QString &newName)
{
	dWarning() << "Renombrando frame " << position << ": " << newName;
	
	QString oldName;
	
	KTScene *scene = m_project->scene(scenePosition);
	
	if ( !scene)
	{
		return oldName;
	}
	
	KTLayer *layer = scene->layer(layerPosition);
	
	if ( layer )
	{
		KTFrame *frame = layer->frame(position);
		
		if ( ! frame ) return oldName;
		
		
		KTProjectRequest request = KTRequestBuilder::createFrameRequest( scenePosition, layerPosition, position, KTProjectRequest::Rename, newName);
		
		oldName = frame->frameName();
		
		frame->setFrameName( newName );
		
		emit commandExecuted(&request, m_state);
	}
	
	return oldName;
}




QString KTCommandExecutor::selectFrame(int scenePosition, int layerPosition, int position, bool prioritary)
{
	KTScene *scene = m_project->scene(scenePosition);
	
	if ( !scene)
	{
		return QString();
	}
	
	KTLayer *layer = scene->layer(layerPosition);
	
	if ( layer )
	{
		KTFrame *frame = layer->frame(position);
		
		if ( ! frame ) return QString();;
		
		KTProjectRequest request = KTRequestBuilder::createFrameRequest( scenePosition, layerPosition, position, KTProjectRequest::Select, prioritary ? "1" : "0" );
		emit commandExecuted(&request, m_state);
	}
	
	return QString();
}

QString KTCommandExecutor::setFrameVisibility(int scenePos, int layerPos, int position, bool view)
{
	KTScene *scene = m_project->scene(scenePos);
	
	if ( !scene)
	{
		return QString();;
	}
	
	KTLayer *layer = scene->layer(layerPos);
	
	if ( layer )
	{
		KTFrame *frame = layer->frame(position);
		
		if ( ! frame ) return QString();;
		
		
		KTProjectRequest request = KTRequestBuilder::createFrameRequest( scenePos, layerPos, position, KTProjectRequest::View, view ? "1" : "0" );
		
		frame->setVisible(view);
		
		emit commandExecuted(&request, m_state);
	}
	
	return QString();
}








