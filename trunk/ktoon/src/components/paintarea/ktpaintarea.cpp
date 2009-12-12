/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#include "ktpaintarea.h"

#include <kcore/kdebug.h>

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QPolygon>
#include <QApplication>
#include <QTimer>
#include <QStyleOptionGraphicsItem>
#include <QClipboard>
#include <QMenu>

#include "ktbrushmanager.h"
#include "ktinputdeviceinformation.h"

#include "ktprojectresponse.h"

#include "ktpaintareaevent.h"
#include "ktpaintarearotator.h"
#include "ktimagedevice.h"
#include "ktgraphicsscene.h"

#include "kcore/kconfig.h"
#include "kgui/kapplication.h"

#include "kttextitem.h"

#include "librarydialog.h"
#include "ktlibraryobject.h"
#include "ktrequestbuilder.h"

#include <kgui/kosd.h>

#include "ktscene.h"
#include "ktproject.h"

/**
 * This class defines all the behavior for the main paint area at ilustration module
 * Here is where all events about the paint area are processed.
 * @author David Cuadrado <krawek@toonka.com>
*/

struct KTPaintArea::Private
{
    const KTProject *project;
    int currentSceneIndex;
    QStringList copiesXml;
};

KTPaintArea::KTPaintArea(const KTProject *project, QWidget * parent) : KTPaintAreaBase(parent), k(new Private)
{
    K_FUNCINFO;

    k->project = project;
    k->currentSceneIndex = 0;

    setCurrentScene(0);

    if (graphicsScene()->scene()) {
        graphicsScene()->setCurrentFrame(0, 0);
    }
}

KTPaintArea::~KTPaintArea()
{
    delete k;
}

void KTPaintArea::setCurrentScene(int index)
{
    #ifdef K_DEBUG
           K_FUNCINFOX("paintarea") << index;
    #endif

    KTScene *scene = k->project->scene(index);
    if (scene) {
        k->currentSceneIndex = index;
        graphicsScene()->setCurrentScene(scene);
    } else {
        setDragMode(QGraphicsView::NoDrag);
        k->currentSceneIndex = -1;
        graphicsScene()->setCurrentScene(0);
    }
}

void KTPaintArea::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::RightButton) {
        if (QGraphicsItem *item = scene()->itemAt( mapToScene(event->pos()))) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            item->setSelected(true);
        }

        QMenu *menu = new QMenu(tr("Drawing area"));
        menu->addAction(kApp->findGlobalAction("undo"));
        menu->addAction(kApp->findGlobalAction("redo"));
        menu->addSeparator();

        QAction *cut = menu->addAction(tr("Cut"), this, SLOT(cutItems()));
        QAction *copy = menu->addAction(tr("Copy"), this, SLOT(copyItems()));
        QAction *paste = menu->addAction(tr("Paste"), this, SLOT(pasteItems()));
        QAction *del = menu->addAction(tr("Delete"), this, SLOT(deleteItems()));

        menu->addSeparator();
        QMenu *order = new QMenu(tr("Order"));

        connect(order, SIGNAL(triggered( QAction* )), this, SLOT(requestMoveSelectedItems( QAction *)));
        order->addAction(tr("Send to back"))->setData(MoveBack);
        order->addAction(tr("Bring to front"))->setData(MoveFront);
        order->addAction(tr("Send backwards"))->setData(MoveBackwards);
        order->addAction(tr("Brind forwards"))->setData(MoveForwards);

        menu->addMenu(order);
        menu->addSeparator();
        menu->addAction(tr("Add to library..."), this, SLOT(addSelectedItemsToLibrary()));
        menu->addSeparator();

        if (scene()->selectedItems().isEmpty()) {
            del->setEnabled(false);
            cut->setEnabled(false);
            copy->setEnabled(false);
        }

        if (k->copiesXml.isEmpty())
            paste->setEnabled(false);

        if (QMenu *toolMenu = graphicsScene()->currentTool()->menu()) {
            menu->addSeparator();
            menu->addMenu(toolMenu);
        }

        menu->exec(event->globalPos());

    } else {

        KTPaintAreaBase::mousePressEvent(event);

    }
}

void KTPaintArea::frameResponse(KTFrameResponse *event)
{
    if (graphicsScene()->isDrawing()) 
        return;

    switch (event->action()) {
            case KTProjectRequest::Paste:
            case KTProjectRequest::Select: 
                 { 
                 KTGraphicsScene *sscene = graphicsScene();
                 if (!sscene->scene()) 
                     return;

                 setUpdatesEnabled(true);

                 sscene->setCurrentFrame(event->layerIndex(), event->frameIndex());
                 kFatal() << "Calling out drawPhotogram() from KTPaintArea / frameResponse"; 
                 sscene->drawPhotogram(event->frameIndex());
                 setCurrentScene( event->sceneIndex() );

                 #ifdef K_DEBUG
                        kDebug("paintarea") << "frame: " << event->frameIndex() << " " << "layer: " << event->layerIndex();
                 #endif
                 }
                 break;

            case KTProjectRequest::Lock:
                 {
                 KTGraphicsScene *sscene = graphicsScene();
                 if (!sscene->scene()) 
                     return;
                 if (sscene->currentFrameIndex() == event->frameIndex())
                     viewport()->update();
                 }
                 break;

            default:
                 break;
    }
}

void KTPaintArea::layerResponse(KTLayerResponse *event)
{
    if (graphicsScene()->isDrawing()) 
        return;

    KTGraphicsScene *sscene = graphicsScene();

    if (!sscene->scene()) 
        return;

    if (event->action() == KTProjectRequest::View)
        sscene->setLayerVisible(event->layerIndex(), event->arg().toBool());

    if (event->action() != KTProjectRequest::Add ||  event->action() != KTProjectRequest::Remove) {
        kFatal() << "Calling out drawPhotogram() from KTPaintArea / layerResponse";
        graphicsScene()->drawCurrentPhotogram();
        viewport()->update(scene()->sceneRect().toRect() );
    }
}

void KTPaintArea::sceneResponse(KTSceneResponse *event)
{
     #ifdef K_DEBUG
            K_FUNCINFOX("paintarea");
     #endif

     if (graphicsScene()->isDrawing()) 
         return;

     switch(event->action()) {
            case KTProjectRequest::Select:
                 setCurrentScene( event->sceneIndex() );
                 break;
            case KTProjectRequest::Remove:
                 if (event->sceneIndex() == k->currentSceneIndex)
                     setCurrentScene( k->currentSceneIndex-1);
                 break;
            default: break;
     }
}

void KTPaintArea::itemResponse(KTItemResponse *event)
{
    if (!graphicsScene()->isDrawing()) {
        switch(event->action()) {
               case KTProjectRequest::Transform:
                    viewport()->update();
                    break;
               default:
                    kFatal() << "Calling out drawPhotogram() from KTPaintArea / itemResponse";
                    graphicsScene()->drawCurrentPhotogram();
                    viewport()->update(scene()->sceneRect().toRect() );
                    break;
        }
     }

     graphicsScene()->itemResponse(event);
}

void KTPaintArea::projectResponse(KTProjectResponse *)
{

}

void KTPaintArea::libraryResponse(KTLibraryResponse *request)
{
    if (graphicsScene()->isDrawing())
        return;

    switch (request->action()) {
            case KTProjectRequest::AddSymbolToProject:
                 graphicsScene()->drawCurrentPhotogram();
                 viewport()->update(scene()->sceneRect().toRect() );
                 break;
    }
}

bool KTPaintArea::canPaint() const
{
    KTGraphicsScene *sscene = graphicsScene();

    if (sscene->scene()) {
        if (sscene->currentFrameIndex() >= 0 && sscene->currentLayerIndex() >= 0) 
            return true;
    }

    return false;
}

void KTPaintArea::deleteItems()
{
    // K_FUNCINFO;
    QList<QGraphicsItem *> selecteds = scene()->selectedItems ();

    if (!selecteds.empty()) {
        QString strItems= "";
        KTGraphicsScene* currentScene = graphicsScene();

        if (currentScene) {
            foreach (QGraphicsItem *item, selecteds) {
                     KTProjectRequest event = KTRequestBuilder::createItemRequest( 
                                              currentScene->currentSceneIndex(), currentScene->currentLayerIndex(), 
                                              currentScene->currentFrameIndex(), 
                                              currentScene->currentFrame()->visualIndexOf(item),
                                              KTProjectRequest::Remove );
                     emit requestTriggered(&event);
            }
        }
    }
}

void KTPaintArea::groupItems()
{
    // K_FUNCINFO;
    QList<QGraphicsItem *> selecteds = scene()->selectedItems();

    if (!selecteds.isEmpty()) {
        QString strItems= "";
        KTGraphicsScene* currentScene = graphicsScene();
        int firstItem = -1;

        if (currentScene) {
            foreach (QGraphicsItem *item, selecteds) {
                     if (currentScene->currentFrame()->visualIndexOf(item) != -1) {
                         if (strItems.isEmpty()) {
                             strItems +="("+ QString::number(currentScene->currentFrame()->visualIndexOf(item));
                             firstItem = currentScene->currentFrame()->visualIndexOf(item);
                         } else {
                             strItems += " , " + 
                             QString::number(currentScene->currentFrame()->visualIndexOf(item));
                         }
                     }
            }
            strItems+= ")";
        }

        if (strItems != ")") {
            KTProjectRequest event = KTRequestBuilder::createItemRequest(currentScene->currentSceneIndex(), 
                                     currentScene->currentLayerIndex(),
                                     currentScene->currentFrameIndex(), firstItem, 
                                     KTProjectRequest::Group, strItems );
            emit requestTriggered(&event);
        }
    }
}

void KTPaintArea::ungroupItems()
{
    QList<QGraphicsItem *> selecteds = scene()->selectedItems();
    if (!selecteds.isEmpty()) {
        KTGraphicsScene* currentScene = graphicsScene();
        if (currentScene) {
            foreach (QGraphicsItem *item, selecteds) {
                     KTProjectRequest event = KTRequestBuilder::createItemRequest( 
                                              currentScene->currentSceneIndex(), 
                                              currentScene->currentLayerIndex(), 
                                              currentScene->currentFrameIndex(), 
                                              currentScene->currentFrame()->visualIndexOf(item),
                                              KTProjectRequest::Ungroup);
                     emit requestTriggered(&event);
            }
        }
    }
}

void KTPaintArea::copyItems()
{
    #ifdef K_DEBUG
           K_FUNCINFOX("paintarea");
    #endif

    k->copiesXml.clear();
    QList<QGraphicsItem *> selecteds = scene()->selectedItems();

    if (!selecteds.isEmpty()) {
        KTGraphicsScene* currentScene = graphicsScene();

        if (currentScene) {
            foreach (QGraphicsItem *item, selecteds) {
                     QDomDocument orig;
                     orig.appendChild(dynamic_cast<KTAbstractSerializable *>(item)->toXml( orig ));
                     k->copiesXml << orig.toString();

                     // Paint it to clipbard
                     QPixmap toPixmap(item->boundingRect().size().toSize());
                     toPixmap.fill(Qt::transparent);

                     QPainter painter(&toPixmap);
                     painter.setRenderHint(QPainter::Antialiasing);

                     QStyleOptionGraphicsItem opt;
                     opt.state = QStyle::State_None;

                     if (item->isEnabled())
                         opt.state |= QStyle::State_Enabled;
                     if (item->hasFocus())
                         opt.state |= QStyle::State_HasFocus;
                     if (item == currentScene->mouseGrabberItem())
                         opt.state |= QStyle::State_Sunken;

                     opt.exposedRect = item->boundingRect();
                     opt.levelOfDetail = 1;
                     opt.matrix = item->sceneMatrix();
                     opt.palette = palette();

                     item->paint(&painter, &opt, this);
                     painter.end();

                     QApplication::clipboard()->setPixmap(toPixmap);
            }
        }
    }
}

void KTPaintArea::pasteItems()
{
    #ifdef K_DEBUG
           K_FUNCINFOX("paintarea");
    #endif

    KTGraphicsScene* currentScene = graphicsScene();

    foreach (QString xml, k->copiesXml) {
             KTProjectRequest event = KTRequestBuilder::createItemRequest(currentScene->currentSceneIndex(),
                                      currentScene->currentLayerIndex(), 
                                      currentScene->currentFrameIndex(), 
                                      currentScene->currentFrame()->graphics().count(), 
                                      KTProjectRequest::Add, xml);
             emit requestTriggered(&event);
     }
}

void KTPaintArea::cutItems()
{
    #ifdef K_DEBUG
           K_FUNCINFOX("paintarea");
    #endif
    copyItems();
    deleteItems();
}

void KTPaintArea::setNextFramesOnionSkinCount(int n)
{
    if (KTGraphicsScene* currentScene = graphicsScene())
        currentScene->setNextOnionSkinCount( n );
}

void KTPaintArea::setPreviousFramesOnionSkinCount(int n)
{
    if (KTGraphicsScene* currentScene = graphicsScene())
        currentScene->setPreviousOnionSkinCount( n );
}

void KTPaintArea::addSelectedItemsToLibrary()
{
    #ifdef K_DEBUG
           kDebug("paintarea") << "Adding to library";
    #endif

    QList<QGraphicsItem *> selecteds = scene()->selectedItems();

    if (selecteds.isEmpty()) {
        KOsd::self()->display(tr("No items selected"), KOsd::Error);
        return;
    }

    LibraryDialog dialog;
    foreach (QGraphicsItem *item, selecteds)
             dialog.addItem( item );
	
    if (dialog.exec() != QDialog::Accepted)
        return;

    foreach (QGraphicsItem *item, selecteds) {
             if (KTAbstractSerializable *itemSerializable = dynamic_cast<KTAbstractSerializable *>(item)) {
                 QString symName = dialog.symbolName(item);

                 QDomDocument doc;
                 doc.appendChild(itemSerializable->toXml(doc));

                 KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::Add, 
                                            symName, KTLibraryObject::Item, doc.toString().toLocal8Bit());
                 emit requestTriggered(&request);
             }
    }
}

void KTPaintArea::requestMoveSelectedItems(QAction *action)
{
    #ifdef K_DEBUG
           K_FUNCINFOX("paintarea");
    #endif

    QList<QGraphicsItem *> selecteds = scene()->selectedItems();

    if (selecteds.isEmpty()) {
        KOsd::self()->display(tr("No items selected"), KOsd::Error);
        return;
    }
	
    KTGraphicsScene* currentScene = graphicsScene();
    KTFrame *currentFrame = currentScene->currentFrame();

    QList<int> positions;
    foreach (QGraphicsItem *item, selecteds) {
             int newPos = 0;
             int  value = currentFrame->visualIndexOf(item);
             bool ok;
             int moveType = action->data().toInt(&ok);

             if (ok) {
                 switch (moveType) {
                         case KTPaintArea::MoveBack:
                              newPos = 0;
                              break;
                         case KTPaintArea::MoveFront:
                              newPos = currentScene->currentFrame()->graphics().count()-1;
                              break;
                         case KTPaintArea::MoveBackwards:
                              newPos = value-1;
                              break;
                         case KTPaintArea::MoveForwards:
                              newPos = value+1;
                              break;
                         default: 
                              return;
                 }
                 KTProjectRequest event = KTRequestBuilder::createItemRequest( currentScene->currentSceneIndex(),
                                          currentScene->currentLayerIndex(), currentScene->currentFrameIndex(),
                                          value, KTProjectRequest::Move, newPos );
                 emit requestTriggered(&event);
             }
    }
}
