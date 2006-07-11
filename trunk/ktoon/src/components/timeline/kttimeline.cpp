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

#include "kttimeline.h"
#include "dapplication.h"
#include "ddebug.h"

#include "ktlayerevent.h"
#include "ktframeevent.h"

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
	
	m_container = new QStackedWidget(this);
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
	
	m_splitter->insertWidget(position, m_layerManager);

	KTFramesTable *m_framesTable = new KTFramesTable(m_splitter);
	m_splitter->addWidget(m_framesTable);
	
	m_framesTable->setItemSize( 10, 20);
	m_layerManager->setRowHeight( 20 );
	
	connect(m_layerManager->verticalScrollBar(), SIGNAL(valueChanged (int)), m_framesTable->verticalScrollBar(), SLOT(setValue(int)));
	connect(m_framesTable->verticalScrollBar(), SIGNAL(valueChanged (int)), m_layerManager->verticalScrollBar(), SLOT(setValue(int)));
	
	m_container->addWidget(m_splitter);
}

void KTTimeLine::removeScene(int position)
{
	if ( position >= 0 && position < m_container->count() )
	{
		QWidget *w = m_container->widget(position);
		m_container->removeWidget(w);
		
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


void KTTimeLine::sceneEvent(KTSceneEvent *e)
{
	switch(e->action())
	{
		case KTProjectEvent::Add:
		{
			insertScene( e->sceneIndex(), e->partName());
		}
		break;
		case KTProjectEvent::Remove:
		{
			removeScene(e->sceneIndex());
		}
		break;
	}
}


void KTTimeLine::layerEvent(KTLayerEvent *e)
{
	switch(e->action())
	{
		case KTProjectEvent::Add:
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
		case KTProjectEvent::Remove:
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
	}
}


void KTTimeLine::frameEvent(KTFrameEvent *e)
{
	switch(e->action())
	{
		case KTProjectEvent::Add:
		{
			KTFramesTable *framesTable = this->framesTable( e->sceneIndex() );
			if ( framesTable )
			{
				framesTable->insertFrame( e->layerIndex(), e->partName() );
			}
		}
		break;
		case KTProjectEvent::Remove:
		{
			KTFramesTable *framesTable = this->framesTable( e->sceneIndex() );
			
			if ( framesTable )
			{
				framesTable->removeFrame( e->layerIndex(), e->frameIndex() );
			}
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
		layerPos = layerManager( scenePos )->currentRow();
		
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
	
	KTProjectEvent *event = 0;
	
	switch(action)
	{
		case KTProjectActionBar::InsertFrame:
		{
			event = new KTFrameEvent(KTProjectEvent::Add, scenePos, layerPos, framePos, this);
			
			emit eventTriggered( event );
			
		}
		break;
		case KTProjectActionBar::RemoveFrame:
		{
			event = new KTFrameEvent(KTProjectEvent::Remove, scenePos, layerPos, framePos, this );
			
			emit eventTriggered( event );
		}
		break;
		case KTProjectActionBar::MoveFrameUp:
		{
		}
		break;
		case KTProjectActionBar::MoveFrameDown:
		{
		}
		break;
		
		
		case KTProjectActionBar::InsertLayer:
		{
			event = new KTLayerEvent(KTProjectEvent::Add, scenePos, layerPos+1, this);
			
			emit eventTriggered( event );
		}
		break;
		case KTProjectActionBar::RemoveLayer:
		{
			event = new KTLayerEvent(KTProjectEvent::Remove, scenePos, layerPos, this);
			
			emit eventTriggered( event );
		}
		break;
		case KTProjectActionBar::MoveLayerUp:
		{
		}
		break;
		case KTProjectActionBar::MoveLayerDown:
		{
		}
		break;
		
		
		case KTProjectActionBar::InsertScene:
		{
			event = new KTSceneEvent(KTProjectEvent::Add, scenePos+1, this);
			
			emit eventTriggered( event );
		}
		break;
		case KTProjectActionBar::RemoveScene:
		{
			event = new KTSceneEvent(KTProjectEvent::Remove, scenePos, this);
			
			emit eventTriggered( event );
		}
		break;
		case KTProjectActionBar::MoveSceneUp:
		{
			
		}
		break;
		case KTProjectActionBar::MoveSceneDown:
		{
			
		}
		break;
		
	}
	
	delete event;
}
