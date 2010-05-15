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

#ifndef KTVIEWCAMERA_H
#define KTVIEWCAMERA_H

#include <kgui/kcirclebuttonbar.h>
#include <kgui/kvhbox.h>

#include <QMainWindow>
#include <QFrame>
#include "ktanimationarea.h"
#include "ktcamerabar.h"
#include "ktcamerastatus.h"

class KTProjectResponse;
class QCheckBox;
class KTCameraStatus;

/**
 * @author David Cuadrado \<krawek@toonka.com\>
*/
class KTViewCamera : public QFrame
{
    Q_OBJECT

    public:
        KTViewCamera(KTProject *work, QWidget *parent = 0);
        ~KTViewCamera();

        QSize sizeHint() const;
        void updateSceneInfo();

    private slots:
        void showSceneInfo(const KTScene *scene);
        void setLoop();
        void doPlay();
        void doPlayBack();

    public slots:
        bool handleProjectResponse(KTProjectResponse *event);
        void setFPS(int fps);
        void updatePhotograms(KTProject *project);
        void exportDialog();
        void doStop();

    signals:
        void requestTriggered(const KTProjectRequest *event);

    private:
        QFrame *m_container;
        KTAnimationArea *m_animationArea;

        //class KTCameraStatus;
        KTCameraStatus *m_status;
    
        KTProject *project;
        //QCheckBox *m_loop;
};

#endif
