/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2007 by Jorge Cuadrado <kuadrosx@toonka.com>            *
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
 
#include "tweenertranslate.h"
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

#include "ktinputdeviceinformation.h"
#include "ktbrushmanager.h"
#include "ktgraphicalgorithm.h"
#include "ktgraphicsscene.h"
#include "ktrequestbuilder.h"
#include "ktprojectrequest.h"
#include "ktscene.h"
#include "ktlayer.h"


struct TweenerTranslate::Private
{
    QMap<QString, KAction *> actions;
    Configurator *configurator;
    KTGraphicsScene *scene;
    bool creatingPath, selecting;
    QGraphicsPathItem *path;
    KNodeGroup *group;
};

TweenerTranslate::TweenerTranslate() : KTToolPlugin(), k(new Private)
{
    setupActions();
    k->configurator = 0;
    k->creatingPath = true;
    k->selecting = false;
    k->path = 0;
    k->group = 0;
}

TweenerTranslate::~TweenerTranslate()
{
    delete k;
}

void TweenerTranslate::init(KTGraphicsScene *scene)
{
    delete k->path;
    k->path = 0;
    delete k->group;
    k->group = 0;

    k->scene = scene;
    setCreatePath();
}

QStringList TweenerTranslate::keys() const
{
    return QStringList() << tr("TweenerTranslater");
}

void TweenerTranslate::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
    if (k->creatingPath) {
        if (!k->path) {
            k->path = new QGraphicsPathItem;
            QColor pen = Qt::black;
            pen.setAlpha(125);
            k->path->setPen(QPen(QBrush(pen),1, Qt::DotLine));
            QPainterPath path;
            path.moveTo(input->pos());
            k->path->setPath(path);
            scene->addItem(k->path);
        } else {
            QPainterPath path = k->path->path();
            // path.lineTo(input->pos());
            path.cubicTo(input->pos(), input->pos(), input->pos());
            k->path->setPath(path);
        }
    }
}

void TweenerTranslate::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
}

void TweenerTranslate::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
    if (k->creatingPath) {
        if (!k->group) {
            k->group = new KNodeGroup(k->path, scene);
            connect(k->group, SIGNAL(nodeClicked()), SLOT(updatePath()));
            k->group->clearChangesNodes();
        } else {
            k->group->createNodes(k->path);
        }
        k->configurator->updateSteps(k->path);
    }
}

QMap<QString, KAction *> TweenerTranslate::actions() const
{
    return k->actions;
}

int TweenerTranslate::toolType() const
{
    return KTToolInterface::Brush;
}


QWidget *TweenerTranslate::configurator()
{
    if (!k->configurator) {
        k->configurator = new Configurator;
        connect(k->configurator, SIGNAL(clikedCreatePath()), this, SLOT(setCreatePath()));
        connect(k->configurator, SIGNAL(clikedSelect()), this, SLOT(setSelect()));
        connect(k->configurator, SIGNAL(clikedApplyTweener()), this, SLOT(applyTweener()));
    }

    return k->configurator;
}

void TweenerTranslate::aboutToChangeScene(KTGraphicsScene *)
{
}

void TweenerTranslate::aboutToChangeTool()
{
    delete k->group;
    k->group = 0;
    delete k->path;
    k->path = 0;
    setCreatePath();
}

bool TweenerTranslate::isComplete() const
{
    return true;
}

void TweenerTranslate::setupActions()
{
    KAction *translater = new KAction(QPixmap(THEME_DIR + "icons/send_to_back.png"), tr("Motion Tween"), this);
    k->actions.insert("TweenerTranslater", translater);
}

void TweenerTranslate::setCreatePath()
{
    if (k->path)
        k->scene->addItem(k->path);

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

void TweenerTranslate::setSelect()
{
    k->creatingPath = false;
    k->selecting = true;

    if (k->path) {
        delete k->group;
        k->group = 0;
    } 

    foreach (QGraphicsView * view, k->scene->views()) {
             view->setDragMode (QGraphicsView::RubberBandDrag);
             foreach (QGraphicsItem *item, view->scene()->items() ) {
                      if (item != k->path)
                          item->setFlags(QGraphicsItem::ItemIsSelectable);
             }
    }
}

void TweenerTranslate::applyTweener()
{
    if (k->path) {
        foreach (QGraphicsItem *item, k->scene->selectedItems()) {
                 KTProjectRequest request = KTRequestBuilder::createItemRequest(
                                            k->scene->currentSceneIndex(),
                                            k->scene->currentLayerIndex(),
                                            k->scene->currentFrameIndex(),
                                            k->scene->currentFrame()->visualIndexOf(item),
                                            KTProjectRequest::Tweening, k->configurator->steps());
                 emit requested(&request);

                 if (KTLayer *layer = k->scene->scene()->layer(k->scene->currentLayerIndex())) {
                     int frames = layer->frames().count();
                     int newFrames = k->configurator->totalSteps() + k->scene->currentFrameIndex() - frames;

                     for (int i = frames; i < newFrames; i++) {
                          KTProjectRequest requestFrame = KTRequestBuilder::createFrameRequest(k->scene->currentSceneIndex(), k->scene->currentLayerIndex(), i, KTProjectRequest::Add);
                          emit requested(&requestFrame);
                     }
                 }
        }
    }
}

void TweenerTranslate::updatePath()
{
    k->configurator->updateSteps(k->path);
}

void TweenerTranslate::saveConfig()
{
}

Q_EXPORT_PLUGIN2(kt_tweenertranslate, TweenerTranslate);

