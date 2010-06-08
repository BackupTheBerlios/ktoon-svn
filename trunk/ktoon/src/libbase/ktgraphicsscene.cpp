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
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "ktgraphicsscene.h"

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

#include <ktscene.h>
#include <ktlayer.h>

#include "ktgraphicobject.h"
#include "ktitemgroup.h"
#include "ktprojectloader.h"
#include "ktitemfactory.h"
#include "kttoolplugin.h"
#include "ktinputdeviceinformation.h"
#include "ktbrushmanager.h"
#include "ktframe.h"
#include "ktitemtweener.h"

#include "ktprojectresponse.h"

#include <kcore/kdebug.h>

#include "ktguideline.h"

/**
 * This class defines the data structure and methods for handling animation scenes.
 * Here is where the set of photograms of one scene are processed.
 * @author David Cuadrado <krawek@toonka.com>
*/

struct KTGraphicsScene::Private
{
    KTToolPlugin *tool;

    KTScene *scene;
    struct OnionSkin
     {
        int previous;
        int next;
        QHash<QGraphicsItem *, double> opacityMap;
     } onionSkin;

    struct FramePosition
     {
        int layer;
        int frame;
     } framePosition;

    bool isDrawing;
    KTBrushManager *brushManager;
    KTInputDeviceInformation *inputInformation;
    //int zLayerCounter;
    int layerCounter;
    int objectCounter;

    QList<KTLineGuide *> lines;
};

KTGraphicsScene::KTGraphicsScene() : QGraphicsScene(), k(new Private)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    setItemIndexMethod(QGraphicsScene::NoIndex);

    //setCurrentFrame(-1, -1);
    setCurrentFrame(0, 0);

    k->onionSkin.next = 0;
    k->onionSkin.previous = 0;
    k->tool = 0;
    k->isDrawing = false;
    //k->zLayerCounter = 0;

    setBackgroundBrush(Qt::gray);

    k->inputInformation = new KTInputDeviceInformation(this);
    k->brushManager = new KTBrushManager(this);
}

KTGraphicsScene::~KTGraphicsScene()
{
    KEND;

    clearFocus();
    clearSelection();

    foreach (QGraphicsView *view, this->views())
             view->setScene(0);

    foreach (QGraphicsItem *item, items())
             removeItem(item);

    delete k;
}

void KTGraphicsScene::setCurrentFrame(int layer, int frame)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    k->framePosition.layer = layer;
    k->framePosition.frame = frame;

    foreach (QGraphicsView *view, views())
             view->setDragMode(QGraphicsView::NoDrag);
}

void KTGraphicsScene::drawCurrentPhotogram()
{
    drawPhotogram(k->framePosition.frame);
}

void KTGraphicsScene::drawPhotogram(int photogram)
{
    #ifdef K_DEBUG
       K_FUNCINFO;
    #endif

    Q_CHECK_PTR(k->scene);

    if (photogram < 0 || !k->scene) 
        return;

    clean();

    bool valid = false;

    // Drawing frames from another layers

    for (int i=0; i < k->scene->layersTotal(); i++) {

             KTLayer *layer = k->scene->layer(i);

             if (layer) {
                 if (layer->isVisible()) {
                     if (k->onionSkin.previous > 0 && photogram > 0) {
                         double opacityFactor = 0.5 / (double)qMin(layer->frames().count(), k->onionSkin.previous);
                         double opacity = 0.5;

                         for (int frameIndex = photogram-1; frameIndex > photogram-k->onionSkin.previous-1; frameIndex--) {
                              KTFrame * frame = layer->frame(frameIndex);
                              if (frame)
                                  addFrame(frame, opacity);

                              opacity -= opacityFactor;
                         }
                     }

                     if (k->onionSkin.next > 0 && layer->framesNumber() > photogram+1) {
                         double opacityFactor = 0.5 / (double)qMin(layer->frames().count(), k->onionSkin.next);
                         double opacity = 0.5;

                         for (int frameIndex = photogram+1; frameIndex < photogram+k->onionSkin.next+1; frameIndex++) {
                              KTFrame * frame = layer->frame(frameIndex);
                              if (frame) {
                                  addFrame(frame, opacity);
                              }
                              opacity -= opacityFactor;
                         }
                     }

                     // TODO: Crashpoint when layers are deleted 
                     KTFrame *frame = layer->frame(photogram);

                     if (frame) {
                         valid = true;
                         k->layerCounter = i;
                         addFrame(frame);
                     }
                 }
             }
    }

    // Drawing tweening objects
    
    if (valid) {
        foreach (KTGraphicObject *object, k->scene->tweeningObjects()) {
                 if (object->frame()->layer()->isVisible()) {
                     int origin = object->frame()->visualIndex();

                     if (KTItemTweener *tweener = object->tweener()) {
                         tweener->setStep(0);

                         if (origin < photogram && photogram < origin+tweener->frames()) {
                             int step = photogram - origin;
                             tweener->setStep(step);
                             addGraphicObject(object);
                         }
                     }
                 }
        }

        update();
    }

    if (k->tool)
        k->tool->updateScene(this);
}

void KTGraphicsScene::addFrame(KTFrame *frame, double opacity)
{
    if (frame) {

        k->objectCounter = 0;
        for (int i=0; i < frame->count(); i++) {
             KTGraphicObject *object = frame->graphic(i);
             addGraphicObject(object, opacity);
        }

    }
}

void KTGraphicsScene::addGraphicObject(KTGraphicObject *object, double opacity)
{
    QGraphicsItem *item = object->item();

    k->onionSkin.opacityMap.insert(item, opacity);

    if (KTItemGroup *group = qgraphicsitem_cast<KTItemGroup *>(item))
        group->recoverChilds();

    if (! qgraphicsitem_cast<KTItemGroup *>(item->parentItem())) {

        item->setSelected(false);
        KTLayer *layer = k->scene->layer(k->framePosition.layer);

        if (layer) {
            KTFrame *frame = layer->frame(k->framePosition.frame);
            if (frame) {
                int factor = k->objectCounter + (k->layerCounter)*100; 
                k->objectCounter++;
                item->setOpacity(opacity);
                item->setZValue(factor);
                addItem(item);
            }
        }

    }
}

void KTGraphicsScene::clean()
{
    k->onionSkin.opacityMap.clear();

    foreach (QGraphicsItem *item, items()) {
             if (item->scene() == this)
                 removeItem(item);
    }

    foreach (KTLineGuide *line, k->lines)
             addItem(line);
}

int KTGraphicsScene::currentFrameIndex() const
{
    return k->framePosition.frame;
}

int KTGraphicsScene::currentLayerIndex() const
{
    return k->framePosition.layer;
}

int KTGraphicsScene::currentSceneIndex() const
{
    if (!k->scene)
        return -1;

    return k->scene->visualIndex();
}

void KTGraphicsScene::setNextOnionSkinCount(int n)
{
    k->onionSkin.next = n;
    drawCurrentPhotogram();
}

void KTGraphicsScene::setPreviousOnionSkinCount(int n)
{
    #ifdef K_DEBUG
       K_FUNCINFO;
    #endif

    k->onionSkin.previous = n;
    drawCurrentPhotogram();
}

KTFrame *KTGraphicsScene::currentFrame()
{
    if (k->scene) {

        if (k->scene->layersTotal() > 0) {

            if (k->framePosition.layer < k->scene->layersTotal()) {
  
                if (k->scene->layers().contains(k->framePosition.layer)) {
                    KTLayer *layer = k->scene->layer(k->framePosition.layer);
                    Q_CHECK_PTR(layer);
                    if (layer) {
                        if (!layer->frames().isEmpty())
                            return layer->frame(k->framePosition.frame);
                    } else {
                        #ifdef K_DEBUG
                               kFatal() << "KTGraphicsScene::currentFrame - No layer available: " << k->framePosition.frame;
                        #endif
                    }
                } else {
                    #ifdef K_DEBUG
                           kFatal() << "KTGraphicsScene::currentFrame - Layer index incorrect!"; 
                    #endif
                }
            } else {
                KTLayer *layer = k->scene->layer(k->scene->layersTotal() - 1);
                if (layer) {
                    if (!layer->frames().isEmpty())
                        return layer->frame(k->framePosition.frame);
                }
            }

        }

    }

    return 0;
}

void KTGraphicsScene::setCurrentScene(KTScene *scene)
{
    Q_CHECK_PTR(scene);

    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    if (k->tool)
        k->tool->aboutToChangeScene(this);

    qDeleteAll(k->lines);
    k->lines.clear();

    clean();
    k->scene = scene;

    drawCurrentPhotogram();
}

void KTGraphicsScene::setLayerVisible(int layerIndex, bool visible)
{
    if (!k->scene)
        return;

    if (KTLayer *layer = k->scene->layer(layerIndex))
        layer->setVisible(visible);
}

KTScene *KTGraphicsScene::scene() const
{
    return k->scene;
}

void KTGraphicsScene::setTool(KTToolPlugin *tool)
{
    drawCurrentPhotogram();

    if (k->tool) {
        if (k->tool->toolType() == KTToolPlugin::Selection) {
            foreach (KTLineGuide *line, k->lines) {
                     line->setFlag(QGraphicsItem::ItemIsMovable, false);
                     line->setEnabledSyncCursor(true);
            }
        }
        k->tool->aboutToChangeTool();
    }

    k->tool = tool;

    if (tool->toolType() == KTToolPlugin::Selection) {
        foreach (KTLineGuide *line, k->lines) {
                 line->setFlag( QGraphicsItem::ItemIsMovable, true);
                 line->setEnabledSyncCursor(false);
        }
    }

    k->tool->init(this);
}

KTToolPlugin *KTGraphicsScene::currentTool() const
{
    return k->tool;
}

void KTGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    QGraphicsScene::mousePressEvent(event);
    k->inputInformation->updateFromMouseEvent(event);
    k->isDrawing = false;

    /* Trashy stuff - ignore it
     if (event->buttons() == Qt::LeftButton &&  (event->modifiers () == (Qt::ShiftModifier | Qt::ControlModifier))) {
        // FIX ME: uggly if
     } else if (k->tool) {
    */

    if ((event->buttons() != Qt::LeftButton) || (event->modifiers () != (Qt::ShiftModifier | Qt::ControlModifier))) {
        if (k->tool) {      
            if (k->tool->toolType() == KTToolPlugin::Brush && event->isAccepted())
                return;

            // If there's no frame... the tool is disabled 

            if (currentFrame()) {
                if (event->buttons() == Qt::LeftButton && !currentFrame()->isLocked()) {
                    k->tool->begin();
                    k->isDrawing = true;
                    k->tool->press(k->inputInformation, k->brushManager, this);
                } else {
                    // TODO: To find the way to enhance this condition only for zoom tool
                    if (event->buttons() == Qt::RightButton && k->tool->toolType() == KTToolPlugin::View)
                        k->tool->press(k->inputInformation, k->brushManager, this);
                }
            } 
        }
    }
}

void KTGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    mouseMoved(event);
}

void KTGraphicsScene::mouseMoved(QGraphicsSceneMouseEvent *event)
{
    k->inputInformation->updateFromMouseEvent(event);

    if (k->tool && k->isDrawing)
        k->tool->move(k->inputInformation, k->brushManager,  this);
}

void KTGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    mouseReleased(event);
}

void KTGraphicsScene::mouseReleased(QGraphicsSceneMouseEvent *event)
{
    k->inputInformation->updateFromMouseEvent(event);

    if (k->tool && k->isDrawing) {
        k->tool->release(k->inputInformation, k->brushManager, this);
        k->tool->end();
    }

    k->isDrawing = false;
}

void KTGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseDoubleClickEvent(event);

    k->inputInformation->updateFromMouseEvent(event);

    if (k->tool)
        k->tool->doubleClick(k->inputInformation, this);
}

void KTGraphicsScene::keyPressEvent(QKeyEvent *event)
{
    if (k->tool) {
        k->tool->keyPressEvent(event);
        
        if (event->isAccepted())
            return;
    }
    
    QGraphicsScene::keyPressEvent(event);
}

/*

// TODO: Check this code, not sure whether it does something or it is helping :S

void KTGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent * event)
{
    if (event->mimeData()->hasFormat("ktoon-ruler"))
        event->acceptProposedAction();

    KTLineGuide *line = 0;
    if (event->mimeData()->data("ktoon-ruler") == "verticalLine") {
        line  = new KTLineGuide(Qt::Vertical, this);
        line->setPos(event->scenePos());
    } else {
        line = new KTLineGuide(Qt::Horizontal, this);
        line->setPos(event->scenePos());
    }

    if (line)
        k->lines << line;
}

// TODO: Check this code, not sure whether it does something or it is helping :S

void  KTGraphicsScene::dragLeaveEvent(QGraphicsSceneDragDropEvent * event)
{
    Q_UNUSED(event);

    removeItem(k->lines.last());
    delete k->lines.takeLast();
}

// TODO: Check this code, not sure whether it does something or it is helping :S

void KTGraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent * event)
{
    if (!k->lines.isEmpty())
        k->lines.last()->setPos(event->scenePos());
}

// TODO: Check this code, not sure whether it does something or it is helping :S

void KTGraphicsScene::dropEvent(QGraphicsSceneDragDropEvent * event)
{
    Q_UNUSED(event);

    if (k->tool) {
        if (k->tool->toolType() == KTToolPlugin::Selection) {
            k->lines.last()->setEnabledSyncCursor(false);
            k->lines.last()->setFlag(QGraphicsItem::ItemIsMovable, true);
        }
    }
}

*/

bool KTGraphicsScene::event(QEvent *e)
{
    return QGraphicsScene::event(e);
}

void KTGraphicsScene::itemResponse(KTItemResponse *event)
{
    #ifdef K_DEBUG
           K_FUNCINFOX("scene");
    #endif

    if (k->tool) {
        // k->tool->init(this); // FIXME:k->tool->init(this); in itemResponse ???
        k->tool->itemResponse(event);
    }
}

bool KTGraphicsScene::isDrawing() const
{
    return k->isDrawing;
}

KTBrushManager *KTGraphicsScene::brushManager() const
{
    return k->brushManager;
}

void KTGraphicsScene::aboutToMousePress()
{
    QHash<QGraphicsItem *, double>::iterator it = k->onionSkin.opacityMap.begin();

    while (it != k->onionSkin.opacityMap.end()) {
            if (it.value() != 1.0) {
                it.key()->setAcceptedMouseButtons(Qt::NoButton);
                it.key()->setFlag(QGraphicsItem::ItemIsSelectable, false);
            } else {
                it.key()->setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton | Qt::MidButton | Qt::XButton1
                                                  | Qt::XButton2);
            }
            ++it;
    }
}

void KTGraphicsScene::includeObject(QGraphicsItem *object)
{
    KTLayer *layer = k->scene->layer(k->framePosition.layer);
    if (layer) {
        KTFrame *frame = layer->frame(k->framePosition.frame);
        if (frame) {
            int zLevel = frame->getTopZLevel() + (k->framePosition.layer*100);    
            object->setZValue(zLevel);
            addItem(object);
        }
    }
}

void KTGraphicsScene::removeScene()
{
    clean();
    k->scene = 0;
}
