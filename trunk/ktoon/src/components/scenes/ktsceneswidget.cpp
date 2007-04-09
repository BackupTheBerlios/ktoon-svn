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

#include "ktsceneswidget.h"

#include <dcore/dglobal.h>
#include <dcore/ddebug.h>
#include <dgui/dimagebutton.h>
#include <dgui/doptionaldialog.h>
#include <dcore/dconfig.h>

#include <QToolTip>
#include <QMessageBox>
#include <QPixmap>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

#include "ktprojectrequest.h"

#include "ktprojectactionbar.h"
#include "ktrequestbuilder.h"

struct KTScenesWidget::Private
{
		QButtonGroup *buttonGroup;
		KTScenesList *tableScenes;
};

KTScenesWidget::KTScenesWidget( QWidget *parent) : KTModuleWidgetBase( parent, "KTScenesWidget"), d(new Private)
{
	DINIT;
	
	setWindowTitle( tr( "Sce&nes manager" ) );
	setWindowIcon(QPixmap(THEME_DIR+"/icons/scenes.png"));
	
	setupButtons();
	setupTableScenes();
}

KTScenesWidget::~KTScenesWidget()
{
	DEND;
	delete d;
}

void KTScenesWidget::setupButtons()
{
	KTProjectActionBar *bar = new KTProjectActionBar(KTProjectActionBar::InsertScene | KTProjectActionBar::RemoveScene );
	
	bar->button(KTProjectActionBar::InsertScene)->setIcon(QIcon(HOME_DIR+"/themes/default/icons/plussign.png" ));
	bar->button(KTProjectActionBar::RemoveScene)->setIcon(QIcon(HOME_DIR+"/themes/default/icons/minussign.png" ));
	
	connect(bar, SIGNAL(actionSelected( int )), this, SLOT(sendEvent(int)));
	
	addChild(bar);
}

void KTScenesWidget::setupTableScenes()
{
	d->tableScenes = new KTScenesList(this);
	
	DTreeWidgetSearchLine *searcher = new DTreeWidgetSearchLine(this, d->tableScenes);
	searcher->setClickMessage( tr("Filter here..."));
	
	addChild(searcher);
	
	addChild( d->tableScenes);
	connect(d->tableScenes, SIGNAL(changeCurrent(QString , int )), this, SLOT(selectScene( QString, int)));
	
	connect(d->tableScenes, SIGNAL(  itemDoubleClicked ( QTreeWidgetItem *, int )), this, SLOT(sceneDobleClick(QTreeWidgetItem *, int )));
	
	connect(d->tableScenes, SIGNAL(itemRenamed(QTreeWidgetItem *)), this, SLOT(emitRequestRenameScene(QTreeWidgetItem *)));
	
}

void KTScenesWidget::sendEvent(int action)
{
	switch(action)
	{
		case KTProjectActionBar::InsertScene:
		{
			emitRequestInsertScene();
		}
		break;
		case KTProjectActionBar::RemoveScene:
		{
			emitRequestRemoveScene();
		}
		break;
		default: break;
	}
}

void KTScenesWidget::selectScene(const QString & name, int index)
{
	KTProjectRequest event = KTRequestBuilder::createSceneRequest( index, KTProjectRequest::Select);
	
	emit localRequestTriggered( &event );
}

void KTScenesWidget::sceneDobleClick(QTreeWidgetItem * item, int )
{
	D_FUNCINFO;
}

void KTScenesWidget::emitRequestInsertScene()
{
	D_FUNCINFO;
	
	int index = d->tableScenes->indexCurrentScene() + 1;
	
	if ( index == 0 )
	{
		index = d->tableScenes->scenesCount();
	}
	
	
	KTProjectRequest event = KTRequestBuilder::createSceneRequest(  index, KTProjectRequest::Add);
	
	emit requestTriggered( &event );
}

void KTScenesWidget::emitRequestRemoveScene()
{
	KTProjectRequest event = KTRequestBuilder::createSceneRequest( d->tableScenes->indexCurrentScene(), KTProjectRequest::Remove );
	
	emit requestTriggered( &event );
}


void KTScenesWidget::closeAllScenes()
{
	d->tableScenes->removeAll();
}

void KTScenesWidget::sceneResponse(KTSceneResponse *e)
{
	D_FUNCINFOX("scenes");
	switch(e->action() )
	{
		case KTProjectRequest::Add:
		{
			d->tableScenes->insertScene(e->sceneIndex(), e->arg().toString() );
		}
		break;
		case KTProjectRequest::Remove:
		{
			d->tableScenes->removeScene(e->sceneIndex());
		}
		break;
		case KTProjectRequest::Rename:
		{
			d->tableScenes->renameScene(e->sceneIndex(), e->arg().toString() );
		}
		break;
		case KTProjectRequest::Select:
		{
			d->tableScenes->selectScene( e->sceneIndex() );
		}
		break;
		default: break;
	}
}

void KTScenesWidget::emitRequestRenameScene(QTreeWidgetItem *item)
{
// 	KTProjectRequest event(KTProjectRequest::Rename, d->tableScenes->indexOfTopLevelItem (item), item->text(0));
	
// 	emit requestTriggered( &event);
}

