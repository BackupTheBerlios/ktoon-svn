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

#include "ktcamerabar.h"
#include <QBoxLayout>

#include <kcore/kdebug.h>
#include <kcore/kglobal.h>

KTCameraBar::KTCameraBar(QWidget *parent)
 : QFrame(parent)
{
    #ifdef K_DEBUG
           KINIT;
    #endif

    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    setMidLineWidth(2);
    setLineWidth(1);

    QBoxLayout *m_mainLayout = new QBoxLayout(QBoxLayout::LeftToRight, parent);
    m_mainLayout->addStretch(1);

    m_mainLayout->setSpacing(0);
    m_mainLayout->setMargin(0);

    m_rew = new KImageButton(QPixmap(THEME_DIR + "icons/rw.png"), 33, this, true);
    m_rew->setToolTip(tr("Rewind"));
    m_mainLayout->addWidget(m_rew);
    connect(m_rew, SIGNAL(clicked()), this, SIGNAL(rew()));

    m_play_back = new KImageButton(QPixmap(THEME_DIR + "icons/play_back.png"), 25, this, true);
    m_play_back->setToolTip(tr("Play in reverse"));
    m_mainLayout->addWidget(m_play_back);
    connect(m_play_back, SIGNAL(clicked()), this, SIGNAL(playBack()));

    m_play = new KImageButton(QPixmap(THEME_DIR + "icons/play.png"), 25, this, true);
    m_play->setToolTip(tr("Play"));
    m_mainLayout->addWidget(m_play);
    connect(m_play, SIGNAL(clicked()), this, SIGNAL(play()));

    m_stop = new KImageButton(QPixmap(THEME_DIR + "icons/stop.png"), 25, this, true);
    m_stop->setToolTip(tr("Stop"));
    m_mainLayout->addWidget(m_stop);
    connect(m_stop, SIGNAL(clicked()), this, SIGNAL(stop()));

    m_ff = new KImageButton(QPixmap(THEME_DIR + "icons/ff.png"), 33, this, true);
    m_ff->setToolTip(tr("Forward"));
    m_mainLayout->addWidget(m_ff);
    connect(m_ff, SIGNAL(clicked()), this, SIGNAL(ff()));

    setLayout(m_mainLayout);
}


KTCameraBar::~KTCameraBar()
{
    #ifdef K_DEBUG
           KEND;
    #endif
}

void KTCameraBar::setPalette(const QPalette &)
{
}
