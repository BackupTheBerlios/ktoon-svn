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
#include "ktexportwidget.h"
#include <kcore/kdebug.h>
#include <kgui/kseparator.h>

#include <QStatusBar>
#include <QLabel>
#include <QHBoxLayout>
#include <QApplication>
#include <QCheckBox>
#include <QSpinBox>

#include "ktprojectresponse.h"
//#include "ktrenderdialog.h"

class KTViewCamera::Status : public QStatusBar
{
    public:
        Status(KTViewCamera *camera = 0, QWidget *parent = 0);
        ~Status();

        void setFPS(int fps);
        void setSceneName(const QString &name);
        void setFramesTotal(const QString &total);
        void addWidget(QWidget *widget, int stretch = 0);
        bool loop();

    private:
        QSpinBox *m_fps;
        QLabel *m_sceneName;
        QLabel *m_total;
        QHBoxLayout *m_sceneInfoLayout;
        QCheckBox *m_loop;
};

KTViewCamera::Status::Status(KTViewCamera *camera, QWidget *parent) : QStatusBar(parent)
{
    setSizeGripEnabled(false);

    QFrame *sceneInfo = new QFrame;
    m_sceneInfoLayout = new QHBoxLayout(sceneInfo);
    m_sceneInfoLayout->setMargin(0);

    QFont font = this->font();
    font.setPointSize(6);

    m_sceneName = new QLabel;
    m_sceneName->setFont(font);

    QLabel *sceneNameText = new QLabel(tr("<B>Scene name:</B> "));
    sceneNameText->setFont(font);

    m_sceneInfoLayout->addWidget(sceneNameText,1);
    m_sceneInfoLayout->addWidget(m_sceneName,1);

    m_sceneInfoLayout->addSpacing(20);

    QLabel *framesTotal = new QLabel(tr("<B>Frames Total:</B> "));
    framesTotal->setFont(font);

    m_total = new QLabel;
    m_total->setFont(font);

    m_sceneInfoLayout->addWidget(framesTotal, 1);
    m_sceneInfoLayout->addWidget(m_total, 1);

    m_sceneInfoLayout->addSpacing(20);

    QLabel *fpsText = new QLabel(tr("<B>FPS:</B> "));
    fpsText->setFont(font);

    m_fps = new QSpinBox;
    m_fps->setMinimum(1);

    m_fps->setFont(font);
    connect(m_fps, SIGNAL(valueChanged(int)), camera, SLOT(setFPS(int)));

    m_sceneInfoLayout->addWidget(fpsText,1);
    m_sceneInfoLayout->addWidget(m_fps,1);

    m_sceneInfoLayout->addSpacing(20);

    m_loop = new QCheckBox(tr("Loop"));
    QPixmap pix(THEME_DIR + "icons/layer.png");
    m_loop->setToolTip(tr("Loop"));
    m_loop->setIcon(pix); 
    m_loop->setFont(font);
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

KTViewCamera::Status::~Status()
{
}

void KTViewCamera::Status::setFPS(int fps)
{
    m_fps->setValue(fps);
}

void KTViewCamera::Status::setSceneName(const QString &name)
{
    m_sceneName->setText(name);
}

void KTViewCamera::Status::setFramesTotal(const QString &total)
{
    m_total->setText(total);
}

void KTViewCamera::Status::addWidget(QWidget *widget, int stretch)
{
    QFont font = widget->font();
    font.setPointSize(6);
    widget->setFont(font);
    m_sceneInfoLayout->addWidget(widget, stretch, Qt::AlignCenter);
}

bool KTViewCamera::Status::loop()
{
    return m_loop->isChecked();
}

KTViewCamera::KTViewCamera(KTProject *work, QWidget *parent) : QFrame(parent)
{
    #ifdef K_DEBUG
           KINIT;
    #endif

    project = work;

    setObjectName("KTViewCamera_");
    setWindowTitle(tr("Render Camera Preview"));

    QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, this);

    QFont font = this->font();
    font.setPointSize(8);
    font.setUnderline(true);
    QLabel *title = new QLabel(tr("Render Camera Preview"));
    title->setFont(font);
    QLabel *icon = new QLabel();
    icon->setPixmap(QPixmap(THEME_DIR + "icons/camera.png"));
    layout->addWidget(icon, 0, Qt::AlignCenter);
    layout->addWidget(title, 0, Qt::AlignCenter);

    m_animationArea = new KTAnimationArea(project);
    layout->addWidget(m_animationArea, 0, Qt::AlignCenter);

    KTCameraBar *m_bar = new KTCameraBar;
    layout->addWidget(m_bar, 0, Qt::AlignCenter);
    m_bar->show();

    connect(m_bar, SIGNAL(play()), this, SLOT(doPlay()));
    connect(m_bar, SIGNAL(playBack()), this, SLOT(doPlayBack()));
    connect(m_bar, SIGNAL(stop()), m_animationArea, SLOT(stop()));
    connect(m_bar, SIGNAL(ff()), m_animationArea, SLOT(nextFrame()));
    connect(m_bar, SIGNAL(rew()), m_animationArea, SLOT(previousFrame()));

    m_status = new Status(this);

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
    m_animationArea->setLoop(m_status->loop());
}

QSize KTViewCamera::sizeHint() const
{
    QSize size = QWidget::sizeHint();
    return size.expandedTo(QApplication::globalStrut());
}

void KTViewCamera::doPlay()
{
    kFatal() << "*** Playing Animation!!!";
    m_animationArea->play();
    updateSceneInfo();
}

void KTViewCamera::doPlayBack()
{
    m_animationArea->playBack();
    updateSceneInfo();
}

void KTViewCamera::doStop()
{
    m_animationArea->stop();
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

void KTViewCamera::updatePhotograms(KTProject *project)
{
    m_animationArea->refreshAnimation(project);
    KTScene *scene = project->scene(0);
    QString total = "";
    total = total.setNum(scene->framesTotal()); 
    m_status->setFramesTotal(total); 
}

void KTViewCamera::exportDialog()
{
    KTExportWidget exportWidget(project, this);
    exportWidget.exec();
}
