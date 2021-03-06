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

#include "ktcamerastatus.h"
#include "ktexportwidget.h"

#include <kcore/kdebug.h>
#include <kcore/kconfig.h>

#include <kgui/kseparator.h>

#include <QStatusBar>
#include <QLabel>
#include <QHBoxLayout>
#include <QApplication>
#include <QCheckBox>
#include <QSpinBox>

KTCameraStatus::KTCameraStatus(KTViewCamera *camera, QWidget *parent) : QFrame(parent)
{
    #ifdef K_DEBUG
           KINIT;
    #endif

    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    setMidLineWidth(2);
    setLineWidth(1);

    QBoxLayout *m_sceneInfoLayout = new QBoxLayout(QBoxLayout::LeftToRight, parent);
    m_sceneInfoLayout->addStretch(1);

    m_sceneInfoLayout->setSpacing(0);
    m_sceneInfoLayout->setMargin(3);

    QFont font = this->font();
    font.setPointSize(6);

    sceneName = new QLabel;
    sceneName->setFont(font);

    QLabel *sceneNameText = new QLabel("<B>" + tr("Scene name") + ":</B> ");
    sceneNameText->setFont(font);

    m_sceneInfoLayout->addWidget(sceneNameText,1);
    m_sceneInfoLayout->addWidget(sceneName,1);

    m_sceneInfoLayout->addSpacing(20);

    QLabel *label = new QLabel("<B>" + tr("Frames total") + ":</B> ");
    label->setFont(font);

    framesTotal = new QLabel;
    framesTotal->setFont(font);

    m_sceneInfoLayout->addWidget(label, 1);
    m_sceneInfoLayout->addWidget(framesTotal, 1);

    m_sceneInfoLayout->addSpacing(20);

    QLabel *fpsText = new QLabel("<B>" + tr("FPS") + ":</B> ");
    fpsText->setFont(font);

    fps = new QSpinBox;
    fps->setMinimum(1);

    fps->setFont(font);
    connect(fps, SIGNAL(valueChanged(int)), camera, SLOT(setFPS(int)));

    m_sceneInfoLayout->addWidget(fpsText,1);
    m_sceneInfoLayout->addWidget(fps,1);

    m_sceneInfoLayout->addSpacing(20);

    loopBox = new QCheckBox();
    QPixmap pix(THEME_DIR + "icons/loop.png");
    loopBox->setToolTip(tr("Loop"));
    loopBox->setIcon(pix); 
    loopBox->setShortcut(QKeySequence(tr("Ctrl+L")));
    connect(loopBox, SIGNAL(clicked()), camera, SLOT(setLoop()));

    KCONFIG->beginGroup("AnimationParameters");
    loop = KCONFIG->value("Loop").toBool();
    if (loop)
        loopBox->setChecked(true);

    m_sceneInfoLayout->addWidget(loopBox,1);

    m_sceneInfoLayout->addSpacing(20);

    QPushButton *exportButton = new QPushButton(tr("Export"));
    exportButton->setIcon(QIcon(THEME_DIR + "icons/export.png"));
    exportButton->setFont(font);
    connect(exportButton, SIGNAL(pressed()), camera, SLOT(exportDialog()));
    m_sceneInfoLayout->addWidget(exportButton,1);

    setLayout(m_sceneInfoLayout);
}

KTCameraStatus::~KTCameraStatus()
{
    #ifdef K_DEBUG
           KINIT;
    #endif
}

void KTCameraStatus::setFPS(int frames)
{
    fps->setValue(frames);
}

int KTCameraStatus::getFPS()
{
    return fps->value();
}

void KTCameraStatus::setSceneName(const QString &name)
{
    sceneName->setText(name);
}

void KTCameraStatus::setFramesTotal(const QString &frames)
{
    framesTotal->setText(frames);
}

bool KTCameraStatus::isLooping()
{
    loop = loopBox->isChecked();
    KCONFIG->beginGroup("AnimationParameters");
    KCONFIG->setValue("Loop", loop);

    return loop;
}
