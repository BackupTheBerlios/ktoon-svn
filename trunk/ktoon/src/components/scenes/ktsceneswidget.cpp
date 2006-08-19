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

#include "dglobal.h"
#include "ddebug.h"
#include "dimagebutton.h"
#include "doptionaldialog.h"
#include "dconfig.h"

#include <QToolTip>
#include <QMessageBox>
#include <QPixmap>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

#include "ktsceneevent.h"

#include "ktprojectactionbar.h"


//--------------- CONSTRUCTOR --------------------

KTScenesWidget::KTScenesWidget( QWidget *parent) : KTModuleWidgetBase( parent, "KTScenesWidget")
{
	DINIT;
	
	setCaption( tr( "Scenes manager" ) );
	
	setupButtons();
	setupTableScenes();
}

KTScenesWidget::~KTScenesWidget()
{
	DEND;
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
	m_tableScenes = new KTScenesList(this);
	
	DTreeWidgetSearchLine *searcher = new DTreeWidgetSearchLine(this, m_tableScenes);
	searcher->setClickMessage( tr("Filter here..."));
	
	addChild(searcher);
	
	addChild( m_tableScenes);
	connect(m_tableScenes, SIGNAL(changeCurrent(QString , int )), this, SLOT(selectScene( QString, int)));
	
	connect(m_tableScenes, SIGNAL(  itemDoubleClicked ( QTreeWidgetItem *, int )), this, SLOT(sceneDobleClick(QTreeWidgetItem *, int )));
	
	connect(m_tableScenes, SIGNAL(itemRenamed(QTreeWidgetItem *)), this, SLOT(emitRequestRenameScene(QTreeWidgetItem *)));
	
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
	KTSceneEvent event(KTProjectEvent::Select, index, this);
	
	emit eventTriggered( &event );
}

void KTScenesWidget::sceneDobleClick(QTreeWidgetItem * item, int )
{
	
}

void KTScenesWidget::emitRequestInsertScene()
{
	D_FUNCINFO;
	
	int index = m_tableScenes->indexCurrentScene() + 1;
	
	if ( index == 0 )
	{
		index = m_tableScenes->scenesCount();
	}
	
	
	KTSceneEvent event(KTProjectEvent::Add,  index, this);
	
	emit eventTriggered( &event );
}

void KTScenesWidget::emitRequestRemoveScene()
{
	KTSceneEvent event(KTProjectEvent::Remove,  m_tableScenes->indexCurrentScene(), this );
	
	emit eventTriggered( &event );
}


void KTScenesWidget::closeAllScenes()
{
	m_tableScenes->removeAll();
}

void KTScenesWidget::sceneEvent(KTSceneEvent *e)
{
	switch(e->action() )
	{
		case KTProjectEvent::Add:
		{
			m_tableScenes->insertScene(e->sceneIndex(), e->partName());
		}
		break;
		case KTProjectEvent::Remove:
		{
			m_tableScenes->removeScene(e->sceneIndex());
		}
		break;
		case KTProjectEvent::Rename:
		{
			m_tableScenes->renameScene(e->sceneIndex(), e->data().toString() );
		}
		break;
		case KTProjectEvent::Select:
		{
			m_tableScenes->selectScene( e->sceneIndex() );
		}
		break;
		default: break;
	}
}

void KTScenesWidget::emitRequestRenameScene(QTreeWidgetItem *item)
{
	KTSceneEvent event(KTProjectEvent::Rename, m_tableScenes->indexOfTopLevelItem (item), item->text(0));
	
	emit eventTriggered( &event);
}

