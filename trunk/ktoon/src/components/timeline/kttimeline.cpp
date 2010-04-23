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

#include <QList>
#include <QHeaderView>

#include "kttimeline.h"
// KOM
#include <kgui/kapplication.h>
#include <kcore/kdebug.h>

#include "ktprojectrequest.h"
#include "ktlibraryobject.h"

#include "ktframestable.h"
#include "ktlayermanager.h"

#include "ktprojectactionbar.h"

#include "ktrequestbuilder.h"

#include "ktlibrary.h"

#define RETURN_IF_NOT_LIBRARY if (!k->library) return;

struct KTTimeLine::Private
{
    Private() : container(0), actionBar(0), selectedLayer(0), library(0) {}
    
    KTabWidget *container;
    KTProjectActionBar *actionBar;
    int selectedLayer; 
    const KTLibrary *library;
};

KTTimeLine::KTTimeLine(QWidget *parent) : KTModuleWidgetBase(parent, "KTTimeLine"), k(new Private)
{
    #ifdef K_DEBUG
        KINIT;
    #endif
    
    setWindowTitle(tr("Time Line"));
    setWindowIcon(QPixmap(THEME_DIR + "icons/time_line.png"));
    
    k->actionBar = new KTProjectActionBar(KTProjectActionBar::InsertLayer |
                        KTProjectActionBar::RemoveLayer |
                        // KTProjectActionBar::MoveLayerUp |
                        // KTProjectActionBar::MoveLayerDown |
                        KTProjectActionBar::Separator |
                        KTProjectActionBar::InsertFrame |
                        KTProjectActionBar::RemoveFrame |
                        KTProjectActionBar::MoveFrameUp |
                        KTProjectActionBar::MoveFrameDown |
                        KTProjectActionBar::LockFrame);
   
    addChild(k->actionBar, Qt::AlignCenter);
    
    k->container = new KTabWidget(this);
    addChild(k->container);
    
    connect(k->actionBar, SIGNAL(actionSelected(int)), this, SLOT(requestCommand(int)));
}

KTTimeLine::~KTTimeLine()
{
    #ifdef K_DEBUG
        KEND;
    #endif
    delete k;
}

KTLayerManager *KTTimeLine::layerManager(int sceneIndex)
{
    QSplitter *splitter = qobject_cast<QSplitter *>(k->container->widget(sceneIndex));

    if (splitter)
        return qobject_cast<KTLayerManager *>(splitter->widget(0));
    
    return 0;
}

KTFramesTable *KTTimeLine::framesTable(int sceneIndex)
{
    QSplitter *splitter = qobject_cast<QSplitter *>(k->container->widget(sceneIndex));
    
    if (splitter)
        return qobject_cast<KTFramesTable *>(splitter->widget(1));
    
    return 0;
}

void KTTimeLine::insertScene(int position, const QString &name)
{
    if (position < 0 || position > k->container->count())
        return;
    
    QSplitter *splitter = new QSplitter(k->container);
    
    KTLayerManager *layerManager = new KTLayerManager(splitter);
    
    layerManager->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    
    splitter->addWidget(layerManager);

    KTFramesTable *framesTable = new KTFramesTable(splitter);
    splitter->addWidget(framesTable);
    
    framesTable->setItemSize(10, 20);
    layerManager->setRowHeight(20);
    
    connect(framesTable, SIGNAL(frameRequest(int, int, int, int, const QVariant&)), this, 
        SLOT(requestFrameAction(int, int, int, int, const QVariant&)));
    
    connect(layerManager->verticalScrollBar(), SIGNAL(valueChanged (int)), framesTable->verticalScrollBar(), SLOT(setValue(int)));
    connect(layerManager, SIGNAL(itemSelectionChanged()), this, SLOT(emitSelectionSignal()));

    connect(framesTable->verticalScrollBar(), SIGNAL(valueChanged (int)), layerManager->verticalScrollBar(), 
        SLOT(setValue(int)));
    
    connect(layerManager, SIGNAL(requestRenameEvent(int, const QString&)), this, SLOT(emitRequestRenameLayer(int,
        const QString &))); // FIXME
    
    k->container->insertTab(position, splitter, name);
}

void KTTimeLine::removeScene(int position)
{
    if (position >= 0 && position < k->container->count()) {
        QWidget *w = k->container->widget(position);
        // k->container->removeWidget(w);
        k->container->removeTab(position);
        
        delete w;
    }
}

void KTTimeLine::closeAllScenes()
{
    while (k->container->currentWidget())
           delete k->container->currentWidget();
}

void KTTimeLine::setLibrary(const KTLibrary *library)
{
    k->library = library;
}

void KTTimeLine::sceneResponse(KTSceneResponse *response)
{
    #ifdef K_DEBUG
        K_FUNCINFOX("timeline");
    #endif

    switch (response->action()) {
            case KTProjectRequest::Add:
            {
                 insertScene(response->sceneIndex(), response->arg().toString());
            }
            break;
            case KTProjectRequest::Remove:
            {
                 removeScene(response->sceneIndex());
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

void KTTimeLine::layerResponse(KTLayerResponse *response)
{
    switch (response->action()) {
            case KTProjectRequest::Add:
            {
            KTLayerManager *layerManager = this->layerManager(response->sceneIndex());
            if (layerManager)
                layerManager->insertLayer(response->layerIndex(), response->arg().toString());
            
            KTFramesTable *framesTable = this->framesTable(response->sceneIndex());
            if (framesTable)
                framesTable->insertLayer(response->layerIndex(), response->arg().toString());
            }
            break;
            case KTProjectRequest::Remove:
            {
                 KTLayerManager *layerManager = this->layerManager(response->sceneIndex());
                 if (layerManager)
                     layerManager->removeLayer(response->layerIndex());
            
                 KTFramesTable *framesTable = this->framesTable(response->sceneIndex());
                 if (framesTable)
                     framesTable->removeLayer(response->layerIndex());
            }
            break;
            case KTProjectRequest::Move:
            {
                 KTLayerManager *layerManager = this->layerManager(response->sceneIndex());
                 if (layerManager)
                     layerManager->moveLayer(response->layerIndex(), response->arg().toInt());
            
                 KTFramesTable *framesTable = this->framesTable(response->sceneIndex());
                 if (framesTable)
                     framesTable->moveLayer(response->layerIndex(), response->arg().toInt());
            }
            break;
            case KTProjectRequest::Lock:
            {
                 KTLayerManager *layerManager = this->layerManager(response->sceneIndex());
            
                 if (layerManager)
                     layerManager->lockLayer(response->layerIndex(), response->arg().toBool());
            }
            break;
            case KTProjectRequest::Rename:
            {
                 KTLayerManager *layerManager = this->layerManager(response->sceneIndex());
            
                 if (layerManager)
                     layerManager->renameLayer(response->layerIndex(), response->arg().toString());
            }
            break;
    }
}

void KTTimeLine::frameResponse(KTFrameResponse *response)
{
    switch (response->action()) {
            case KTProjectRequest::Add:
            {
                 KTFramesTable *framesTable = this->framesTable(response->sceneIndex());

                 if (framesTable)
                     framesTable->insertFrame(response->layerIndex(), response->arg().toString());
            }
            break;
            case KTProjectRequest::Remove:
            {
                 KTFramesTable *framesTable = this->framesTable(response->sceneIndex());
            
                 if (framesTable)
                     framesTable->removeFrame(response->layerIndex(), response->frameIndex());
            }
            break;
            case KTProjectRequest::Move:
            {
            }
            break;
            case KTProjectRequest::Lock:
            {
                 KTFramesTable *framesTable = this->framesTable(response->sceneIndex());

                 if (framesTable)
                     framesTable->lockFrame(response->layerIndex(), response->frameIndex(), response->arg().toBool());
            }
            break;
            case KTProjectRequest::Rename:
            {
            }
            break;
            case KTProjectRequest::Select:
            {
                 kFatal() << "KTTimeLine::frameResponse - Layer Index: " << response->layerIndex();
                 if (k->selectedLayer != response->layerIndex()) {
                     // layerManager(response->sceneIndex())->setCurrentCell(response->layerIndex(), 0);
                     layerManager(response->sceneIndex())->setCurrentCell(0, 0);
                 }
            }
            break;

    }
}

void KTTimeLine::libraryResponse(KTLibraryResponse *response)
{
    #ifdef K_DEBUG
        K_FUNCINFOX("timeline") << response->symbolType();    
    #endif

    if (response->action() == KTProjectRequest::AddSymbolToProject) {
        switch (response->symbolType()) {
                case KTLibraryObject::Sound:
                {
                     KTLayerManager *layerManager = this->layerManager(response->sceneIndex());
                     if (layerManager) {
                         layerManager->insertSoundLayer(response->layerIndex()+1, 
                                                        response->arg().toString());
                    
                         KTFramesTable *framesTable = this->framesTable(response->sceneIndex());
                         if (framesTable) {
                             framesTable->insertSoundLayer(response->layerIndex()+1, 
                                                           response->arg().toString());
                             framesTable->insertFrame(response->layerIndex()+1,"");
                         }
                     }
                }
                break;
        };
    }
}

void KTTimeLine::requestCommand(int action)
{
    int scenePos = k->container->currentIndex();
    int layerPos = -1;
    int framePos = -1;
    
    if (scenePos >= 0) {
        // layerPos = layerManager(scenePos)->verticalHeader()->visualIndex(layerManager(scenePos)->currentRow());
        layerPos = layerManager(scenePos)->rowCount();
        framePos = framesTable(scenePos)->lastFrameByLayer(layerPos);
    }
    
    if (!requestFrameAction(action, framePos, layerPos, scenePos)) {
        if (!requestLayerAction(action, layerPos, scenePos)) {
            if (!requestSceneAction(action, scenePos)) {
                #ifdef K_DEBUG
                    kFatal("timeline") << "Can't handle action";
                #endif
            }
        }
    }
    
}

bool KTTimeLine::requestFrameAction(int action, int framePos, int layerPos, int scenePos, const QVariant &arg)
{
    kFatal() << "KTTimeLine::requestFrameAction - hey!";

    if (scenePos < 0)
        scenePos = k->container->currentIndex();
    
    if (scenePos >= 0) {
        if (layerPos < 0)
            layerPos = layerManager(scenePos)->verticalHeader()->visualIndex(
                                    layerManager(scenePos)->currentRow());
        
        if (framePos < 0)
            framePos = framesTable(scenePos)->lastFrameByLayer(layerPos);
    }
    
    switch (action) {
            case KTProjectActionBar::InsertFrame:
            {
                 KTProjectRequest event = KTRequestBuilder::createFrameRequest(scenePos, layerPos, framePos + 1,
                                          KTProjectRequest::Add, arg);
                 emit requestTriggered(&event);
            
                 return true;
            }
            break;
            case KTProjectActionBar::RemoveFrame:
            {
                 KTProjectRequest event = KTRequestBuilder::createFrameRequest(scenePos, layerPos, framePos,
                                          KTProjectRequest::Remove, arg);
                 emit requestTriggered(&event);

                 return true;
            }
            break;
            case KTProjectActionBar::MoveFrameUp:
            {
                 KTProjectRequest event = KTRequestBuilder::createFrameRequest(scenePos, layerPos, framePos,
                                          KTProjectRequest::Move, framePos-1);
                 emit requestTriggered(&event);

                 return true;
            }
            break;
            case KTProjectActionBar::MoveFrameDown:
            {
                 KTProjectRequest event = KTRequestBuilder::createFrameRequest(scenePos, layerPos, framePos,
                                          KTProjectRequest::Move, framePos+1);
                 emit requestTriggered(&event);

                 return true;
            }
            break;
        
            case KTProjectRequest::Select:
            {
                 kFatal() << "KTTimeLine::requestFrameAction: " << scenePos << " / " << layerPos << " / " << framePos;

                 KTProjectRequest event = KTRequestBuilder::createFrameRequest(scenePos, layerPos, framePos,
                                          KTProjectRequest::Select, arg);
                 emit localRequestTriggered(&event);

                 return true;
            }
            break;
    }
    
    return false;
}

bool KTTimeLine::requestLayerAction(int action, int layerPos, int scenePos, const QVariant &arg)
{
    if (scenePos < 0)
        scenePos = k->container->currentIndex();
    
    if (scenePos >= 0) {
        if (layerPos < 0)
            layerPos = layerManager(scenePos)->verticalHeader()->visualIndex(
                                    layerManager(scenePos)->currentRow());
    }
    
    switch (action) {
            case KTProjectActionBar::InsertLayer:
            {
                 KTProjectRequest event = KTRequestBuilder::createLayerRequest(scenePos, layerPos,
                                          KTProjectRequest::Add, arg);
                 emit requestTriggered(&event);

                 return true;
            }
            break;
            case KTProjectActionBar::RemoveLayer:
            {
                 KTProjectRequest event = KTRequestBuilder::createLayerRequest(scenePos, layerPos, 
                                          KTProjectRequest::Remove, arg);
            
                 emit requestTriggered(&event);

                 return true;
            }
            break;
            case KTProjectActionBar::MoveLayerUp:
            {
                 KTProjectRequest event = KTRequestBuilder::createLayerRequest(scenePos, layerPos, 
                                          KTProjectRequest::Move, layerPos - 1);
                 emit requestTriggered(&event);

                 return true;
            }
            break;
            case KTProjectActionBar::MoveLayerDown:
            {
                 KTProjectRequest event = KTRequestBuilder::createLayerRequest(scenePos, layerPos, 
                                          KTProjectRequest::Move, layerPos + 1);
                 emit requestTriggered(&event);
                 return true;
            }
            break;
    }
    
    return false;
}

bool KTTimeLine::requestSceneAction(int action, int scenePos, const QVariant &arg)
{
    if (scenePos < 0)
        scenePos = k->container->currentIndex();
    
    switch (action) {
            case KTProjectActionBar::InsertScene:
            {
                 KTProjectRequest event = KTRequestBuilder::createSceneRequest(scenePos + 1, 
                                           KTProjectRequest::Add, arg);
            
                 emit requestTriggered(&event);
            
                 return true;
            }
            break;
            case KTProjectActionBar::RemoveScene:
            {
                 KTProjectRequest event = KTRequestBuilder::createSceneRequest(scenePos, 
                                          KTProjectRequest::Remove, arg);
            
                 emit requestTriggered(&event);

                 return true;
            }
            break;
            case KTProjectActionBar::MoveSceneUp:
            {
                 KTProjectRequest event = KTRequestBuilder::createSceneRequest(scenePos, KTProjectRequest::Move,
                                          scenePos + 1);
                 emit requestTriggered(&event);

                 return true;
            }
            break;
            case KTProjectActionBar::MoveSceneDown:
            {
            KTProjectRequest event = KTRequestBuilder::createSceneRequest(scenePos, KTProjectRequest::Move,
                                            scenePos - 1);
            emit requestTriggered(&event);
            return true;
            }
            break;
    }
    
    return false;
}


void KTTimeLine::emitRequestRenameLayer(int layer, const QString &name)
{
    #ifdef K_DEBUG
        K_FUNCINFO << name;
    #endif
    int scenePos = k->container->currentIndex();
    
    KTProjectRequest event = KTRequestBuilder::createLayerRequest(scenePos, layer, KTProjectRequest::Rename, name);
    
    emit requestTriggered(&event);
}

void KTTimeLine::emitSelectionSignal()
{
    kFatal() << "KTLayerManager::emitSelectionSignal() : HERE WE GOOO!";
    int scenePos = k->container->currentIndex();
    int layerPos = layerManager(scenePos)->currentRow();
    k->selectedLayer = layerPos;
    int frame = framesTable(scenePos)->lastFrameByLayer(layerPos);
    requestFrameAction(KTProjectRequest::Select, scenePos, layerPos, frame);
}
