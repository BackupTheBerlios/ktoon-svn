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

#include <qcursor.h>
#include <qapplication.h>

#include "tlframe.h"
#include "../ktoon.h"

//--------------- CONSTRUCTOR --------------------

TLFrame::TLFrame( QWidget *parent )
    : QFrame( parent )
{
    Q_CHECK_PTR( parent );
    
    //Initializations
    resize( 10, 24 );
    setFrameStyle( QFrame::MenuBarPanel | QFrame::Plain );
    parent_widget = parent;
    is_special = false;
    is_selected = false;
    is_used = false;
    is_last = false;
    is_key = false;
    is_offset = false;
    is_drag_offset = false;
    is_unknown_motion = false;
    is_motion = false;
    has_drawing = false;
    
    k_toon = ( Ktoon * )( parent_widget -> parentWidget() -> parentWidget() -> parentWidget() -> parentWidget() -> parentWidget() -> parentWidget() -> parentWidget() );
    setPaletteBackgroundColor( *( k_toon -> tlDefaultColor() ) );

    right_click_menu = new QPopupMenu( this );
    right_click_menu -> setFont( QFont( "helvetica", 10 ) );
    right_click_menu -> insertItem( tr( "Create Motion Tween" ), parent, SLOT( slotCreateMotionTween() ) );
    right_click_menu -> insertItem( tr( "Remove Motion Tween" ), parent, SLOT( slotRemoveMotionTween() ) );
    right_click_menu -> insertSeparator();
    right_click_menu -> insertItem( tr( "Add Frames" ), parent, SLOT( slotInsertFrame() ) );
    right_click_menu -> insertItem( tr( "Remove Frames" ), parent, SLOT( slotRemoveFrame() ) );
}

TLFrame::TLFrame( TLFrame *in_frame )
{
    Q_CHECK_PTR( in_frame );
    
    QFrame( in_frame -> parentWidget() );
    resize( 10, 24 );
    setFrameStyle( QFrame::MenuBarPanel | QFrame::Plain );
    parent_widget = in_frame -> parentWidget();
    is_special = in_frame -> isSpecial();
    is_selected = in_frame -> isSelected();
    is_used = in_frame -> isUsed();
    is_last = in_frame -> isLast();
    is_key = in_frame -> isKey();
    is_offset = in_frame -> isOffset();
    is_unknown_motion = in_frame -> isUnknownMotion();
    is_motion = in_frame -> isMotion();
    is_drag_offset = in_frame -> isDragOffset();
    has_drawing = in_frame -> hasDrawing();
    
    k_toon = ( Ktoon * )( parent_widget -> parentWidget() -> parentWidget() -> parentWidget() -> parentWidget() -> parentWidget() -> parentWidget() -> parentWidget() );
    setPaletteBackgroundColor( *( k_toon -> tlDefaultColor() ) );

    right_click_menu = new QPopupMenu( this );
    right_click_menu -> setFont( QFont( "helvetica", 10 ) );
    right_click_menu -> insertItem( tr( "Create Motion Tween" ), parent_widget, SLOT( slotCreateMotionTween() ) );
    right_click_menu -> insertItem( tr( "Remove Motion Tween" ), parent_widget, SLOT( slotRemoveMotionTween() ) );
    right_click_menu -> insertSeparator();
    right_click_menu -> insertItem( tr( "Add Frames" ), parent_widget, SLOT( slotInsertFrame() ) );
    right_click_menu -> insertItem( tr( "Remove Frames" ), parent_widget, SLOT( slotRemoveFrame() ) );
}

//--------------- DESTRUCTOR --------------------

TLFrame::~TLFrame()
{
    delete right_click_menu;
}

//-------------- PUBLIC MEMBERS ----------------

bool TLFrame::isSpecial()
{
    return is_special;
}

bool TLFrame::isSelected()
{
    return is_selected;
}

bool TLFrame::isUsed()
{
    return is_used;
}

bool TLFrame::isLast()
{
    return is_last;
}

bool TLFrame::isKey()
{
    return is_key;
}

bool TLFrame::isOffset()
{
    return is_offset;
}

bool TLFrame::isDragOffset()
{
    return is_drag_offset;
}

bool TLFrame::isUnknownMotion()
{
    return is_unknown_motion;
}

bool TLFrame::isMotion()
{
    return is_motion;
}

bool TLFrame::hasDrawing()
{
    return has_drawing;
}

void TLFrame::setSpecial( bool in_is_special )
{
    is_special = in_is_special;
    update();
}

void TLFrame::setSelected( bool in_is_selected )
{
    is_selected = in_is_selected;
    update();
}

void TLFrame::setUsed( bool in_is_used )
{
    is_used = in_is_used;
    if ( !is_used )
    {
	setKey( false );
	setLast( false );
	setHasDrawing( false );
    }
    update();
}

void TLFrame::setLast( bool in_is_last )
{
    is_last = in_is_last;
    if ( is_last )
    	setUsed( true );
    update();
}

void TLFrame::setKey( bool in_is_key )
{
    is_key = in_is_key;
    if ( is_key )
    	setUsed( true );
    update();
}

void TLFrame::setOffset( bool in_is_offset )
{
    is_offset = in_is_offset;
    update();
}

void TLFrame::setDragOffset( bool in_is_drag_offset )
{
    is_drag_offset = in_is_drag_offset;
    update();
}

void TLFrame::setUnknownMotion( bool in_is_unknown_motion )
{
    is_unknown_motion = in_is_unknown_motion;
    if ( is_unknown_motion )
    {
        setUsed( true );
	setMotion( false );
    }
    update();
}

void TLFrame::setMotion( bool in_is_motion )
{
    is_motion = in_is_motion;
    if ( is_motion )
    {
        setUsed( true );
	setUnknownMotion( false );
    }
    update();
}

void TLFrame::setHasDrawing( bool in_has_drawing )
{
    has_drawing = in_has_drawing;
    if ( has_drawing )
        setUsed( true );
    update();
}

void TLFrame::setUseProperties( TLFrame *in_frame )
{
    Q_CHECK_PTR( in_frame );
    setUsed( in_frame -> isUsed() );
    setKey( in_frame -> isKey() );
    setLast( in_frame -> isLast() );
    setUnknownMotion( in_frame -> isUnknownMotion() );
    setMotion( in_frame -> isMotion() );
    setHasDrawing( in_frame -> hasDrawing() );
}

//-------------- SLOTS --------------------

//-------------- EVENTS AND PROTECTED MEMBERS --------------

void TLFrame::mousePressEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    setSelected( true );
    mouse_event -> accept();
    emit selected();
    if ( mouse_event -> button() == Qt::RightButton )
        right_click_menu -> exec( QCursor::pos() );
}

void TLFrame::paintEvent( QPaintEvent *paint_event )
{
    Q_CHECK_PTR( paint_event );
    if ( paint_event -> erased() )
    {
    	p.begin( this );
    	
	//Default paint settings
    	p.setPen( *( k_toon -> tlBorderColor() ) );
    	p.setBrush( *( k_toon -> tlDefaultColor() ) );
    	p.drawRect( 0, 0, 10, 24 );

    	if ( is_selected )
	    paintSelected();
    	else
	    paintNotSelected();

    	//Draw the offset line if it applies
    	if ( is_offset )
        {
	    p.setPen( *( k_toon -> tlOffsetColor() ) );
	    p.drawLine( width() / 2, 0, width() / 2, 23 );
        }

    	//Draw the drag offset line if it applies
    	if ( is_drag_offset )
    	{
	    p.setPen( *( k_toon -> tlDragOffsetColor() ) );
	    p.drawLine( width() / 2, 0, width() / 2, 23 );
        }

        p.end();
    }
}

void TLFrame::paintNotSelected()
{
    if ( !is_used )
    {
        if ( is_special )
	{
	    p.setPen( *( k_toon -> tlBorderColor() ) );
	    p.setBrush( *( k_toon -> tlSpecialColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	}
    }
    else if ( is_used )
    {
	//*** (has_drawing == false)
        if ( !is_key && !is_last && !is_unknown_motion && !is_motion && !has_drawing )
	{
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.drawLine( 0, 0, 9, 0 );
	    p.drawLine( 0, 23, 9, 23 );
	    p.setPen( *( k_toon -> tlDefaultColor() ) );
	    p.drawLine( 0, 1, 0, 22 );
	    p.drawLine( 9, 1, 9, 22 );
	}
	if ( is_key && is_last && !is_unknown_motion && !is_motion && !has_drawing )
	{
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.setBrush( *( k_toon -> tlDefaultColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.drawEllipse( 1, 12, 7, 7 );
	}
	if ( is_key && !is_last && !is_unknown_motion && !is_motion && !has_drawing )
	{
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.drawLine( 0, 0, 9, 0 );
	    p.drawLine( 0, 23, 9, 23 );
	    p.drawLine( 0, 0, 0, 23 );
	    p.drawEllipse( 1, 12, 7, 7 );
	    p.setPen( *( k_toon -> tlDefaultColor() ) );
	    p.drawLine( 9, 1, 9, 22 );
	}
	if ( !is_key && is_last && !is_unknown_motion && !is_motion && !has_drawing )
	{
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.drawLine( 0, 0, 9, 0 );
	    p.drawLine( 0, 23, 9, 23 );
	    p.drawLine( 9, 0, 9, 23 );
	    p.drawRect( 2, 10, 6, 10 );
	    p.setPen( *( k_toon -> tlDefaultColor() ) );
	    p.drawLine( 0, 1, 0, 22 );
	}
	//*** (has_drawing == true)
        if ( !is_key && !is_last && !is_unknown_motion && !is_motion && has_drawing )
	{
	    p.setPen( *( k_toon -> tlDrawingColor() ) );
	    p.setBrush( *( k_toon -> tlDrawingColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.drawLine( 0, 0, 9, 0 );
	    p.drawLine( 0, 23, 9, 23 );
	}
	if ( is_key && is_last && !is_unknown_motion && !is_motion && has_drawing )
	{
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.setBrush( *( k_toon -> tlDrawingColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setBrush( *( k_toon -> tlUseBorderColor() ) );
	    p.drawEllipse( 1, 12, 7, 7 );
	}
	if ( is_key && !is_last && !is_unknown_motion && !is_motion && has_drawing )
	{
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.setBrush( *( k_toon -> tlDrawingColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setBrush( *( k_toon -> tlUseBorderColor() ) );
	    p.drawEllipse( 1, 12, 7, 7 );
	    p.setPen( *( k_toon -> tlDrawingColor() ) );
	    p.drawLine( 9, 1, 9, 22 );
	}
	if ( !is_key && is_last && !is_unknown_motion && !is_motion && has_drawing )
	{
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.setBrush( *( k_toon -> tlDrawingColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setPen( *( k_toon -> tlDrawingColor() ) );
	    p.drawLine( 0, 1, 0, 22 );
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.drawRect( 2, 10, 6, 10 );
	}
	//***
        if ( !is_key && !is_last && is_unknown_motion && !is_motion )
	{
	    p.setPen( *( k_toon -> tlMotionColor() ) );
	    p.setBrush( *( k_toon -> tlMotionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.drawLine( 0, 0, 9, 0 );
	    p.drawLine( 0, 23, 9, 23 );
	    p.drawLine( 2, 14, 7, 14 );
	}
	if ( is_key && is_last && is_unknown_motion && !is_motion )
	{
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.setBrush( *( k_toon -> tlMotionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setBrush( *( k_toon -> tlUseBorderColor() ) );
	    p.drawEllipse( 1, 12, 7, 7 );
	}
	if ( is_key && !is_last && is_unknown_motion && !is_motion )
	{
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.setBrush( *( k_toon -> tlMotionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setBrush( *( k_toon -> tlUseBorderColor() ) );
	    p.drawEllipse( 1, 12, 7, 7 );
	    p.setPen( *( k_toon -> tlMotionColor() ) );
	    p.drawLine( 9, 1, 9, 22 );
	}
	if ( !is_key && is_last && is_unknown_motion && !is_motion )
	{
	    p.setPen( *( k_toon -> tlMotionColor() ) );
	    p.setBrush( *( k_toon -> tlMotionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.drawLine( 0, 0, 9, 0 );
	    p.drawLine( 0, 23, 9, 23 );
	    p.drawLine( 9, 0, 9, 23 );
	    p.drawLine( 2, 14, 7, 14 );
	}
	//***
        if ( !is_key && !is_last && !is_unknown_motion && is_motion )
	{
	    p.setPen( *( k_toon -> tlMotionColor() ) );
	    p.setBrush( *( k_toon -> tlMotionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.drawLine( 0, 0, 9, 0 );
	    p.drawLine( 0, 23, 9, 23 );
	    p.drawLine( 0, 14, 9, 14 );
	}
	if ( is_key && is_last && !is_unknown_motion && is_motion )
	{
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.setBrush( *( k_toon -> tlMotionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setBrush( *( k_toon -> tlUseBorderColor() ) );
	    p.drawEllipse( 1, 12, 7, 7 );
	}
	if ( is_key && !is_last && !is_unknown_motion && is_motion )
	{
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.setBrush( *( k_toon -> tlMotionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setBrush( *( k_toon -> tlUseBorderColor() ) );
	    p.drawEllipse( 1, 12, 7, 7 );
	    p.setPen( *( k_toon -> tlMotionColor() ) );
	    p.drawLine( 9, 1, 9, 22 );
	}
	if ( !is_key && is_last && !is_unknown_motion && is_motion )
	{
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.setBrush( *( k_toon -> tlMotionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setPen( *( k_toon -> tlMotionColor() ) );
	    p.drawLine( 0, 1, 0, 22 );
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.drawLine( 0, 14, 7, 14 );
	    p.drawLine( 7, 14, 5, 12 );
	    p.drawLine( 7, 14, 5, 16 );
	}
    }
}

void TLFrame::paintSelected()
{
    if ( !is_used )
    {
	p.setPen( *( k_toon -> tlBorderColor() ) );
	p.setBrush( *( k_toon -> tlSelectionColor() ) );
	p.drawRect( 0, 0, 10, 24 );
    }
    else if ( is_used )
    {
	//***
        if ( !is_key && !is_last && !is_unknown_motion && !is_motion )
	{
	    p.setBrush( *( k_toon -> tlSelectionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setPen( *( k_toon -> tlDefaultColor() ) );
	    p.drawLine( 0, 0, 9, 0 );
	    p.drawLine( 0, 23, 9, 23 );
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.drawLine( 0, 1, 0, 22 );
	    p.drawLine( 9, 1, 9, 22 );
	}
	if ( is_key && is_last && !is_unknown_motion && !is_motion )
	{
	    p.setPen( *( k_toon -> tlDefaultColor() ) );
	    p.setBrush( *( k_toon -> tlUseBorderColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.drawEllipse( 1, 12, 7, 7 );
	}
	if ( is_key && !is_last && !is_unknown_motion && !is_motion )
	{
	    p.setBrush( *( k_toon -> tlSelectionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setPen( *( k_toon -> tlDefaultColor() ) );
	    p.drawLine( 0, 0, 9, 0 );
	    p.drawLine( 0, 23, 9, 23 );
	    p.drawLine( 0, 0, 0, 23 );
	    p.drawEllipse( 1, 12, 7, 7 );
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.drawLine( 9, 1, 9, 22 );
	}
	if ( !is_key && is_last && !is_unknown_motion && !is_motion )
	{
	    p.setBrush( *( k_toon -> tlSelectionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setPen( *( k_toon -> tlDefaultColor() ) );
	    p.drawLine( 0, 0, 9, 0 );
	    p.drawLine( 0, 23, 9, 23 );
	    p.drawLine( 9, 0, 9, 23 );
	    p.drawRect( 2, 10, 6, 10 );
	    p.setPen( *( k_toon -> tlUseBorderColor() ) );
	    p.drawLine( 0, 1, 0, 22 );
	}
	//***
        if ( !is_key && !is_last && is_unknown_motion && !is_motion )
	{
	    p.setPen( *( k_toon -> tlSelectionColor() ) );
	    p.setBrush( *( k_toon -> tlSelectionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setPen( *( k_toon -> tlDefaultColor() ) );
	    p.drawLine( 0, 0, 9, 0 );
	    p.drawLine( 0, 23, 9, 23 );
	    p.drawLine( 2, 14, 7, 14 );
	}
	if ( is_key && is_last && ( ( is_unknown_motion && !is_motion ) || has_drawing ) )
	{
	    p.setPen( *( k_toon -> tlDefaultColor() ) );
	    p.setBrush( *( k_toon -> tlSelectionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setBrush( *( k_toon -> tlDefaultColor() ) );
	    p.drawEllipse( 1, 12, 7, 7 );
	}
	if ( is_key && !is_last && ( ( is_unknown_motion && !is_motion ) || has_drawing ) )
	{
	    p.setPen( *( k_toon -> tlDefaultColor() ) );
	    p.setBrush( *( k_toon -> tlSelectionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setBrush( *( k_toon -> tlDefaultColor() ) );
	    p.drawEllipse( 1, 12, 7, 7 );
	    p.setPen( *( k_toon -> tlSelectionColor() ) );
	    p.drawLine( 9, 1, 9, 22 );
	}
	if ( !is_key && is_last && is_unknown_motion && !is_motion )
	{
	    p.setPen( *( k_toon -> tlSelectionColor() ) );
	    p.setBrush( *( k_toon -> tlSelectionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setPen( *( k_toon -> tlDefaultColor() ) );
	    p.drawLine( 0, 0, 9, 0 );
	    p.drawLine( 0, 23, 9, 23 );
	    p.drawLine( 9, 0, 9, 23 );
	    p.drawLine( 2, 14, 7, 14 );
	}
	//***
        if ( !is_key && !is_last && !is_unknown_motion && is_motion )
	{
	    p.setPen( *( k_toon -> tlSelectionColor() ) );
	    p.setBrush( *( k_toon -> tlSelectionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setPen( *( k_toon -> tlDefaultColor() ) );
	    p.drawLine( 0, 0, 9, 0 );
	    p.drawLine( 0, 23, 9, 23 );
	    p.drawLine( 0, 14, 9, 14 );
	}
	if ( is_key && is_last && !is_unknown_motion && is_motion )
	{
	    p.setPen( *( k_toon -> tlDefaultColor() ) );
	    p.setBrush( *( k_toon -> tlSelectionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setBrush( *( k_toon -> tlDefaultColor() ) );
	    p.drawEllipse( 1, 12, 7, 7 );
	}
	if ( is_key && !is_last && !is_unknown_motion && is_motion )
	{
	    p.setPen( *( k_toon -> tlDefaultColor() ) );
	    p.setBrush( *( k_toon -> tlSelectionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setBrush( *( k_toon -> tlDefaultColor() ) );
	    p.drawEllipse( 1, 12, 7, 7 );
	    p.setPen( *( k_toon -> tlSelectionColor() ) );
	    p.drawLine( 9, 1, 9, 22 );
	}
	if ( !is_key && is_last && !is_unknown_motion && is_motion )
	{
	    p.setPen( *( k_toon -> tlDefaultColor() ) );
	    p.setBrush( *( k_toon -> tlSelectionColor() ) );
	    p.drawRect( 0, 0, 10, 24 );
	    p.setPen( *( k_toon -> tlSelectionColor() ) );
	    p.drawLine( 0, 1, 0, 22 );
	    p.setPen( *( k_toon -> tlDefaultColor() ) );
	    p.drawLine( 0, 14, 7, 14 );
	    p.drawLine( 7, 14, 5, 12 );
	    p.drawLine( 7, 14, 5, 16 );
	}
    }
}
