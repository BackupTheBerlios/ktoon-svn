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

#include <qapplication.h>
#include <qcursor.h>

#include "eslayer.h"

//--------------- CONSTRUCTOR --------------------

ESLayer::ESLayer( const QString &initial_text, QWidget *parent, QWidget *grandparent )
    : QPushButton( initial_text, parent )
{
    Q_CHECK_PTR( parent );
    Q_CHECK_PTR( grandparent );

    //Initializations
    resize( 70, 25 );
    setAutoDefault( false );
    setFocusPolicy( QWidget::NoFocus );
//     setFont( QFont( "helvetica", 10 ) );
    parent_widget = parent;
    grandparent_widget = grandparent;
    default_color = paletteBackgroundColor();
    selection_color = QColor( 210, 210, 255 );

    description = new QLineEdit( this );
    description -> resize( 66, 21 );
    description -> move( 2, 2 );
    description -> hide();
    connect( description, SIGNAL( lostFocus() ), SLOT( slotSetDescription() ) );
    connect( description, SIGNAL( returnPressed() ), SLOT( slotSetDescription() ) );

    right_click_menu = new QPopupMenu( this );
//     right_click_menu -> setFont( QFont( "helvetica", 10 ) );
    right_click_menu -> insertItem( tr( "Rename Layer" ), this, SLOT( slotSendDoubleClickEvent() ) );
    right_click_menu -> insertItem( tr( "Remove this Layer" ), grandparent, SLOT( slotRemoveLayer() ) );
    right_click_menu -> insertSeparator();
    right_click_menu -> insertItem( tr( "Insert Frames" ), grandparent, SLOT( slotInsertFrame() ) );
    right_click_menu -> insertItem( tr( "Remove Frames" ), grandparent, SLOT( slotRemoveFrame() ) );
}

//--------------- DESTRUCTOR --------------------

ESLayer::~ESLayer()
{
    delete right_click_menu;
    delete description;
}

//-------------- PUBLIC MEMBERS ----------------

bool ESLayer::isSelected()
{
    return is_selected;
}

void ESLayer::setSelected( bool in_is_selected )
{
    is_selected = in_is_selected;

    if ( is_selected )
        setPaletteBackgroundColor( selection_color );
    else
        setPaletteBackgroundColor( default_color );
}

void ESLayer::clearTextfieldFocus()
{
    description -> clearFocus();
}

//-------------- SLOTS --------------------

void ESLayer::slotSetDescription()
{
    setText( description -> text() );
    description -> hide();
    emit renamed( text() );
}

void ESLayer::slotSendDoubleClickEvent()
{
    QMouseEvent mouse_event( QEvent::MouseButtonDblClick, QPoint( x(), y() ), Qt::LeftButton, 0 );
    QApplication::sendEvent( this, &mouse_event );
}

//-------------- EVENTS AND PROTECTED MEMBERS --------------

void ESLayer::mousePressEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );

    is_selected = true;
    setPaletteBackgroundColor( selection_color );
    description -> setText( text() );

    emit clicked();

    if ( mouse_event -> button() == Qt::RightButton )
        right_click_menu -> exec( QCursor::pos() );

    mouse_event -> accept();
}

void ESLayer::mouseDoubleClickEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );

    if ( mouse_event -> button() == Qt::LeftButton )
    {
        description -> show();
        description -> setText( text() );
        description -> setFocus();
	mouse_event -> accept();
    }
    else
        mouse_event -> ignore();
}
