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

#include "ktviewcamera.h"
#include <kcore/kdebug.h>

#include <QStatusBar>
#include <QLabel>
#include <QHBoxLayout>
#include <QApplication>
#include <QCheckBox>

#include "ktprojectresponse.h"

class KTViewCamera::Status : public QStatusBar
{
    public:
        Status(QWidget *parent = 0);
        ~Status();

        void setFPS(int fps);
        void setSceneName(const QString &name);
        void addWidget(QWidget *widget, int stretch = 0);

    private:
        QLabel *m_fps;
        QLabel *m_sceneName;
        QHBoxLayout *m_sceneInfoLayout;
};


KTViewCamera::Status::Status(QWidget *parent) : QStatusBar(parent)
{
    setSizeGripEnabled(false);

    QFrame *sceneInfo = new QFrame;
    m_sceneInfoLayout = new QHBoxLayout(sceneInfo);
    m_sceneInfoLayout->setMargin(0);

    QFont font = this->font();
    font.setPointSize(6);

    QLabel *fpsText = new QLabel(tr("<B>FPS:</B> "));
    fpsText->setFont(font);
    m_fps = new QLabel;
    m_fps->setFont(font);

    m_sceneInfoLayout->addWidget(fpsText,1);
    m_sceneInfoLayout->addWidget(m_fps,1);

    m_sceneInfoLayout->addSpacing(20);

    m_sceneName = new QLabel;
    m_sceneName->setFont(font);

    QLabel *sceneNameText = new QLabel(tr("<B>Scene name:</B> "));
    sceneNameText->setFont(font);

    m_sceneInfoLayout->addWidget(sceneNameText,1);
    m_sceneInfoLayout->addWidget(m_sceneName,1);

    m_sceneInfoLayout->addSpacing(20);

    addPermanentWidget(sceneInfo,2);
    sceneInfo->show();
}

KTViewCamera::Status::~Status()
{

}

void KTViewCamera::Status::setFPS(int fps)
{
    m_fps->setText(QString::number(fps));
}

void KTViewCamera::Status::setSceneName(const QString &name)
{
    m_sceneName->setText(name);
}

void KTViewCamera::Status::addWidget(QWidget *widget, int stretch)
{
    QFont font = widget->font();
    font.setPointSize(6);
    widget->setFont(font);
    m_sceneInfoLayout->addWidget(widget, stretch, Qt::AlignCenter);
}

KTViewCamera::KTViewCamera(KTProject *project, QWidget *parent) : QFrame(parent)
{
    #ifdef K_DEBUG
           KINIT;
    #endif

    setObjectName("KTViewCamera_");
    setWindowTitle(tr("Render Camera Preview"));
    setWindowIcon(QPixmap(THEME_DIR + "/icons/camera_preview.png"));

    QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, this);

    QFont font = this->font();
    font.setPointSize(8);
    QLabel *title = new QLabel(tr("Render Camera Preview"));
    title->setFont(font);
    layout->addWidget(title, 0, Qt::AlignCenter);

    QFrame *animationAreaContainer = new QFrame;
    animationAreaContainer->setMidLineWidth(2);
    animationAreaContainer->setLineWidth(2);
    animationAreaContainer->setFrameStyle(QFrame::Box | QFrame::Raised);
    animationAreaContainer->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    QBoxLayout *animationAreaLayout = new QBoxLayout(QBoxLayout::TopToBottom, animationAreaContainer);
    animationAreaLayout->setMargin(0);

    m_animationArea = new KTAnimationArea(project);
    animationAreaLayout->addWidget(m_animationArea);
    layout->addWidget(animationAreaContainer, 0, Qt::AlignCenter);

    KTCameraBar *m_bar = new KTCameraBar;
    layout->addWidget(m_bar, 0, Qt::AlignCenter);
    m_bar->show();

    connect(m_bar, SIGNAL(play()), this, SLOT(doPlay()));
    connect(m_bar, SIGNAL(stop()), m_animationArea, SLOT(stop()));
    connect(m_bar, SIGNAL(ff()), m_animationArea, SLOT(nextFrame()));
    connect(m_bar, SIGNAL(rew()), m_animationArea, SLOT(previousFrame()));

    m_status = new Status;
    m_loop = new QCheckBox(tr("Loop"));
    connect(m_loop, SIGNAL(clicked()), this, SLOT(setLoop()));
    m_status->addWidget(m_loop);
    //showSceneInfo(m_animationArea->currentScene());

    KTScene *scene = project->scene(0);
    if (scene)
        m_status->setSceneName(scene->sceneName());

    m_status->setFPS(project->fps());

    layout->addWidget(m_status, 0, Qt::AlignCenter|Qt::AlignTop);
    setLayout(layout);
}


KTViewCamera::~KTViewCamera()
{
    #ifdef K_DEBUG
           KEND;
    #endif
}

void KTViewCamera::showSceneInfo(const KTScene *scene)
{
    if (scene)
        m_status->setSceneName(scene->sceneName());
}

void KTViewCamera::setLoop()
{
    m_animationArea->setLoop(m_loop->isChecked());
}

QSize KTViewCamera::sizeHint() const
{
    QSize size = QWidget::sizeHint();
    return size.expandedTo(QApplication::globalStrut());
}

void KTViewCamera::doPlay()
{
    m_animationArea->play();
    updateSceneInfo();
}

void KTViewCamera::updateSceneInfo()
{
    showSceneInfo(m_animationArea->currentScene());
}

bool KTViewCamera::handleProjectResponse(KTProjectResponse *response)
{
    if (response->part() == KTProjectRequest::Scene)
        updateSceneInfo();

    return m_animationArea->handleResponse(response);
}

void KTViewCamera::setFPS(int fps)
{
    m_status->setFPS(fps);
    m_animationArea->setFPS(fps);
}
