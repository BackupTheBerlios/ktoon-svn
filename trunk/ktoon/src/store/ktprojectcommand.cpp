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

#include "ktprojectcommand.h"

#include <ddebug.h>

#include "ktproject.h"
#include "ktprojectevent.h"
#include "ktframeevent.h"

KTProjectCommand::KTProjectCommand(KTProject *project, const KTProjectEvent *event) : QUndoCommand(), m_project(project), m_event(event)
{
	
}


KTProjectCommand::~KTProjectCommand()
{
}


void KTProjectCommand::redo()
{
	D_FUNCINFO;
	
	switch(m_event->id() )
	{
		case KTProjectEvent::Project:
		{
			projectCommand(m_event, true);
		}
		break;
		case KTProjectEvent::Frame:
		{
			frameCommand( static_cast<const KTFrameEvent *>(m_event), true);
		}
		break;
		case KTProjectEvent::Layer:
		{
			layerCommand( static_cast<const KTLayerEvent *>(m_event), true);
		}
		break;
		case KTProjectEvent::Scene:
		{
			sceneCommand( static_cast<const KTSceneEvent *>(m_event), true);
		}
		break;
		default:
		{
			D_FUNCINFO << ("Unknown project event!");
		}
		break;
	}
	
	
}

void KTProjectCommand::undo()
{
	D_FUNCINFO;
	switch(m_event->id() )
	{
		case KTProjectEvent::Project:
		{
			projectCommand(m_event, false);
		}
		break;
		case KTProjectEvent::Frame:
		{
			frameCommand( static_cast<const KTFrameEvent *>(m_event), false);
		}
		break;
		case KTProjectEvent::Layer:
		{
			layerCommand( static_cast<const KTLayerEvent *>(m_event), false);
		}
		break;
		case KTProjectEvent::Scene:
		{
			sceneCommand( static_cast<const KTSceneEvent *>(m_event), false);
		}
		break;
		default:
		{
			D_FUNCINFO << ("Unknown project event!");
		}
		break;
	}
}


void KTProjectCommand::projectCommand(const KTProjectEvent *event, bool redo)
{
	if ( redo )
	{
		switch(event->action())
		{
			case KTProjectEvent::Add:
			{
			}
			break;
			case KTProjectEvent::Remove:
			{
			}
			break;
// 			case KTProjectEvent:::
// 			{
// 			}
// 			break;
// 			case KTProjectEvent::Add:
// 			{
// 			}
// 			break;
			
		}
	}
	else
	{
		switch(event->action())
		{
			case KTProjectEvent::Add:
			{
// 				m_project
			}
			break;
			case KTProjectEvent::Remove:
			{
			}
			break;
// 			case KTProjectEvent:::
// 			{
// 			}
// 			break;
// 			case KTProjectEvent::Add:
// 			{
// 			}
// 			break;
			
		}
	}
}

void KTProjectCommand::frameCommand(const KTFrameEvent *event, bool redo)
{
	if ( redo )
	{
		switch(event->action())
		{
			case KTProjectEvent::Add:
			{
				m_project->createFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex() );
			}
			break;
			case KTProjectEvent::Remove:
			{
			}
			break;
// 			case KTProjectEvent:::
// 			{
// 			}
// 			break;
// 			case KTProjectEvent::Add:
// 			{
// 			}
// 			break;
			
		}
	}
	else
	{
		switch(event->action())
		{
			case KTProjectEvent::Add:
			{
			}
			break;
			case KTProjectEvent::Remove:
			{
			}
			break;
// 			case KTProjectEvent:::
// 			{
// 			}
// 			break;
// 			case KTProjectEvent::Add:
// 			{
// 			}
// 			break;
			
		}
	}
}

void KTProjectCommand::layerCommand(const KTLayerEvent *event, bool redo)
{
	if ( redo )
	{
		switch(event->action())
		{
			case KTProjectEvent::Add:
			{
				m_project->createLayer( event->sceneIndex(), event->layerIndex());
			}
			break;
			case KTProjectEvent::Remove:
			{
			}
			break;
// 			case KTProjectEvent:::
// 			{
// 			}
// 			break;
// 			case KTProjectEvent::Add:
// 			{
// 			}
// 			break;
			
		}
	}
	else
	{
		switch(event->action())
		{
			case KTProjectEvent::Add:
			{
			}
			break;
			case KTProjectEvent::Remove:
			{
			}
			break;
// 			case KTProjectEvent:::
// 			{
// 			}
// 			break;
// 			case KTProjectEvent::Add:
// 			{
// 			}
// 			break;
			
		}
	}
}

void KTProjectCommand::sceneCommand(const KTSceneEvent *event, bool redo)
{
	if ( redo )
	{
		switch(event->action())
		{
			case KTProjectEvent::Add:
			{
				m_project->createScene( event->sceneIndex() );
			}
			break;
			case KTProjectEvent::Remove:
			{
			}
			break;
// 			case KTProjectEvent:::
// 			{
// 			}
// 			break;
// 			case KTProjectEvent::Add:
// 			{
// 			}
// 			break;
			
		}
	}
	else
	{
		switch(event->action())
		{
			case KTProjectEvent::Add:
			{
			}
			break;
			case KTProjectEvent::Remove:
			{
			}
			break;
// 			case KTProjectEvent:::
// 			{
// 			}
// 			break;
// 			case KTProjectEvent::Add:
// 			{
// 			}
// 			break;
			
		}
	}
}

