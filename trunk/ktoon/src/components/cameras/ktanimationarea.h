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

#ifndef AANIMATIONAREA_H
#define AANIMATIONAREA_H

#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QFrame>

#include "ktscene.h"
#include "ktglobal.h"
#include "ktabstractprojectresponsehandler.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTOON_EXPORT KTAnimationArea : public QFrame, public KTAbstractProjectResponseHandler
{
    Q_OBJECT
    public:
        KTAnimationArea(const KTProject *project, QWidget *parent = 0);
        ~KTAnimationArea();

        QSize sizeHint() const;
        void setLoop(bool l);
        void setCurrentScene(int index);
        KTScene *currentScene() const;
        void setFPS(int fps);
        void refreshAnimation(const KTProject *project);
        void renderFrame(int index);

    public slots:
        virtual void render();
        virtual void play();
        virtual void playBack();
        virtual void stop();
        virtual void nextFrame();
        virtual void previousFrame();

    private slots:
        void advance();
        void back();

    protected:
        void frameResponse(KTFrameResponse *event);
        void layerResponse(KTLayerResponse *event);
        void sceneResponse(KTSceneResponse *event);
        void projectResponse(KTProjectResponse *event);
        void itemResponse(KTItemResponse *event);
        void libraryResponse(KTLibraryResponse *request);

    signals:
        void progressStep(int, int);
        void toStatusBar(const QString &, int);
        void sceneChanged(const KTScene *newScene );

        void requestTriggered(const KTProjectRequest *event);

    protected:
        void paintEvent(QPaintEvent *e);
        void resizeEvent(QResizeEvent * event);

    private:
        struct Private;
        Private *const k;
};

#endif
