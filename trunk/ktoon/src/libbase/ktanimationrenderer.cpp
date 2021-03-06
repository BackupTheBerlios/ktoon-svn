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

#include "ktanimationrenderer.h"
#include "ktgraphicsscene.h"
#include "ktlayer.h"
#include "ktframe.h"
#include "ktscene.h"
#include "ktgraphicobject.h"

#include <kcore/kdebug.h>

#include <QPainter>


struct KTAnimationRenderer::Private
{
    KTGraphicsScene *scene;
    int totalPhotograms;
    int currentPhotogram;

    Private() : scene(0), totalPhotograms(-1), currentPhotogram(0) {}

    ~Private() {
         delete scene;
    }

    int calculateTotalPhotograms(KTScene *scene);
};

int KTAnimationRenderer::Private::calculateTotalPhotograms(KTScene *scene)
{
    Layers layers = scene->layers();

    int total = 0;

    foreach (KTLayer *layer, layers.values()) {
             if (layer)
                 total = qMax(total, layer->frames().count());
    }

    return total;
}

KTAnimationRenderer::KTAnimationRenderer() : k(new Private)
{
    k->scene = new KTGraphicsScene;
    k->scene->setBackgroundBrush(Qt::white);
}

KTAnimationRenderer::~KTAnimationRenderer()
{
    delete k;
}

void KTAnimationRenderer::setScene(KTScene *scene)
{
    k->scene->setCurrentScene(scene);
    k->scene->setSceneRect(QRectF(QPointF(0,0), QSizeF(520, 380))); // FIXME: this isn't real size

    k->scene->drawPhotogram(0); // ###: Mental Note: Why whithout this doesn't work?
    k->currentPhotogram = -1;

    k->totalPhotograms = k->calculateTotalPhotograms(scene);
}

bool KTAnimationRenderer::nextPhotogram()
{
    if (k->totalPhotograms < 0) 
        return false;

    k->currentPhotogram++;

    if (k->currentPhotogram == k->totalPhotograms)
        return false;

    k->scene->drawPhotogram(k->currentPhotogram);

    return true;
}

void KTAnimationRenderer::renderPhotogram(int index) 
{
    k->scene->drawPhotogram(index);
}

void KTAnimationRenderer::render(QPainter *painter)
{
    k->scene->render(painter, QRect(0, 0, painter->device()->width(), painter->device()->height()), 
                     k->scene->sceneRect().toRect(), Qt::IgnoreAspectRatio);
}

int KTAnimationRenderer::currentPhotogram() const
{
    return k->currentPhotogram;
}

int KTAnimationRenderer::totalPhotograms() const
{
    return k->totalPhotograms;
}

