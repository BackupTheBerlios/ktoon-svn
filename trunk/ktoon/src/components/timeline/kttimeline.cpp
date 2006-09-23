/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#include <QList>
#include <QHeaderView>

#include "kttimeline.h"
#include "dapplication.h"
#include "ddebug.h"

#include "ktlayerrequest.h"
#include "ktframerequest.h"

#include "ktframestable.h"
#include "ktlayermanager.h"

#include "ktprojectactionbar.h"


KTTimeLine::KTTimeLine(QWidget *parent) : KTModuleWidgetBase(parent, "KTTimeLine"), m_actionBar(0)
{
	DINIT;

	setCaption(tr("The time line"));
	
	m_actionBar = new KTProjectActionBar(KTProjectActionBar::AllActions );
	
	m_actionBar->insertSeparator( 4 );
	m_actionBar->insertSeparator( 9 );
	
	addChild( m_actionBar, Qt::AlignCenter );
	
	m_container = new DTabWidget(this);
	addChild(m_container);
	
	connect(m_actionBar, SIGNAL(actionSelected( int )), this, SLOT(requestCommand(int)));
	
}

KTLayerManager *KTTimeLine::layerManager(int sceneIndex)
{
	QSplitter *splitter = qobject_cast<QSplitter *>(m_container->widget(sceneIndex));
	if ( splitter )
	{
		return qobject_cast<KTLayerManager *>(splitter->widget(0));
	}
	
	return 0;
}

KTFramesTable *KTTimeLine::framesTable(int sceneIndex)
{
	QSplitter *splitter = qobject_cast<QSplitter *>(m_container->widget(sceneIndex));
	
	if ( splitter )
	{
		return qobject_cast<KTFramesTable *>(splitter->widget(1));
	}
	
	return 0;
}

void KTTimeLine::insertScene(int position, const QString &name)
{
	if ( position < 0 || position > m_container->count() )
	{
		return;
	}
	
	QSplitter *m_splitter = new QSplitter( m_container );
	
	KTLayerManager *m_layerManager = new KTLayerManager( m_splitter );
	
	m_layerManager->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	
	m_splitter->addWidget(m_layerManager);

	KTFramesTable *m_framesTable = new KTFramesTable(m_splitter);
	m_splitter->addWidget(m_framesTable);
	
	m_framesTable->setItemSize( 10, 20);
	m_layerManager->setRowHeight( 20 );
	
	connect(m_layerManager->verticalScrollBar(), SIGNAL(valueChanged (int)), m_framesTable->verticalScrollBar(), SLOT(setValue(int)));
	connect(m_framesTable->verticalScrollBar(), SIGNAL(valueChanged (int)), m_layerManager->verticalScrollBar(), SLOT(setValue(int)));
	
	connect(m_layerManager, SIGNAL(requestRenameEvent( int, const QString& )), this, SLOT(emitRequestRenameLayer(int, const QString &)));
	
	m_container->insertTab(position, m_splitter, name );
}

void KTTimeLine::removeScene(int position)
{
	if ( position >= 0 && position < m_container->count() )
	{
		QWidget *w = m_container->widget(position);
// 		m_container->removeWidget(w);
		m_container->removeTab(position);
		
		delete w;
	}
}


KTTimeLine::~KTTimeLine()
{
	DEND;
}

void KTTimeLine::closeAllScenes()
{
	while(m_container->currentWidget())
	{
		delete m_container->currentWidget();
	}
}


void KTTimeLine::sceneRequest(KTSceneRequest *e)
{
	switch(e->action())
	{
		case KTProjectRequest::Add:
		{
			insertScene( e->sceneIndex(), e->partName());
		}
		break;
		case KTProjectRequest::Remove:
		{
			removeScene(e->sceneIndex());
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
		
	}
}


void KTTimeLine::layerRequest(KTLayerRequest *e)
{
	switch(e->action())
	{
		case KTProjectRequest::Add:
		{
			KTLayerManager *layerManager = this->layerManager( e->sceneIndex() );
			if ( layerManager )
			{
				layerManager->insertLayer( e->layerIndex(), e->partName() );
			}
			
			KTFramesTable *framesTable = this->framesTable( e->sceneIndex() );
			if ( framesTable )
			{
				framesTable->insertLayer(e->layerIndex(), e->partName() );
			}
		}
		break;
		case KTProjectRequest::Remove:
		{
			KTLayerManager *layerManager = this->layerManager( e->sceneIndex() );
			
			if ( layerManager )
			{
				layerManager->removeLayer( e->layerIndex());
			}
			
			KTFramesTable *framesTable = this->framesTable( e->sceneIndex() );
			if ( framesTable )
			{
				framesTable->removeLayer(e->layerIndex() );
			}
		}
		break;
		
		case KTProjectRequest::Move:
		{
			KTLayerManager *layerManager = this->layerManager( e->sceneIndex() );
			
			if ( layerManager )
			{
				layerManager->moveLayer( e->layerIndex(), e->data().toInt() );
			}
			
			KTFramesTable *framesTable = this->framesTable( e->sceneIndex() );
			if ( framesTable )
			{
				framesTable->moveLayer(e->layerIndex(), e->data().toInt() );
			}
		}
		break;
		case KTProjectRequest::Lock:
		{
			KTLayerManager *layerManager = this->layerManager( e->sceneIndex() );
			
			if ( layerManager )
			{
				layerManager->lockLayer( e->layerIndex(), e->data().toBool() );
			}
		}
		break;
		case KTProjectRequest::Rename:
		{
			KTLayerManager *layerManager = this->layerManager( e->sceneIndex() );
			
			if ( layerManager )
			{
				layerManager->renameLayer( e->layerIndex(), e->data().toString() );
			}
		}
		break;
	}
}


void KTTimeLine::frameRequest(KTFrameRequest *e)
{
	switch(e->action())
	{
		case KTProjectRequest::Add:
		{
			KTFramesTable *framesTable = this->framesTable( e->sceneIndex() );
			if ( framesTable )
			{
				framesTable->insertFrame( e->layerIndex(), e->partName() );
			}
		}
		break;
		case KTProjectRequest::Remove:
		{
			KTFramesTable *framesTable = this->framesTable( e->sceneIndex() );
			
			if ( framesTable )
			{
				framesTable->removeFrame( e->layerIndex(), e->frameIndex() );
			}
		}
		break;
		case KTProjectRequest::Move:
		{
		}
		break;
		case KTProjectRequest::Lock:
		{
			KTFramesTable *framesTable = this->framesTable( e->sceneIndex() );
			if ( framesTable )
			{
				framesTable->lockFrame(e->layerIndex(), e->frameIndex(), e->data().toBool() );
			}
		}
		break;
		case KTProjectRequest::Rename:
		{
		}
		break;
	}
}

void KTTimeLine::requestCommand(int action)
{
	int scenePos = m_container->currentIndex();
	int layerPos = -1;
	int framePos = -1;
	
	if ( scenePos >= 0 )
	{
		layerPos = layerManager(scenePos)->verticalHeader()->visualIndex(layerManager( scenePos )->currentRow());
		
// 		if ( layerPos < 0 )
// 		{
// 			layerPos = 0;
// 		}
		
		framePos = framesTable( scenePos )->currentColumn();
		
// 		if ( framePos < 0 )
// 		{
// 			framePos = 0;
// 		}
	}
	
// 	if ( scenePos < 0 )
// 	{
// 		scenePos = 0;
// 	}
	
	switch(action)
	{
		case KTProjectActionBar::InsertFrame:
		{
			KTFrameRequest event(KTProjectRequest::Add, scenePos, layerPos, framePos);
			
			emit requestTriggered( &event );
			
		}
		break;
		case KTProjectActionBar::RemoveFrame:
		{
			KTFrameRequest event(KTProjectRequest::Remove, scenePos, layerPos, framePos );
			
			emit requestTriggered( &event );
		}
		break;
		case KTProjectActionBar::MoveFrameUp:
		{
			KTFrameRequest event(KTProjectRequest::Move, scenePos, layerPos, framePos, framePos+1);
			emit requestTriggered(&event);
		}
		break;
		case KTProjectActionBar::MoveFrameDown:
		{
			KTFrameRequest event(KTProjectRequest::Move,scenePos, layerPos, framePos, framePos-1);
			emit requestTriggered(&event);
		}
		break;
		case KTProjectActionBar::InsertLayer:
		{
			KTLayerRequest event(KTProjectRequest::Add, scenePos, layerPos+1);
			emit requestTriggered( &event );
		}
		break;
		case KTProjectActionBar::RemoveLayer:
		{
			KTLayerRequest event(KTProjectRequest::Remove, scenePos, layerPos, this);
			
			emit requestTriggered( &event );
		}
		break;
		case KTProjectActionBar::MoveLayerUp:
		{
			KTLayerRequest event(KTProjectRequest::Move,scenePos, layerPos, layerPos-1);
			emit requestTriggered(&event);
		}
		break;
		case KTProjectActionBar::MoveLayerDown:
		{
			KTLayerRequest event(KTProjectRequest::Move,scenePos, layerPos, layerPos+1);
			emit requestTriggered(&event);
		}
		break;
		case KTProjectActionBar::InsertScene:
		{
			KTSceneRequest event(KTProjectRequest::Add, scenePos+1);
			
			emit requestTriggered( &event );
		}
		break;
		case KTProjectActionBar::RemoveScene:
		{
			KTSceneRequest event(KTProjectRequest::Remove, scenePos);
			
			emit requestTriggered( &event );
		}
		break;
		case KTProjectActionBar::MoveSceneUp:
		{
			KTSceneRequest event(KTProjectRequest::Move,scenePos, scenePos+1);
			emit requestTriggered(&event);
		}
		break;
		case KTProjectActionBar::MoveSceneDown:
		{
			KTSceneRequest event(KTProjectRequest::Move,scenePos, scenePos-1);
			emit requestTriggered(&event);
		}
		break;
	}
}


void KTTimeLine::emitRequestRenameLayer(int layer, const QString &name)
{
	D_FUNCINFO << name;
	int scenePos = m_container->currentIndex();
	
	KTLayerRequest event(KTProjectRequest::Rename, scenePos, layer, name);
	
	emit requestTriggered( &event );
}


