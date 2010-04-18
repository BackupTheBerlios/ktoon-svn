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

#include "ktpaintarea.h"
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

#include "ktlibrarydialog.h"
#include "ktlibraryobject.h"
#include "ktrequestbuilder.h"

#include <kgui/kosd.h>

#include "ktscene.h"
#include "ktproject.h"

/**
 * This class defines the behavior of the main paint area when ilustration module is on
 * Here is where all the events about the paint area are processed.
 * @author David Cuadrado <krawek@toonka.com>
*/

struct KTPaintArea::Private
{
    //const KTProject *project;
    KTProject *project;
    int currentSceneIndex;
    QStringList copiesXml;
    QString currentTool; 
};

// KTPaintArea::KTPaintArea(const KTProject *project, QWidget * parent) : KTPaintAreaBase(parent), k(new Private)

KTPaintArea::KTPaintArea(KTProject *project, QWidget * parent) : KTPaintAreaBase(parent), k(new Private)
{
    K_FUNCINFO;

    k->project = project;
    k->currentSceneIndex = 0;

    setCurrentScene(0);

    if (graphicsScene()->scene())
        graphicsScene()->setCurrentFrame(0, 0);
}

KTPaintArea::~KTPaintArea()
{
    delete k;
}

void KTPaintArea::setCurrentScene(int index)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    if (k->project->scenesTotal() > 0) {

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
}

void KTPaintArea::mousePressEvent(QMouseEvent *event)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    if (event->buttons() == Qt::RightButton) {

        if (QGraphicsItem *item = scene()->itemAt(mapToScene(event->pos()))) {
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

        connect(order, SIGNAL(triggered(QAction*)), this, SLOT(requestMoveSelectedItems(QAction*)));
        order->addAction(tr("Send to back"))->setData(MoveBack);
        order->addAction(tr("Bring to front"))->setData(MoveFront);
        order->addAction(tr("Send backwards"))->setData(MoveBackwards);
        order->addAction(tr("Brind forwards"))->setData(MoveForwards);

        menu->addMenu(order);
        menu->addSeparator();

        // Code commented temporary while SQA is done
        QAction *addItem = menu->addAction(tr("Add to library..."), this, SLOT(addSelectedItemsToLibrary()));
        menu->addSeparator();

        if (scene()->selectedItems().isEmpty()) {
            del->setEnabled(false);
            cut->setEnabled(false);
            copy->setEnabled(false);
            addItem->setEnabled(false);
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
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

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

                 setCurrentScene(event->sceneIndex());
                 sscene->setCurrentFrame(event->layerIndex(), event->frameIndex());
                 sscene->drawPhotogram(event->frameIndex());
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
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    if (graphicsScene()->isDrawing())
        return;

    KTGraphicsScene *sscene = graphicsScene();

    if (!sscene->scene()) 
        return;

    if (event->action() == KTProjectRequest::View) {
        sscene->setLayerVisible(event->layerIndex(), event->arg().toBool());
    }

    if (event->action() != KTProjectRequest::Add && event->action() != KTProjectRequest::Remove) {
        graphicsScene()->drawCurrentPhotogram();
        viewport()->update(scene()->sceneRect().toRect());
    } else {
        if (event->action() == KTProjectRequest::Remove) {
            // kFatal() << "KTPaintArea::layerResponse <- KTProjectRequest::Remove : " << event->layerIndex();

            KTGraphicsScene *sscene = graphicsScene();
            if (!sscene->scene())
                return;

            if (event->layerIndex() == 0)
                sscene->clear();

            viewport()->update();
        }
    }
}

void KTPaintArea::sceneResponse(KTSceneResponse *event)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    if (graphicsScene()->isDrawing()) 
        return;

    switch(event->action()) {
           case KTProjectRequest::Select:
                kFatal() << "KTPaintArea::sceneResponse <- KTProjectRequest::Select : " << event->sceneIndex();
                if (event->sceneIndex() >= 0)
                    setCurrentScene(event->sceneIndex());
                break;
           case KTProjectRequest::Remove:
                kFatal() << "KTPaintArea::sceneResponse <- KTProjectRequest::Remove : " << k->currentSceneIndex << " - " << event->sceneIndex();
                if (k->currentSceneIndex > 0) {
                    setCurrentScene(k->currentSceneIndex);
                } else {
                    if (k->currentSceneIndex == 0) {
                        k->project->clear();

                        KTGraphicsScene *sscene = graphicsScene();
                        if (!sscene->scene())
                            return;

                        sscene->removeScene();
                        viewport()->update();
                    }
                }
                break;
           default: 
                kFatal() << "KTPaintArea::sceneResponse <- KTProjectRequest::Default";
                break;
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
                    graphicsScene()->drawCurrentPhotogram();
                    viewport()->update(scene()->sceneRect().toRect());
                    break;
        }
    } else {
        /*
        kFatal() << "Updating Paint Area!!! OOOOOO_OOOOOOOO";
        graphicsScene()->drawCurrentPhotogram();
        viewport()->update(scene()->sceneRect().toRect());
        */
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
                 viewport()->update(scene()->sceneRect().toRect());

                 if (k->currentTool.compare(tr("Object Selection")) == 0)
                     emit itemAddedOnSelection(graphicsScene());

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
    QList<QGraphicsItem *> selected = scene()->selectedItems();

    if (!selected.empty()) {
        QString strItems= "";
        KTGraphicsScene* currentScene = graphicsScene();

        if (currentScene) {
            foreach (QGraphicsItem *item, selected) {
                     KTProjectRequest event = KTRequestBuilder::createItemRequest( 
                                              currentScene->currentSceneIndex(), currentScene->currentLayerIndex(), 
                                              currentScene->currentFrameIndex(), 
                                              currentScene->currentFrame()->visualIndexOf(item),
                                              KTProjectRequest::Remove);
                     emit requestTriggered(&event);
            }
        }
    }
}

void KTPaintArea::groupItems()
{
    // K_FUNCINFO;
    QList<QGraphicsItem *> selected = scene()->selectedItems();

    if (!selected.isEmpty()) {
        QString strItems= "";
        KTGraphicsScene* currentScene = graphicsScene();
        int firstItem = -1;

        if (currentScene) {
            foreach (QGraphicsItem *item, selected) {
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
    QList<QGraphicsItem *> selected = scene()->selectedItems();
    if (!selected.isEmpty()) {
        KTGraphicsScene* currentScene = graphicsScene();
        if (currentScene) {
            foreach (QGraphicsItem *item, selected) {
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
    QList<QGraphicsItem *> selected = scene()->selectedItems();

    if (!selected.isEmpty()) {
        KTGraphicsScene* currentScene = graphicsScene();

        if (currentScene) {
            foreach (QGraphicsItem *item, selected) {
                     QDomDocument orig;
                     orig.appendChild(dynamic_cast<KTAbstractSerializable *>(item)->toXml(orig));
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
        currentScene->setPreviousOnionSkinCount(n);
}

void KTPaintArea::addSelectedItemsToLibrary()
{
    #ifdef K_DEBUG
           kDebug("paintarea") << "Adding to library";
    #endif

    QList<QGraphicsItem *> selected = scene()->selectedItems();

    if (selected.isEmpty()) {
        KOsd::self()->display(tr("No items selected"), KOsd::Error);
        return;
    }

    KTLibraryDialog dialog;
    foreach (QGraphicsItem *item, selected)
             dialog.addItem(item);
	
    if (dialog.exec() != QDialog::Accepted)
        return;

    foreach (QGraphicsItem *item, selected) {
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

    QList<QGraphicsItem *> selected = scene()->selectedItems();

    if (selected.isEmpty()) {
        KOsd::self()->display(tr("No items selected"), KOsd::Error);
        return;
    }
	
    KTGraphicsScene* currentScene = graphicsScene();
    KTFrame *currentFrame = currentScene->currentFrame();

    QList<int> positions;
    foreach (QGraphicsItem *item, selected) {
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
                 KTProjectRequest event = KTRequestBuilder::createItemRequest(currentScene->currentSceneIndex(),
                                          currentScene->currentLayerIndex(), currentScene->currentFrameIndex(),
                                          value, KTProjectRequest::Move, newPos);
                 emit requestTriggered(&event);
             }
    }
}

void KTPaintArea::updatePaintArea() 
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    KTGraphicsScene* currentScene = graphicsScene();
    currentScene->drawCurrentPhotogram();
}

void KTPaintArea::setCurrentTool(QString tool) 
{
    k->currentTool = tool;
}
