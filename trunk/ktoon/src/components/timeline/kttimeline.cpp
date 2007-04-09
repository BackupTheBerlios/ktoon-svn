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
#include <dgui/dapplication.h>
#include <dcore/ddebug.h>

#include "ktprojectrequest.h"

#include "ktframestable.h"
#include "ktlayermanager.h"

#include "ktprojectactionbar.h"

#include "ktrequestbuilder.h"

struct KTTimeLine::Private
{
	Private() : container(0), actionBar(0) {}
	
	DTabWidget *container;
	KTProjectActionBar *actionBar;
};

KTTimeLine::KTTimeLine(QWidget *parent) : KTModuleWidgetBase(parent, "KTTimeLine"), d(new Private)
{
	DINIT;
	
	setWindowTitle(tr("&Time Line"));
	setWindowIcon(QPixmap(THEME_DIR+"/icons/time_line.png"));
	
	d->actionBar = new KTProjectActionBar(KTProjectActionBar::AllActions );
	
	d->actionBar->insertSeparator( 4 );
	d->actionBar->insertSeparator( 9 );
	
	addChild( d->actionBar, Qt::AlignCenter );
	
	d->container = new DTabWidget(this);
	addChild(d->container);
	
	connect(d->actionBar, SIGNAL(actionSelected( int )), this, SLOT(requestCommand(int)));
}

KTTimeLine::~KTTimeLine()
{
	DEND;
	delete d;
}

KTLayerManager *KTTimeLine::layerManager(int sceneIndex)
{
	QSplitter *splitter = qobject_cast<QSplitter *>(d->container->widget(sceneIndex));
	if ( splitter )
	{
		return qobject_cast<KTLayerManager *>(splitter->widget(0));
	}
	
	return 0;
}

KTFramesTable *KTTimeLine::framesTable(int sceneIndex)
{
	QSplitter *splitter = qobject_cast<QSplitter *>(d->container->widget(sceneIndex));
	
	if ( splitter )
	{
		return qobject_cast<KTFramesTable *>(splitter->widget(1));
	}
	
	return 0;
}

void KTTimeLine::insertScene(int position, const QString &name)
{
	if ( position < 0 || position > d->container->count() )
	{
		return;
	}
	
	QSplitter *splitter = new QSplitter( d->container );
	
	KTLayerManager *layerManager = new KTLayerManager( splitter );
	
	layerManager->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	
	splitter->addWidget(layerManager);

	KTFramesTable *framesTable = new KTFramesTable(splitter);
	splitter->addWidget(framesTable);
	
	framesTable->setItemSize( 10, 20);
	layerManager->setRowHeight( 20 );
	
	connect(framesTable, SIGNAL(frameRequest(int, int, int, int, const QVariant&)), this, SLOT(requestFrameAction(int, int, int, int, const QVariant&)));
	
	connect(layerManager->verticalScrollBar(), SIGNAL(valueChanged (int)), framesTable->verticalScrollBar(), SLOT(setValue(int)));
	connect(framesTable->verticalScrollBar(), SIGNAL(valueChanged (int)), layerManager->verticalScrollBar(), SLOT(setValue(int)));
	
	connect(layerManager, SIGNAL(requestRenameEvent( int, const QString& )), this, SLOT(emitRequestRenameLayer(int, const QString &))); // FIXME
	
	d->container->insertTab(position, splitter, name );
}

void KTTimeLine::removeScene(int position)
{
	if ( position >= 0 && position < d->container->count() )
	{
		QWidget *w = d->container->widget(position);
// 		d->container->removeWidget(w);
		d->container->removeTab(position);
		
		delete w;
	}
}

void KTTimeLine::closeAllScenes()
{
	while(d->container->currentWidget())
	{
		delete d->container->currentWidget();
	}
}


void KTTimeLine::sceneResponse(KTSceneResponse *e)
{
	D_FUNCINFOX("timeline");
	switch(e->action())
	{
		case KTProjectRequest::Add:
		{
			insertScene( e->sceneIndex(), e->arg().toString() );
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


void KTTimeLine::layerResponse(KTLayerResponse *e)
{
	switch(e->action())
	{
		case KTProjectRequest::Add:
		{
			KTLayerManager *layerManager = this->layerManager( e->sceneIndex() );
			if ( layerManager )
			{
				layerManager->insertLayer( e->layerIndex(), e->arg().toString() );
			}
			
			KTFramesTable *framesTable = this->framesTable( e->sceneIndex() );
			if ( framesTable )
			{
				framesTable->insertLayer(e->layerIndex(), e->arg().toString() );
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
				layerManager->moveLayer( e->layerIndex(), e->arg().toInt() );
			}
			
			KTFramesTable *framesTable = this->framesTable( e->sceneIndex() );
			if ( framesTable )
			{
				framesTable->moveLayer(e->layerIndex(), e->arg().toInt() );
			}
		}
		break;
		case KTProjectRequest::Lock:
		{
			KTLayerManager *layerManager = this->layerManager( e->sceneIndex() );
			
			if ( layerManager )
			{
				layerManager->lockLayer( e->layerIndex(), e->arg().toBool() );
			}
		}
		break;
		case KTProjectRequest::Rename:
		{
			KTLayerManager *layerManager = this->layerManager( e->sceneIndex() );
			
			if ( layerManager )
			{
				layerManager->renameLayer( e->layerIndex(), e->arg().toString() );
			}
		}
		break;
	}
}


void KTTimeLine::frameResponse(KTFrameResponse *e)
{
	switch(e->action())
	{
		case KTProjectRequest::Add:
		{
			KTFramesTable *framesTable = this->framesTable( e->sceneIndex() );
			if ( framesTable )
			{
				framesTable->insertFrame( e->layerIndex(), e->arg().toString() );
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
				framesTable->lockFrame(e->layerIndex(), e->frameIndex(), e->arg().toBool() );
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
	int scenePos = d->container->currentIndex();
	int layerPos = -1;
	int framePos = -1;
	
	if ( scenePos >= 0 )
	{
		layerPos = layerManager(scenePos)->verticalHeader()->visualIndex(layerManager( scenePos )->currentRow());
		
		framePos = framesTable( scenePos )->lastFrameByLayer(layerPos);
	}
	
	if ( !requestFrameAction(action, framePos, layerPos, scenePos) )
	{
		if ( !requestLayerAction(action, layerPos, scenePos))
		{
			if ( !requestSceneAction(action, scenePos) )
			{
				dFatal("timeline") << "Can't handle action";
			}
		}
	}
	
}

bool KTTimeLine::requestFrameAction(int action, int framePos, int layerPos, int scenePos, const QVariant &arg)
{
	if ( scenePos < 0 )
	{
		scenePos = d->container->currentIndex();
	}
	
	if ( scenePos >= 0 )
	{
		if ( layerPos < 0 )
		{
			layerPos = layerManager(scenePos)->verticalHeader()->visualIndex(layerManager( scenePos )->currentRow());
		}
		
		if ( framePos < 0 )
		{
			framePos = framesTable( scenePos )->lastFrameByLayer(layerPos);
		}
	}
	
	switch(action)
	{
		case KTProjectActionBar::InsertFrame:
		{
			KTProjectRequest event = KTRequestBuilder::createFrameRequest(scenePos, layerPos, framePos+1, KTProjectRequest::Add, arg);
			
			emit requestTriggered( &event );
			
			return true;
		}
		break;
		case KTProjectActionBar::RemoveFrame:
		{
			KTProjectRequest event = KTRequestBuilder::createFrameRequest(scenePos, layerPos, framePos, KTProjectRequest::Remove, arg);
			
			emit requestTriggered( &event );
			return true;
		}
		break;
		case KTProjectActionBar::MoveFrameUp:
		{
			KTProjectRequest event = KTRequestBuilder::createFrameRequest(scenePos, layerPos, framePos, KTProjectRequest::Move, framePos-1 );
			emit requestTriggered(&event);
			return true;
		}
		break;
		case KTProjectActionBar::MoveFrameDown:
		{
			KTProjectRequest event = KTRequestBuilder::createFrameRequest(scenePos, layerPos, framePos, KTProjectRequest::Move, framePos+1 );
			
			emit requestTriggered(&event);
			return true;
		}
		break;
		
		case KTProjectRequest::Select:
		{
			KTProjectRequest event = KTRequestBuilder::createFrameRequest(scenePos, layerPos, framePos, KTProjectRequest::Select, arg);
		
			emit localRequestTriggered( &event );
			return true;
		}
		break;
	}
	
	return false;
}

bool KTTimeLine::requestLayerAction(int action, int layerPos, int scenePos, const QVariant &arg)
{
	if ( scenePos < 0 )
	{
		scenePos = d->container->currentIndex();
	}
	
	if ( scenePos >= 0 )
	{
		if ( layerPos < 0 )
		{
			layerPos = layerManager(scenePos)->verticalHeader()->visualIndex(layerManager( scenePos )->currentRow());
		}
	}
	
	switch(action)
	{
		case KTProjectActionBar::InsertLayer:
		{
			KTProjectRequest event = KTRequestBuilder::createLayerRequest( scenePos, layerPos+1, KTProjectRequest::Add, arg );
			emit requestTriggered( &event );
			return true;
		}
		break;
		case KTProjectActionBar::RemoveLayer:
		{
			KTProjectRequest event = KTRequestBuilder::createLayerRequest(scenePos, layerPos, KTProjectRequest::Remove, arg);
			
			emit requestTriggered( &event );
			return true;
		}
		break;
		case KTProjectActionBar::MoveLayerUp:
		{
			KTProjectRequest event = KTRequestBuilder::createLayerRequest(scenePos, layerPos, KTProjectRequest::Move, layerPos-1 );
			emit requestTriggered(&event);
			return true;
		}
		break;
		case KTProjectActionBar::MoveLayerDown:
		{
			KTProjectRequest event = KTRequestBuilder::createLayerRequest(scenePos, layerPos, KTProjectRequest::Move, layerPos+1 );
			emit requestTriggered(&event);
			return true;
		}
		break;
	}
	
	return false;
}

bool KTTimeLine::requestSceneAction(int action, int scenePos, const QVariant &arg)
{
	if ( scenePos < 0 )
	{
		scenePos = d->container->currentIndex();
	}
	
	switch(action)
	{
		case KTProjectActionBar::InsertScene:
		{
			KTProjectRequest event = KTRequestBuilder::createSceneRequest( scenePos+1, KTProjectRequest::Add, arg);
			
			emit requestTriggered( &event );
			
			return true;
		}
		break;
		case KTProjectActionBar::RemoveScene:
		{
			KTProjectRequest event = KTRequestBuilder::createSceneRequest(scenePos, KTProjectRequest::Remove, arg);
			
			emit requestTriggered( &event );
			return true;
		}
		break;
		case KTProjectActionBar::MoveSceneUp:
		{
			KTProjectRequest event = KTRequestBuilder::createSceneRequest(scenePos, KTProjectRequest::Move, scenePos+1 );
			emit requestTriggered(&event);
			return true;
		}
		break;
		case KTProjectActionBar::MoveSceneDown:
		{
			KTProjectRequest event = KTRequestBuilder::createSceneRequest(scenePos, KTProjectRequest::Move, scenePos-1 );
			emit requestTriggered(&event);
			return true;
		}
		break;
	}
	
	return false;
}


void KTTimeLine::emitRequestRenameLayer(int layer, const QString &name)
{
	D_FUNCINFO << name;
	int scenePos = d->container->currentIndex();
	
	KTProjectRequest event = KTRequestBuilder::createLayerRequest( scenePos, layer, KTProjectRequest::Rename, name );
	
	emit requestTriggered( &event );
}


