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

#include "esframe.h"
#include "../ktoon.h"

//--------------- CONSTRUCTOR --------------------

ESFrame::ESFrame( const QString &initial_text, QWidget *parent, QWidget *grandparent )
    : QLabel( initial_text, parent )
{
    Q_CHECK_PTR( parent );
    Q_CHECK_PTR( grandparent );

    //Initializations
    setFont( QFont( "helvetica", 7 ) );
    setFrameStyle( QFrame::Panel | QFrame::Raised );
    setLineWidth( 2 );
    resize( 70, 25 );
    parent_widget = parent;
    grandparent_widget = grandparent;
    is_used = false;
    is_selected = false;
    is_locked = false;
    is_motion = false;
    has_drawing = false;
    k_toon = ( Ktoon * )( grandparent_widget -> parentWidget() );

    description = new QLineEdit( this );
    description -> resize( 66, 21 );
    description -> move( 2, 2 );
    description -> hide();
    connect( description, SIGNAL( lostFocus() ), SLOT( slotSetDescription() ) );
    connect( description, SIGNAL( returnPressed() ), SLOT( slotSetDescription() ) );

    right_click_menu = new QPopupMenu( this );
    right_click_menu -> setFont( QFont( "helvetica", 10 ) );
    id_rename = right_click_menu -> insertItem( tr( "Rename Frame" ), this, SLOT( slotSendDoubleClickEvent() ) );
    id_remove = right_click_menu -> insertItem( tr( "Remove this Frame" ), grandparent, SLOT( slotRemoveFrame() ) );
    id_lock = right_click_menu -> insertItem( tr( "Lock this Frame" ), grandparent, SLOT( slotLockFrame() ) );
    id_copy = right_click_menu -> insertItem( tr( "Copy this Frame" ), grandparent, SLOT( slotCopyFrame() ) );
    id_paste = right_click_menu -> insertItem( tr( "Paste into this Frame" ), grandparent, SLOT( slotPasteFrame() ) );
}

//--------------- DESTRUCTOR --------------------

ESFrame::~ESFrame()
{
    delete right_click_menu;
}

//-------------- PUBLIC MEMBERS ----------------

bool ESFrame::isUsed()
{
    return is_used;
}

bool ESFrame::isSelected()
{
    return is_selected;
}

bool ESFrame::isLocked()
{
    return is_locked;
}

bool ESFrame::isMotion()
{
    return is_motion;
}

bool ESFrame::hasDrawing()
{
    return has_drawing;
}

void ESFrame::setUsed( bool in_is_used )
{
    is_used = in_is_used;

    if ( is_selected == true && in_is_used == true )
    {
        setPaletteBackgroundColor( *( k_toon -> esSelectAndUseColor() ) );
	setPaletteForegroundColor( *( k_toon -> esDefaultTextColor() ) );
    }
    else if ( is_selected == true && in_is_used == false )
    {
        setPaletteBackgroundColor( *( k_toon -> esSelectionColor() ) );
	setPaletteForegroundColor( *( k_toon -> esTextColorWhenSelected() ) );
    }
    else if ( is_selected == false && in_is_used == true )
    {
        setPaletteBackgroundColor( *( k_toon -> esUseColor() ) );
	setPaletteForegroundColor( *( k_toon -> esDefaultTextColor() ) );
    }
    else
    {
        setPaletteBackgroundColor( *( k_toon -> esDefaultColor() ) );
	setPaletteForegroundColor( *( k_toon -> esDefaultTextColor() ) );
    }
}

void ESFrame::setSelected( bool in_is_selected )
{
    is_selected = in_is_selected;

    if ( in_is_selected == true && is_used == true && is_locked == false )
    {
        setPaletteBackgroundColor( *( k_toon -> esSelectAndUseColor() ) );
	setPaletteForegroundColor( *( k_toon -> esDefaultTextColor() ) );
    }
    else if ( in_is_selected == true && is_used == false && is_locked == false )
    {
        setPaletteBackgroundColor( *( k_toon -> esSelectionColor() ) );
	setPaletteForegroundColor( *( k_toon -> esTextColorWhenSelected() ) );
    }
    else if ( in_is_selected == false && is_used == true && is_locked == false )
    {
        setPaletteBackgroundColor( *( k_toon -> esUseColor() ) );
	setPaletteForegroundColor( *( k_toon -> esDefaultTextColor() ) );
    }
    else if ( in_is_selected == false && is_used == false && is_locked == false )
    {
        setPaletteBackgroundColor( *( k_toon -> esDefaultColor() ) );
	setPaletteForegroundColor( *( k_toon -> esDefaultTextColor() ) );
    }
    else if ( is_locked && in_is_selected )
    {
        setPaletteBackgroundColor( *( k_toon -> esLockAndSelectColor() ) );
	setPaletteForegroundColor( *( k_toon -> esDefaultTextColor() ) );
    }
    else if ( is_locked && !in_is_selected )
    {
        setPaletteBackgroundColor( *( k_toon -> esLockColor() ) );
	setPaletteForegroundColor( *( k_toon -> esDefaultTextColor() ) );
    }
}

void ESFrame::setLocked( bool in_is_locked )
{
    if ( is_used )
    {
        is_locked = in_is_locked;

	if ( is_selected && is_locked )
	{
	    setPaletteBackgroundColor( *( k_toon -> esLockAndSelectColor() ) );
	    setPaletteForegroundColor( *( k_toon -> esDefaultTextColor() ) );
	}
	else if ( is_selected && !is_locked )
	{
	    setPaletteBackgroundColor( *( k_toon -> esSelectionColor() ) );
	    setPaletteForegroundColor( *( k_toon -> esDefaultTextColor() ) );
	}
	else if ( !is_selected && is_locked )
	{
	    setPaletteBackgroundColor( *( k_toon -> esLockColor() ) );
	    setPaletteForegroundColor( *( k_toon -> esDefaultTextColor() ) );
	}
	else
	{
	    setPaletteBackgroundColor( *( k_toon -> esUseColor() ) );
	    setPaletteForegroundColor( *( k_toon -> esDefaultTextColor() ) );
	}
    }
}

void ESFrame::setMotion( bool in_is_motion )
{
    is_motion = in_is_motion;
    update();
}

void ESFrame::setHasDrawing( bool in_has_drawing )
{
    has_drawing = in_has_drawing;
    update();
}

void ESFrame::setName( const QString &new_name )
{
    description -> setText( new_name );
    setText( new_name );
}

void ESFrame::clearTextfieldFocus()
{
    description -> clearFocus();
}

void ESFrame::setAllProperties( ESFrame *in_esframe )
{
    setUsed( in_esframe -> isUsed() );
    setLocked( in_esframe -> isLocked() );
    setText( in_esframe -> text() );
    setSelected( in_esframe -> isSelected() );
    setMotion( in_esframe -> isMotion() );
    setHasDrawing( in_esframe -> hasDrawing() );
}

//-------------- SLOTS --------------------

void ESFrame::slotSetDescription()
{
    setText( description -> text() );
    description -> hide();
    emit renamed( text() );
}

void ESFrame::slotSendDoubleClickEvent()
{
    QMouseEvent mouse_event( QEvent::MouseButtonDblClick, QPoint( x(), y() ), Qt::LeftButton, 0 );
    QApplication::sendEvent( this, &mouse_event );
}

//-------------- EVENTS AND PROTECTED MEMBERS --------------

void ESFrame::mousePressEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    if ( !is_used )
    {
        is_selected = true;
	setPaletteBackgroundColor( *( k_toon -> esSelectionColor() ) );
	setPaletteForegroundColor( *( k_toon -> esTextColorWhenSelected() ) );
    }
    else if ( is_used && !is_locked )
    {
        is_selected = true;
	setPaletteBackgroundColor( *( k_toon -> esSelectAndUseColor() ) );
	setPaletteForegroundColor( *( k_toon -> esDefaultTextColor() ) );
    }
    else if ( is_locked )
    {
        is_selected = true;
	setPaletteBackgroundColor( *( k_toon -> esLockAndSelectColor() ) );
	setPaletteForegroundColor( *( k_toon -> esDefaultTextColor() ) );
    }

    emit selected();

    if ( mouse_event -> button() == Qt::RightButton && is_used )
    {
        right_click_menu -> setItemVisible( id_rename, true );
        right_click_menu -> setItemVisible( id_remove, true );
        right_click_menu -> setItemVisible( id_lock, true );
        right_click_menu -> setItemVisible( id_copy, true );
        right_click_menu -> setItemVisible( id_paste, true );
        right_click_menu -> exec( QCursor::pos() );
    }
    else if ( mouse_event -> button() == Qt::RightButton && !is_used )
    {
        right_click_menu -> setItemVisible( id_rename, false );
        right_click_menu -> setItemVisible( id_remove, false );
        right_click_menu -> setItemVisible( id_lock, false );
        right_click_menu -> setItemVisible( id_copy, false );
        right_click_menu -> setItemVisible( id_paste, true );
        right_click_menu -> exec( QCursor::pos() );
    }

    mouse_event -> accept();
}

void ESFrame::mouseDoubleClickEvent( QMouseEvent *mouse_event )
{
   Q_CHECK_PTR( mouse_event );
   if ( is_used == true && mouse_event -> button() == Qt::LeftButton )
    {
        description -> show();
        description -> setText( text() );
        description -> setFocus();
	mouse_event -> accept();
    }
    else
        mouse_event -> ignore();
}

void ESFrame::drawContents( QPainter *painter )
{
    Q_CHECK_PTR( painter );
    if ( has_drawing )
        painter -> setPen( *( k_toon -> esHasDrawingColor() ) );
    else
        painter -> setPen( *( k_toon -> esDefaultTextColor() ) );
    painter -> drawText( 12, 16, text() );
    if ( is_motion )
    {
    	painter -> setPen( QPen( QColor( 120, 120, 120 ), 2 ) );
	painter -> drawLine( 4, 14, 4, 24 );
	painter -> drawLine( 4, 14, 8, 14 );
    }
}
