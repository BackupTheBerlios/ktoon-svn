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

#ifndef KTVIEWCAMERA_H
#define KTVIEWCAMERA_H

#include <kgui/kcirclebuttonbar.h>
#include <kgui/kvhbox.h>

#include <QMainWindow>
#include <QFrame>
#include "ktanimationarea.h"
#include "ktcamerabar.h"


class KTProjectResponse;
class QCheckBox;

/**
 * @author David Cuadrado \<krawek@toonka.com\>
*/
class KTViewCamera : public QFrame
{
    Q_OBJECT
    public:
        KTViewCamera(KTProject *project, QWidget *parent = 0);
        ~KTViewCamera();

        QSize sizeHint() const;
        void updateSceneInfo();
        void setFPS(int fps);

    private slots:
        void showSceneInfo(const KTScene *scene);
        void setLoop();
        void doPlay();

    public slots:
        bool handleProjectResponse(KTProjectResponse *event);

    signals:
        void requestTriggered(const KTProjectRequest *event);

    private:
        QFrame *m_container;
        KTAnimationArea *m_animationArea;

        class Status;
        Status *m_status;

        QCheckBox *m_loop;
};

#endif
