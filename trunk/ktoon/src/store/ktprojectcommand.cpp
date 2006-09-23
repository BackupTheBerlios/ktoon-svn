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
#include "ktprojectrequest.h"
#include "ktframerequest.h"
#include "ktitemrequest.h"
#include "ktpaintareaevent.h"

#include "ktsvg2qt.h"

KTProjectCommand::KTProjectCommand(KTProject *project, const KTProjectRequest *event) : QUndoCommand(), m_project(project)
{
	m_event = event->clone();
	
	switch(m_event->id())
	{
		case KTProjectRequest::Frame:
		{
			setText(actionString( m_event->action() ) +" frame" );
		}
		break;
		case KTProjectRequest::Layer:
		{
			setText(actionString( m_event->action() )+" layer");
		}
		break;
		case KTProjectRequest::Scene:
		{
			setText(actionString( m_event->action() )+" scene");
		}
		break;
		case KTProjectRequest::Item:
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
		case KTProjectRequest::Add:
		{
			return QObject::tr("add");
		}
		break;
		case KTProjectRequest::Remove:
		{
			return QObject::tr("remove");
		}
		break;
		case KTProjectRequest::Move:
		{
			return QObject::tr("move");
		}
		break;
		case KTProjectRequest::Lock:
		{
			return QObject::tr("lock");
		}
		break;
		case KTProjectRequest::Rename:
		{
			return QObject::tr("rename");
		}
		break;
		case KTProjectRequest::Select:
		{
			return QObject::tr("select");
		}
		break;
		case KTProjectRequest::EditNodes:
		{
			return QObject::tr("edit node");
		}
		break;
		case KTProjectRequest::View:
		{
			return QObject::tr("view");
		}
		break;
		case KTProjectRequest::Transform:
		{
			return QObject::tr("transform");
		}
		break;
		case KTProjectRequest::Convert:
		{
			return QObject::tr("convert");
		}
		break;
	}
	
	return QString("Unknown");
}


KTProjectCommand::~KTProjectCommand()
{
	if ( m_event ) delete m_event;
}


void KTProjectCommand::redo()
{
	D_FUNCINFO << m_data.toString();
	
	switch(m_event->id() )
	{
		case KTProjectRequest::Project:
		{
			dDebug() << "Project event isn't handle";
		}
		break;
		case KTProjectRequest::Frame:
		{
			frameCommand( static_cast<const KTFrameRequest *>(m_event), true);
		}
		break;
		case KTProjectRequest::Layer:
		{
			layerCommand( static_cast<const KTLayerRequest *>(m_event), true);
		}
		break;
		case KTProjectRequest::Scene:
		{
			sceneCommand( static_cast<const KTSceneRequest *>(m_event), true);
		}
		break;
		case KTProjectRequest::Item:
		{
			itemCommand( static_cast<const KTItemRequest *>(m_event), true);
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
	D_FUNCINFO << m_data.toString();
	switch(m_event->id() )
	{
		case KTProjectRequest::Project:
		{
			dDebug() << "Project event isn't handle";
		}
		break;
		case KTProjectRequest::Frame:
		{
			frameCommand( static_cast<const KTFrameRequest *>(m_event), false);
		}
		break;
		case KTProjectRequest::Layer:
		{
			layerCommand( static_cast<const KTLayerRequest *>(m_event), false);
		}
		break;
		case KTProjectRequest::Scene:
		{
			sceneCommand( static_cast<const KTSceneRequest *>(m_event), false);
		}
		break;
		case KTProjectRequest::Item:
		{
			itemCommand( static_cast<const KTItemRequest *>(m_event), false);
		}
		break;
		default:
		{
			D_FUNCINFO << ("Unknown project event!");
		}
		break;
	}
}

void KTProjectCommand::frameCommand(const KTFrameRequest *event, bool redo)
{
	if ( redo )
	{
		switch(event->action())
		{
			case KTProjectRequest::Add:
			{
				m_project->createFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), m_data.toString() );
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_data = m_project->removeFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex() );
			}
			break;
			case KTProjectRequest::Move:
			{
				m_project->moveFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->data().toInt() );
			}
			break;
			case KTProjectRequest::Lock:
			{
				m_project->lockFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->data().toBool() );
			}
			break;
			case KTProjectRequest::Rename:
			{
				m_event->setPartName(m_project->renameFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->data().toString() ));
			}
			break;
			case KTProjectRequest::Select:
			{
				m_project->selectFrame(event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->data().toBool() );
			}
			break;
			case KTProjectRequest::View:
			{
				m_project->setFrameVisibility(event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->data().toBool());
			}
			break;
			default: break;
		}
	}
	else
	{
		switch(event->action())
		{
			case KTProjectRequest::Add:
			{
				m_data = m_project->removeFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex());
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_project->createFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), m_data.toString());
			}
			break;
			case KTProjectRequest::Move:
			{
				m_project->moveFrame(event->sceneIndex(), event->layerIndex(), event->data().toInt(), event->frameIndex() );
			}
			break;
			case KTProjectRequest::Lock:
			{
				m_project->lockFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), !event->data().toBool() );
			}
			break;
			case KTProjectRequest::Rename:
			{
				m_project->renameFrame( event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->partName() );
			}
			break;
			case KTProjectRequest::View:
			{
				m_project->setFrameVisibility(event->sceneIndex(), event->layerIndex(), event->frameIndex(), !event->data().toBool());
			}
			break;
			default: break;
		}
	}
}

void KTProjectCommand::layerCommand(const KTLayerRequest *event, bool redo)
{
	if ( redo )
	{
		switch(event->action())
		{
			case KTProjectRequest::Add:
			{
				m_project->createLayer( event->sceneIndex(), event->layerIndex(), m_data.toString());
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_data = m_project->removeLayer( event->sceneIndex(), event->layerIndex());
			}
			break;
			case KTProjectRequest::Move:
			{
				m_project->moveLayer( event->sceneIndex(), event->layerIndex(), event->data().toInt() );
			}
			break;
			case KTProjectRequest::Lock:
			{
				m_project->lockLayer( event->sceneIndex(), event->layerIndex(), event->data().toBool() );
			}
			break;
			case KTProjectRequest::Rename:
			{
				m_event->setPartName(m_project->renameLayer( event->sceneIndex(), event->layerIndex(), event->data().toString()));
			}
			break;
			case KTProjectRequest::Select:
			{
				m_project->selectLayer(event->sceneIndex(), event->layerIndex(), event->data().toBool() );
			}
			break;
			case KTProjectRequest::View:
			{
				m_project->setLayerVisibility(event->sceneIndex(), event->layerIndex(), event->data().toBool());
			}
			break;
			default: break;
		}
	}
	else
	{
		switch(event->action())
		{
			case KTProjectRequest::Add:
			{
				m_data = m_project->removeLayer( event->sceneIndex(), event->layerIndex());
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_project->createLayer( event->sceneIndex(), event->layerIndex(), m_data.toString());
			}
			break;
			case KTProjectRequest::Move:
			{
				m_project->moveLayer( event->sceneIndex(), event->data().toInt(),event->layerIndex()  );
			}
			break;
			case KTProjectRequest::Lock:
			{
				m_project->lockLayer( event->sceneIndex(), event->layerIndex(), !event->data().toBool() );
			}
			break;
			case KTProjectRequest::Rename:
			{
				m_project->renameLayer( event->sceneIndex(), event->layerIndex(), event->partName() );
			}
			break;
			case KTProjectRequest::View:
			{
				m_project->setLayerVisibility(event->sceneIndex(), event->layerIndex(), !event->data().toBool());
			}
			break;
			default: break;
		}
	}
}

void KTProjectCommand::sceneCommand(const KTSceneRequest *event, bool redo)
{
	if ( redo )
	{
		switch(event->action())
		{
			case KTProjectRequest::Add:
			{
				m_project->createScene( event->sceneIndex(), m_data.toString() );
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_data = m_project->removeScene( event->sceneIndex() );
			}
			break;
			case KTProjectRequest::Move:
			{
				m_project->moveScene( event->sceneIndex(), event->data().toInt() );
			}
			break;
			case KTProjectRequest::Lock:
			{
				m_project->lockScene( event->sceneIndex(), event->data().toBool() );
			}
			break;
			case KTProjectRequest::Rename:
			{
				m_event->setPartName(m_project->renameScene( event->sceneIndex(), event->data().toString()));
			}
			break;
			case KTProjectRequest::Select:
			{
				m_project->selectScene(event->sceneIndex(), event->data().toBool() );
			}
			break;
			case KTProjectRequest::View:
			{
				m_project->setSceneVisibility(event->sceneIndex(),  event->data().toBool());
			}
			break;
			default: break;
		}
	}
	else
	{
		switch(event->action())
		{
			case KTProjectRequest::Add:
			{
				m_data = m_project->removeScene( event->sceneIndex() );
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_project->createScene( event->sceneIndex(), m_data.toString() );
			}
			break;
			case KTProjectRequest::Move:
			{
				m_project->moveScene( event->data().toInt(), event->sceneIndex() );
			}
			break;
			case KTProjectRequest::Lock:
			{
				m_project->lockScene( event->sceneIndex(), !event->data().toBool() );
			}
			break;
			case KTProjectRequest::Rename:
			{
				m_project->renameScene( event->sceneIndex(), event->partName() );
			}
			break;
			case KTProjectRequest::View:
			{
				m_project->setSceneVisibility(event->sceneIndex(),  !event->data().toBool());
			}
			break;
			default: break;
		}
	}
}


void KTProjectCommand::itemCommand(const KTItemRequest *event, bool redo)
{
	D_FUNCINFO;
	if ( redo )
	{
		switch(event->action())
		{
			case KTProjectRequest::Add:
			{
				m_project->createItem(event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->itemIndex(), event->data().toString());
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_data = m_project->removeItems( event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->itemIndex(), event->data().toString());
			}
			break;
			case KTProjectRequest::Move:
			{
			}
			break;
			case KTProjectRequest::Lock:
			{
			}
			break;
			case KTProjectRequest::Rename:
			{
				
			}
			break;
			case KTProjectRequest::Convert:
			{
				m_data = m_project->convertItem(event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->itemIndex(), event->data().toString());
			}
			break;
			case KTProjectRequest::EditNodes:
			{
				m_data =  m_project->setPathItem( event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->itemIndex(), event->data().toString() );
			}
			break;
			case KTProjectRequest::Select:
			{
			}
			break;
			case KTProjectRequest::View:
			{
			}
			break;
			case KTProjectRequest::Transform:
			{
				m_data = m_project->transformItem(event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->itemIndex(), event->data().toString());
			}
			break;
			case KTProjectRequest::Group:
			{
				m_data = m_project->groupItems(event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->itemIndex(), event->data().toString());
			}
			break;
			default: break;
		}
	}
	else
	{
		switch(event->action())
		{
			case KTProjectRequest::Add:
			{
				m_project->removeItem(event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->itemIndex());
			}
			break;
			case KTProjectRequest::Remove:
			{
				QString::const_iterator itr = event->data().toString().constBegin();
				QList<qreal> positions = KTSvg2Qt::parseNumbersList(++itr);
				qSort(positions.begin(), positions.end());
				int count = 0;
				
				foreach(QString xml, m_data.toStringList())
				{
					m_project->createItem(event->sceneIndex(), event->layerIndex(), event->frameIndex(), positions[count], xml);
					count++;
				}
			}
			break;
			case KTProjectRequest::Move:
			{
			}
			break;
			case KTProjectRequest::Lock:
			{
			}
			break;
			case KTProjectRequest::Rename:
			{
			}
			break;
			case KTProjectRequest::Convert:
			{
				m_project->convertItem(event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->itemIndex(), m_data.toString());
			}
			break;
			case KTProjectRequest::EditNodes:
			{
				m_data =  m_project->setPathItem( event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->itemIndex(), m_data.toString() );
			}
			case KTProjectRequest::View:
			{
			}
			break;
			case KTProjectRequest::Transform:
			{
				m_project->transformItem(event->sceneIndex(), event->layerIndex(), event->frameIndex(), event->itemIndex(), m_data.toString());
			}
			break;
			default: break;
		}
	}
}

void KTProjectCommand::paintAreaCommand(const KTPaintAreaEvent *event, bool redo)
{
	Q_UNUSED(event);
	if ( redo )
	{
		m_project->reemitEvent( m_event );
	}
	else
	{
		m_project->reemitEvent( m_event );
	}
}


