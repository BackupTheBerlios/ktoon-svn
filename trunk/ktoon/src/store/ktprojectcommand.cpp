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

KTProjectCommand::KTProjectCommand(KTProject *project, const KTProjectEvent *event) : QUndoCommand(), m_project(project)
{
	DINIT;
	
	m_event = event->clone();
	m_event->setParent(0);
}


KTProjectCommand::~KTProjectCommand()
{
	DEND;
	
	if ( m_event ) delete m_event;
}


void KTProjectCommand::redo()
{
	D_FUNCINFO << m_xml;
	
	switch(m_event->id() )
	{
		case KTProjectEvent::Project:
		{
			dDebug() << "Project event isn't handle";
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
	D_FUNCINFO << m_xml;
	switch(m_event->id() )
	{
		case KTProjectEvent::Project:
		{
			dDebug() << "Project event isn't handle";
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

void KTProjectCommand::frameCommand(const KTFrameEvent *event, bool redo)
{
	if ( redo )
	{
		switch(event->action())
		{
			case KTProjectEvent::Add:
			{
				m_project->createFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), m_xml );
			}
			break;
			case KTProjectEvent::Remove:
			{
				m_xml = m_project->removeFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex() );
			}
			break;
			case KTProjectEvent::Move:
			{
				const KTMoveFrameEvent *moveEvent = static_cast<const KTMoveFrameEvent *>(event);
				
				m_project->moveFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), moveEvent->newFrameIndex() );
			}
			break;
			case KTProjectEvent::Lock:
			{
				const KTLockFrameEvent *lockEvent = static_cast<const KTLockFrameEvent *>(event);
				
				m_project->lockFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), lockEvent->isLocked() );
			}
			break;
			case KTProjectEvent::Rename:
			{
				const KTRenameFrameEvent *renameEvent = static_cast<const KTRenameFrameEvent *>(event);
				
				m_project->renameFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), renameEvent->newName() );
			}
			break;
		}
	}
	else
	{
		switch(event->action())
		{
			case KTProjectEvent::Add:
			{
				m_xml = m_project->removeFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex());
			}
			break;
			case KTProjectEvent::Remove:
			{
				m_project->createFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), m_xml);
			}
			break;
			case KTProjectEvent::Move:
			{
				const KTMoveFrameEvent *moveEvent = static_cast<const KTMoveFrameEvent *>(event);
				
				m_project->moveFrame(event->sceneIndex(), event->layerIndex(), moveEvent->newFrameIndex(), event->frameIndex() );
			}
			break;
			case KTProjectEvent::Lock:
			{
				const KTLockFrameEvent *lockEvent = static_cast<const KTLockFrameEvent *>(event);
				
				m_project->lockFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), !lockEvent->isLocked() );
			}
			break;
			case KTProjectEvent::Rename:
			{
				const KTRenameFrameEvent *renameEvent = static_cast<const KTRenameFrameEvent *>(event);
				
				m_project->renameFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), renameEvent->partName() );
			}
			break;
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
				m_project->createLayer( event->sceneIndex(), event->layerIndex(), m_xml);
			}
			break;
			case KTProjectEvent::Remove:
			{
				m_xml = m_project->removeLayer( event->sceneIndex(), event->layerIndex());
			}
			break;
			case KTProjectEvent::Move:
			{
				const KTMoveLayerEvent *moveEvent = static_cast<const KTMoveLayerEvent *>(event);
				
				m_project->moveLayer( event->sceneIndex(), event->layerIndex(), moveEvent->newLayerIndex() );
			}
			break;
			case KTProjectEvent::Lock:
			{
				const KTLockLayerEvent *lockEvent = static_cast<const KTLockLayerEvent *>(event);
				
				m_project->lockLayer( event->sceneIndex(), event->layerIndex(), lockEvent->isLocked() );
			}
			break;
			case KTProjectEvent::Rename:
			{
				const KTRenameLayerEvent *renameEvent = static_cast<const KTRenameLayerEvent *>(event);
				
				m_project->renameLayer( event->sceneIndex(), event->layerIndex(), renameEvent->newName() );
			}
			break;
		}
	}
	else
	{
		switch(event->action())
		{
			case KTProjectEvent::Add:
			{
				m_xml = m_project->removeLayer( event->sceneIndex(), event->layerIndex());
			}
			break;
			case KTProjectEvent::Remove:
			{
				m_project->createLayer( event->sceneIndex(), event->layerIndex(), m_xml);
			}
			break;
			case KTProjectEvent::Move:
			{
				const KTMoveLayerEvent *moveEvent = static_cast<const KTMoveLayerEvent *>(event);
				
				m_project->moveLayer( event->sceneIndex(), moveEvent->newLayerIndex(),event->layerIndex()  );
			}
			break;
			case KTProjectEvent::Lock:
			{
				const KTLockLayerEvent *lockEvent = static_cast<const KTLockLayerEvent *>(event);
				
				m_project->lockLayer( event->sceneIndex(), event->layerIndex(), !lockEvent->isLocked() );
			}
			break;
			case KTProjectEvent::Rename:
			{
				const KTRenameLayerEvent *renameEvent = static_cast<const KTRenameLayerEvent *>(event);
				
				m_project->renameLayer( event->sceneIndex(), event->layerIndex(), renameEvent->partName() );
			}
			break;
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
				m_project->createScene( event->sceneIndex(), m_xml );
			}
			break;
			case KTProjectEvent::Remove:
			{
				m_xml = m_project->removeScene( event->sceneIndex() );
			}
			break;
			case KTProjectEvent::Move:
			{
				const KTMoveSceneEvent *moveEvent = static_cast<const KTMoveSceneEvent *>(event);
				
				m_project->moveScene( event->sceneIndex(), moveEvent->newSceneIndex() );
			}
			break;
			case KTProjectEvent::Lock:
			{
				const KTLockSceneEvent *lockEvent = static_cast<const KTLockSceneEvent *>(event);
				
				m_project->lockScene( event->sceneIndex(), lockEvent->isLocked() );
			}
			break;
			case KTProjectEvent::Rename:
			{
				const KTRenameSceneEvent *renameEvent = static_cast<const KTRenameSceneEvent *>(event);
				
				m_project->renameScene( event->sceneIndex(), renameEvent->newName() );
			}
			break;
		}
	}
	else
	{
		switch(event->action())
		{
			case KTProjectEvent::Add:
			{
				m_xml = m_project->removeScene( event->sceneIndex() );
			}
			break;
			case KTProjectEvent::Remove:
			{
				m_project->createScene( event->sceneIndex(), m_xml );
			}
			break;
			case KTProjectEvent::Move:
			{
				const KTMoveSceneEvent *moveEvent = static_cast<const KTMoveSceneEvent *>(event);
				
				m_project->moveScene( moveEvent->newSceneIndex(), event->sceneIndex() );
			}
			break;
			case KTProjectEvent::Lock:
			{
				const KTLockSceneEvent *lockEvent = static_cast<const KTLockSceneEvent *>(event);
				
				m_project->lockScene( event->sceneIndex(), !lockEvent->isLocked() );
			}
			break;
			case KTProjectEvent::Rename:
			{
				const KTRenameSceneEvent *renameEvent = static_cast<const KTRenameSceneEvent *>(event);
				
				m_project->renameScene( event->sceneIndex(), renameEvent->partName() );
			}
			break;
		}
	}
}

