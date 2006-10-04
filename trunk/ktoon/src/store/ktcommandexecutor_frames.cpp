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
#include "ktframerequest.h"

#include <dcore/ddebug.h>

QString KTCommandExecutor::createFrame(int scenePosition, int layerPosition, int position, const QString &xml)
{
	KTScene *scene = m_project->scene(scenePosition);
	
	if ( !scene)
	{
		return 0;
	}
	
	KTLayer *layer = scene->layer(layerPosition);
	
	if ( layer )
	{
		KTFrame *frame = layer->createFrame(position);
		
		if ( ! frame ) return 0;
		
		KTFrameRequest event(KTProjectRequest::Add, scenePosition, layerPosition, position );
		
		
		QDomDocument document;
		if ( document.setContent(xml) )
		{
			QDomElement root = document.documentElement();
	
			frame->setFrameName( root.attribute( "name", frame->frameName() ) );
			
			event.setPartName(frame->frameName());
			emit commandExecuted(&event, m_isRedo);
			
			QDomNode n = root.firstChild();
	
			while( !n.isNull() )
			{
				QDomElement e = n.toElement();
		
				if(!e.isNull())
				{
// 					dDebug() << "Item??? " << e.tagName();
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
			emit commandExecuted(&event, m_isRedo);
		}
		
// 		dDebug() << "Añadiendo frame en layer: " << layer->layerName();
		
		return document.toString(0);
	}
	else
	{
		D_CHECKPTR(layer);
	}
	
	return QString();
}

QString KTCommandExecutor::removeFrame(int scenePos, int layerPos, int position)
{
	KTScene *scene = m_project->scene(scenePos);
	
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
					KTFrameRequest event(KTProjectRequest::Remove, scenePos, layerPos, position);
					
					emit commandExecuted(&event, m_isRedo);
					
					return document.toString(0);
				}
			}
		}
	}
	
	return QString();
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
			KTFrameRequest event(KTProjectRequest::Move, scenePosition, layerPosition, position, newPosition);
			emit commandExecuted(&event, m_isRedo);
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
		
		KTFrameRequest event(KTProjectRequest::Lock, scenePosition, layerPosition, position, lock);
		emit commandExecuted(&event, m_isRedo);
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
		
		
		KTFrameRequest event(KTProjectRequest::Rename, scenePosition, layerPosition, position, newName);
		event.setPartName(frame->frameName());
		
		oldName = frame->frameName();
		
		frame->setFrameName( newName );
		
		emit commandExecuted(&event, m_isRedo);
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
		
		KTFrameRequest event(KTProjectRequest::Select, scenePosition, layerPosition, position, prioritary);
		emit commandExecuted(&event, m_isRedo);
	}
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
		
		
		KTFrameRequest event(KTProjectRequest::View, scenePos, layerPos, position, view);
		
		frame->setVisible(view);
		
		emit commandExecuted(&event, m_isRedo);
	}
	
	return QString();
}








