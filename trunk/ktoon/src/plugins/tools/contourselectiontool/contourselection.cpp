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
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
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
#include "contourselection.h"

#include <QPointF>
#include <QKeySequence>
#include <QList>

#include <kgui/kaction.h>
#include <kcore/kdebug.h>
#include <kcore/kglobal.h>
#include <kcore/kalgorithm.h>
#include <kgui/knodegroup.h>

#include "ktgraphicalgorithm.h"
#include "ktscene.h"
#include "ktlayer.h"
#include "ktserializer.h"

#include <QGraphicsView>
#include "ktinputdeviceinformation.h"
#include "ktgraphicsscene.h"
#include "ktprojectrequest.h"
#include "ktprojectresponse.h"
#include "ktrequestbuilder.h"
#include "ktproxyitem.h"
#include "ktlibraryobject.h"

#include <QDebug>
#include <QTimer>

struct ContourSelection::Private
{
    QMap<QString, KAction *> actions;
    QList<KNodeGroup*> nodeGroups; 
    KTGraphicsScene *scene;

};

ContourSelection::ContourSelection(): k(new Private)
{
    setupActions();
}


ContourSelection::~ContourSelection()
{
    delete k;
}

void ContourSelection::init(KTGraphicsScene *scene)
{
    qDeleteAll(k->nodeGroups);
    k->nodeGroups.clear();

    foreach (QGraphicsView * view, scene->views()) {
             view->setDragMode (QGraphicsView::RubberBandDrag);
             foreach (QGraphicsItem *item, view->scene()->items()) {
                      if (!qgraphicsitem_cast<KControlNode *>(item))
                          item->setFlags (QGraphicsItem::ItemIsSelectable);
             }
    }
}

QStringList ContourSelection::keys() const
{
    return QStringList() << tr("Contour Selection") ;
}

void ContourSelection::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
    foreach (QGraphicsView * view, scene->views())
             view->setDragMode (QGraphicsView::RubberBandDrag);

    Q_UNUSED(input);
    Q_UNUSED(brushManager);
    
    k->scene = scene;
}

void ContourSelection::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
    Q_UNUSED(input);
    Q_UNUSED(brushManager);
    Q_UNUSED(scene);
}

void ContourSelection::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
    Q_UNUSED(input);
    Q_UNUSED(brushManager);
    
    if (scene->selectedItems().count() > 0) {

        QList<QGraphicsItem *> selecteds = scene->selectedItems();
        QList<KNodeGroup *>::iterator it = k->nodeGroups.begin();
        QList<KNodeGroup *>::iterator itEnd = k->nodeGroups.end();

        while (it != itEnd) {
               int parentIndex = scene->selectedItems().indexOf((*it)->parentItem());
               if (parentIndex != -1)
                   selecteds.removeAt(parentIndex);
               else
                   delete k->nodeGroups.takeAt(k->nodeGroups.indexOf((*it)));
               ++it;
        }

        foreach (QGraphicsItem *item, selecteds) {
                 if (item) {
                     if (!qgraphicsitem_cast<KControlNode *>(item)) {
                         if (!qgraphicsitem_cast<KTPathItem*>(item)) {
                             KTProjectRequest event = KTRequestBuilder::createItemRequest(scene->currentSceneIndex(), 
                                                      scene->currentLayerIndex(), scene->currentFrameIndex(), 
                                                      scene->currentFrame()->indexOf(item), QPointF(), KTLibraryObject::Item, 
                                                      KTProjectRequest::Convert, 2);
                             emit requested(&event);
                         } else {
                             k->nodeGroups << new KNodeGroup(item, scene);
                         }
                     }
            }
        }
        
        foreach (KNodeGroup *group, k->nodeGroups) {
                 if (!group->changedNodes().isEmpty()) {
                     int position  = scene->currentFrame()->indexOf(group->parentItem());
                     if (position != -1 && qgraphicsitem_cast<QGraphicsPathItem *>(group->parentItem())) {
                         QDomDocument doc;
                         doc.appendChild(qgraphicsitem_cast<KTPathItem *>(group->parentItem())->toXml(doc));
                    
                         KTProjectRequest event = KTRequestBuilder::createItemRequest(scene->currentSceneIndex(), 
                                                  scene->currentLayerIndex(), scene->currentFrameIndex(), position, 
                                                  QPointF(), KTLibraryObject::Item, KTProjectRequest::EditNodes, doc.toString());
                    
                         foreach (QGraphicsView * view, scene->views())
                                  view->setUpdatesEnabled(false);
                    
                         group->restoreItem();
                         emit requested(&event);
                     } else {
                         kDebug("selection") << "position is " << position; 
                     }
                     group->clearChangesNodes();
                 }
        }

    } else {

        qDeleteAll(k->nodeGroups);
        k->nodeGroups.clear();

    }
}

void ContourSelection::itemResponse(const KTItemResponse *response)
{
    K_FUNCINFOX("selection");
    KTProject *project = k->scene->scene()->project();
    QGraphicsItem *item = 0;
    KTScene *scene = 0;
    KTLayer *layer = 0;
    KTFrame *frame = 0;

    if (project) {
        scene = project->scene(response->sceneIndex());
        if (scene) {
            layer = scene->layer(response->layerIndex());
            if (layer) {
                frame = layer->frame(response->frameIndex());
                if (frame)
                    item = frame->item(response->itemIndex());
            }
        }
    } else {
        #ifdef K_DEBUG
               kFatal() << "Project not exist";
        #endif
    }
    
    switch (response->action()) {
        
            case KTProjectRequest::Convert:
            {
                 if (item && scene) {
                     KNodeGroup *node = new KNodeGroup(item, k->scene);
                     k->nodeGroups << node;
                 }
            }
            break;

            case KTProjectRequest::EditNodes:
            {
                 if (item) {
                     foreach (QGraphicsView * view, k->scene->views())
                              view->setUpdatesEnabled(true);

                     foreach (KNodeGroup* group, k->nodeGroups) {
                              if (qgraphicsitem_cast<QGraphicsPathItem *>(group->parentItem()) == item) {
                                  group->show();
                                  group->syncNodesFromParent();
                                  group->saveParentProperties();
                                  break;
                              }
                     }
                 }
            }
            break;

            case KTProjectRequest::Remove:
            {
                 return;
            }
            break;

            default:
            {
                foreach (KNodeGroup* node, k->nodeGroups) {
                         if (node) {
                             node->show();
                             if (node->parentItem()) {
                                 node->parentItem()->setSelected(true);
                                 node->syncNodesFromParent();
                             }
                         }
                }
            }
            break;
    }
}

void ContourSelection::keyPressEvent(QKeyEvent *event)
{
    if (event->key() != Qt::Key_Delete) {
        event->ignore();
        return;
    }
    
    bool deleted = false;
    
    foreach (KNodeGroup *nodegroup, k->nodeGroups)
             deleted = deleted || (nodegroup->removeSelectedNodes() > 0);

    if (deleted)
        event->accept();
}

void ContourSelection::setupActions()
{
    KAction *select = new KAction(QPixmap(THEME_DIR + "icons/nodes.png"), tr("Nodes selection "), this);
    select->setShortcut(QKeySequence(tr("N")));
    // select->setCursor(QCursor(THEME_DIR + "cursors/contour.png"));

    k->actions.insert(tr("Contour Selection"), select);
}

QMap<QString, KAction *> ContourSelection::actions() const
{
    return k->actions;
}

int ContourSelection::toolType() const
{
    return KTToolInterface::Selection;
}

QWidget *ContourSelection::configurator() 
{
    return 0;
}

void ContourSelection::aboutToChangeScene(KTGraphicsScene *scene)
{
    init(scene);
}

void ContourSelection::aboutToChangeTool()
{
    qDeleteAll(k->nodeGroups);
    k->nodeGroups.clear();
}

void ContourSelection::saveConfig()
{
}

Q_EXPORT_PLUGIN2(kt_editNodes, ContourSelection)
