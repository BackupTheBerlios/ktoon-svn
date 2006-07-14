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
				m_project->moveFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->data().toInt() );
			}
			break;
			case KTProjectEvent::Lock:
			{
				m_project->lockFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->data().toBool() );
			}
			break;
			case KTProjectEvent::Rename:
			{
				m_event->setPartName(m_project->renameFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->data().toString() ));
			}
			break;
			case KTProjectEvent::Select:
			{
				m_project->selectFrame(event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->data().toBool() );
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
				m_project->moveFrame(event->sceneIndex(), event->layerIndex(), event->data().toInt(), event->frameIndex() );
			}
			break;
			case KTProjectEvent::Lock:
			{
				m_project->lockFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), !event->data().toBool() );
			}
			break;
			case KTProjectEvent::Rename:
			{
				m_project->renameFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->partName() );
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
				m_project->moveLayer( event->sceneIndex(), event->layerIndex(), event->data().toInt() );
			}
			break;
			case KTProjectEvent::Lock:
			{
				m_project->lockLayer( event->sceneIndex(), event->layerIndex(), event->data().toBool() );
			}
			break;
			case KTProjectEvent::Rename:
			{
				m_event->setPartName(m_project->renameLayer( event->sceneIndex(), event->layerIndex(), event->data().toString()));
			}
			break;
			case KTProjectEvent::Select:
			{
				m_project->selectLayer(event->sceneIndex(), event->layerIndex(), event->data().toBool() );
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
				m_project->moveLayer( event->sceneIndex(), event->data().toInt(),event->layerIndex()  );
			}
			break;
			case KTProjectEvent::Lock:
			{
				m_project->lockLayer( event->sceneIndex(), event->layerIndex(), !event->data().toBool() );
			}
			break;
			case KTProjectEvent::Rename:
			{
				m_project->renameLayer( event->sceneIndex(), event->layerIndex(), event->partName() );
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
				m_project->moveScene( event->sceneIndex(), event->data().toInt() );
			}
			break;
			case KTProjectEvent::Lock:
			{
				m_project->lockScene( event->sceneIndex(), event->data().toBool() );
			}
			break;
			case KTProjectEvent::Rename:
			{
				m_event->setPartName(m_project->renameScene( event->sceneIndex(), event->data().toString()));
			}
			break;
			case KTProjectEvent::Select:
			{
				m_project->selectScene(event->sceneIndex(), event->data().toBool() );
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
				m_project->moveScene( event->data().toInt(), event->sceneIndex() );
			}
			break;
			case KTProjectEvent::Lock:
			{
				m_project->lockScene( event->sceneIndex(), !event->data().toBool() );
			}
			break;
			case KTProjectEvent::Rename:
			{
				m_project->renameScene( event->sceneIndex(), event->partName() );
			}
			break;
		}
	}
}




