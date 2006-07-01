/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado   *
 *   krawek@gmail.com   *
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

#include <QtGlobal> 
#ifdef Q_WS_X11

#include "dterm.h"
#include <QApplication>


DTerm::DTerm(QWidget *w) : QX11EmbedContainer(w)
{
	m_process = new QProcess(this);
	
	connect(m_process, SIGNAL(finished ( int, QProcess::ExitStatus)), this, SLOT(closeTerm(int, QProcess::ExitStatus)));
	
}


DTerm::~DTerm()
{
	m_process->kill();
	m_process->waitForFinished();
}

QSize DTerm::sizeHint() const
{
	QSize size(400,300);
	
	return size.expandedTo(QApplication::globalStrut());
}

void DTerm::showTerm()
{
	QStringList args;

	args << QStringList() << "-into" << QString::number(winId()) << "-bg" << palette().color(QPalette::Background).name() << "-fg" << palette().color(QPalette::Foreground).name();

	m_process->start("xterm", args);
	
	if( m_process->error() == QProcess::FailedToStart )
	{
		qWarning("-> Please install xterm first");
	}
}

void DTerm::closeTerm(int ec, QProcess::ExitStatus s)
{
	emit termClosed();
	close();
}

#endif // Q_WS_X11


