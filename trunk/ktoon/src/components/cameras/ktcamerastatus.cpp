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

#include "ktcamerastatus.h"
#include "ktexportwidget.h"
#include <kcore/kdebug.h>
#include <kgui/kseparator.h>

#include <QStatusBar>
#include <QLabel>
#include <QHBoxLayout>
#include <QApplication>
#include <QCheckBox>
#include <QSpinBox>

KTCameraStatus::KTCameraStatus(KTViewCamera *camera, QWidget *parent) : QStatusBar(parent)
{
    setSizeGripEnabled(false);

    QFrame *sceneInfo = new QFrame;
    m_sceneInfoLayout = new QHBoxLayout(sceneInfo);
    m_sceneInfoLayout->setMargin(0);

    QFont font = this->font();
    font.setPointSize(6);

    m_sceneName = new QLabel;
    m_sceneName->setFont(font);

    QLabel *sceneNameText = new QLabel("<B>" + tr("Scene name") + ":</B> ");
    sceneNameText->setFont(font);

    m_sceneInfoLayout->addWidget(sceneNameText,1);
    m_sceneInfoLayout->addWidget(m_sceneName,1);

    m_sceneInfoLayout->addSpacing(20);

    QLabel *framesTotal = new QLabel("<B>" + tr("Frames total") + ":</B> ");
    framesTotal->setFont(font);

    m_total = new QLabel;
    m_total->setFont(font);

    m_sceneInfoLayout->addWidget(framesTotal, 1);
    m_sceneInfoLayout->addWidget(m_total, 1);

    m_sceneInfoLayout->addSpacing(20);

    QLabel *fpsText = new QLabel("<B>" + tr("FPS") + ":</B> ");
    fpsText->setFont(font);

    m_fps = new QSpinBox;
    m_fps->setMinimum(1);

    m_fps->setFont(font);
    connect(m_fps, SIGNAL(valueChanged(int)), camera, SLOT(setFPS(int)));

    m_sceneInfoLayout->addWidget(fpsText,1);
    m_sceneInfoLayout->addWidget(m_fps,1);

    m_sceneInfoLayout->addSpacing(20);

    m_loop = new QCheckBox();
    QPixmap pix(THEME_DIR + "icons/loop.png");
    m_loop->setToolTip(tr("Loop"));
    m_loop->setIcon(pix); 
    connect(m_loop, SIGNAL(clicked()), camera, SLOT(setLoop()));
    m_sceneInfoLayout->addWidget(m_loop,1);

    m_sceneInfoLayout->addSpacing(20);

    QPushButton *exportButton = new QPushButton("Export");
    exportButton->setIcon(QIcon(THEME_DIR + "icons/export.png"));
    exportButton->setFont(font);
    connect(exportButton, SIGNAL(pressed()), camera, SLOT(exportDialog()));
    m_sceneInfoLayout->addWidget(exportButton,1);

    addPermanentWidget(sceneInfo,2);
    sceneInfo->show();
}

KTCameraStatus::~KTCameraStatus()
{
}

void KTCameraStatus::setFPS(int fps)
{
    m_fps->setValue(fps);
}

int KTCameraStatus::getFPS()
{
    m_fps->value();
}

void KTCameraStatus::setSceneName(const QString &name)
{
    m_sceneName->setText(name);
}

void KTCameraStatus::setFramesTotal(const QString &total)
{
    m_total->setText(total);
}

void KTCameraStatus::addWidget(QWidget *widget, int stretch)
{
    QFont font = widget->font();
    font.setPointSize(6);
    widget->setFont(font);
    m_sceneInfoLayout->addWidget(widget, stretch, Qt::AlignCenter);
}

bool KTCameraStatus::loop()
{
    return m_loop->isChecked();
}
