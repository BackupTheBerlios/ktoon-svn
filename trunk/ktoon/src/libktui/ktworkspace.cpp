/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include "ktworkspace.h"

#include <QMouseEvent>
#include <QDropEvent>

#include <kcore/kdebug.h>

#include <QLinearGradient>

#include <QBoxLayout>

KTWorkspace::KTWorkspace(QWidget *parent) : QWorkspace(parent)
{
	setAcceptDrops( true );
	setMouseTracking(true);
	
#if 0
	QPalette pal = palette();
	
	QLinearGradient gradient(0, 0, width(), height());
	gradient.setColorAt(1, pal.background().color());
	gradient.setColorAt(0.5, pal.foreground().color());
	gradient.setColorAt(0, pal.highlight().color());
		
	gradient.setSpread(QGradient::ReflectSpread);
	
	setBackground(gradient);
#endif

// 	QBoxLayout *layout = new QBoxLayout(QBoxLayout::BottomToTop, this);
// 	
// 	KTCameraBar *bar = new KTCameraBar;
// 
// 	layout->addWidget(bar,2,Qt::AlignBottom | Qt::AlignHCenter);
// 	bar->show();
}


KTWorkspace::~KTWorkspace()
{
}


void KTWorkspace::mousePressEvent(QMouseEvent *e)
{
	if ( e->button () == Qt::RightButton )
	{
		emit contextMenu( mapToGlobal(e->pos()) );
	}
}

void KTWorkspace::mouseMoveEvent(QMouseEvent *e)
{
}

void KTWorkspace::mouseReleaseEvent(QMouseEvent *e)
{
}

// void KTWorkspace::dropEvent( QDropEvent *event )
// {
// 	if (event->mimeData()->hasColor())
// 	{
// 		QColor color = qvariant_cast<QColor>(event->mimeData()->colorData());
// 		
// 		QPalette pal = palette();
// 		pal.setColor(QPalette::Dark, color);
// 		
// 		dDebug() << "Co: " << color;
// 		setPalette(pal);
// 		
// 		event->acceptProposedAction();
// 	} 
// 	else 
// 	{
// 		event->ignore();
// 	}
// }
// 
// void KTWorkspace::dragEnterEvent( QDragEnterEvent *event )
// {
// 	setFocus();
// 
// 	if (event->mimeData()->hasColor()) 
// 	{
// 		event->acceptProposedAction();
// 	} 
// 	else 
// 	{
// 		event->ignore();
// 	}
// }


