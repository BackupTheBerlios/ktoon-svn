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

#include "soundwidget.h"
#include "tlframesequence.h"

#include <qcolor.h>
#include <qpainter.h>

//--------------- CONSTRUCTOR --------------------

SoundWidget::SoundWidget( QWidget *parent ) : QGridView( parent )
{
    parent_widget = parent;
    resize( 3000, 25 );
    setCellWidth( 10 );
    setCellHeight( 21 );
    setNumRows( 1 );
    setNumCols( TLFrameSequence::TL_MAX_NUMBER_OF_FRAMES );
    setHScrollBarMode( QScrollView::AlwaysOff );
    default_color = QColor( 255, 255, 255 );
    border_color = QColor( 180, 180, 180 );
    special_color = QColor( 200, 200, 200 );
    waveform_color = QColor( 0, 0, 255 );
    offset_color = QColor( 255, 0, 0 );
    drag_offset_color = QColor( 0, 0, 0 );
    offset_cell = 1;
    drag_offset_cell = 1;
}

//--------------- DESTRUCTOR -------------------

SoundWidget::~SoundWidget()
{

}

//------------- PUBLIC MEMBERS ---------------

//------------ SLOTS ----------------

void SoundWidget::slotMoveSoundWidget( int value )
{
    move( value * -10, y() );
}

void SoundWidget::slotChangeOffset( int current_offset )
{
    int prev_offset_cell = offset_cell;
    int prev_drag_offset_cell = drag_offset_cell;
    drag_offset_cell = -1;
    offset_cell = current_offset;
    updateCell( 0, prev_drag_offset_cell - 1 );
    updateCell( 0, prev_offset_cell - 1 );
    updateCell( 0, offset_cell - 1 );
}

void SoundWidget::slotChangeDragOffset( int current_offset )
{
    int prev_drag_offset_cell = drag_offset_cell;
    int prev_offset_cell = offset_cell;
    offset_cell = -1;
    drag_offset_cell = current_offset;
    updateCell( 0, prev_offset_cell - 1 );
    updateCell( 0, prev_drag_offset_cell - 1 );
    updateCell( 0, drag_offset_cell - 1 );
}

//------------ EVENTS AND PROTECTED MEMBERS ------------

void SoundWidget::mousePressEvent( QMouseEvent *mouse_event )
{
    mouse_event -> accept();
}

void SoundWidget::paintCell( QPainter *painter, int, int col )
{
    painter -> setPen( border_color );
    if ( ( col + 1 ) % 5 == 0 )
        painter -> setBrush( special_color );
    else
        painter -> setBrush( default_color );
    painter -> drawRect( 0, 0, cellWidth(), cellHeight() );

    if ( drag_offset_cell - 1 == col )
    {
	painter -> setPen( drag_offset_color );
	painter -> drawLine( cellWidth() / 2, 0, cellWidth() / 2, cellHeight() );
    }

    if ( offset_cell - 1 == col )
    {
	painter -> setPen( offset_color );
	painter -> drawLine( cellWidth() / 2, 0, cellWidth() / 2, cellHeight() );
    }
}
