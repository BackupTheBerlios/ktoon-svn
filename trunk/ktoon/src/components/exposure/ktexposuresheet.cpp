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
	
	m_scenes->insertTab (index, newScene, name);
	
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
	D_FUNCINFO;
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
			bool locked = m_currentTable->frameIsLocked(m_currentTable->currentColumn(),  m_currentTable->currentFrame());
			
			KTFrameEvent event(KTProjectEvent::Lock, m_scenes->currentIndex(), m_currentTable->currentLayer(), m_currentTable->currentFrame(), !locked);
			emit eventTriggered( &event );
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
// 	dDebug() << "KTExposureSheet::emitRequestChangeScene(" << index << ")";
	KTSceneEvent event(KTProjectEvent::Select, index);
	emit eventTriggered( &event );
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

void KTExposureSheet::selectFrame(int indexLayer, int indexFrame)
{
// 	dDebug() << "KTExposureSheet::selectFrame("  << indexLayer << "," << indexFrame << ")";
	KTFrameEvent event(KTProjectEvent::Select, m_scenes->currentIndex() , indexLayer, indexFrame, true);
	emit eventTriggered( &event );
}

void KTExposureSheet::changeVisiblityLayer(int visualIndexLayer, bool visibility)
{
	KTLayerEvent event(KTProjectEvent::View, m_scenes->currentIndex() , visualIndexLayer, visibility);
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
		case KTProjectEvent::Select:
		{
			setScene( e->sceneIndex() );
		}
		break;
	}
}

void KTExposureSheet::layerEvent(KTLayerEvent *e)
{
	KTExposureTable *scene = dynamic_cast<KTExposureTable*>(m_scenes->widget((e->sceneIndex())));
	if(scene)
	{
		switch(e->action() )
		{
			case KTProjectEvent::Add:
			{
				scene->insertLayer(e->layerIndex(),  e->partName());
			}
			break;
			case KTProjectEvent::Remove:
			{
				scene->removeLayer( e->layerIndex());
			}
			break;
			case KTProjectEvent::Move:
			{
				scene->moveLayer( e->layerIndex(), e->data().toInt());
			}
			break;
			case KTProjectEvent::Rename:
			{
				scene->setLayerName( e->layerIndex(), e->data().toString());
			}
			break;
			case KTProjectEvent::Lock:
			{
				scene->setLockLayer( e->layerIndex(), e->data().toBool());
			}
			break;
			case KTProjectEvent::Select:
			{
				setScene(e->sceneIndex());
				scene->blockSignals(true );
				scene->selectFrame( e->layerIndex(), 0);
				scene->blockSignals(false );
			}
			case KTProjectEvent::View:
			{
				scene->setVisibilityChanged( e->layerIndex(), e->data().toBool());
			}
			break;
		}
	}
}

void KTExposureSheet::frameEvent(KTFrameEvent *e)
{
	D_FUNCINFO;
	KTExposureTable *scene = dynamic_cast<KTExposureTable*>(m_scenes->widget((e->sceneIndex())));
	if(scene)
	{
		switch(e->action() )
		{
			case KTProjectEvent::Add:
			{
				scene->setUseFrame( e->layerIndex(), e->frameIndex(),  e->partName());
			}
			break;
			case KTProjectEvent::Remove:
			{
				scene->removeFrame(  e->layerIndex(), e->frameIndex());
			}
			break;
			case KTProjectEvent::Move:
			{
				scene->moveFrame( e->layerIndex(), e->frameIndex(), e->layerIndex(), e->data().toInt());
			}
			break;
			case KTProjectEvent::Lock:
			{
				scene->setLockFrame( e->layerIndex(), e->frameIndex(),  e->data().toBool());
			}
			break;
			case KTProjectEvent::Rename:
			{
				scene->setFrameName( e->layerIndex(), e->frameIndex(), e->data().toString());
			}
			break;
			case KTProjectEvent::Select:
			{
				setScene(e->sceneIndex());
				scene->selectFrame( e->layerIndex(), e->frameIndex());
			}
			break;
		}
	}
}
