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

#include "ktcommandexecutor.h"

#include "ktprojectrequest.h"
#include "ktpaintareaevent.h"


#include "ktrequestparser.h"
#include "ktprojectresponse.h"

#include "ktsvg2qt.h"

KTProjectCommand::KTProjectCommand(KTCommandExecutor *executor, const KTProjectRequest *event) : QUndoCommand(), m_executor(executor), m_executed(false)
{
	KTRequestParser parser;
	
	if ( ! parser.parse( event->data().toString() ) )
	{
		qFatal("KTProjectCommand::KTProjectCommand()");
	}
	
	m_response = parser.response();
	
	switch( m_response->part() )
	{
		case KTProjectRequest::Frame:
		{
			setText(actionString( m_response->action() ) +" frame" );
		}
		break;
		case KTProjectRequest::Layer:
		{
			setText(actionString( m_response->action() )+" layer");
		}
		break;
		case KTProjectRequest::Scene:
		{
			setText(actionString( m_response->action() )+" scene");
		}
		break;
		case KTProjectRequest::Item:
		{
			setText(actionString( m_response->action() )+" item");
		}
		break;
		
		case KTProjectRequest::Library:
		{
			setText(actionString( m_response->action() )+" symbol");
		}
		break;
		
		default:
		{
			dfDebug << "CAN'T HANDLE ID: " << m_response->part();
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
	delete m_response;
}


void KTProjectCommand::redo()
{
	D_FUNCINFO << m_data;
	
	
	if ( m_executed )
	{
		m_executor->setState( KTCommandExecutor::Redo );
	}
	else
	{
		m_executor->setState(KTCommandExecutor::Do );
		m_executed = true;
	}
	
	switch(m_response->part() )
	{
		case KTProjectRequest::Project:
		{
			dDebug() << "Project response isn't handle";
		}
		break;
		case KTProjectRequest::Frame:
		{
			frameCommand(true);
		}
		break;
		case KTProjectRequest::Layer:
		{
			layerCommand(true);
		}
		break;
		case KTProjectRequest::Scene:
		{
			sceneCommand(true);
		}
		break;
		case KTProjectRequest::Item:
		{
			itemCommand( true );
		}
		break;
		case KTProjectRequest::Library:
		{
			libraryCommand(true);
		}
		break;
		default:
		{
			D_FUNCINFO << ("Unknown project response!");
		}
		break;
	}
	
	
}

void KTProjectCommand::undo()
{
	D_FUNCINFO << m_data;
	
	m_executor->setState( KTCommandExecutor::Undo );
	
	switch(m_response->part() )
	{
		case KTProjectRequest::Project:
		{
			dDebug() << "Project response isn't handle";
		}
		break;
		case KTProjectRequest::Frame:
		{
			frameCommand(false);
		}
		break;
		case KTProjectRequest::Layer:
		{
			layerCommand( false);
		}
		break;
		case KTProjectRequest::Scene:
		{
			sceneCommand( false);
		}
		break;
		case KTProjectRequest::Item:
		{
			itemCommand( false);
		}
		break;
		
		case KTProjectRequest::Library:
		{
			libraryCommand(false);
		}
		break;
		
		default:
		{
			D_FUNCINFO << ("Unknown project response!");
		}
		break;
	}
}

void KTProjectCommand::frameCommand(bool redo)
{
	KTFrameResponse *response = static_cast<KTFrameResponse *>(m_response);
	if ( redo )
	{
		switch(response->action())
		{
			case KTProjectRequest::Add:
			{
				m_executor->createFrame( m_response );
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_data = m_executor->removeFrame( m_response);
			}
			break;
			case KTProjectRequest::Move:
			{
				m_executor->moveFrame( response->sceneIndex(), response->layerIndex(), response->frameIndex(), response->arg().toInt() );
			}
			break;
			case KTProjectRequest::Lock:
			{
				m_executor->lockFrame( response->sceneIndex(), response->layerIndex(), response->frameIndex(), response->arg().toBool() );
			}
			break;
			case KTProjectRequest::Rename:
			{
				m_data = m_executor->renameFrame( response->sceneIndex(), response->layerIndex(), response->frameIndex(), response->arg().toString() );
			}
			break;
			case KTProjectRequest::Select:
			{
				m_executor->selectFrame(response->sceneIndex(), response->layerIndex(), response->frameIndex(), response->arg().toBool() );
			}
			break;
			case KTProjectRequest::View:
			{
				m_executor->setFrameVisibility(response->sceneIndex(), response->layerIndex(), response->frameIndex(), response->arg().toBool());
			}
			break;
			default: break;
		}
	}
	else
	{
		switch(response->action())
		{
			case KTProjectRequest::Add:
			{
				m_data = m_executor->removeFrame( m_response );
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_executor->createFrame( m_response );
			}
			break;
			case KTProjectRequest::Move:
			{
				m_executor->moveFrame(response->sceneIndex(), response->layerIndex(), response->arg().toInt(), response->frameIndex() );
			}
			break;
			case KTProjectRequest::Lock:
			{
				m_executor->lockFrame( response->sceneIndex(), response->layerIndex(), response->frameIndex(), !response->arg().toBool() );
			}
			break;
			case KTProjectRequest::Rename:
			{
				m_executor->renameFrame( response->sceneIndex(), response->layerIndex(), response->frameIndex(), response->arg().toString() );
			}
			break;
			case KTProjectRequest::View:
			{
				m_executor->setFrameVisibility(response->sceneIndex(), response->layerIndex(), response->frameIndex(), !response->arg().toBool());
			}
			break;
			default: break;
		}
	}
}

void KTProjectCommand::layerCommand(bool redo)
{
	KTLayerResponse *response = static_cast<KTLayerResponse *>(m_response);
	if ( redo )
	{
		switch(response->action())
		{
			case KTProjectRequest::Add:
			{
				m_executor->createLayer( response->sceneIndex(), response->layerIndex(), m_data);
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_data = m_executor->removeLayer( response->sceneIndex(), response->layerIndex());
			}
			break;
			case KTProjectRequest::Move:
			{
				m_executor->moveLayer( response->sceneIndex(), response->layerIndex(), response->arg().toInt() );
			}
			break;
			case KTProjectRequest::Lock:
			{
				m_executor->lockLayer( response->sceneIndex(), response->layerIndex(), response->arg().toBool() );
			}
			break;
			case KTProjectRequest::Rename:
			{
				m_data = m_executor->renameLayer( response->sceneIndex(), response->layerIndex(), response->arg().toString());
			}
			break;
			case KTProjectRequest::Select:
			{
				m_executor->selectLayer(response->sceneIndex(), response->layerIndex(), response->arg().toBool() );
			}
			break;
			case KTProjectRequest::View:
			{
				m_executor->setLayerVisibility(response->sceneIndex(), response->layerIndex(), response->arg().toBool());
			}
			break;
			default: break;
		}
	}
	else
	{
		switch(response->action())
		{
			case KTProjectRequest::Add:
			{
				m_data = m_executor->removeLayer( response->sceneIndex(), response->layerIndex());
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_executor->createLayer( response->sceneIndex(), response->layerIndex(), m_data);
			}
			break;
			case KTProjectRequest::Move:
			{
				m_executor->moveLayer( response->sceneIndex(), response->arg().toInt(),response->layerIndex()  );
			}
			break;
			case KTProjectRequest::Lock:
			{
				m_executor->lockLayer( response->sceneIndex(), response->layerIndex(), !response->arg().toBool() );
			}
			break;
			case KTProjectRequest::Rename:
			{
				m_executor->renameLayer( response->sceneIndex(), response->layerIndex(), response->arg().toString() );
			}
			break;
			case KTProjectRequest::View:
			{
				m_executor->setLayerVisibility(response->sceneIndex(), response->layerIndex(), !response->arg().toBool());
			}
			break;
			default: break;
		}
	}
}

void KTProjectCommand::sceneCommand(bool redo)
{
	KTSceneResponse *response = static_cast<KTSceneResponse *>(m_response);
	if ( redo )
	{
		switch(response->action())
		{
			case KTProjectRequest::Add:
			{
				m_executor->createScene( response->sceneIndex(), m_data );
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_data = m_executor->removeScene( response->sceneIndex() );
			}
			break;
			case KTProjectRequest::Move:
			{
				m_executor->moveScene( response->sceneIndex(), response->arg().toInt() );
			}
			break;
			case KTProjectRequest::Lock:
			{
				m_executor->lockScene( response->sceneIndex(), response->arg().toBool() );
			}
			break;
			case KTProjectRequest::Rename:
			{
				m_data = m_executor->renameScene( response->sceneIndex(), response->arg().toString());
			}
			break;
			case KTProjectRequest::Select:
			{
				m_executor->selectScene(response->sceneIndex(), response->arg().toBool() );
			}
			break;
			case KTProjectRequest::View:
			{
				m_executor->setSceneVisibility(response->sceneIndex(),  response->arg().toBool());
			}
			break;
			default: break;
		}
	}
	else
	{
		switch(response->action())
		{
			case KTProjectRequest::Add:
			{
				m_data = m_executor->removeScene( response->sceneIndex() );
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_executor->createScene( response->sceneIndex(), m_data );
			}
			break;
			case KTProjectRequest::Move:
			{
				m_executor->moveScene( response->arg().toInt(), response->sceneIndex() );
			}
			break;
			case KTProjectRequest::Lock:
			{
				m_executor->lockScene( response->sceneIndex(), !response->arg().toBool() );
			}
			break;
			case KTProjectRequest::Rename:
			{
				m_executor->renameScene( response->sceneIndex(), response->arg().toString() );
			}
			break;
			case KTProjectRequest::View:
			{
				m_executor->setSceneVisibility(response->sceneIndex(),  !response->arg().toBool());
			}
			break;
			default: break;
		}
	}
}


void KTProjectCommand::itemCommand(bool redo)
{
	D_FUNCINFO;
	
	KTItemResponse *response = static_cast<KTItemResponse *>(m_response);
	if ( redo )
	{
		switch(response->action())
		{
			case KTProjectRequest::Add:
			{
				m_executor->createItem(response->sceneIndex(), response->layerIndex(), response->frameIndex(), response->itemIndex(), response->arg().toString());
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_data = m_executor->removeItems( response->sceneIndex(), response->layerIndex(), response->frameIndex(), response->itemIndex(), response->arg().toString()).join(",");
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
				m_data = m_executor->convertItem(response->sceneIndex(), response->layerIndex(), response->frameIndex(), response->itemIndex(), response->arg().toString());
			}
			break;
			case KTProjectRequest::EditNodes:
			{
				m_data =  m_executor->setPathItem( response->sceneIndex(), response->layerIndex(), response->frameIndex(), response->itemIndex(), response->arg().toString() );
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
				m_data = m_executor->transformItem(response->sceneIndex(), response->layerIndex(), response->frameIndex(), response->itemIndex(), response->arg().toString());
			}
			break;
			case KTProjectRequest::Group:
			{
				m_data = m_executor->groupItems(response->sceneIndex(), response->layerIndex(), response->frameIndex(), response->itemIndex(), response->arg().toString()).join(",");
			}
			break;
			default: break;
		}
	}
	else
	{
		switch(response->action())
		{
			case KTProjectRequest::Add:
			{
				m_executor->removeItems(response->sceneIndex(), response->layerIndex(), response->frameIndex(), response->itemIndex(), "<delete positions=\"("+QString::number(response->itemIndex())+")\"/>" );
			}
			break;
			case KTProjectRequest::Remove:
			{
				QString::const_iterator itr = response->arg().toString().constBegin();
				QList<qreal> positions = KTSvg2Qt::parseNumbersList(++itr);
				qSort(positions.begin(), positions.end());
				int count = 0;
				
				foreach(QString xml, m_data.split(","))
				{
					m_executor->createItem(response->sceneIndex(), response->layerIndex(), response->frameIndex(), positions[count], xml);
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
				m_executor->convertItem(response->sceneIndex(), response->layerIndex(), response->frameIndex(), response->itemIndex(), m_data);
			}
			break;
			case KTProjectRequest::EditNodes:
			{
				m_data =  m_executor->setPathItem( response->sceneIndex(), response->layerIndex(), response->frameIndex(), response->itemIndex(), m_data );
			}
			case KTProjectRequest::View:
			{
			}
			break;
			case KTProjectRequest::Transform:
			{
				m_executor->transformItem(response->sceneIndex(), response->layerIndex(), response->frameIndex(), response->itemIndex(), m_data);
			}
			break;
			default: break;
		}
	}
}

void KTProjectCommand::libraryCommand(bool redo)
{
	// FIXME FIXME FIXME
// 	KTLibraryResponse *response = static_cast<KTLibraryResponse *>(response);
// 	if ( redo )
// 	{
// 		switch(response->action())
// 		{
// 			case KTProjectRequest::Add:
// 			{
// 				m_executor->createSymbol(response->arg().toString());
// 			}
// 			break;
// 			case KTProjectRequest::Remove:
// 			{
// 				m_executor->removeSymbol(response->arg().toString());
// 			}
// 			break;
// 		}
// 	}
// 	else
// 	{
// 		switch(response->action())
// 		{
// 			case KTProjectRequest::Add:
// 			{
// 				m_executor->removeSymbol(response->arg().toString());
// 			}
// 			break;
// 			case KTProjectRequest::Remove:
// 			{
// 				m_executor->createSymbol(response->arg().toString());
// 			}
// 			break;
// 		}
// 	}
	
}

void KTProjectCommand::paintAreaCommand(bool redo)
{
	qFatal("FIX ME in ktprojectcommand.cpp ");
// 	if ( redo )
// 	{
// 		m_executor->reemitEvent( response );
// 	}
// 	else
// 	{
// 		m_executor->reemitEvent( response );
// 	}
}


