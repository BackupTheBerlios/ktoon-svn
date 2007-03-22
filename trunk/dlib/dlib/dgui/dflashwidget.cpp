/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#include "dflashwidget.h"

#include <QtDebug>
#include <QMouseEvent>
#include <QPainter>


struct DFlashWidget::Private
{
	QString movie;
	QProcess *process;
	bool isOk;
};

DFlashWidget::DFlashWidget(const QString &swf, QWidget *parent) : QX11EmbedContainer(parent), d(new Private)
{
	setWindowTitle(tr("Flashing"));
	
	d->movie = swf;
	d->isOk = true;
	
// 	setAttribute(Qt::WA_OpaquePaintEvent, true);
	
	d->process = new QProcess(this);
	
	connect(d->process, SIGNAL(started ()), this, SLOT(updateSize()));
// 	d->process->setEnvironment(QStringList()<<"SESSION_MANAGER=\"\"");
	
	resize(640, 480); // FIXME
}


DFlashWidget::~DFlashWidget()
{
	d->process->kill();
	d->process->waitForFinished();
	
	delete d;
}

void DFlashWidget::play()
{
	QStringList args;

	args << QStringList() << "-x" << QString::number(winId()) << "-s" << "1.5" << d->movie;

	d->process->start("gnash", args);
	
	d->process->waitForStarted();
	
	if ( d->process->error() == QProcess::FailedToStart || d->process->state() == QProcess::NotRunning)
	{
		qWarning("Please install gnash");
		d->isOk = false;
	}
	else
	{
		d->isOk = true;
	}
	repaint();
}

void DFlashWidget::stop()
{
	d->process->terminate();
	d->process->waitForFinished();
}

void DFlashWidget::mousePressEvent(QMouseEvent *e)
{
	if ( e->button() == Qt::RightButton )
	{
		emit contextMenu( mapToGlobal( e->pos() ));
	}
	
	e->ignore();
}

void DFlashWidget::mouseDoubleClickEvent( QMouseEvent *e)
{
	stop();
	play();
	
	e->ignore();
}

void DFlashWidget::updateSize()
{
	// TODO: Update the widget size
}

void DFlashWidget::paintEvent (QPaintEvent *e)
{
	QX11EmbedContainer::paintEvent(e);
	
	if ( !d->isOk )
	{
		QPainter painter(this);
		
		QString text = tr("Please install gnash from http://gnash.org");
		
		QFontMetrics fm(painter.font());
		
		QRect r = fm.boundingRect(text);
		
		QPoint pos = (rect().center() - r.topLeft())/2;
		
		painter.drawText(pos, text);
	}
}

#endif // Q_WS_X11
