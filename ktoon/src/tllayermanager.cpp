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

#include <qtooltip.h>
#include <qapplication.h>
#include <qwmatrix.h>

#include "tllayermanager.h"
#include "images.h"

//--------------- CONSTRUCTOR --------------------

TLLayerManager::TLLayerManager( QWidget *parent, QWidget *grandparent )
    : QWidget( parent )
{
    //Initializations
    resize( 200, 150 );
    parent_widget = parent;
    grandparent_widget = grandparent;
    eye_status = EYE_DESELECT_ALL;
    lock_status = LOCK_ALL;
    square_status = SQUARE_SELECT_ALL;
    current_time = 0.0;
    total_time = 0.04;

    //------------- Icon initializations --------------------

    i_eye = QPixmap( show_hide_all_layers_xpm );
    i_lock = QPixmap( kilit_pic_xpm );
    i_square = QPixmap( outline_pic_xpm );
    i_insert_layer = QPixmap( layerplus_xpm );
    i_remove_layer = QPixmap( layerminus_xpm );
    i_move_layer_up = QPixmap( arrowup_xpm );
    i_move_layer_down = QPixmap( arrowdown_xpm );

    //---------- Top Panel ---------------

    utils = new QFrame( this );
    utils -> setFrameStyle( QFrame::Box | QFrame::Plain );
    utils -> resize( 192, 24 );
    utils -> setMinimumSize( 192, 24 );
    utils -> move( 0, 0 );

    square = new QPushButton( i_square, tr( "" ), utils );
    square -> setFlat( true );
    square -> setAutoDefault( false );
    square -> resize( 20, 20 );
    square -> move( utils -> width() - square -> width() - 22, 2 );
    connect( square, SIGNAL( clicked() ), SLOT( slotSquare() ) );
    QToolTip::add( square, tr( "Show only outlines" ) );

    lock = new QPushButton( i_lock, tr( "" ), utils );
    lock -> setFlat( true );
    lock -> setAutoDefault( false );
    lock -> resize( 20, 20 );
    lock -> move( square -> x() - lock -> width() - 2, 2 );
    connect( lock, SIGNAL( clicked() ), SLOT( slotLock() ) );
    QToolTip::add( lock, tr( "Lock all Layers" ) );

    eye = new QPushButton( i_eye, tr( "" ), utils );
    eye -> setFlat( true );
    eye -> setAutoDefault( false );
    eye -> resize( 20, 20 );
    eye -> move( lock -> x() - eye -> width() - 2, 2 );
    connect( eye, SIGNAL( clicked() ), SLOT( slotEye() ) );
    QToolTip::add( eye, tr( "Show / Hide all Layers" ) );

    //Layer Sequence Initialization
    layer_sequence = new TLLayerSequence( this );
    layer_sequence -> move( 0, utils -> height() );
    layer_sequence -> resize( 200, 90 );
    layer_sequence -> resizeContents( 200, 90 );
    connect( this, SIGNAL( frameSelected( TLFrameSequence * ) ), layer_sequence, SLOT( slotSelectLayerFromFrameSelected( TLFrameSequence * ) ) );
    connect( layer_sequence, SIGNAL( layerSelected( TLLayer* ) ), SIGNAL( layerSelected( TLLayer* ) ) );
    connect( layer_sequence, SIGNAL( layerReleasedAbove( int, int ) ), SIGNAL( layerReleasedAbove( int, int ) ) );
    connect( layer_sequence, SIGNAL( layerReleasedBelow( int, int ) ), SIGNAL( layerReleasedBelow( int, int ) ) );

    //---------- Bottom Panel ---------------

    utils2 = new QFrame( this );
    utils2 -> setFrameStyle( QFrame::Box | QFrame::Plain );
    utils2 -> resize( 192, 24 );
    utils2 -> setMinimumSize( 192, 24 );
    utils2 -> move( 0, layer_sequence -> height() + layer_sequence -> y() );

    insert_layer = new QPushButton( i_insert_layer, tr( "" ), utils2 );
    insert_layer -> setFlat( true );
    insert_layer -> setAutoDefault( false );
    insert_layer -> resize( 20, 20 );
    insert_layer -> move( 2, 2 );
    connect( insert_layer, SIGNAL( clicked() ), layer_sequence, SLOT( slotInsertLayer() ) );
    connect( insert_layer, SIGNAL( clicked() ), grandparent, SIGNAL( insertLayerClicked() ) );
    QToolTip::add( insert_layer, tr( "Insert Layer" ) );

    remove_layer = new QPushButton( i_remove_layer, tr( "" ), utils2 );
    remove_layer -> setFlat( true );
    remove_layer -> setAutoDefault( false );
    remove_layer -> resize( 20, 20 );
    remove_layer -> move( insert_layer -> x() + insert_layer -> width() + 2, 2 );
    connect( remove_layer, SIGNAL( clicked() ), layer_sequence, SLOT( slotRemoveLayer() ) );
    connect( remove_layer, SIGNAL( clicked() ), grandparent, SIGNAL( removeLayerClicked() ) );
    QToolTip::add( remove_layer, tr( "Remove Layer" ) );

    move_layer_up = new QPushButton( i_move_layer_up, tr( "" ), utils2 );
    move_layer_up -> setFlat( true );
    move_layer_up -> setAutoDefault( false );
    move_layer_up -> resize( 20, 20 );
    move_layer_up -> move( remove_layer -> x() + remove_layer -> width() + 2, 2 );
    connect( move_layer_up, SIGNAL( clicked() ), layer_sequence, SLOT( slotMoveLayerUp() ) );
    connect( move_layer_up, SIGNAL( clicked() ), grandparent, SIGNAL( layerMovedUp() ) );
    QToolTip::add( move_layer_up, tr( "Move Layer Up" ) );

    move_layer_down = new QPushButton( i_move_layer_down, tr( "" ), utils2 );
    move_layer_down -> setFlat( true );
    move_layer_down -> setAutoDefault( false );
    move_layer_down -> resize( 20, 20 );
    move_layer_down -> move( move_layer_up -> x() + move_layer_up -> width() + 2, 2 );
    connect( move_layer_down, SIGNAL( clicked() ), layer_sequence, SLOT( slotMoveLayerDown() ) );
    connect( move_layer_down, SIGNAL( clicked() ), grandparent, SIGNAL( layerMovedDown() ) );
    QToolTip::add( move_layer_down, tr( "Move Layer Down" ) );

    time = new QLabel( QString::number( current_time, 'f', 2 ) + " / " + QString::number( total_time, 'f', 2 ), utils2 );
    time -> setFont( QFont( "helvetica", 7 ) );
    time -> setAlignment( Qt::AlignCenter );
    time -> resize( 80, 20 );
    time -> move( utils2 -> width() - time -> width(), 2 );
}

//--------------- DESTRUCTOR --------------------

TLLayerManager::~TLLayerManager()
{

}

//-------------- PUBLIC MEMBERS ----------------

QScrollBar *TLLayerManager::verticalScrollBar()
{
    return layer_sequence -> verticalScrollBar();
}

TLLayerSequence *TLLayerManager::layerSequence()
{
    return layer_sequence;
}

QPushButton *TLLayerManager::insertLayerButton()
{
    return insert_layer;
}

QPushButton *TLLayerManager::removeLayerButton()
{
    return remove_layer;
}

void TLLayerManager::setAllLayersVisible( bool visible )
{
    TLLayer *layer_iterator;
    QPtrList<TLLayer> layer_list = layer_sequence -> listOfLayers();

    if ( visible )
    {
        for ( layer_iterator = layer_list.first(); layer_iterator; layer_iterator = layer_list.next() )
	    layer_iterator -> setVisible( true );
	eye_status = EYE_DESELECT_ALL;
    }
    else
    {
        for ( layer_iterator = layer_list.first(); layer_iterator; layer_iterator = layer_list.next() )
	    layer_iterator -> setVisible( false );
        eye_status = EYE_SELECT_ALL;
    }
}

//-------------- SLOTS --------------------

void TLLayerManager::slotSetTotalTime( float new_time )
{
    total_time = ( double )new_time;
    time -> setText( QString::number( current_time, 'f', 2 ) + " / " + QString::number( total_time, 'f', 2 ) );
}

void TLLayerManager::slotSetCurrentTime( float new_time )
{
    current_time = ( double )new_time;
    time -> setText( QString::number( current_time, 'f', 2 ) + " / " + QString::number( total_time, 'f', 2 ) );
}

void TLLayerManager::slotEye()
{
    TLLayer *layer_iterator;
    QPtrList<TLLayer> layer_list = layer_sequence -> listOfLayers();

    if ( eye_status == EYE_SELECT_ALL )
    {
        for ( layer_iterator = layer_list.first(); layer_iterator; layer_iterator = layer_list.next() )
	    layer_iterator -> setVisible( true );
	eye_status = EYE_DESELECT_ALL;
    }
    else
    {
        for ( layer_iterator = layer_list.first(); layer_iterator; layer_iterator = layer_list.next() )
	    layer_iterator -> setVisible( false );
        eye_status = EYE_SELECT_ALL;
    }
}

void TLLayerManager::slotLock()
{
    TLLayer *layer_iterator;
    QPtrList<TLLayer> layer_list = layer_sequence -> listOfLayers();

    if ( lock_status == LOCK_ALL )
    {
        for ( layer_iterator = layer_list.first(); layer_iterator; layer_iterator = layer_list.next() )
	    layer_iterator -> setLocked( true );
	lock_status = UNLOCK_ALL;
    }
    else
    {
        for ( layer_iterator = layer_list.first(); layer_iterator; layer_iterator = layer_list.next() )
	    layer_iterator -> setLocked( false );
        lock_status = LOCK_ALL;
    }
}

void TLLayerManager::slotSquare()
{
    TLLayer *layer_iterator;
    QPtrList<TLLayer> layer_list = layer_sequence -> listOfLayers();

    if ( square_status == SQUARE_SELECT_ALL )
    {
        for ( layer_iterator = layer_list.first(); layer_iterator; layer_iterator = layer_list.next() )
	    layer_iterator -> seeOnlyOutlines( true );
	square_status = SQUARE_DESELECT_ALL;
    }
    else
    {
        for ( layer_iterator = layer_list.first(); layer_iterator; layer_iterator = layer_list.next() )
	    layer_iterator -> seeOnlyOutlines( false );
        square_status = SQUARE_SELECT_ALL;
    }
}

//-------------- EVENTS AND PROTECTED MEMBERS --------------

//Event for widget resize control
void TLLayerManager::resizeEvent( QResizeEvent *resize_event )
{
    QSize new_size = resize_event -> size();

    utils -> resize( new_size.width(), utils -> height() );
    square -> move( utils -> width() - square -> width() - 22, 2 );
    lock -> move( square -> x() - lock -> width() - 2, 2 );
    eye -> move( lock -> x() - eye -> width() - 2, 2 );

    utils2 -> resize( new_size.width(), utils2 -> height() );
    utils2 -> move( 0, new_size.height() - utils2 -> height() );

    layer_sequence -> resize( new_size.width(), new_size.height() - 48 );

    update();

    emit widthChanged( new_size.width() );
}
