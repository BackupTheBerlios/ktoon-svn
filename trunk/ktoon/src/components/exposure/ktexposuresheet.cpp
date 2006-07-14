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

#include "ktsceneevent.h"
#include "ktlayerevent.h"
#include "ktframeevent.h"

KTExposureSheet::KTExposureSheet( QWidget *parent) : KTModuleWidgetBase(parent, "Exposure Sheet"), m_currentTable(0)
{
	DINIT;
	setCaption( tr( "Exposure Sheet" ) );
	
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
	D_FUNCINFO;
	KTExposureTable *newScene = new KTExposureTable;
	
	m_tables << newScene;
	
	m_scenes->insertTab (index, newScene, name);
	
	connect(newScene, SIGNAL(requestSetUsedFrame(int, int)), this, SLOT(insertItem( int, int )));
	connect(newScene, SIGNAL(requestRenameFrame(int, int,const QString & )), this, SLOT(renameItem( int, int, const QString &  )));
	connect(newScene, SIGNAL(requestRenameLayer(int, const QString & )), this, SLOT(renameLayer( int, const QString &  )));
	connect(newScene, SIGNAL(requestMoveLayer(int, int )), this, SLOT(moveLayer( int, int  )));
	
	m_currentTable = newScene;
	m_scenes->setCurrentWidget(m_currentTable);
}

void KTExposureSheet::renameScene( int index, const QString &name)
{
	m_scenes->setTabText(index, name);
}


void KTExposureSheet::applyAction(int action)
{
	D_FUNCINFO;
	if ( m_tables.count() == 0 || m_currentTable == 0 )
	{
		dFatal() << "KTExposureSheet::applyAction: No layer view!!" << endl;
		return;
	}
	m_currentTable = static_cast<KTExposureTable*>( m_scenes->currentWidget());
	switch(action)
	{
		case KTProjectActionBar::InsertLayer:
		{
			KTLayerEvent event(KTProjectEvent::Add, m_scenes->currentIndex() , m_currentTable->currentLayer()+1);
			emit eventTriggered( &event );
			break;
		}
		case KTProjectActionBar::RemoveLayer:
		{
			KTLayerEvent event(KTProjectEvent::Remove, m_scenes->currentIndex(), m_currentTable->currentLayer());
			
			emit eventTriggered( &event );
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
			KTFrameEvent event(KTProjectEvent::Remove, m_scenes->currentIndex(), m_currentTable->currentLayer(), m_currentTable->currentFrame());
			emit eventTriggered( &event );
			break;
		}

		case KTProjectActionBar::MoveFrameUp:
		{
			KTFrameEvent event(KTProjectEvent::Move, m_scenes->currentIndex(), m_currentTable->currentLayer(), m_currentTable->currentFrame(), m_currentTable->currentFrame()-1);
			emit eventTriggered( &event );
			break;
		}
		case KTProjectActionBar::MoveFrameDown:
		{
			KTFrameEvent event(KTProjectEvent::Move, m_scenes->currentIndex(), m_currentTable->currentLayer(), m_currentTable->currentFrame(), m_currentTable->currentFrame()+1);
			emit eventTriggered( &event );
			break;
		}
		case KTProjectActionBar::LockFrame:
		{
			
			bool locked = m_currentTable->frameIsLocked(m_currentTable->currentLayer(),  m_currentTable->currentFrame());
			dDebug() << locked;
			KTFrameEvent event(KTProjectEvent::Lock, m_scenes->currentIndex(), m_currentTable->currentLayer(), m_currentTable->currentFrame(), !locked);
			
			
			emit eventTriggered( &event );
			break;
		}
	}
}


void KTExposureSheet::setScene(int index)
{
	D_FUNCINFO;
	if(index != m_scenes->indexOf(m_currentTable) && m_tables.count() >= index)
	{
		m_currentTable = m_tables[index];
		m_scenes->setCurrentWidget(m_tables[index]);
	}
}

void KTExposureSheet::emitRequestChangeScene(int index)
{
	if(index != m_scenes->indexOf(m_currentTable))
	{
		//TODO
	}
}

void KTExposureSheet::insertItem(int indexLayer, int indexFrame)
{
	KTFrameEvent event(KTProjectEvent::Add, m_scenes->currentIndex() , indexLayer, indexFrame);
	emit eventTriggered( &event );
}


void KTExposureSheet::renameFrame(int indexLayer, int indexFrame, const QString & name)
{
	KTFrameEvent event(KTProjectEvent::Rename, m_scenes->currentIndex() , indexLayer, indexFrame, name);
	emit eventTriggered( &event );
}

void KTExposureSheet::renameLayer(int indexLayer, const QString & name)
{
	KTLayerEvent event(KTProjectEvent::Rename, m_scenes->currentIndex() , indexLayer, name);
	
	emit eventTriggered( &event );
}

void KTExposureSheet::moveLayer(int oldIndex, int newIndex)
{
	KTLayerEvent event(KTProjectEvent::Move, m_scenes->currentIndex() , oldIndex, newIndex);
	emit eventTriggered( &event );
}

void KTExposureSheet::closeAllScenes()
{
	D_FUNCINFO;
	
	delete m_currentTable;
	
	m_scenes->removeAllTabs();
	m_tables.clear();
	
	m_currentTable = 0;
}


void KTExposureSheet::sceneEvent(KTSceneEvent *e)
{
	switch(e->action() )
	{
		case KTProjectEvent::Add:
		{
			addScene(e->sceneIndex(), e->partName());
		}
		break;
		case KTProjectEvent::Remove:
		{
			QWidget * widget = m_scenes->widget( e->sceneIndex() );
			m_scenes->removeTab(e->sceneIndex());
			delete widget;
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
			renameScene (e->sceneIndex(), e->data().toString());
		}
		break;
	}
}

void KTExposureSheet::layerEvent(KTLayerEvent *e)
{
	switch(e->action() )
	{
		case KTProjectEvent::Add:
		{
			
			KTExposureTable *scene = dynamic_cast<KTExposureTable*>(m_scenes->widget((e->sceneIndex())));
			if(scene)
			{
				scene->insertLayer(e->layerIndex(),  e->partName());
			}
		}
		break;
		case KTProjectEvent::Remove:
		{
// 			removeLayer( e->sceneIndex());
			KTExposureTable *scene = dynamic_cast<KTExposureTable*>(m_scenes->widget((e->sceneIndex())));
			if(scene)
			{
				scene->removeLayer( e->layerIndex());
			}
		}
		break;
		case KTProjectEvent::Move:
		{
			KTExposureTable *scene = dynamic_cast<KTExposureTable*>(m_scenes->widget((e->sceneIndex())));
			if(scene)
			{
				scene->moveLayer( e->layerIndex(), e->data().toInt());
			}
		}
		break;
		case KTProjectEvent::Rename:
		{
			KTExposureTable *scene = dynamic_cast<KTExposureTable*>(m_scenes->widget((e->sceneIndex())));
			if(scene)
			{
				scene->setLayerName( e->layerIndex(), e->data().toString());
			}
		}
		break;
		case KTProjectEvent::Lock:
		{
		}
		break;
	}
}

void KTExposureSheet::frameEvent(KTFrameEvent *e)
{
	D_FUNCINFO;
	switch(e->action() )
	{
		case KTProjectEvent::Add:
		{
			KTExposureTable *scene = dynamic_cast<KTExposureTable*>(m_scenes->widget((e->sceneIndex())));
			if(scene)
			{
				scene->setUseFrame( e->layerIndex(), e->frameIndex(),  e->partName());
			}
		}
		break;
		case KTProjectEvent::Remove:
		{
			KTExposureTable *scene = dynamic_cast<KTExposureTable*>(m_scenes->widget((e->sceneIndex())));
			if(scene)
			{
				scene->removeFrame(  e->layerIndex(), e->frameIndex());
			}
		}
		break;
		case KTProjectEvent::Move:
		{
			KTExposureTable *scene = dynamic_cast<KTExposureTable*>(m_scenes->widget((e->sceneIndex())));
			if(scene)
			{
				scene->moveFrame( e->layerIndex(), e->frameIndex(), e->layerIndex(), e->data().toInt());
			}
		}
		break;
		case KTProjectEvent::Lock:
		{
			KTExposureTable *scene = dynamic_cast<KTExposureTable*>(m_scenes->widget((e->sceneIndex())));
			if(scene)
			{
				scene->setLockFrame( e->layerIndex(), e->frameIndex(),  e->data().toBool());
			}
		}
		break;
		case KTProjectEvent::Rename:
		{
			KTExposureTable *scene = dynamic_cast<KTExposureTable*>(m_scenes->widget((e->sceneIndex())));
			if(scene)
			{
				scene->setName( e->layerIndex(), e->frameIndex(), e->data().toString());
			}
		}
		break;
	}
}
