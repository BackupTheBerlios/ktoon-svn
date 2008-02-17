/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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
#include <kgui/kapplication.h>

#include <kcore/kdebug.h>
#include <kgui/koptionaldialog.h>

#include <QToolTip>
#include <QPixmap>
#include <QHBoxLayout>
#include <QList>
#include <QMenu>

#include "ktprojectrequest.h"
#include "ktrequestbuilder.h"

struct KTExposureSheet::Private
{
	KTabWidget *scenes;
	KTExposureTable *currentTable;
	KTProjectActionBar *actionBar;
	QMenu *menu;
	QString nameCopyFrame;
	// QString copyFrame;
};

KTExposureSheet::KTExposureSheet( QWidget *parent) : KTModuleWidgetBase(parent, "Exposure Sheet"), k(new Private)
{
	#ifdef K_DEBUG
		KINIT;
	#endif

	k->currentTable = 0;
	setWindowTitle( tr( "&Exposure Sheet" ) );
	setWindowIcon(QPixmap(THEME_DIR+"/icons/exposure_sheet.png"));
	
	k->actionBar = new KTProjectActionBar( KTProjectActionBar::InsertLayer |
			KTProjectActionBar::RemoveLayer |
			KTProjectActionBar::InsertFrame |
			KTProjectActionBar::RemoveFrame |
			KTProjectActionBar::MoveFrameUp |
			KTProjectActionBar::MoveFrameDown| 
			KTProjectActionBar::LockFrame);
	
	connect(k->actionBar, SIGNAL(actionSelected( int )), this, SLOT(applyAction( int)));
	addChild( k->actionBar, Qt::AlignCenter );
	
	k->scenes = new KTabWidget(this);
	connect( k->scenes , SIGNAL(currentChanged ( int )), this, SLOT(emitRequestChangeScene( int ) ));
	addChild(k->scenes);
	
	createMenu();
}

KTExposureSheet::~KTExposureSheet()
{
	delete k;
	#ifdef K_DEBUG
		KEND;
	#endif
}

void KTExposureSheet::createMenu()
{
	k->menu = new QMenu(tr("actions"));
	k->menu->addAction( tr("Insert layer"))->setData(KTProjectActionBar::InsertLayer);
	k->menu->addAction( tr("Remove layer") )->setData(KTProjectActionBar::RemoveLayer);
	k->menu->addAction(tr("Insert frame"))->setData(KTProjectActionBar::InsertFrame);
	k->menu->addAction( tr("Remove frame"))->setData(KTProjectActionBar::RemoveFrame);
	k->menu->addAction( tr("Lock frame"))->setData(KTProjectActionBar::LockFrame);
	
	k->menu->addAction( tr("Copy frame"), this, SLOT(emitRequestCopyCurrentFrame()));
	k->menu->addAction( tr("Paste in frame"), this, SLOT(emitRequestPasteInCurrentFrame()));
	k->menu->addAction( tr("Expand frame"), this, SLOT(emitRequestExpandCurrentFrame()));
	
	connect(k->menu,  SIGNAL(triggered( QAction * )), this, SLOT(actionTiggered(QAction*)));
}

void KTExposureSheet::addScene(int index, const QString &name)
{

	#ifdef K_DEBUG
		K_FUNCINFO << " index: " << index << " name: " << name;
	#endif

	KTExposureTable *newScene = new KTExposureTable;
	newScene->setMenu(k->menu);
	k->scenes->insertTab(index, newScene, name);
	
	connect(newScene, SIGNAL(requestSetUsedFrame(int, int)), this, SLOT(insertItem( int, int )));
	connect(newScene, SIGNAL(requestRenameFrame(int, int,const QString & )), this, SLOT(renameFrame( int, int, 
												const QString &  )));
	connect(newScene, SIGNAL(requestSelectFrame(int, int)), SLOT(selectFrame( int, int )));
	
	connect(newScene, SIGNAL(requestRenameLayer(int, const QString & )), this, SLOT(renameLayer( int, 
												const QString &  )));
	connect(newScene, SIGNAL(requestMoveLayer(int, int )), this, SLOT(moveLayer( int, int  )));
	
	connect(newScene, SIGNAL(requestChangeVisiblityLayer(int , bool )),  this, SLOT(changeVisiblityLayer( int,
														bool  )));
	// k->currentTable = newScene;
	// k->scenes->setCurrentWidget(k->currentTable);
}

void KTExposureSheet::renameScene( int index, const QString &name)
{
	k->scenes->setTabText(index, name);
}

void KTExposureSheet::applyAction(int action)
{
	#ifdef K_DEBUG
		K_FUNCINFO<< "action: " << action;
	#endif

	if (  k->currentTable == 0 )
	{
		#ifdef K_DEBUG
			kFatal() << "KTExposureSheet::applyAction: No layer view!!" << endl;
		#endif
		return;
	}
	k->currentTable = static_cast<KTExposureTable*>( k->scenes->currentWidget());	
	switch(action)
	{
		case KTProjectActionBar::InsertLayer:
		{
			int layer = k->currentTable->currentLayer()+1;
			
			KTProjectRequest event = KTRequestBuilder::createLayerRequest( k->scenes->currentIndex(), layer,
												KTProjectRequest::Add);
			
			emit requestTriggered( &event );
		}
		break;
		case KTProjectActionBar::RemoveLayer:
		{
			KTProjectRequest event = KTRequestBuilder::createLayerRequest(k->scenes->currentIndex(), 
								k->currentTable->currentLayer(), KTProjectRequest::Remove);
			
			emit requestTriggered( &event );
		}
		break;
		case KTProjectActionBar::InsertFrame:
		{
			int used = k->currentTable->numUsed( k->currentTable->currentColumn() );
			int finish = k->currentTable->currentFrame()+1;
			
			if(used < finish)
			{
				for(int i = used; i <= finish; i++)
				{
					insertItem( k->currentTable->currentLayer(), i);
				}
			}
			else
			{
				insertItem( k->currentTable->currentLayer(), finish);
			}
			
		}
		break;
		case KTProjectActionBar::RemoveFrame:
		{
			KTProjectRequest event = KTRequestBuilder::createFrameRequest( k->scenes->currentIndex(), 
						k->currentTable->currentLayer(), k->currentTable->currentFrame(),
						KTProjectRequest::Remove);
			emit requestTriggered( &event );
			break;
		}
		case KTProjectActionBar::MoveFrameUp:
		{
			KTProjectRequest event = KTRequestBuilder::createFrameRequest( k->scenes->currentIndex(), 
						k->currentTable->currentLayer(), k->currentTable->currentFrame(),
						KTProjectRequest::Move, k->currentTable->currentFrame()-1);
			emit requestTriggered( &event );
		}
		break;
		case KTProjectActionBar::MoveFrameDown:
		{
			KTProjectRequest event = KTRequestBuilder::createFrameRequest( k->scenes->currentIndex(), 
						k->currentTable->currentLayer(), k->currentTable->currentFrame(),
						KTProjectRequest::Move, k->currentTable->currentFrame()+1 );
			emit requestTriggered( &event );
		}
		break;
		case KTProjectActionBar::LockFrame:
		{
			bool locked = k->currentTable->frameIsLocked(k->currentTable->currentColumn(), 
										k->currentTable->currentFrame());
			
			KTProjectRequest event = KTRequestBuilder::createFrameRequest (k->scenes->currentIndex(), 
											k->currentTable->currentLayer(), 
											k->currentTable->currentFrame(),
											KTProjectRequest::Lock, !locked);
			emit requestTriggered( &event );
		}
		break;
	}
}

void KTExposureSheet::setScene(int index)
{
	#ifdef K_DEBUG
		K_FUNCINFO;
	#endif
	if(k->scenes->count() >= index)
	{
		k->scenes->blockSignals(true);
		k->scenes->setCurrentIndex(index);		
		k->currentTable = qobject_cast<KTExposureTable *>(k->scenes-> currentWidget());
		k->scenes->blockSignals(false);
	}
}

void KTExposureSheet::emitRequestChangeScene(int index)
{
	KTProjectRequest request = KTRequestBuilder::createSceneRequest(index, KTProjectRequest::Select);
	emit localRequestTriggered( &request );
}

void KTExposureSheet::emitRequestCopyCurrentFrame()
{
	KTProjectRequest request = KTRequestBuilder::createFrameRequest(k->scenes->currentIndex(), 
									k->currentTable->currentLayer(), 
									k->currentTable->currentFrame(), 
									KTProjectRequest::Copy);
	emit localRequestTriggered( &request );
}

void KTExposureSheet::emitRequestPasteInCurrentFrame()
{
	/*if(!d->copyFrame.isEmpty())
	{
		KTProjectRequest request = KTRequestBuilder::createFrameRequest(k->scenes->currentIndex(), 
										k->currentTable->currentLayer(), 
										k->currentTable->currentFrame()+1,
										KTProjectRequest::Paste, k->copyFrame );
		emit requestTriggered( &request );
	}
	*/
	
	if(k->nameCopyFrame.isEmpty())
	{
		return;
	}
	
	if( k->currentTable->numUsed(k->currentTable->currentLayer()) <= k->currentTable->currentRow() )
	{
	   for(int i = k->currentTable->numUsed(k->currentTable->currentLayer()); i <= k->currentTable->currentRow(); i++)
		{
			insertItem(k->currentTable->currentLayer(), i);
			
			KTProjectRequest request = KTRequestBuilder::createFrameRequest(k->scenes->currentIndex(), 
											k->currentTable->currentLayer(), 
											i, KTProjectRequest::Paste);
			emit localRequestTriggered( &request );
		}
	}
	else
	{
		KTProjectRequest request = KTRequestBuilder::createFrameRequest(k->scenes->currentIndex(), 
										k->currentTable->currentLayer(), 
										k->currentTable->currentFrame(),
										KTProjectRequest::Paste);
		emit localRequestTriggered( &request );
	}
}

void KTExposureSheet::emitRequestExpandCurrentFrame()
{
	#ifdef K_DEBUG
		K_FUNCINFOX("exposure");
	#endif

	KTProjectRequest request = KTRequestBuilder::createFrameRequest(k->scenes->currentIndex(), 
									k->currentTable->currentLayer(), 
									k->currentTable->currentFrame(), 
									KTProjectRequest::Expand, 1);
	emit requestTriggered( &request );
}

void KTExposureSheet::insertItem(int indexLayer, int indexFrame)
{
	KTProjectRequest request = KTRequestBuilder::createFrameRequest( k->scenes->currentIndex() , 
									indexLayer, indexFrame, KTProjectRequest::Add );
	emit requestTriggered( &request );
}

void KTExposureSheet::renameFrame(int indexLayer, int indexFrame, const QString & name)
{
	KTProjectRequest event = KTRequestBuilder::createFrameRequest( k->scenes->currentIndex() , indexLayer, indexFrame,
									KTProjectRequest::Rename, name);
	emit requestTriggered( &event );
}

void KTExposureSheet::selectFrame(int indexLayer, int indexFrame)
{
	KTProjectRequest request = KTRequestBuilder::createFrameRequest( k->scenes->currentIndex() , indexLayer, indexFrame,
									 KTProjectRequest::Select, "1" );
	
	emit localRequestTriggered( &request );
}

void KTExposureSheet::changeVisiblityLayer(int visualIndexLayer, bool visibility)
{
	KTProjectRequest event = KTRequestBuilder::createLayerRequest( k->scenes->currentIndex(), visualIndexLayer,
										KTProjectRequest::View,  visibility );
	emit requestTriggered( &event );
}

void KTExposureSheet::renameLayer(int indexLayer, const QString & name)
{
	KTProjectRequest event = KTRequestBuilder::createLayerRequest(k->scenes->currentIndex(), indexLayer,
									KTProjectRequest::Rename, name);
	
	emit requestTriggered( &event );
}

void KTExposureSheet::moveLayer(int oldIndex, int newIndex)
{
	KTProjectRequest event = KTRequestBuilder::createLayerRequest(k->scenes->currentIndex(), oldIndex,
									KTProjectRequest::Move, newIndex);
	emit requestTriggered( &event );
}

void KTExposureSheet::actionTiggered(QAction *action)
{
	bool ok;
	int id = action->data().toInt(&ok);
	if(ok)
	{
		applyAction(id);
	}
}

void KTExposureSheet::closeAllScenes()
{
	#ifdef K_DEBUG
		K_FUNCINFO;
	#endif
	
	delete k->currentTable;
	k->scenes->removeAllTabs();
	k->currentTable = 0;
}


void KTExposureSheet::sceneResponse(KTSceneResponse *e)
{
	#ifdef K_DEBUG
	K_FUNCINFOX("exposure");
	#endif
	switch(e->action() )
	{
		case KTProjectRequest::Add:
		{
			addScene(e->sceneIndex(), e->arg().toString());
		}
		break;
		case KTProjectRequest::Remove:
		{
			k->scenes->blockSignals(true);
			QWidget * widget = k->scenes->widget( e->sceneIndex() );
			k->scenes->removeTab(e->sceneIndex());
			delete widget;
			k->scenes->blockSignals(false);
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
			#ifdef K_DEBUG
				kFatal() << "Select scene " << e->sceneIndex();
			#endif
			setScene( e->sceneIndex() );
		}
		break;
	}
}

void KTExposureSheet::layerResponse(KTLayerResponse *e)
{
	KTExposureTable *scene = dynamic_cast<KTExposureTable*>(k->scenes->widget((e->sceneIndex())));
	if(scene)
	{
		switch(e->action() )
		{
			case KTProjectRequest::Add:
			{
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
	#ifdef K_DEBUG
		K_FUNCINFO;
	#endif

	KTExposureTable *scene = dynamic_cast<KTExposureTable*>(k->scenes->widget((e->sceneIndex())));
	if(scene)
	{
		switch(e->action() )
		{
			case KTProjectRequest::Add:
			{
				scene->setUseFrame( e->layerIndex(), e->frameIndex(),  e->arg().toString(), e->external());
			}
			break;
			case KTProjectRequest::Remove:
			{
				scene->removeFrame(  e->layerIndex(), e->frameIndex());
			}
			break;
			case KTProjectRequest::Move:
			{
				scene->moveFrame( e->layerIndex(), e->frameIndex(), e->layerIndex(), e->arg().toInt(), 
						  e->external());
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
			case KTProjectRequest::Expand:
			{
				for(int i = 0; i < e->arg().toInt(); i++)
				{
					scene->setUseFrame( e->layerIndex(), e->frameIndex()+i+1, 
							    scene->frameName(e->layerIndex(), e->frameIndex()), 
							    e->external() );
				}
			}
			break;
			case KTProjectRequest::Copy:
			{
				k->nameCopyFrame = scene->frameName(e->layerIndex(), e->frameIndex());
				// k->copyFrame = e->arg().toString();
			}
			break;
			case KTProjectRequest::Paste:
			{
				if(e->frameIndex() >= scene->numUsed( e->layerIndex() ))
				{
					if(e->mode() == KTProjectResponse::Undo)
					{
						if(e->arg().toString().isEmpty())
						{
							scene->removeFrame(e->layerIndex(), e->frameIndex());
						}
					}
					else
					{
						scene->setUseFrame( e->layerIndex(), e->frameIndex(), 
									k->nameCopyFrame + "- copy", e->external());
					}
				}
			}
			break;
		}
	}
}
