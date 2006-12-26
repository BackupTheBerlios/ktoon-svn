/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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

#include "ktexposuresheet.h"
#include "dapplication.h"

#include "ddebug.h"
#include "doptionaldialog.h"

#include <QToolTip>
#include <QPixmap>
#include <QHBoxLayout>
#include <QList>

#include "ktprojectrequest.h"
#include "ktrequestbuilder.h"

KTExposureSheet::KTExposureSheet( QWidget *parent) : KTModuleWidgetBase(parent, "Exposure Sheet"), m_currentTable(0)
{
	DINIT;
	setWindowTitle( tr( "Exposure Sheet" ) );
	setWindowIcon(QPixmap(THEME_DIR+"/icons/exposure_sheet.png"));
	
	m_actionBar = new KTProjectActionBar( KTProjectActionBar::InsertLayer |
			KTProjectActionBar::RemoveLayer |
			KTProjectActionBar::InsertFrame |
			KTProjectActionBar::RemoveFrame |
			KTProjectActionBar::MoveFrameUp |
			KTProjectActionBar::MoveFrameDown| 
			KTProjectActionBar::LockFrame);
	
	connect(m_actionBar, SIGNAL(actionSelected( int )), this, SLOT(applyAction( int)));
	addChild( m_actionBar, Qt::AlignCenter );
	
	m_scenes = new DTabWidget(this);
	connect( m_scenes , SIGNAL(currentChanged ( int )), this, SLOT(emitRequestChangeScene( int ) ));
	addChild(m_scenes);
}

KTExposureSheet::~KTExposureSheet()
{
	DEND;
}

void KTExposureSheet::addScene(int index, const QString &name)
{
	D_FUNCINFO << " index: " << index << " name: " << name;
	KTExposureTable *newScene = new KTExposureTable;
	
	m_scenes->insertTab(index, newScene, name);
	
	connect(newScene, SIGNAL(requestSetUsedFrame(int, int)), this, SLOT(insertItem( int, int )));
	connect(newScene, SIGNAL(requestRenameFrame(int, int,const QString & )), this, SLOT(renameFrame( int, int, const QString &  )));
	connect(newScene, SIGNAL(requestSelectFrame(int, int)), SLOT(selectFrame( int, int )));
	
	connect(newScene, SIGNAL(requestRenameLayer(int, const QString & )), this, SLOT(renameLayer( int, const QString &  )));
	connect(newScene, SIGNAL(requestMoveLayer(int, int )), this, SLOT(moveLayer( int, int  )));
	
	connect(newScene, SIGNAL(requestChangeVisiblityLayer(int , bool )),  this, SLOT(changeVisiblityLayer( int, bool  )));
	
	
	m_currentTable = newScene;
	m_scenes->setCurrentWidget(m_currentTable);
}

void KTExposureSheet::renameScene( int index, const QString &name)
{
	m_scenes->setTabText(index, name);
}


void KTExposureSheet::applyAction(int action)
{
	D_FUNCINFO<< "action: " << action;
	if (  m_currentTable == 0 )
	{
		dFatal() << "KTExposureSheet::applyAction: No layer view!!" << endl;
		return;
	}
	m_currentTable = static_cast<KTExposureTable*>( m_scenes->currentWidget());	
	switch(action)
	{
		case KTProjectActionBar::InsertLayer:
		{
			int layer = m_currentTable->currentLayer()+1;
			
			KTProjectRequest event = KTRequestBuilder::createLayerRequest( m_scenes->currentIndex(), layer, KTProjectRequest::Add);
			
			emit requestTriggered( &event );
			
			break;
		}
		case KTProjectActionBar::RemoveLayer:
		{
			KTProjectRequest event = KTRequestBuilder::createLayerRequest(m_scenes->currentIndex(), m_currentTable->currentLayer(), KTProjectRequest::Remove);
			
			emit requestTriggered( &event );
			break;
		}
		case KTProjectActionBar::InsertFrame:
		{
			int used = m_currentTable->numUsed();
			int finish = m_currentTable->currentFrame()+1;
			
			
			for(int i = used; i <= finish; i++)
			{
				insertItem( m_currentTable->currentLayer(), i);
			}
			break;
		}
		case KTProjectActionBar::RemoveFrame:
		{
			KTProjectRequest event = KTRequestBuilder::createFrameRequest( m_scenes->currentIndex(), m_currentTable->currentLayer(), m_currentTable->currentFrame(), KTProjectRequest::Remove);
			emit requestTriggered( &event );
			break;
		}

		case KTProjectActionBar::MoveFrameUp:
		{
			KTProjectRequest event = KTRequestBuilder::createFrameRequest( m_scenes->currentIndex(), m_currentTable->currentLayer(), m_currentTable->currentFrame(), KTProjectRequest::Move, m_currentTable->currentFrame()-1);
			emit requestTriggered( &event );
			break;
		}
		case KTProjectActionBar::MoveFrameDown:
		{
			KTProjectRequest event = KTRequestBuilder::createFrameRequest( m_scenes->currentIndex(), m_currentTable->currentLayer(), m_currentTable->currentFrame(), KTProjectRequest::Move, m_currentTable->currentFrame()+1 );
			emit requestTriggered( &event );
			break;
		}
		case KTProjectActionBar::LockFrame:
		{
			bool locked = m_currentTable->frameIsLocked(m_currentTable->currentColumn(),  m_currentTable->currentFrame());
			
			KTProjectRequest event = KTRequestBuilder::createFrameRequest (m_scenes->currentIndex(), m_currentTable->currentLayer(), m_currentTable->currentFrame(), KTProjectRequest::Lock, locked);
			emit requestTriggered( &event );
			break;
		}
	}
}


void KTExposureSheet::setScene(int index)
{
	D_FUNCINFO;
	if(index != m_scenes->indexOf(m_currentTable) && m_scenes->count() >= index)
	{
		m_scenes->blockSignals(true);
		m_scenes->setCurrentIndex(index);
		m_scenes->blockSignals(false);
	}
}

void KTExposureSheet::emitRequestChangeScene(int index)
{
	KTProjectRequest request = KTRequestBuilder::createSceneRequest(index, KTProjectRequest::Select);
	emit requestTriggered( &request );
}

void KTExposureSheet::insertItem(int indexLayer, int indexFrame)
{
	KTProjectRequest request = KTRequestBuilder::createFrameRequest( m_scenes->currentIndex() , indexLayer, indexFrame, KTProjectRequest::Add );
	emit requestTriggered( &request );
}


void KTExposureSheet::renameFrame(int indexLayer, int indexFrame, const QString & name)
{
	KTProjectRequest event = KTRequestBuilder::createFrameRequest( m_scenes->currentIndex() , indexLayer, indexFrame, KTProjectRequest::Rename, name);
	emit requestTriggered( &event );
}

void KTExposureSheet::selectFrame(int indexLayer, int indexFrame)
{
	KTProjectRequest request = KTRequestBuilder::createFrameRequest( m_scenes->currentIndex() , indexLayer, indexFrame,KTProjectRequest::Select, "1" );
	
	emit requestTriggered( &request );
}

void KTExposureSheet::changeVisiblityLayer(int visualIndexLayer, bool visibility)
{
	KTProjectRequest event = KTRequestBuilder::createLayerRequest(KTProjectRequest::View, m_scenes->currentIndex() , visualIndexLayer,  visibility );
	emit requestTriggered( &event );
}

void KTExposureSheet::renameLayer(int indexLayer, const QString & name)
{
	KTProjectRequest event = KTRequestBuilder::createLayerRequest(m_scenes->currentIndex(), indexLayer, KTProjectRequest::Rename, name);
	
	emit requestTriggered( &event );
}

void KTExposureSheet::moveLayer(int oldIndex, int newIndex)
{
	KTProjectRequest event = KTRequestBuilder::createLayerRequest(m_scenes->currentIndex(), oldIndex, KTProjectRequest::Move, newIndex);
	emit requestTriggered( &event );
}

void KTExposureSheet::closeAllScenes()
{
	D_FUNCINFO;
	
	delete m_currentTable;
	
	m_scenes->removeAllTabs();
	
	m_currentTable = 0;
}


void KTExposureSheet::sceneResponse(KTSceneResponse *e)
{
	switch(e->action() )
	{
		case KTProjectRequest::Add:
		{
			addScene(e->sceneIndex(), e->arg().toString());
		}
		break;
		case KTProjectRequest::Remove:
		{
			QWidget * widget = m_scenes->widget( e->sceneIndex() );
			m_scenes->removeTab(e->sceneIndex());
			delete widget;
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
			renameScene (e->sceneIndex(), e->arg().toString());
		}
		break;
		case KTProjectRequest::Select:
		{
			setScene( e->sceneIndex() );
		}
		break;
	}
}

void KTExposureSheet::layerResponse(KTLayerResponse *e)
{
	KTExposureTable *scene = dynamic_cast<KTExposureTable*>(m_scenes->widget((e->sceneIndex())));
	if(scene)
	{
		switch(e->action() )
		{
			case KTProjectRequest::Add:
			{
				dDebug() << "INSERT LAYER: " << e->layerIndex();
				scene->insertLayer(e->layerIndex(),  e->arg().toString());
			}
			break;
			case KTProjectRequest::Remove:
			{
				scene->removeLayer( e->layerIndex());
			}
			break;
			case KTProjectRequest::Move:
			{
				scene->moveLayer( e->layerIndex(), e->arg().toInt());
			}
			break;
			case KTProjectRequest::Rename:
			{
				scene->setLayerName( e->layerIndex(), e->arg().toString());
			}
			break;
			case KTProjectRequest::Lock:
			{
				scene->setLockLayer( e->layerIndex(), e->arg().toBool());
			}
			break;
			case KTProjectRequest::Select:
			{
				setScene(e->sceneIndex());
				scene->blockSignals(true );
				scene->selectFrame( e->layerIndex(), 0);
				scene->blockSignals(false );
			}
			case KTProjectRequest::View:
			{
				scene->setVisibilityChanged( e->layerIndex(), e->arg().toBool());
			}
			break;
		}
	}
}

void KTExposureSheet::frameResponse(KTFrameResponse *e)
{
	D_FUNCINFO;
	KTExposureTable *scene = dynamic_cast<KTExposureTable*>(m_scenes->widget((e->sceneIndex())));
	if(scene)
	{
		switch(e->action() )
		{
			case KTProjectRequest::Add:
			{
				scene->setUseFrame( e->layerIndex(), e->frameIndex(),  e->arg().toString());
			}
			break;
			case KTProjectRequest::Remove:
			{
				scene->removeFrame(  e->layerIndex(), e->frameIndex());
			}
			break;
			case KTProjectRequest::Move:
			{
				scene->moveFrame( e->layerIndex(), e->frameIndex(), e->layerIndex(), e->data().toInt());
			}
			break;
			case KTProjectRequest::Lock:
			{
				scene->setLockFrame( e->layerIndex(), e->frameIndex(),  e->arg().toBool());
			}
			break;
			case KTProjectRequest::Rename:
			{
				scene->setFrameName( e->layerIndex(), e->frameIndex(), e->arg().toString());
			}
			break;
			case KTProjectRequest::Select:
			{
				setScene(e->sceneIndex());
				scene->selectFrame( e->layerIndex(), e->frameIndex());
			}
			break;
		}
	}
}
