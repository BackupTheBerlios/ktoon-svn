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

#include <QtGlobal> 
#ifdef Q_WS_X11

#include "kterm.h"
#include <QApplication>

KTerm::KTerm(QWidget *w) : QX11EmbedContainer(w)
{
    m_process = new QProcess(this);
    connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(closeTerm(int, QProcess::ExitStatus)));
}

KTerm::~KTerm()
{
    m_process->kill();
    m_process->waitForFinished();
}

QSize KTerm::sizeHint() const
{
    QSize size(400,300);
    
    return size.expandedTo(QApplication::globalStrut());
}

void KTerm::showTerm()
{
    QStringList args;

    args << QStringList() << "-into" << QString::number(winId()) << "-bg" << palette().color(QPalette::Background).name() << "-fg" << palette().color(QPalette::Foreground).name();

    m_process->start("xterm", args);
    
    if (m_process->error() == QProcess::FailedToStart)
        qWarning("-> Please install xterm first");
}

void KTerm::closeTerm(int ec, QProcess::ExitStatus s)
{
    emit termClosed();
    close();
}

#endif // Q_WS_X11
