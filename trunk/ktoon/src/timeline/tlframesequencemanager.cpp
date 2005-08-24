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

#include "tlframesequencemanager.h"
#include "../images/images.h"

#include <qwmatrix.h>
#include <qtooltip.h>

//--------------- CONSTRUCTOR --------------------

TLFrameSequenceManager::TLFrameSequenceManager( QWidget *parent, QWidget *grandparent )
    : QWidget( parent )
{
    Q_CHECK_PTR( parent );
    Q_CHECK_PTR( grandparent );
    
    //Initializations
    resize( 590, 150 );
    parent_widget = parent;
    grandparent_widget = grandparent;
    max_value_frame = 1;

    //Work with pixmaps
    QPixmap tmp( sound_widget_xpm );
    QWMatrix m;
    m.scale( 0.6, 0.6 );
    i_show_sound_widget = tmp.xForm( m );

    //Ruler initialization
    ruler = new TLRuler( this );
    ruler -> move( 0, 0 );

    //Frame Layout Initialization
    frame_layout = new TLFrameSequenceLayout( this );
    frame_layout -> move( 0, ruler -> height() );
    frame_layout -> resize( 590, 90 );
    frame_layout -> resizeContents( 590, 90 );
    
    frame_layout->setPaletteBackgroundColor(Qt::blue); // FIXME: remove this
    
    connect( frame_layout, SIGNAL( frameSelected( TLFrameSequence * ) ), SIGNAL( frameSelected( TLFrameSequence * ) ) );
    connect( this, SIGNAL( layerSelected( TLLayer* ) ), frame_layout, SLOT( slotConsequencesOfALayerSelection( TLLayer* ) ) );
    connect( this, SIGNAL( layerReleasedAbove( int, int ) ), frame_layout, SLOT( slotSwapFrameSequences( int, int ) ) );
    connect( this, SIGNAL( layerReleasedBelow( int, int ) ), frame_layout, SLOT( slotSwapFrameSequences( int, int ) ) );
    connect( grandparent, SIGNAL( insertLayerClicked() ), frame_layout, SLOT( slotInsertFrameSequence() ) );
    connect( grandparent, SIGNAL( removeLayerClicked() ), frame_layout, SLOT( slotRemoveFrameSequence() ) );
    connect( grandparent, SIGNAL( removeLayerClicked() ), frame_layout, SLOT( slotUpdateMaxUsedFrames() ) );
    connect( grandparent, SIGNAL( layerMovedUp() ), frame_layout, SLOT( slotMoveFrameSequenceUp() ) );
    connect( grandparent, SIGNAL( layerMovedDown() ), frame_layout, SLOT( slotMoveFrameSequenceDown() ) );
    connect( frame_layout, SIGNAL( maxUsedFramesUpdated( int ) ), ruler, SLOT( slotUpdateMaxOffset( int ) ) );
    connect( ruler, SIGNAL( offsetChanged( int ) ), frame_layout, SLOT( slotChangeOffset( int ) ) );
    connect( ruler, SIGNAL( offsetDragged( int ) ), frame_layout, SLOT( slotChangeDragOffset( int ) ) );

    //------- Bottom Panel --------------

    utils = new QFrame( this );
    utils -> setFrameStyle( QFrame::Box | QFrame::Plain );
    utils -> resize( 590, 24 );
    utils -> move( 0, frame_layout -> y() + frame_layout -> height() );

    show_sound_widget = new QPushButton( i_show_sound_widget, tr( "" ), utils );
    show_sound_widget -> resize( 20, 20 );
    show_sound_widget -> move( 7, 2 );
    show_sound_widget -> setToggleButton( true );
    show_sound_widget -> setAutoDefault( false );
    show_sound_widget -> setFlat( true );
    connect( show_sound_widget, SIGNAL( toggled( bool ) ), SLOT( slotShowSoundWidget( bool ) ) );
    QToolTip::add( show_sound_widget, tr( "Shows or hides the sound widget" ) );

    horizontal_scroll_bar = new QScrollBar( 0, TLFrameSequence::TL_MAX_NUMBER_OF_FRAMES - 59, 1, 5, 0, Qt::Horizontal, utils );
    horizontal_scroll_bar -> resize( 525, 18 );
    horizontal_scroll_bar -> move( 60, 4 );
//     horizontal_scroll_bar -> setPaletteBackgroundColor( QColor( 239, 237, 223 ) );
    connect( horizontal_scroll_bar, SIGNAL( valueChanged( int ) ), SLOT( slotMoveRulerAndFrames() ) );
}

//--------------- DESTRUCTOR --------------------

TLFrameSequenceManager::~TLFrameSequenceManager()
{
    delete ruler;
    delete show_sound_widget;
    delete utils;
    delete frame_layout;
}

//-------------- PUBLIC MEMBERS ----------------

TLFrameSequenceLayout *TLFrameSequenceManager::frameLayout()
{
    return frame_layout;
}

QScrollBar *TLFrameSequenceManager::verticalScrollBar()
{
    return frame_layout -> verticalScrollBar();
}

QScrollBar *TLFrameSequenceManager::horizontalScrollBar()
{
    return horizontal_scroll_bar;
}

TLRuler *TLFrameSequenceManager::getRuler()
{
    return ruler;
}

QPushButton *TLFrameSequenceManager::soundButton()
{
    return show_sound_widget;
}

//-------------- SLOTS --------------------

void TLFrameSequenceManager::slotMoveRulerAndFrames()
{
    ruler -> move( horizontal_scroll_bar -> value() * -10, ruler -> y() );
    frame_layout -> moveFrames( horizontal_scroll_bar -> value() * -10 );
}

void TLFrameSequenceManager::slotShowSoundWidget( bool show )
{
    if ( show )
        grandparent_widget -> resize( grandparent_widget -> width(), grandparent_widget -> height() + 45 );
    else
        grandparent_widget -> resize( grandparent_widget -> width(), grandparent_widget -> height() - 45 );
}

//-------------- EVENTS AND PROTECTED MEMBERS --------------

void TLFrameSequenceManager::resizeEvent( QResizeEvent *resize_event )
{
    QSize new_size = resize_event -> size();

    utils -> resize( new_size.width(), utils -> height() );
    utils -> move( 0, new_size.height() - utils -> height() );

    horizontal_scroll_bar -> resize( new_size.width() - 65, horizontal_scroll_bar -> height() );

    frame_layout -> resize( new_size.width(), new_size.height() - 48 );

    update();

    emit widthChanged( new_size.width() );
}
