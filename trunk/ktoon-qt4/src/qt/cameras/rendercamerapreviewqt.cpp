/***************************************************************************
 *   Copyright (C) 2004 by Fernando Jose Roldan Correa                     *
 *   froldan@toonka.com                                                    *
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

#include "rendercamerapreviewqt.h"
//Added by qt3to4:
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>

//--------------- CONSTRUCTOR --------------

RenderCameraPreviewQt::RenderCameraPreviewQt( QWidget *parent, QWidget *grandparent, Q3Canvas *share )
    : Q3CanvasView( share, parent ), parent_widget( parent ), grandparent_widget( grandparent )
{

}

//--------------- DESTRUCTOR ---------------

RenderCameraPreviewQt::~RenderCameraPreviewQt()
{

}

//--------------- PUBLIC MEMBERS --------------

//-------------- EVENTS AND PROTECTED MEMBERS -------------

void RenderCameraPreviewQt::contentsMousePressEvent( QMouseEvent *mouse_event )
{
    mouse_event -> accept();
}

void RenderCameraPreviewQt::contentsMouseMoveEvent( QMouseEvent *mouse_event )
{
    mouse_event -> accept();
}

void RenderCameraPreviewQt::contentsMouseReleaseEvent( QMouseEvent *mouse_event )
{
    mouse_event -> accept();
}

void RenderCameraPreviewQt::contentsMouseDoubleClickEvent( QMouseEvent *mouse_event )
{
    mouse_event -> accept();
}

void RenderCameraPreviewQt::paintEvent( QPaintEvent *paint_event )
{
    if ( paint_event -> erased() )
	updateContents( paint_event -> rect() );
}

void RenderCameraPreviewQt::keyPressEvent( QKeyEvent *key_event )
{
    key_event -> accept();
}
