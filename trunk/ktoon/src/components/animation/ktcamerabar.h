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

#ifndef KTCAMERABAR_H
#define KTCAMERABAR_H

#include <QFrame>
#include <QPainter>
#include <QPushButton>

#include <kgui/kimagebutton.h>

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTCameraBar : public QFrame
{
    Q_OBJECT
    public:
        KTCameraBar(QWidget *parent = 0);
        ~KTCameraBar();
        void setPalette(const QPalette &);

    private:
        KImageButton *m_rew;
        KImageButton *m_play;
        KImageButton *m_play_back;
        KImageButton *m_stop;
        KImageButton *m_ff;

    signals:
        void playBack();
        void play();
        void stop();
        void ff();
        void rew();

    protected:
};

#endif
