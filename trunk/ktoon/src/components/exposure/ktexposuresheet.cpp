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
			KTProjectActionBar::MoveFrameDown);
	
	connect(m_actionBar, SIGNAL(actionSelected( int )), this, SLOT(applyAction( int)));
	addChild( m_actionBar, Qt::AlignCenter );
	
	m_scenes = new DTabWidget(this);
	connect( m_scenes , SIGNAL(currentChanged ( int )), this, SLOT(emitRequestChangeScene( int ) ));
	addChild(m_scenes);
	
	
// 	show();
}

KTExposureSheet::~KTExposureSheet()
{
	DEND;
}




void KTExposureSheet::addScene(int index, const QString &name)
{
	D_FUNCINFO;
	KTExposureTable *newScene = new KTExposureTable/*(100, 0)*/;
	
	m_tables << newScene;
	
	m_scenes->insertTab (index, newScene, name);
	
	connect(newScene, SIGNAL(requestSetUsedFrame(int, int)), this, SLOT(insertItem( int, int )));
	
	m_currentTable = newScene;
	m_scenes->setCurrentWidget(m_currentTable);
}

void KTExposureSheet::renameScene(const QString &name, int id)
{
	m_scenes->setTabText(id, name);
}


void KTExposureSheet::applyAction(int action)
{
	D_FUNCINFO;
	
// 	dDebug() << m_tables.count();
	
	if ( m_tables.count() == 0 || m_currentTable == 0 )
	{
		dFatal() << "KTExposureSheet::applyAction: No layer view!!" << endl;
		return;
	}

	switch(action)
	{
		case KTProjectActionBar::InsertLayer:
		{
			
// 			dDebug() << "KTProjectActionBar::InsertLayer" << m_currentTable->currentLayer() ;
			KTLayerEvent *event = new KTLayerEvent(KTProjectEvent::Add, m_scenes->currentIndex() , m_currentTable->currentLayer()+1);
			emit eventTriggered( event );
			
// 			emit requestInsertLayer();
// 			emit layerSelected(m_currentTable->currentLayer());
// 			m_currentTable->insertFrames();
			break;
		}
		case KTProjectActionBar::RemoveLayer:
		{
			DCONFIG->beginGroup("Exposure");
			bool noAsk = qvariant_cast<bool>(DCONFIG->value("RemoveWithoutAskLayer", false));
			if ( ! noAsk )
			{
				DOptionalDialog dialog(tr("Do you want to remove this layer?"),tr("Remove?"), this);
		
				if( dialog.exec() == QDialog::Rejected )
				{
					return;
				}
				DCONFIG->setValue("RemoveWithoutAskLayer", dialog.shownAgain());
				DCONFIG->sync();
			}
// 			m_currentTable->currentLayer();
			
			
			KTLayerEvent *event = new KTLayerEvent(KTProjectEvent::Remove, m_scenes->indexOf(m_currentTable), m_currentTable->currentLayer());
			emit eventTriggered( event );
// 			emit requestRemoveLayer(m_currentTable->currentLayer());
			break;
		}
		case KTProjectActionBar::InsertFrame:
		{
			dDebug() << "aki" << m_currentTable->numUsed() << " " << m_currentTable->currentFrame()+1;
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
			DCONFIG->beginGroup("Exposure");
			
			emit requestRemoveFrame();
			break;
		}
// 		case LockFrame:
// 		{
// // 			m_currentTable->testLockFrame();
// 			emit requestLockFrame();
// 			break;
// 		}
		case KTProjectActionBar::MoveFrameUp:
		{
			emit requestMoveFrame(true);
			break;
		}
		case KTProjectActionBar::MoveFrameDown:
		{
			emit requestMoveFrame(false);
			break;
		}
	}
}

void KTExposureSheet::addFrame(int idLayer, const QString &name, bool addedToEnd)
{
// 	dDebug( ) << "KTExposureSheet::addFrame(" << idLayer << " , "<< name << " , "  << addedToEnd << ")" << endl;
// 	m_currentTable->setUseFrame(idLayer, name, addedToEnd);
}


void KTExposureSheet::setCurrentCell( int idLayer, int idFrame)
{
	if(m_currentTable)
	{
// 		m_currentTable->setCurrentCell( idLayer, idFrame);
	}
}


// void KTExposureSheet::insertLayer(const QString& name)
// {
// // 	D_FUNCINFO;
// // 	dDebug() << "KTExposureSheet::insertLayer(const QString& " << name  << ")";
// 	if ( m_currentTable )
// 	{
// 		m_currentTable->insertLayer( name);
// 		
// 	}
// 	else
// 	{
// 		dFatal() << "KTExposureSheet::insertLayer: No layer view!" << endl;
// 	}
// }

void KTExposureSheet::removeLayer(int index)
{
	dDebug() << "removeLayer(int" <<  index << ")";
	if ( m_currentTable )
		m_currentTable->removeLayer( index );
}


void KTExposureSheet::removeCurrentFrame()
{
// 	if ( m_currentTable )
// 	m_currentTable->removeFrame();
}

void KTExposureSheet::moveFrame(bool up)
{
// 	if(up)
// 	{
// 		m_currentTable->moveCurrentFrame(KTTableExposure::Up );
// 	}
// 	else
// 	{
// 		m_currentTable->moveCurrentFrame(KTTableExposure::Down );
// 	}
}

void KTExposureSheet::moveLayer(bool left)
{
	D_FUNCINFO;
// 	m_currentTable->moveCurrentLayer(left);
}


void KTExposureSheet::lockCurrentFrame()
{
// 	m_currentTable->/*/*lock*/*/CurrentFrame();
}

void KTExposureSheet::setScene(int index)
{
	D_FUNCINFO;
// 	dDebug() << "KTExposureSheet::setScene(int "<< index << ")";
	if(index != m_scenes->indexOf(m_currentTable) && m_tables.count() >= index)
	{
		m_currentTable = m_tables[index];
		m_scenes->setCurrentWidget(m_tables[index]);
	}
}

void KTExposureSheet::emitRequestChangeScene(int index)
{
// 	dDebug() << "KTExposureSheet::emitRequestChangeScene(int" <<  index << ")" << m_scenes->indexOf(m_currentTable);
	
	if(index != m_scenes->indexOf(m_currentTable))
	{
		emit requestChangeScene(index);
	}
}

void KTExposureSheet::insertItem(int indexLayer, int indexFrame)
{
	KTFrameEvent *event = new KTFrameEvent(KTProjectEvent::Add, m_scenes->currentIndex() , indexLayer, indexFrame);
	emit eventTriggered( event );
// 	requestSetUsedFrame
}

void KTExposureSheet::closeAllScenes()
{
	D_FUNCINFO;
	
	delete m_currentTable;
	
	m_scenes->removeAllTabs();
	m_tables.clear();
	
	m_currentTable = 0;
}

void KTExposureSheet::setLayer(int index)
{
// 	m_currentTable->setLayer( index );
}

void KTExposureSheet::setFrameName(int indexLayer, int indexFrame, const QString& name )
{
	dDebug() << "setFrameName(" << indexLayer << "," << indexFrame << "," << name  << ")";
	// 	m_currentTable->setFrameName(indexLayer, indexFrame, name); // setText
}

void KTExposureSheet::setLayerName(int indexLayer, const QString& name )
{
	dDebug() << "setLayerName(" << indexLayer << "," << name << ")";
	// 	m_currentTable->setLayerName( indexLayer, name ); // setText
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
			removeLayer( e->sceneIndex());
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
			dDebug() << scene;
			if(scene)
			{
				scene->setUseFrame( e->layerIndex(), e->frameIndex(),  e->partName());
			}
		}
		break;
		case KTProjectEvent::Remove:
		{
// 			removeLayer( e->sceneIndex());
		}
		break;
	}
}
