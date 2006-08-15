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
#include "ktitemevent.h"

KTProjectCommand::KTProjectCommand(KTProject *project, const KTProjectEvent *event) : QUndoCommand(), m_project(project)
{
	DINIT;
	
	m_event = event->clone();
	
	switch(m_event->id())
	{
		case KTProjectEvent::Frame:
		{
			setText(actionString( m_event->action() ) +" frame" );
		}
		break;
		case KTProjectEvent::Layer:
		{
			setText(actionString( m_event->action() )+" layer");
		}
		break;
		case KTProjectEvent::Scene:
		{
			setText(actionString( m_event->action() )+" scene");
		}
		break;
		case KTProjectEvent::Item:
		{
			setText(actionString( m_event->action() )+" item");
		}
		break;
	}
}

QString KTProjectCommand::actionString(int action)
{
	switch(action)
	{
		case KTProjectEvent::Add:
		{
			return QObject::tr("add");
		}
		break;
		case KTProjectEvent::Remove:
		{
			return QObject::tr("remove");
		}
		break;
		case KTProjectEvent::Move:
		{
			return QObject::tr("move");
		}
		break;
		case KTProjectEvent::Lock:
		{
			return QObject::tr("lock");
		}
		break;
		case KTProjectEvent::Rename:
		{
			return QObject::tr("rename");
		}
		break;
		case KTProjectEvent::Select:
		{
			return QObject::tr("select");
		}
		break;
		case KTProjectEvent::View:
		{
			return QObject::tr("view");
		}
		break;
		case KTProjectEvent::Transform:
		{
			return QObject::tr("transform");
		}
		break;
	}
	
	return QString();
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
		case KTProjectEvent::Item:
		{
			itemCommand( static_cast<const KTItemEvent *>(m_event), true);
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
		case KTProjectEvent::Item:
		{
			itemCommand( static_cast<const KTItemEvent *>(m_event), false);
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
			case KTProjectEvent::View:
			{
				m_project->setFrameVisibility(event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->data().toBool());
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
			case KTProjectEvent::View:
			{
				m_project->setFrameVisibility(event->sceneIndex(), event->layerIndex(), event->frameIndex(), !event->data().toBool());
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
			case KTProjectEvent::View:
			{
				m_project->setLayerVisibility(event->sceneIndex(), event->layerIndex(), event->data().toBool());
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
			case KTProjectEvent::View:
			{
				m_project->setLayerVisibility(event->sceneIndex(), event->layerIndex(), !event->data().toBool());
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
			case KTProjectEvent::View:
			{
				m_project->setSceneVisibility(event->sceneIndex(),  event->data().toBool());
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
			case KTProjectEvent::View:
			{
				m_project->setSceneVisibility(event->sceneIndex(),  !event->data().toBool());
			}
			break;
		}
	}
}


void KTProjectCommand::itemCommand(const KTItemEvent *event, bool redo)
{
	D_FUNCINFO;
	if ( redo )
	{
		switch(event->action())
		{
			case KTProjectEvent::Add:
			{
				m_project->createItem(event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->itemIndex(), event->data().toString());
			}
			break;
			case KTProjectEvent::Remove:
			{
				m_project->removeItem(event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->itemIndex());
			}
			break;
			case KTProjectEvent::Move:
			{
			}
			break;
			case KTProjectEvent::Lock:
			{
			}
			break;
			case KTProjectEvent::Rename:
			{
			}
			break;
			case KTProjectEvent::Select:
			{
			}
			break;
			case KTProjectEvent::View:
			{
			}
			break;
			case KTProjectEvent::Transform:
			{
				m_xml = m_project->transformItem(event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->itemIndex(), event->data().toString());
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
				m_project->removeItem(event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->itemIndex());
			}
			break;
			case KTProjectEvent::Remove:
			{
				m_project->createItem(event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->itemIndex(), event->data().toString());
			}
			break;
			case KTProjectEvent::Move:
			{
			}
			break;
			case KTProjectEvent::Lock:
			{
			}
			break;
			case KTProjectEvent::Rename:
			{
			}
			break;
			case KTProjectEvent::View:
			{
			}
			break;
			case KTProjectEvent::Transform:
			{
				m_project->transformItem(event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->itemIndex(), m_xml);
			}
			break;
		}
	}
}


