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

#include "ktsvg2qt.h"

KTProjectCommand::KTProjectCommand(KTCommandExecutor *executor, const KTProjectRequest *event) : QUndoCommand(), m_executor(executor), m_executed(false)
{
	m_parser = new KTRequestParser;
	m_parser->parse( event->data().toString() );
	
	switch( m_parser->part() )
	{
		case KTProjectRequest::Frame:
		{
			setText(actionString( m_parser->action() ) +" frame" );
		}
		break;
		case KTProjectRequest::Layer:
		{
			setText(actionString( m_parser->action() )+" layer");
		}
		break;
		case KTProjectRequest::Scene:
		{
			setText(actionString( m_parser->action() )+" scene");
		}
		break;
		case KTProjectRequest::Item:
		{
			setText(actionString( m_parser->action() )+" item");
		}
		break;
		
		case KTProjectRequest::Library:
		{
			setText(actionString( m_parser->action() )+" symbol");
		}
		break;
		
		default:
		{
			dfDebug << "CAN'T HANDLE ID: " << m_parser->part();
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
	delete m_parser;
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
	
	switch(m_parser->part() )
	{
		case KTProjectRequest::Project:
		{
			dDebug() << "Project m_parser isn't handle";
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
			D_FUNCINFO << ("Unknown project m_parser!");
		}
		break;
	}
	
	
}

void KTProjectCommand::undo()
{
	D_FUNCINFO << m_data;
	
	m_executor->setState( KTCommandExecutor::Undo );
	
	switch(m_parser->part() )
	{
		case KTProjectRequest::Project:
		{
			dDebug() << "Project m_parser isn't handle";
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
			D_FUNCINFO << ("Unknown project m_parser!");
		}
		break;
	}
}

void KTProjectCommand::frameCommand(bool redo)
{
	if ( redo )
	{
		switch(m_parser->action())
		{
			case KTProjectRequest::Add:
			{
				m_executor->createFrame( m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), m_parser->arg().toString() );
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_data = m_executor->removeFrame( m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex() );
			}
			break;
			case KTProjectRequest::Move:
			{
				m_executor->moveFrame( m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), m_parser->arg().toInt() );
			}
			break;
			case KTProjectRequest::Lock:
			{
				m_executor->lockFrame( m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), m_parser->arg().toBool() );
			}
			break;
			case KTProjectRequest::Rename:
			{
				m_data = m_executor->renameFrame( m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), m_parser->arg().toString() );
			}
			break;
			case KTProjectRequest::Select:
			{
				m_executor->selectFrame(m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), m_parser->arg().toBool() );
			}
			break;
			case KTProjectRequest::View:
			{
				m_executor->setFrameVisibility(m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), m_parser->arg().toBool());
			}
			break;
			default: break;
		}
	}
	else
	{
		switch(m_parser->action())
		{
			case KTProjectRequest::Add:
			{
				m_data = m_executor->removeFrame( m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex());
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_executor->createFrame( m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), m_data );
			}
			break;
			case KTProjectRequest::Move:
			{
				m_executor->moveFrame(m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->arg().toInt(), m_parser->frameIndex() );
			}
			break;
			case KTProjectRequest::Lock:
			{
				m_executor->lockFrame( m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), !m_parser->arg().toBool() );
			}
			break;
			case KTProjectRequest::Rename:
			{
				m_executor->renameFrame( m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), m_parser->arg().toString() );
			}
			break;
			case KTProjectRequest::View:
			{
				m_executor->setFrameVisibility(m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), !m_parser->arg().toBool());
			}
			break;
			default: break;
		}
	}
}

void KTProjectCommand::layerCommand(bool redo)
{
	if ( redo )
	{
		switch(m_parser->action())
		{
			case KTProjectRequest::Add:
			{
				m_executor->createLayer( m_parser->sceneIndex(), m_parser->layerIndex(), m_data);
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_data = m_executor->removeLayer( m_parser->sceneIndex(), m_parser->layerIndex());
			}
			break;
			case KTProjectRequest::Move:
			{
				m_executor->moveLayer( m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->arg().toInt() );
			}
			break;
			case KTProjectRequest::Lock:
			{
				m_executor->lockLayer( m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->arg().toBool() );
			}
			break;
			case KTProjectRequest::Rename:
			{
				m_data = m_executor->renameLayer( m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->arg().toString());
			}
			break;
			case KTProjectRequest::Select:
			{
				m_executor->selectLayer(m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->arg().toBool() );
			}
			break;
			case KTProjectRequest::View:
			{
				m_executor->setLayerVisibility(m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->arg().toBool());
			}
			break;
			default: break;
		}
	}
	else
	{
		switch(m_parser->action())
		{
			case KTProjectRequest::Add:
			{
				m_data = m_executor->removeLayer( m_parser->sceneIndex(), m_parser->layerIndex());
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_executor->createLayer( m_parser->sceneIndex(), m_parser->layerIndex(), m_data);
			}
			break;
			case KTProjectRequest::Move:
			{
				m_executor->moveLayer( m_parser->sceneIndex(), m_parser->arg().toInt(),m_parser->layerIndex()  );
			}
			break;
			case KTProjectRequest::Lock:
			{
				m_executor->lockLayer( m_parser->sceneIndex(), m_parser->layerIndex(), !m_parser->arg().toBool() );
			}
			break;
			case KTProjectRequest::Rename:
			{
				m_executor->renameLayer( m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->arg().toString() );
			}
			break;
			case KTProjectRequest::View:
			{
				m_executor->setLayerVisibility(m_parser->sceneIndex(), m_parser->layerIndex(), !m_parser->arg().toBool());
			}
			break;
			default: break;
		}
	}
}

void KTProjectCommand::sceneCommand(bool redo)
{
	if ( redo )
	{
		switch(m_parser->action())
		{
			case KTProjectRequest::Add:
			{
				m_executor->createScene( m_parser->sceneIndex(), m_data );
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_data = m_executor->removeScene( m_parser->sceneIndex() );
			}
			break;
			case KTProjectRequest::Move:
			{
				m_executor->moveScene( m_parser->sceneIndex(), m_parser->arg().toInt() );
			}
			break;
			case KTProjectRequest::Lock:
			{
				m_executor->lockScene( m_parser->sceneIndex(), m_parser->arg().toBool() );
			}
			break;
			case KTProjectRequest::Rename:
			{
				m_data = m_executor->renameScene( m_parser->sceneIndex(), m_parser->arg().toString());
			}
			break;
			case KTProjectRequest::Select:
			{
				m_executor->selectScene(m_parser->sceneIndex(), m_parser->arg().toBool() );
			}
			break;
			case KTProjectRequest::View:
			{
				m_executor->setSceneVisibility(m_parser->sceneIndex(),  m_parser->arg().toBool());
			}
			break;
			default: break;
		}
	}
	else
	{
		switch(m_parser->action())
		{
			case KTProjectRequest::Add:
			{
				m_data = m_executor->removeScene( m_parser->sceneIndex() );
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_executor->createScene( m_parser->sceneIndex(), m_data );
			}
			break;
			case KTProjectRequest::Move:
			{
				m_executor->moveScene( m_parser->arg().toInt(), m_parser->sceneIndex() );
			}
			break;
			case KTProjectRequest::Lock:
			{
				m_executor->lockScene( m_parser->sceneIndex(), !m_parser->arg().toBool() );
			}
			break;
			case KTProjectRequest::Rename:
			{
				m_executor->renameScene( m_parser->sceneIndex(), m_parser->arg().toString() );
			}
			break;
			case KTProjectRequest::View:
			{
				m_executor->setSceneVisibility(m_parser->sceneIndex(),  !m_parser->arg().toBool());
			}
			break;
			default: break;
		}
	}
}


void KTProjectCommand::itemCommand(bool redo)
{
	D_FUNCINFO;
	if ( redo )
	{
		switch(m_parser->action())
		{
			case KTProjectRequest::Add:
			{
				m_executor->createItem(m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), m_parser->itemIndex(), m_parser->arg().toString());
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_data = m_executor->removeItems( m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), m_parser->itemIndex(), m_parser->arg().toString()).join(",");
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
				m_data = m_executor->convertItem(m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), m_parser->itemIndex(), m_parser->arg().toString());
			}
			break;
			case KTProjectRequest::EditNodes:
			{
				m_data =  m_executor->setPathItem( m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), m_parser->itemIndex(), m_parser->arg().toString() );
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
				m_data = m_executor->transformItem(m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), m_parser->itemIndex(), m_parser->arg().toString());
			}
			break;
			case KTProjectRequest::Group:
			{
				m_data = m_executor->groupItems(m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), m_parser->itemIndex(), m_parser->arg().toString()).join(",");
			}
			break;
			default: break;
		}
	}
	else
	{
		switch(m_parser->action())
		{
			case KTProjectRequest::Add:
			{
				m_executor->removeItems(m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), m_parser->itemIndex(), "<delete positions=\"("+QString::number(m_parser->itemIndex())+")\"/>" );
			}
			break;
			case KTProjectRequest::Remove:
			{
				QString::const_iterator itr = m_parser->arg().toString().constBegin();
				QList<qreal> positions = KTSvg2Qt::parseNumbersList(++itr);
				qSort(positions.begin(), positions.end());
				int count = 0;
				
				foreach(QString xml, m_data.split(","))
				{
					m_executor->createItem(m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), positions[count], xml);
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
				m_executor->convertItem(m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), m_parser->itemIndex(), m_data);
			}
			break;
			case KTProjectRequest::EditNodes:
			{
				m_data =  m_executor->setPathItem( m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), m_parser->itemIndex(), m_data );
			}
			case KTProjectRequest::View:
			{
			}
			break;
			case KTProjectRequest::Transform:
			{
				m_executor->transformItem(m_parser->sceneIndex(), m_parser->layerIndex(), m_parser->frameIndex(), m_parser->itemIndex(), m_data);
			}
			break;
			default: break;
		}
	}
}

void KTProjectCommand::libraryCommand(bool redo)
{
	if ( redo )
	{
		switch(m_parser->action())
		{
			case KTProjectRequest::Add:
			{
				m_executor->createSymbol(m_parser->arg().toString());
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_executor->removeSymbol(m_parser->arg().toString());
			}
			break;
		}
	}
	else
	{
		switch(m_parser->action())
		{
			case KTProjectRequest::Add:
			{
				m_executor->removeSymbol(m_parser->arg().toString());
			}
			break;
			case KTProjectRequest::Remove:
			{
				m_executor->createSymbol(m_parser->arg().toString());
			}
			break;
		}
	}
	
}

void KTProjectCommand::paintAreaCommand(bool redo)
{
	Q_UNUSED(m_parser);
	qFatal("FIX ME in ktprojectcommand.cpp ");
// 	if ( redo )
// 	{
// 		m_executor->reemitEvent( m_parser );
// 	}
// 	else
// 	{
// 		m_executor->reemitEvent( m_parser );
// 	}
}


