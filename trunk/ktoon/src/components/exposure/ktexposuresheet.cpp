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
#include <dgui/dapplication.h>

#include <dcore/ddebug.h>
#include <dgui/doptionaldialog.h>

#include <QToolTip>
#include <QPixmap>
#include <QHBoxLayout>
#include <QList>

#include "ktprojectrequest.h"
#include "ktrequestbuilder.h"

struct KTExposureSheet::Private
{
	DTabWidget *scenes;
	KTExposureTable *currentTable;
	KTProjectActionBar *actionBar;
};

KTExposureSheet::KTExposureSheet( QWidget *parent) : KTModuleWidgetBase(parent, "Exposure Sheet"), d(new Private)
{
	DINIT;
	d->currentTable = 0;
	setWindowTitle( tr( "&Exposure Sheet" ) );
	setWindowIcon(QPixmap(THEME_DIR+"/icons/exposure_sheet.png"));
	
	d->actionBar = new KTProjectActionBar( KTProjectActionBar::InsertLayer |
			KTProjectActionBar::RemoveLayer |
			KTProjectActionBar::InsertFrame |
			KTProjectActionBar::RemoveFrame |
			KTProjectActionBar::MoveFrameUp |
			KTProjectActionBar::MoveFrameDown| 
			KTProjectActionBar::LockFrame);
	
	connect(d->actionBar, SIGNAL(actionSelected( int )), this, SLOT(applyAction( int)));
	addChild( d->actionBar, Qt::AlignCenter );
	
	d->scenes = new DTabWidget(this);
	connect( d->scenes , SIGNAL(currentChanged ( int )), this, SLOT(emitRequestChangeScene( int ) ));
	addChild(d->scenes);
}

KTExposureSheet::~KTExposureSheet()
{
	delete d;
	DEND;
}

void KTExposureSheet::addScene(int index, const QString &name)
{
	D_FUNCINFO << " index: " << index << " name: " << name;
	KTExposureTable *newScene = new KTExposureTable;
	
	d->scenes->insertTab(index, newScene, name);
	
	connect(newScene, SIGNAL(requestSetUsedFrame(int, int)), this, SLOT(insertItem( int, int )));
	connect(newScene, SIGNAL(requestRenameFrame(int, int,const QString & )), this, SLOT(renameFrame( int, int, const QString &  )));
	connect(newScene, SIGNAL(requestSelectFrame(int, int)), SLOT(selectFrame( int, int )));
	
	connect(newScene, SIGNAL(requestRenameLayer(int, const QString & )), this, SLOT(renameLayer( int, const QString &  )));
	connect(newScene, SIGNAL(requestMoveLayer(int, int )), this, SLOT(moveLayer( int, int  )));
	
	connect(newScene, SIGNAL(requestChangeVisiblityLayer(int , bool )),  this, SLOT(changeVisiblityLayer( int, bool  )));
	
	
// 	d->currentTable = newScene;
// 	d->scenes->setCurrentWidget(d->currentTable);
}

void KTExposureSheet::renameScene( int index, const QString &name)
{
	d->scenes->setTabText(index, name);
}


void KTExposureSheet::applyAction(int action)
{
	D_FUNCINFO<< "action: " << action;
	if (  d->currentTable == 0 )
	{
		dFatal() << "KTExposureSheet::applyAction: No layer view!!" << endl;
		return;
	}
	d->currentTable = static_cast<KTExposureTable*>( d->scenes->currentWidget());	
	switch(action)
	{
		case KTProjectActionBar::InsertLayer:
		{
			int layer = d->currentTable->currentLayer()+1;
			
			KTProjectRequest event = KTRequestBuilder::createLayerRequest( d->scenes->currentIndex(), layer, KTProjectRequest::Add);
			
			emit requestTriggered( &event );
		}
		break;
		case KTProjectActionBar::RemoveLayer:
		{
			KTProjectRequest event = KTRequestBuilder::createLayerRequest(d->scenes->currentIndex(), d->currentTable->currentLayer(), KTProjectRequest::Remove);
			
			emit requestTriggered( &event );
		}
		break;
		case KTProjectActionBar::InsertFrame:
		{
			int used = d->currentTable->numUsed();
			int finish = d->currentTable->currentFrame()+1;
			
			if(used < finish)
			{
				for(int i = used; i <= finish; i++)
				{
					insertItem( d->currentTable->currentLayer(), i);
				}
			}
			else
			{
				insertItem( d->currentTable->currentLayer(), finish);
			}
			
		}
		break;
		case KTProjectActionBar::RemoveFrame:
		{
			KTProjectRequest event = KTRequestBuilder::createFrameRequest( d->scenes->currentIndex(), d->currentTable->currentLayer(), d->currentTable->currentFrame(), KTProjectRequest::Remove);
			emit requestTriggered( &event );
			break;
		}
		case KTProjectActionBar::MoveFrameUp:
		{
			KTProjectRequest event = KTRequestBuilder::createFrameRequest( d->scenes->currentIndex(), d->currentTable->currentLayer(), d->currentTable->currentFrame(), KTProjectRequest::Move, d->currentTable->currentFrame()-1);
			emit requestTriggered( &event );
		}
		break;
		case KTProjectActionBar::MoveFrameDown:
		{
			KTProjectRequest event = KTRequestBuilder::createFrameRequest( d->scenes->currentIndex(), d->currentTable->currentLayer(), d->currentTable->currentFrame(), KTProjectRequest::Move, d->currentTable->currentFrame()+1 );
			emit requestTriggered( &event );
		}
		break;
		case KTProjectActionBar::LockFrame:
		{
			bool locked = d->currentTable->frameIsLocked(d->currentTable->currentColumn(),  d->currentTable->currentFrame());
			
			KTProjectRequest event = KTRequestBuilder::createFrameRequest (d->scenes->currentIndex(), d->currentTable->currentLayer(), d->currentTable->currentFrame(), KTProjectRequest::Lock, !locked);
			emit requestTriggered( &event );
		}
		break;
	}
}


void KTExposureSheet::setScene(int index)
{
	D_FUNCINFO;
	if(d->scenes->count() >= index)
	{
		d->scenes->blockSignals(true);
		d->scenes->setCurrentIndex(index);
		
		d->currentTable = qobject_cast<KTExposureTable *>(d->scenes-> currentWidget());
		d->scenes->blockSignals(false);
	}
}

void KTExposureSheet::emitRequestChangeScene(int index)
{
	KTProjectRequest request = KTRequestBuilder::createSceneRequest(index, KTProjectRequest::Select);
	emit localRequestTriggered( &request );
}

void KTExposureSheet::insertItem(int indexLayer, int indexFrame)
{
	KTProjectRequest request = KTRequestBuilder::createFrameRequest( d->scenes->currentIndex() , indexLayer, indexFrame, KTProjectRequest::Add );
	emit requestTriggered( &request );
}


void KTExposureSheet::renameFrame(int indexLayer, int indexFrame, const QString & name)
{
	KTProjectRequest event = KTRequestBuilder::createFrameRequest( d->scenes->currentIndex() , indexLayer, indexFrame, KTProjectRequest::Rename, name);
	emit requestTriggered( &event );
}

void KTExposureSheet::selectFrame(int indexLayer, int indexFrame)
{
	KTProjectRequest request = KTRequestBuilder::createFrameRequest( d->scenes->currentIndex() , indexLayer, indexFrame,KTProjectRequest::Select, "1" );
	
	emit localRequestTriggered( &request );
}

void KTExposureSheet::changeVisiblityLayer(int visualIndexLayer, bool visibility)
{
	KTProjectRequest event = KTRequestBuilder::createLayerRequest( d->scenes->currentIndex(), visualIndexLayer, KTProjectRequest::View,  visibility );
	emit requestTriggered( &event );
}

void KTExposureSheet::renameLayer(int indexLayer, const QString & name)
{
	KTProjectRequest event = KTRequestBuilder::createLayerRequest(d->scenes->currentIndex(), indexLayer, KTProjectRequest::Rename, name);
	
	emit requestTriggered( &event );
}

void KTExposureSheet::moveLayer(int oldIndex, int newIndex)
{
	KTProjectRequest event = KTRequestBuilder::createLayerRequest(d->scenes->currentIndex(), oldIndex, KTProjectRequest::Move, newIndex);
	emit requestTriggered( &event );
}

void KTExposureSheet::closeAllScenes()
{
	D_FUNCINFO;
	
	delete d->currentTable;
	
	d->scenes->removeAllTabs();
	
	d->currentTable = 0;
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
			d->scenes->blockSignals(true);
			QWidget * widget = d->scenes->widget( e->sceneIndex() );
			d->scenes->removeTab(e->sceneIndex());
			delete widget;
			d->scenes->blockSignals(false);
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
			dFatal() << "Select scene " << e->sceneIndex();
			setScene( e->sceneIndex() );
		}
		break;
	}
}

void KTExposureSheet::layerResponse(KTLayerResponse *e)
{
	KTExposureTable *scene = dynamic_cast<KTExposureTable*>(d->scenes->widget((e->sceneIndex())));
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
	KTExposureTable *scene = dynamic_cast<KTExposureTable*>(d->scenes->widget((e->sceneIndex())));
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
				scene->moveFrame( e->layerIndex(), e->frameIndex(), e->layerIndex(), e->arg().toInt());
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
