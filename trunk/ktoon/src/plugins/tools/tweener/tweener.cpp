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
 
#include "tweener.h"
#include "configurator.h"

#include <QPointF>
#include <QKeySequence>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QMatrix>
#include <QGraphicsLineItem>
#include <QGraphicsView>
#include <QDomDocument>
#include <QGraphicsPathItem>

#include <kcore/kalgorithm.h>
#include <kcore/kglobal.h>
#include <kcore/kdebug.h>
#include <kgui/kaction.h>
#include <kgui/knodegroup.h>
#include <ktsvg2qt.h>

#include "ktinputdeviceinformation.h"
#include "ktbrushmanager.h"
#include "ktgraphicalgorithm.h"
#include "ktgraphicsscene.h"
#include "ktrequestbuilder.h"
#include "ktprojectrequest.h"
#include "ktscene.h"
#include "ktlayer.h"

struct Tweener::Private
{
    QMap<QString, KAction *> actions;
    Configurator *configurator;
    KTGraphicsScene *scene;
    bool creatingPath;
    bool selecting;
    QGraphicsPathItem *path;
    KNodeGroup *group;
};

Tweener::Tweener() : KTToolPlugin(), k(new Private)
{
    kFatal() << "Tweener::Tweener() -> Object starting!";

    setupActions();
    k->configurator = 0;
    k->creatingPath = true;
    k->selecting = false;
    k->path = 0;
    k->group = 0;
}

Tweener::~Tweener()
{
    delete k;
}

void Tweener::init(KTGraphicsScene *scene)
{
    kFatal() << "Tweener::init -> Activating plugin!";

    delete k->path;
    k->path = 0;
    delete k->group;
    k->group = 0;

    k->scene = scene;
    k->configurator->activatePathMode();
    setCreatePath();
}

QStringList Tweener::keys() const
{
    return QStringList() << tr("Tweener");
}

void Tweener::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
    Q_UNUSED(brushManager);

    if (k->creatingPath) {
        if (!k->path) {
            k->path = new QGraphicsPathItem;
            QColor color = Qt::lightGray;
            //color.setAlpha(200);
            QPen pen(QBrush(color), 1, Qt::DotLine);  
            k->path->setPen(pen);
            QPainterPath path;
            path.moveTo(input->pos());
            k->path->setPath(path);
            scene->addItem(k->path);
        } else {
            QPainterPath path = k->path->path();
            path.cubicTo(input->pos(), input->pos(), input->pos());
            k->path->setPath(path);
        }
    } else {
        kFatal() << "Tweener::press -> Starting selection!";
    }
}

void Tweener::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
    Q_UNUSED(input);
    Q_UNUSED(brushManager);
    Q_UNUSED(scene);
}

void Tweener::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
    Q_UNUSED(input);
    Q_UNUSED(brushManager);

    if (k->creatingPath) {
        if (!k->group) {
            k->group = new KNodeGroup(k->path, scene);
            connect(k->group, SIGNAL(nodeClicked()), SLOT(updatePath()));
            k->group->clearChangesNodes();
        } else {
            k->group->createNodes(k->path);
        }

        k->configurator->updateSteps(k->path);
    } else {
        kFatal() << "Tweener::release -> Ending selection!"; 
    }
}

QMap<QString, KAction *> Tweener::actions() const
{
    return k->actions;
}

int Tweener::toolType() const
{
    return KTToolInterface::Brush;
}

QWidget *Tweener::configurator()
{
    if (!k->configurator) {
        k->configurator = new Configurator;
        connect(k->configurator, SIGNAL(clikedCreatePath()), this, SLOT(setCreatePath()));
        connect(k->configurator, SIGNAL(clikedSelect()), this, SLOT(setSelect()));
        connect(k->configurator, SIGNAL(clikedApplyTweener()), this, SLOT(applyTweener()));
    }

    return k->configurator;
}

void Tweener::aboutToChangeScene(KTGraphicsScene *)
{
}

void Tweener::aboutToChangeTool()
{
    delete k->group;
    k->group = 0;
    delete k->path;
    k->path = 0;
    setCreatePath();
}

bool Tweener::isComplete() const
{
    return true;
}

void Tweener::setupActions()
{
    kFatal() << "Tweener::setupActions() -> init!";

    KAction *translater = new KAction(QPixmap(THEME_DIR + "icons/tweener.png"), 
                                      tr("Motion Tween"), this);

    translater->setCursor(QCursor(THEME_DIR + "cursors/tweener.png"));

    k->actions.insert("Tweener", translater);
}

void Tweener::setCreatePath()
{
    if (k->path) {
        k->scene->addItem(k->path);
        if (k->group)
            k->group->createNodes(k->path);
    }

    k->creatingPath = true;
    k->selecting = false;

    foreach (QGraphicsView * view, k->scene->views()) {
             view->setDragMode (QGraphicsView::NoDrag);
             foreach (QGraphicsItem *item, view->scene()->items()) {
                      if (item != k->path)
                          item->setFlag(QGraphicsItem::ItemIsSelectable, false);
             }
    }
}

void Tweener::setSelect()
{
    k->creatingPath = false;
    k->selecting = true;

    /*
    if (k->path) {
        delete k->group;
        k->group = 0;
    } 
    */

    foreach (QGraphicsView * view, k->scene->views()) {
             view->setDragMode(QGraphicsView::RubberBandDrag);
             foreach (QGraphicsItem *item, view->scene()->items()) {
                      if (item != k->path)
                          item->setFlags(QGraphicsItem::ItemIsSelectable);
             }
    }
}

QString Tweener::pathToCoords()
{
    QString strPath = "";
    QChar t;

    for (int i=0; i < k->path->path().elementCount(); i++) {
         QPainterPath::Element e = k->path->path().elementAt(i);
         switch (e.type) {
            case QPainterPath::MoveToElement:
            {
                if (t != 'M') {
                    t = 'M';
                    strPath += "M " + QString::number(e.x) + " " + QString::number(e.y) + " ";
                } else {
                    strPath += QString::number(e.x) + " " + QString::number(e.y) + " ";
                }
            }
            break;
            case QPainterPath::LineToElement:
            {
                if (t != 'L') {
                    t = 'L';
                    strPath += " L " + QString::number(e.x) + " " + QString::number(e.y) + " ";
                } else {
                    strPath += QString::number(e.x) + " " + QString::number(e.y) + " ";
                }
            }
            break;
            case QPainterPath::CurveToElement:
            {
                if (t != 'C') {
                    t = 'C';
                    strPath += " C " + QString::number(e.x) + " " + QString::number(e.y) + " ";
                } else {
                    strPath += "  "+ QString::number(e.x) + " " + QString::number(e.y) + " ";
                }
            }
            break;
            case QPainterPath::CurveToDataElement:
            {
                if (t == 'C')
                    strPath +=  " " + QString::number(e.x) + "  " + QString::number(e.y) + " ";
            }
            break;
        }
    }

    return strPath;
}

void Tweener::applyTweener()
{
    if (k->path) {

        foreach (QGraphicsItem *item, k->scene->selectedItems()) {

                 // kFatal() << "Tweener::applyTweener(): " << k->configurator->steps(k->scene->currentFrameIndex());

                 QString route = pathToCoords();

                 kFatal() << "Tweener::applyTweener(): Map -> " << route;

                 KTProjectRequest request = KTRequestBuilder::createItemRequest(
                                            k->scene->currentSceneIndex(),
                                            k->scene->currentLayerIndex(),
                                            k->scene->currentFrameIndex(),
                                            k->scene->currentFrame()->visualIndexOf(item),
                                            KTProjectRequest::Tweening, 
                                            k->configurator->steps(k->scene->currentFrameIndex(), route));
                 emit requested(&request);

                 if (KTLayer *layer = k->scene->scene()->layer(k->scene->currentLayerIndex())) {
                     int frames = layer->frames().count();
                     int newFrames = k->configurator->totalSteps() + k->scene->currentFrameIndex() - frames;
                     int total = k->scene->currentFrameIndex() + newFrames + 1;
                     int start = k->scene->currentFrameIndex() + 1;

                     for (int i = start; i < total; i++) {
                          KTProjectRequest requestFrame = KTRequestBuilder::createFrameRequest(k->scene->currentSceneIndex(), 
                                                                            k->scene->currentLayerIndex(), 
                                                                            i, KTProjectRequest::Add);
                          emit requested(&requestFrame);
                     }
                 }

        }
    }
}

void Tweener::updatePath()
{
    k->configurator->updateSteps(k->path);
}

void Tweener::saveConfig()
{
}

void Tweener::updateScene(KTGraphicsScene *scene)
{
    kFatal() << "Tweener::updateScene <- Hey!";

    /*
     k->scene->currentSceneIndex(),
     k->scene->currentLayerIndex(),
     k->scene->currentFrameIndex(),
     */

    if (k->creatingPath) {
        if (k->path && k->group) {
            k->scene->addItem(k->path);            
            k->group->createNodes(k->path);
        }
    }
}

Q_EXPORT_PLUGIN2(kt_tweener, Tweener);

