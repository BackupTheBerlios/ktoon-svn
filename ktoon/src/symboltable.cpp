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

#include "symboltable.h"
#include "library.h"

#include <qcursor.h>

//--------------- CONSTRUCTOR --------------

SymbolTable::SymbolTable( QWidget *parent ) : QListView( parent, "", Qt::WStyle_NoBorder )
{
    Q_CHECK_PTR( parent );
    parent_widget = parent;
    dragging = false;
    valid_drag = false;
    dragged_item = NULL;
}

//--------------- DESTRUCTOR ---------------

SymbolTable::~SymbolTable()
{

}

//--------------- PUBLIC MEMBERS --------------

//---------------- SLOTS -----------------

//-------------- EVENTS AND PROTECTED MEMBERS -------------

void SymbolTable::contentsMousePressEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    QListView::contentsMousePressEvent( mouse_event );
    mouse_event -> accept();
    dragging = true;
}

void SymbolTable::contentsMouseMoveEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    QListView::contentsMouseMoveEvent( mouse_event );
    if ( dragging )
    {
        mouse_event -> accept();
	dragged_item = ( SymbolItem * )selectedItem();
        if ( contentsToViewport( mouse_event -> pos() ).y() > viewport() -> height() )
	    scrollBy( 0, dragged_item -> height() );
        else if ( contentsToViewport( mouse_event -> pos() ).y() < 0 )
	    scrollBy( 0, ( dragged_item -> height() ) * -1 );
	SymbolItem *item_to_process;
	if ( ( item_to_process = ( SymbolItem * )itemAt( contentsToViewport( mouse_event -> pos() ) ) ) != dragged_item )
	{
	    if ( item_to_process && item_to_process -> getKind() == SymbolItem::FOLDER )
	    {
	        valid_drag = true;
	        setCursor( Qt::PointingHandCursor );
	    }
	    else
	    {
	        valid_drag = false;
	        setCursor( Qt::ForbiddenCursor );
	    }
	}
	else
	{
	    valid_drag = false;
	    setCursor( Qt::ForbiddenCursor );
	}
    }
    else
        mouse_event -> ignore();
}

void SymbolTable::contentsMouseReleaseEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    QListView::contentsMouseReleaseEvent( mouse_event );
    mouse_event -> accept();
    dragging = false;
    setCursor( Qt::ArrowCursor );
    if ( valid_drag )
    {
        SymbolItem *folder = ( SymbolItem * )itemAt( contentsToViewport( mouse_event -> pos() ) );
	folder -> insertItem( dragged_item );
	triggerUpdate();
	emit released( dragged_item );
    }
    valid_drag = false;
}

void SymbolTable::keyPressEvent( QKeyEvent *key_event )
{
    Q_CHECK_PTR( key_event );
    switch( key_event -> key() )
    {
	case Qt::Key_Delete: ( ( Library * )parent_widget ) -> slotDeleteSymbol();
	    key_event -> accept();
	    break;
	default: key_event -> ignore();
    }
}
