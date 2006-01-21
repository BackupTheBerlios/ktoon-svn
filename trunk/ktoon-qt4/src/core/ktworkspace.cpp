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

#include "ktdebug.h"

KTWorkspace::KTWorkspace(QWidget *parent) : QWorkspace(parent)
{
	setAcceptDrops( true );
}


KTWorkspace::~KTWorkspace()
{
}


void KTWorkspace::mousePressEvent(QMouseEvent *e)
{
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
// 		ktDebug() << "Co: " << color;
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


