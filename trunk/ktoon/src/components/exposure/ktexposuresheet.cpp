/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   License:                                                              *
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
    KTSceneTabWidget *scenes;
    KTExposureTable *currentTable;
    KTProjectActionBar *actionBar;
    QMenu *menu;
    QString nameCopyFrame;
};

KTExposureSheet::KTExposureSheet(QWidget *parent) : KTModuleWidgetBase(parent, "Exposure Sheet"), k(new Private)
{
    #ifdef K_DEBUG
           KINIT;
    #endif

    k->currentTable = 0;
    setWindowTitle(tr("Exposure Sheet"));
    setWindowIcon(QPixmap(THEME_DIR + "icons/exposure_sheet.png"));

    k->actionBar = new KTProjectActionBar(QString("Exposure"), KTProjectActionBar::InsertLayer |
                        KTProjectActionBar::RemoveLayer |
                        KTProjectActionBar::Separator |
                        KTProjectActionBar::InsertFrame |
                        KTProjectActionBar::RemoveFrame |
                        KTProjectActionBar::MoveFrameUp |
                        KTProjectActionBar::MoveFrameDown | 
                        KTProjectActionBar::LockFrame);

    connect(k->actionBar, SIGNAL(actionSelected(int)), this, SLOT(applyAction(int)));
    addChild(k->actionBar, Qt::AlignCenter);

    k->scenes = new KTSceneTabWidget(this);
    connect(k->scenes->TabWidget(), SIGNAL(currentChanged(int)), this, SLOT(emitRequestChangeScene(int)));
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
    //k->menu->addAction(tr("Insert layer"))->setData(KTProjectActionBar::InsertLayer);
    //k->menu->addAction(tr("Remove layer"))->setData(KTProjectActionBar::RemoveLayer);
    k->menu->addAction(tr("Insert frame"))->setData(KTProjectActionBar::InsertFrame);
    k->menu->addAction(tr("Remove frame"))->setData(KTProjectActionBar::RemoveFrame);
    k->menu->addAction(tr("Lock frame"))->setData(KTProjectActionBar::LockFrame);

    k->menu->addAction(tr("Copy frame"), this, SLOT(emitRequestCopyCurrentFrame()));
    k->menu->addAction(tr("Paste in frame"), this, SLOT(emitRequestPasteInCurrentFrame()));
    k->menu->addAction(tr("Expand frame"), this, SLOT(emitRequestExpandCurrentFrame()));

    connect(k->menu,  SIGNAL(triggered(QAction *)), this, SLOT(actionTiggered(QAction*)));
}

void KTExposureSheet::addScene(int index, const QString &name)
{
    #ifdef K_DEBUG
           K_FUNCINFO << " index: " << index << " name: " << name;
    #endif

    KTExposureTable *newScene = new KTExposureTable;
    newScene->setMenu(k->menu);

    connect(newScene, SIGNAL(requestSetUsedFrame(int, int)), 
                             this, SLOT(insertItem(int, int)));

    connect(newScene, SIGNAL(requestRenameFrame(int, int,const QString &)), 
                             this, SLOT(renameFrame(int, int, const QString &)));

    connect(newScene, SIGNAL(requestSelectFrame(int, int)), SLOT(selectFrame(int, int)));

    connect(newScene, SIGNAL(requestRenameLayer(int, const QString &)), 
                             this, SLOT(renameLayer(int, const QString &)));

    connect(newScene, SIGNAL(requestMoveLayer(int,int)), this, SLOT(moveLayer(int, int)));

    connect(newScene, SIGNAL(requestChangeVisibilityLayer(int , bool)),
                             this, SLOT(changeVisibilityLayer(int, bool)));

    k->scenes->addScene(index, name, newScene);
}

void KTExposureSheet::renameScene(int index, const QString &name)
{
    k->scenes->TabWidget()->setTabText(index, name);
}

void KTExposureSheet::applyAction(int action)
{
    #ifdef K_DEBUG
           K_FUNCINFO<< "action: " << action;
    #endif

     k->currentTable = k->scenes->getCurrentTable();

    if (k->currentTable == 0) {
        #ifdef K_DEBUG
               kFatal() << "KTExposureSheet::applyAction: No layer view!!" << endl;
        #endif
        return;
    }

    switch (action) {
            case KTProjectActionBar::InsertLayer:
               {
                 int layer = k->currentTable->columnCount();

                 KTProjectRequest event = KTRequestBuilder::createLayerRequest(k->scenes->currentIndex(),
                                                                               layer, KTProjectRequest::Add);
                 emit requestTriggered(&event);

                 int framesNum = k->currentTable->usedFrames(k->currentTable->currentColumn());

                 for (int i=0;i < framesNum;i++) {
                      event = KTRequestBuilder::createFrameRequest(k->scenes->currentIndex(), layer, i, 
                                                                   KTProjectRequest::Add, QString());
                      emit requestTriggered(&event);
                 }
               }
               break;
            case KTProjectActionBar::RemoveLayer:
               {
                 KTProjectRequest event = KTRequestBuilder::createLayerRequest(k->scenes->currentIndex(), 
                                                                               k->currentTable->currentLayer(), 
                                                                               KTProjectRequest::Remove);
                 emit requestTriggered(&event);
               }
               break;
            case KTProjectActionBar::InsertFrame:
               {
                 int usedFrames = k->currentTable->usedFrames(k->currentTable->currentColumn());

                 if (k->currentTable->currentRow() > usedFrames) {
                     for (int layer=0; layer < k->currentTable->layersTotal(); layer++) { 
                          int finish = k->currentTable->currentFrame() + 1;
                          for (int frame = usedFrames; frame <= finish; frame++)
                               insertItem(layer, frame);
                     }
                 } else {
                     int border = k->currentTable->framesTotal();
                     for (int layer=0; layer < k->currentTable->layersTotal(); layer++)
                          insertItem(layer, border);

                     /*
                     int row = k->currentTable->currentFrame() + 1;
                     int initLayer = k->currentTable->currentLayer();

                     for (int layer=0; layer < k->currentTable->layersTotal(); layer++)
                          insertItem(layer, row);

                     if (k->currentTable->layersTotal() > 1)
                         selectFrame(initLayer, row);           
                     */
                 }
               }
               break;
            case KTProjectActionBar::RemoveFrame:
               {
                 KTProjectRequest event = KTRequestBuilder::createFrameRequest(k->scenes->currentIndex(), 
                                          k->currentTable->currentLayer(), k->currentTable->currentFrame(),
                                          KTProjectRequest::Remove);
                 emit requestTriggered(&event);
               }
               break;
            case KTProjectActionBar::MoveFrameUp:
               {
                 KTProjectRequest event = KTRequestBuilder::createFrameRequest(k->scenes->currentIndex(), 
                 k->currentTable->currentLayer(), k->currentTable->currentFrame(),
                 KTProjectRequest::Move, k->currentTable->currentFrame()-1);
                 emit requestTriggered(&event);
               }
               break;
            case KTProjectActionBar::MoveFrameDown:
               {
                 KTProjectRequest event = KTRequestBuilder::createFrameRequest(k->scenes->currentIndex(), 
                                          k->currentTable->currentLayer(), k->currentTable->currentFrame(),
                                          KTProjectRequest::Move, k->currentTable->currentFrame()+1);
                 emit requestTriggered(&event);
               }
               break;
            case KTProjectActionBar::LockFrame:
               {
                 bool locked = k->currentTable->frameIsLocked(k->currentTable->currentColumn(), 
                               k->currentTable->currentFrame());

                 KTProjectRequest event = KTRequestBuilder::createFrameRequest (k->scenes->currentIndex(), 
                                          k->currentTable->currentLayer(), k->currentTable->currentFrame(),
                                          KTProjectRequest::Lock, !locked);
                                          emit requestTriggered(&event);
               }
               break;
    }
}

void KTExposureSheet::setScene(int index)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    //if (k->scenes->TabWidget()->count() >= index) {

    if (k->scenes->count() >= index) {
        k->scenes->blockSignals(true);
        k->scenes->TabWidget()->setCurrentIndex(index);		
        k->currentTable = k->scenes->getCurrentTable();
        k->scenes->blockSignals(false);
    }
}

void KTExposureSheet::emitRequestChangeScene(int index)
{
    if (k->scenes->count() > 1) {
        KTProjectRequest request = KTRequestBuilder::createSceneRequest(index, KTProjectRequest::Select);
        emit localRequestTriggered(&request);
    }
}

void KTExposureSheet::emitRequestCopyCurrentFrame()
{
    KTProjectRequest request = KTRequestBuilder::createFrameRequest(k->scenes->currentIndex(), 
                                                                    k->currentTable->currentLayer(), 
                                                                    k->currentTable->currentFrame(), 
                                                                    KTProjectRequest::Copy);
    emit localRequestTriggered(&request);
}

void KTExposureSheet::emitRequestPasteInCurrentFrame()
{
    if (k->nameCopyFrame.isEmpty())
        return;

    if (k->currentTable->usedFrames(k->currentTable->currentLayer()) <= k->currentTable->currentRow()) {
        for(int i = k->currentTable->usedFrames(k->currentTable->currentLayer()); 
            i <= k->currentTable->currentRow(); i++) {
            insertItem(k->currentTable->currentLayer(), i);

            KTProjectRequest request = KTRequestBuilder::createFrameRequest(k->scenes->currentIndex(), 
                                                         k->currentTable->currentLayer(), i, 
                                                         KTProjectRequest::Paste);
            emit localRequestTriggered(&request);
        }
    } else {
            KTProjectRequest request = KTRequestBuilder::createFrameRequest(k->scenes->currentIndex(), 
                                                         k->currentTable->currentLayer(), 
                                                         k->currentTable->currentFrame(),
                                                         KTProjectRequest::Paste);
            emit localRequestTriggered(&request);
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
    emit requestTriggered(&request);
}

void KTExposureSheet::insertItem(int indexLayer, int indexFrame)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    KTProjectRequest request = KTRequestBuilder::createFrameRequest(k->scenes->currentIndex(), 
                                                 indexLayer, indexFrame, KTProjectRequest::Add);
    emit requestTriggered(&request);
}

void KTExposureSheet::renameFrame(int indexLayer, int indexFrame, const QString & name)
{
    KTProjectRequest event = KTRequestBuilder::createFrameRequest(k->scenes->currentIndex() , indexLayer, indexFrame,
                                               KTProjectRequest::Rename, name);
    emit requestTriggered(&event);
}

void KTExposureSheet::selectFrame(int indexLayer, int indexFrame)
{
    KTProjectRequest request = KTRequestBuilder::createFrameRequest(k->scenes->currentIndex() , indexLayer, 
                                                 indexFrame, KTProjectRequest::Select, "1");
    emit requestTriggered(&request);

    //emit localRequestTriggered(&request);
}

void KTExposureSheet::changeVisibilityLayer(int visualIndexLayer, bool visibility)
{
    KTProjectRequest event = KTRequestBuilder::createLayerRequest(k->scenes->currentIndex(), visualIndexLayer,
                                               KTProjectRequest::View, visibility);
    emit requestTriggered(&event);
}

void KTExposureSheet::renameLayer(int indexLayer, const QString & name)
{
    KTProjectRequest event = KTRequestBuilder::createLayerRequest(k->scenes->currentIndex(), indexLayer,
                                               KTProjectRequest::Rename, name);
    emit requestTriggered(&event);
}

void KTExposureSheet::moveLayer(int oldIndex, int newIndex)
{
    KTProjectRequest event = KTRequestBuilder::createLayerRequest(k->scenes->currentIndex(), oldIndex,
                                               KTProjectRequest::Move, newIndex);
    emit requestTriggered(&event);
}

void KTExposureSheet::actionTiggered(QAction *action)
{
    bool ok;
    int id = action->data().toInt(&ok);
    if (ok)
        applyAction(id);
}

void KTExposureSheet::closeAllScenes()
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    k->scenes->blockSignals(true);

    delete k->currentTable;
    k->scenes->removeAllTabs();
    k->currentTable = 0;

    k->scenes->blockSignals(false);
}

void KTExposureSheet::sceneResponse(KTSceneResponse *e)
{
    #ifdef K_DEBUG
           K_FUNCINFOX("exposure");
    #endif

    switch(e->action()) {
           case KTProjectRequest::Add:
            {
                addScene(e->sceneIndex(), e->arg().toString());
            }
           break;
           case KTProjectRequest::Remove:
            {
                k->scenes->blockSignals(true);
                //QWidget * widget = k->scenes->getTable(e->sceneIndex());
                k->scenes->TabWidget()->removeTab(e->sceneIndex());
                //delete widget;
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
                setScene(e->sceneIndex());
            }
           break;
    }
}

void KTExposureSheet::layerResponse(KTLayerResponse *e)
{
    KTExposureTable *scene = k->scenes->getTable(e->sceneIndex());

    if (scene) {
        switch (e->action()) {
                case KTProjectRequest::Add:
                 {
                     scene->insertLayer(e->layerIndex(), e->arg().toString());
                 }
                break;
                case KTProjectRequest::Remove:
                 {
                     scene->removeLayer(e->layerIndex());
                 }
                break;
                case KTProjectRequest::Move:
                 {
                     scene->moveLayer(e->layerIndex(), e->arg().toInt());
                 }
                break;
                case KTProjectRequest::Rename:
                 {
                     scene->setLayerName(e->layerIndex(), e->arg().toString());
                 }
                break;
                case KTProjectRequest::Lock:
                 {
                     scene->setLockLayer(e->layerIndex(), e->arg().toBool());
                 }
                break;
                case KTProjectRequest::Select:
                 {
                     setScene(e->sceneIndex());
                     scene->blockSignals(true);
                     scene->selectFrame(e->layerIndex(), 0);
                     scene->blockSignals(false);
                 }
                case KTProjectRequest::View:
                 {
                     scene->setVisibilityChanged(e->layerIndex(), e->arg().toBool());
                 }
                break;
                default:
                     kFatal() << "KTExposureSheet::layerResponse - Layer option undefined! -> " << e->action();
                break;
        }
    } else {
        kFatal() << "KTExposureSheet::layerResponse -> Scene index invalid: " << e->sceneIndex();
    }
}

void KTExposureSheet::frameResponse(KTFrameResponse *e)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    KTExposureTable *scene = k->scenes->getTable(e->sceneIndex());

    if (scene) {
        switch (e->action()) {
                case KTProjectRequest::Add:
                 {
                     scene->setUseFrame(e->layerIndex(), e->frameIndex(), e->arg().toString(), e->external());

                     if (e->layerIndex() == 0 && e->frameIndex() == 0) {
                         setScene(e->sceneIndex());
                         scene->selectFrame(e->layerIndex(), e->frameIndex());
                     }
                 }
                break;
                case KTProjectRequest::Remove:
                 {
                     scene->removeFrame(e->layerIndex(), e->frameIndex());
                 }
                break;
                case KTProjectRequest::Move:
                 {
                     scene->moveFrame(e->layerIndex(), e->frameIndex(), e->layerIndex(), e->arg().toInt(), 
                                      e->external());
                 }
                break;
                case KTProjectRequest::Lock:
                 {
                     scene->setLockFrame(e->layerIndex(), e->frameIndex(),  e->arg().toBool());
                 }
                break;
                case KTProjectRequest::Rename:
                 {
                     scene->setFrameName(e->layerIndex(), e->frameIndex(), e->arg().toString());
                 }
                break;
                case KTProjectRequest::Select:
                 {
                     setScene(e->sceneIndex());
                     scene->blockSignals(true);
                     scene->selectFrame(e->layerIndex(), e->frameIndex());
                     scene->blockSignals(false);
                 }
                break;
                case KTProjectRequest::Expand:
                 {
                     for(int i = 0; i < e->arg().toInt(); i++)
                         scene->setUseFrame(e->layerIndex(), e->frameIndex()+i+1, 
                                            scene->frameName(e->layerIndex(), e->frameIndex()), 
                                            e->external());
                 }
                break;
                case KTProjectRequest::Copy:
                 {
                     k->nameCopyFrame = scene->frameName(e->layerIndex(), e->frameIndex());
                 }
                break;
                case KTProjectRequest::Paste:
                 {
                     if (e->frameIndex() >= scene->usedFrames(e->layerIndex())) {
                         if (e->mode() == KTProjectResponse::Undo) {
                             if (e->arg().toString().isEmpty())
                                 scene->removeFrame(e->layerIndex(), e->frameIndex());
                         } else {
                                 scene->setUseFrame(e->layerIndex(), e->frameIndex(), 
                                                     k->nameCopyFrame + "- copy", e->external());
                         }
                     }
                 }
                break;
        }
    } else {
        kFatal() << "KTExposureSheet::frameResponse -> Scene index invalid: " << e->sceneIndex();
    }
}
