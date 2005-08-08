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

#include "tllayer.h"
#include "tllayersequence.h"
#include "tllayermanager.h"
#include "images.h"

//--------------- CONSTRUCTOR --------------------

TLLayer::TLLayer( int in_position, QWidget *parent, QWidget *grandparent, const QString &initial_name )
    : QFrame( parent )
{
    Q_CHECK_PTR( parent );
    Q_CHECK_PTR( grandparent );
    
    //Initializations
    setMinimumSize( 192, 24 );
    setFrameStyle( QFrame::Panel | QFrame::Raised );
    setLineWidth( 2 );
    position_ = in_position;
    parent_widget = parent;
    grandparent_widget = grandparent;
    is_being_edited = false;
    is_visible = true;
    is_locked = false;
    is_selected = false;
    is_in_drag_way = false;
    dragging = false;
    default_color = paletteBackgroundColor();
    text_default_color = paletteForegroundColor();
    selection_color = QColor( 0, 0, 0 );
    text_selection_color = QColor( 255, 255, 255 );
    drag_color = QColor( 255, 255, 255 );
    right_click_menu = NULL;

    //Icon Initializations
    i_static_layer_image = QPixmap( layer_pic_xpm );
    i_edition_image = QPixmap( written_pic_xpm );
    i_enabled = QPixmap( enable_xpm );
    i_disabled = QPixmap( disable_xpm );

    //----------- Main Component Initializations -------------

    static_layer_image = new QLabel( this );
    static_layer_image -> setPixmap( i_static_layer_image );
    static_layer_image -> resize( 20, 20 );
    static_layer_image -> move( 2, 2 );

    layer_name = new QLabel( initial_name, this );
    layer_name -> resize( 50, 20 );
    layer_name -> setFont( QFont( font().family(), 9 ) );
    layer_name -> move( static_layer_image -> x() + static_layer_image -> width(), 2 );

    edit_name = new QLineEdit( this );
    edit_name -> resize( 50, 20 );
    edit_name -> setFont( QFont( font().family(), 9 ) );
    edit_name -> move( layer_name -> x(), 2 );
    edit_name -> hide();
    connect( edit_name, SIGNAL( lostFocus() ), SLOT( slotEditName() ) );
    connect( edit_name, SIGNAL( returnPressed() ), SLOT( slotEditName() ) );

    only_outlines = new QCheckBox( this );
    only_outlines -> resize( 20, 20 );
    only_outlines -> move( width() - only_outlines -> width() - 22, 2 );
    connect( only_outlines, SIGNAL( clicked() ), SLOT( slotOnlyOutlines() ) );

    lock_image = new QLabel( this );
    lock_image -> setPixmap( i_disabled );
    lock_image -> resize( 20, 20 );
    lock_image -> move( only_outlines -> x() - lock_image -> width() - 2, 2 );

    visibility_image = new QLabel( this );
    visibility_image -> setPixmap( i_enabled );
    visibility_image -> resize( 20, 20 );
    visibility_image -> move( lock_image -> x() - visibility_image -> width() - 2, 2 );

    edition_image = new QLabel( this );
    edition_image -> resize( 20, 20 );
    edition_image -> move( visibility_image -> x() - edition_image -> width() - 2, 2 );
}

//--------------- DESTRUCTOR --------------------

TLLayer::~TLLayer()
{
    delete static_layer_image;
    delete layer_name;
    delete edition_image;
    delete visibility_image;
    delete lock_image;
    delete only_outlines;
    delete edit_name;
    
    if ( right_click_menu != NULL )
        delete right_click_menu;
}

//-------------- PUBLIC MEMBERS ----------------

int TLLayer::position()
{
    return position_;
}

QString TLLayer::name()
{
    return layer_name -> text();
}

bool TLLayer::isBeingEdited()
{
    return is_being_edited;
}

bool TLLayer::isLocked()
{
    return is_locked;
}

bool TLLayer::isVisible()
{
    return is_visible;
}

bool TLLayer::isSelected()
{
    return is_selected;
}

bool TLLayer::isInDragWay()
{
    return is_in_drag_way;
}

void TLLayer::setPosition( int in_position )
{
    position_ = in_position;
}

void TLLayer::setName( const QString &new_name )
{
    layer_name -> setText( new_name );
    edit_name -> setText( new_name );
}

void TLLayer::setEdited( bool in_is_being_edited )
{
    is_being_edited = in_is_being_edited;

    if ( is_being_edited )
        edition_image -> setPixmap( i_edition_image );
    else
        edition_image -> setPixmap( QString::null );
}

void TLLayer::setLocked( bool in_is_locked )
{
    is_locked = in_is_locked;

    if ( is_locked )
        lock_image -> setPixmap( i_enabled );
    else
        lock_image -> setPixmap( i_disabled );
}

void TLLayer::setVisible( bool in_is_visible )
{
    is_visible = in_is_visible;

    if ( is_visible )
        visibility_image -> setPixmap( i_enabled );
    else
        visibility_image -> setPixmap( i_disabled );

    emit visibilityChanged( is_visible );
}

void TLLayer::setSelected( bool in_is_selected )
{
    is_selected = in_is_selected;

    if ( is_selected )
    {
    	layer_name -> setPaletteForegroundColor( text_selection_color );
    	layer_name -> setPaletteBackgroundColor( selection_color );
	setPaletteBackgroundColor( selection_color );
    }
    else
    {
    	layer_name -> setPaletteForegroundColor( text_default_color );
    	layer_name -> setPaletteBackgroundColor( default_color );
	setPaletteBackgroundColor( default_color );
    }
}

void TLLayer::setInDragWay( bool in_is_in_drag_way )
{
    is_in_drag_way = in_is_in_drag_way;

    if ( is_in_drag_way )
    {
    	layer_name -> setPaletteForegroundColor( text_default_color );
    	layer_name -> setPaletteBackgroundColor( drag_color );
	setPaletteBackgroundColor( drag_color );
    }
    else if ( is_selected )
    {
    	layer_name -> setPaletteForegroundColor( text_selection_color );
    	layer_name -> setPaletteBackgroundColor( selection_color );
	setPaletteBackgroundColor( selection_color );
    }
    else
    {
    	layer_name -> setPaletteForegroundColor( text_default_color );
    	layer_name -> setPaletteBackgroundColor( default_color );
	setPaletteBackgroundColor( default_color );
    }
}

void TLLayer::clearTextfieldFocus()
{
    edit_name -> clearFocus();
}

void TLLayer::seeOnlyOutlines( bool outline_property )
{
    only_outlines -> setChecked( outline_property );
}

//-------------- SLOTS --------------------

void TLLayer::slotEditName()
{
    layer_name -> setText( edit_name -> text() );
    edit_name -> hide();
    emit renamed( layer_name -> text() );
}

void TLLayer::slotOnlyOutlines()
{
   setSelected( true );
   setEdited( true );

   emit selected();
}

void TLLayer::slotSendDoubleClickEvent()
{
    QMouseEvent mouse_event( QEvent::MouseButtonDblClick, QPoint( 25, 5 ), Qt::LeftButton, 0 );
    QApplication::sendEvent( this, &mouse_event );
}

//-------------- EVENTS AND PROTECTED MEMBERS --------------

void TLLayer::resizeEvent( QResizeEvent *resize_event )
{
    Q_CHECK_PTR( resize_event );
    QSize new_size = resize_event -> size();

    layer_name -> resize( new_size.width() - 142, layer_name -> height() );
    edit_name -> resize( new_size.width() - 142, layer_name -> height() );

    only_outlines -> move( width() - only_outlines -> width() - 22, 2 );
    lock_image -> move( only_outlines -> x() - lock_image -> width() - 2, 2 );
    visibility_image -> move( lock_image -> x() - visibility_image -> width() - 2, 2 );
    edition_image -> move( visibility_image -> x() - edition_image -> width() - 2, 2 );
}

void TLLayer::mousePressEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    setSelected( true );
    setEdited( true );

    if ( childAt( mouse_event -> pos() ) == visibility_image )
        setVisible( !is_visible );
    else if ( childAt( mouse_event -> pos() ) == lock_image )
        setLocked( !is_locked );

    if ( mouse_event -> button() == Qt::LeftButton )
    {
        dragging = true;
    	TLLayerSequence *ls_parent = ( TLLayerSequence * )grandparent_widget;
    	ls_parent -> setDragging( true );
    }
    emit selected();

    if ( mouse_event -> button() == Qt::RightButton )
    {
        TLLayerManager *lm = ( TLLayerManager * )( grandparent_widget -> parentWidget() );
	if ( right_click_menu != NULL )
	    delete right_click_menu;
        right_click_menu = new QPopupMenu( this );
// 	right_click_menu -> setFont( QFont( "helvetica", 10 ) );
        right_click_menu -> insertItem( tr( "Rename Layer" ), this, SLOT( slotSendDoubleClickEvent() ) );
        right_click_menu -> insertSeparator();
        right_click_menu -> insertItem( tr( "Insert Layer" ), lm -> insertLayerButton(), SLOT( animateClick() ) );
        right_click_menu -> insertItem( tr( "Delete Layer" ), lm -> removeLayerButton(), SLOT( animateClick() ) );
        right_click_menu -> exec( QCursor::pos() );
    }
    mouse_event -> accept();
}

void TLLayer::mouseMoveEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    if ( dragging )
    {
	TLLayerSequence *ls_parent = ( TLLayerSequence * )grandparent_widget;

	if ( mouse_event -> y() > height() && position_ < ls_parent -> numberOfLayers() )
	    emit draggedBelow( mouse_event -> y() - height() );
	else if ( mouse_event -> y() < 0 && position_ > 1 )
	    emit draggedAbove( mouse_event -> y() );
    	mouse_event -> accept();
    }
    else
    	mouse_event -> ignore();
}

void TLLayer::mouseReleaseEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    dragging = false;
    TLLayerSequence *ls_parent = ( TLLayerSequence * )grandparent_widget;
    ls_parent -> setDragging( false );

    if ( mouse_event -> y() > height() && position_ < ls_parent -> numberOfLayers() )
        emit releasedBelow( mouse_event -> y() - height() );
    else if ( mouse_event -> y() < 0 && position_ > 1 )
        emit releasedAbove( mouse_event -> y() );

    mouse_event -> accept();
}

void TLLayer::mouseDoubleClickEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    if ( childAt( mouse_event -> pos() ) == layer_name && mouse_event -> button() == Qt::LeftButton )
    {
    	edit_name -> setText( layer_name -> text() );
    	edit_name -> show();
    	edit_name -> setFocus();
	mouse_event -> accept();
    }
    else
        mouse_event -> ignore();
}
