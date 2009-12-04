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

#include "ktanimationarea.h"

#include <QGraphicsItem>

#include "ktprojectresponse.h"
#include "ktgraphicobject.h"
#include "ktgraphicsscene.h"
#include "ktanimationrenderer.h"

#include "ktsoundlayer.h"
#include <kcore/kdebug.h>

struct KTAnimationArea::Private
{
    QFrame *container;
    QImage renderCamera;

    const KTProject *project;

    bool draw;
    bool ciclicAnimation;

    int currentFramePosition;
    int currentSceneIndex;
    int fps;

    QTimer *timer;

    QList<QImage> photograms;
    QList<KTSoundLayer *> sounds;

    bool isRendered;
};

KTAnimationArea::KTAnimationArea(const KTProject *project, QWidget *parent) : QFrame(parent), k(new Private)
{
    k->project = project;

    k->draw = false;
    k->ciclicAnimation = false;
    k->isRendered = false;

    k->currentSceneIndex = -1;
    k->currentFramePosition = -1;

    k->fps = 14;

    setAttribute(Qt::WA_StaticContents);

    k->renderCamera = QImage(size(), QImage::Format_RGB32);
    k->renderCamera.fill(qRgb(255, 255, 255));
    k->timer = new QTimer(this);

    connect(k->timer, SIGNAL(timeout()), this, SLOT(advance()));
    setCurrentScene( 0 );
}


KTAnimationArea::~KTAnimationArea()
{
    delete k;
}

void KTAnimationArea::setFPS(int fps)
{
   k->fps = fps;

   if (k->timer->isActive()) {
       k->timer->stop();
       play();
   }
}

void KTAnimationArea::paintEvent(QPaintEvent *)
{
   if (k->currentFramePosition >= 0 && k->currentFramePosition < k->photograms.count())
       k->renderCamera = k->photograms[k->currentFramePosition];

   QPainter painter;
   painter.begin(this);
   painter.drawImage(QPoint(0, 0), k->renderCamera);
}

void KTAnimationArea::play()
{
   #ifdef K_DEBUG
          kDebug("camera") << "Playing!";
   #endif

   k->draw = true;

   if (k->project && !k->timer->isActive()) {
       if(!k->isRendered) 
          render();
       k->timer->start(1000 / k->fps);
   }

   // emit toStatusBar( tr("Playing... "), 2000 );
}

void KTAnimationArea::stop()
{
    #ifdef K_DEBUG
           kDebug("camera") << "Stopping";
    #endif

    k->timer->stop();

    foreach (KTSoundLayer *sound, k->sounds)
             sound->stop();
	
    k->currentFramePosition = 0;
    repaint();
}

void KTAnimationArea::nextFrame()
{
    if (! k->isRendered)
        render();

    if (k->currentFramePosition >= k->photograms.count()) 
        return;

    k->currentFramePosition += 1;
    repaint();
}

void KTAnimationArea::previousFrame()
{
    if (! k->isRendered) 
        render();

    if (k->currentFramePosition < 1) 
        return;

    k->currentFramePosition -= 1;
    repaint();
}

void KTAnimationArea::advance()
{
    if (k->project) {
        if (k->ciclicAnimation && k->currentFramePosition >= k->photograms.count())
            k->currentFramePosition = 0;

            if (k->currentFramePosition == 0) {
                foreach (KTSoundLayer *sound, k->sounds)
                         sound->play();
                }

            if (k->currentFramePosition < k->photograms.count()) {
                repaint();
                k->currentFramePosition++;
            } else if (!k->ciclicAnimation) {
                stop();
            }
    }
}

void KTAnimationArea::frameResponse(KTFrameResponse *)
{
}

void KTAnimationArea::layerResponse(KTLayerResponse *)
{
}

void KTAnimationArea::sceneResponse(KTSceneResponse *event)
{
     #ifdef K_DEBUG
            K_FUNCINFOX("animation");
     #endif

     switch (event->action()) {
             case KTProjectRequest::Select:
              {
                  setCurrentScene(event->sceneIndex());
              }
             break;
             case KTProjectRequest::Remove:
              {
                  if (event->sceneIndex() == k->currentSceneIndex) {
                      if (k->currentSceneIndex != 0)
                          setCurrentScene(k->currentSceneIndex-1);
                  }
              }
             break;
             default: 
             break;
    }
}

void KTAnimationArea::projectResponse(KTProjectResponse *)
{
}

void KTAnimationArea::itemResponse(KTItemResponse *)
{
}

void KTAnimationArea::libraryResponse(KTLibraryResponse *)
{
}

void KTAnimationArea::render()
{
    KTScene *scene = k->project->scene(k->currentSceneIndex);

    if (!scene) 
        return;

    k->sounds.clear();

    foreach (KTSoundLayer *layer, scene->soundLayers().values() )
             k->sounds << layer;

    k->photograms.clear();

    KTAnimationRenderer renderer;
    renderer.setScene(scene);

    while (renderer.nextPhotogram()) {
           QImage renderized = QImage(size(), QImage::Format_RGB32);
           renderized.fill(qRgb(255, 255, 255));

           QPainter painter(&renderized);
           painter.setRenderHint(QPainter::Antialiasing);
           renderer.render(&painter);

           k->photograms << renderized;
    }

    k->isRendered = true;
}

QSize KTAnimationArea::sizeHint() const
{
    return k->renderCamera.size();
}

void  KTAnimationArea::resizeEvent ( QResizeEvent * e)
{
    QFrame::resizeEvent(e);

    stop();

    k->renderCamera = QImage(size(), QImage::Format_RGB32);
    k->renderCamera.fill(qRgb(255, 255, 255));
    k->isRendered = false;

    update();
}

void KTAnimationArea::setLoop(bool l)
{
    k->ciclicAnimation = l;
}

void KTAnimationArea::setCurrentScene(int index)
{
    k->currentSceneIndex = index;
}

KTScene *KTAnimationArea::currentScene() const
{
    return k->project->scene( k->currentSceneIndex );
}

