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

#ifndef KTCAMERAWIDGET_H
#define KTCAMERAWIDGET_H

#include <ktmodulewidgetbase.h>

class KTProject;
class KTViewCamera;
class QSpinBox;

/**
 * @author David Cuadrado <krawek@gmail.com>
 * @todo:
 * @li
 * - range of frames
 * - aspect ratio
*/

class KTCameraWidget : public KTModuleWidgetBase
{
    Q_OBJECT
    public:
        KTCameraWidget(KTProject *project, QWidget *parent = 0);
        ~KTCameraWidget();
        KTViewCamera *viewCamera();

    public slots:
        void setFPS(int fps);

    private:
        KTViewCamera *m_viewCamera;
        QSpinBox *m_fps;
};

#endif
