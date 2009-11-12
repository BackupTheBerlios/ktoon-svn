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
    setSizeGripEnabled (false);
    QWidget *sceneInfo = new QWidget;
    m_sceneInfoLayout = new QHBoxLayout(sceneInfo);
    m_sceneInfoLayout->setMargin(0);

    QFont font = this->font();
    font.setPointSize(6);

    QLabel *fpsText = new QLabel(tr("<B>FPS:</B> "));
    fpsText->setFont(font);
    m_fps = new QLabel;
    m_fps->setFont(font);
    m_sceneInfoLayout->addWidget(fpsText);
    m_sceneInfoLayout->addWidget(m_fps,2);

    m_sceneName = new QLabel;
    m_sceneName->setFont(font);

    QLabel *sceneNameText = new QLabel(tr("<B>Scene name:</B> "));
    sceneNameText->setFont(font);

    m_sceneInfoLayout->addWidget(sceneNameText);
    m_sceneInfoLayout->addWidget(m_sceneName,2);

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

void KTViewCamera::Status::addWidget(QWidget *widget, int stretch )
{
    QFont font = widget->font();
    font.setPointSize(6);
    widget->setFont(font);
    m_sceneInfoLayout->addWidget(widget, stretch);
}

KTViewCamera::KTViewCamera(KTProject *project, QWidget *parent) : QMainWindow(parent)
{
    #ifdef KDEBUG
           K_DEBUG;
    #endif

    m_status = new Status;
    m_loop = new QCheckBox(tr("Loop"));
    connect(m_loop, SIGNAL(clicked()), this, SLOT(setLoop()));

    m_status->addWidget(m_loop);
    setStatusBar(m_status);

    setObjectName("KTViewCamera_");

    setWindowTitle(tr("Render Camera Preview"));
    setWindowIcon( QPixmap(THEME_DIR+"/icons/camera_preview.png" ));

    m_container = new QFrame(this);
    QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, m_container);

    QFrame *animationAreaContainer = new QFrame;
    animationAreaContainer->setMidLineWidth(2);
    animationAreaContainer->setLineWidth(2);
    animationAreaContainer->setFrameStyle(QFrame::Box | QFrame::Raised );
    animationAreaContainer->setSizePolicy( QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding) );

    QBoxLayout *animationAreaLayout = new QBoxLayout(QBoxLayout::TopToBottom, animationAreaContainer);
    animationAreaLayout->setMargin(0);

    m_animationArea = new KTAnimationArea(project);
    animationAreaLayout->addWidget(m_animationArea);
    layout->addWidget( animationAreaContainer);

#if 1
    KTCameraBar *m_bar = new KTCameraBar;
    layout->addWidget( m_bar, 0, Qt::AlignTop | Qt::AlignCenter );
    m_bar->show();

    connect(m_bar, SIGNAL(play()), this, SLOT(doPlay()));
    connect(m_bar, SIGNAL(stop()), m_animationArea, SLOT(stop()));
    connect(m_bar, SIGNAL(ff()), m_animationArea, SLOT(nextFrame()));
    connect(m_bar, SIGNAL(rew()), m_animationArea, SLOT(previousFrame()));
#else
    KCircleButtonBar *m_bar = new DCircleButtonBar(40);
    layout->addWidget(m_bar, 0, Qt::AlignTop | Qt::AlignCenter );
    m_bar->show();

    qobject_cast<QBoxLayout *>(m_bar->layout())->insertStretch(0, 2);

    KCircleButton *rew = m_bar->addButton(QPixmap(THEME_DIR+"/icons/rw.png" ));
    KCircleButton *play = m_bar->addButton(QPixmap(THEME_DIR+"/icons/play.png" ));
    KCircleButton *stop = m_bar->addButton(QPixmap(THEME_DIR+"/icons/stop.png" ));
    KCircleButton *ff = m_bar->addButton(QPixmap(THEME_DIR+"/icons/ff.png" ));

    qobject_cast<QBoxLayout *>(m_bar->layout())->addStretch(2);

    connect(play, SIGNAL(clicked()), this, SLOT(doPlay()));
    connect(stop, SIGNAL(clicked()), m_animationArea, SLOT(stop()));
#endif

    m_container->setLayout(layout);

    setCentralWidget(m_container);
}


KTViewCamera::~KTViewCamera()
{
    #ifdef KDEBUG
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
