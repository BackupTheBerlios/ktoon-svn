/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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

#include <kcore/kglobal.h>
#include <kcore/kdebug.h>
#include <kgui/kimagebutton.h>
#include <kgui/koptionaldialog.h>
#include <kcore/kconfig.h>

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

KTScenesWidget::KTScenesWidget( QWidget *parent) : KTModuleWidgetBase( parent, "KTScenesWidget"), k(new Private)
{
    #ifdef K_DEBUG
           KINIT;
    #endif

    setWindowTitle( tr( "Sce&nes manager" ) );
    setWindowIcon(QPixmap(THEME_DIR+"/icons/scenes.png"));
    setupButtons();
    setupTableScenes();
}

KTScenesWidget::~KTScenesWidget()
{
    #ifdef K_DEBUG
           KEND;
    #endif
    delete k;
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
    k->tableScenes = new KTScenesList(this);

    KTreeWidgetSearchLine *searcher = new KTreeWidgetSearchLine(this, k->tableScenes);
    searcher->setClickMessage( tr("Filter here..."));

    addChild(searcher);
    addChild( k->tableScenes);

    connect(k->tableScenes, SIGNAL(changeCurrent(QString , int )), this, SLOT(selectScene( QString, int)));
    connect(k->tableScenes, SIGNAL(itemDoubleClicked (QTreeWidgetItem *, int)), this, SLOT(sceneDobleClick(QTreeWidgetItem *, int )));
    connect(k->tableScenes, SIGNAL(itemRenamed(QTreeWidgetItem *)), this, SLOT(emitRequestRenameScene(QTreeWidgetItem *)));
}

void KTScenesWidget::sendEvent(int action)
{
    switch (action) {
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
            default: 
                 break;
    }
}

void KTScenesWidget::selectScene(const QString & name, int index)
{
    KTProjectRequest event = KTRequestBuilder::createSceneRequest( index, KTProjectRequest::Select);
    emit localRequestTriggered( &event );
}

void KTScenesWidget::sceneDobleClick(QTreeWidgetItem * item, int )
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif
}

void KTScenesWidget::emitRequestInsertScene()
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    int index = k->tableScenes->indexCurrentScene() + 1;

    if (index == 0)
        index = k->tableScenes->scenesCount();

    KTProjectRequest event = KTRequestBuilder::createSceneRequest(  index, KTProjectRequest::Add);

    emit requestTriggered( &event );
}

void KTScenesWidget::emitRequestRemoveScene()
{
    KTProjectRequest event = KTRequestBuilder::createSceneRequest( k->tableScenes->indexCurrentScene(), KTProjectRequest::Remove );

    emit requestTriggered( &event );
}

void KTScenesWidget::closeAllScenes()
{
    k->tableScenes->removeAll();
}

void KTScenesWidget::sceneResponse(KTSceneResponse *e)
{
    #ifdef K_DEBUG
           K_FUNCINFOX("scenes");
    #endif

    switch (e->action()) {
            case KTProjectRequest::Add:
             {
               k->tableScenes->insertScene(e->sceneIndex(), e->arg().toString() );
             }
            break;
            case KTProjectRequest::Remove:
             {
               k->tableScenes->removeScene(e->sceneIndex());
             }
            break;
            case KTProjectRequest::Rename:
             {
               k->tableScenes->renameScene(e->sceneIndex(), e->arg().toString() );
             }
            break;
            case KTProjectRequest::Select:
             {
               k->tableScenes->selectScene( e->sceneIndex() );
             }
            break;
            default: 
            break;
    }
}

void KTScenesWidget::emitRequestRenameScene(QTreeWidgetItem *item)
{
}
