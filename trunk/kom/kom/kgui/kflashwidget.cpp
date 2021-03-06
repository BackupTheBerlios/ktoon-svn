/***************************************************************************
 *   Project KOM: KToon Open Media library                                 *
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

#include "kflashwidget.h"

#ifdef Q_WS_X11

#include <QtGlobal>
#include <QtDebug>
#include <QMouseEvent>
#include <QPainter>

struct KFlashWidget::Private
{
    QString movie;
    QProcess *process;
    bool isOk;
};

KFlashWidget::KFlashWidget(const QString &swf, QWidget *parent) : QX11EmbedContainer(parent), k(new Private)
{
    setWindowTitle(tr("Flashing"));
    
    k->movie = swf;
    k->isOk = true;
    
    // setAttribute(Qt::WA_OpaquePaintEvent, true);
    
    k->process = new QProcess(this);
    
    connect(k->process, SIGNAL(started ()), this, SLOT(updateSize()));
    // k->process->setEnvironment(QStringList()<<"SESSION_MANAGER=\"\"");
    
    resize(640, 480); // FIXME
}

KFlashWidget::~KFlashWidget()
{
    k->process->kill();
    k->process->waitForFinished();
    
    delete k;
}

void KFlashWidget::play()
{
    QStringList args;

    args << QStringList() << "-x" << QString::number(winId()) << "-s" << "1.5" << k->movie;

    k->process->start("gnash", args);
    
    k->process->waitForStarted();
    
    if (k->process->error() == QProcess::FailedToStart || k->process->state() == QProcess::NotRunning) {
        qWarning("Please install gnash");
        k->isOk = false;
    } else {
        k->isOk = true;
    }

    repaint();
}

void KFlashWidget::stop()
{
    k->process->terminate();
    k->process->waitForFinished();
}

void KFlashWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
        emit contextMenu(mapToGlobal(e->pos()));
    
    e->ignore();
}

void KFlashWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    stop();
    play();
    
    e->ignore();
}

void KFlashWidget::updateSize()
{
    // TODO: Update the widget size
}

void KFlashWidget::paintEvent(QPaintEvent *e)
{
    QX11EmbedContainer::paintEvent(e);
    
    if (!k->isOk) {

        QPainter painter(this);
        QString text = tr("Please install gnash from http://gnash.org");
        QFontMetrics fm(painter.font());
        QRect r = fm.boundingRect(text);
        QPoint pos = (rect().center() - r.topLeft())/2;
        
        painter.drawText(pos, text);

    }
}

#endif // Q_WS_X11
