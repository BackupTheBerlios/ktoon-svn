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

#include <qlabel.h>
#include <qtimer.h>

#include "tlruler.h"
#include "ktoon.h"

//--------------- CONSTRUCTOR --------------------

TLRuler::TLRuler( QWidget *parent )
    : QFrame( parent )
{
    //Initializations
    parent_widget = parent;
    offset = 1;
    max_offset = 1;
    dragging = false;
    playing = false;
    setFrameStyle( QFrame::Panel | QFrame::Raised );
    setLineWidth( 2 );
    resize( 3000, 24 );
    offset_outline_color = QColor( 255, 0, 0 );
    offset_fill_color = QColor( 255, 150, 150 );
    offset_drag_outline_color = QColor( 0, 0, 0 );
    offset_drag_fill_color = QColor( 50, 50, 50 );
    offset_rectangle = QRect( 1, 2, 9, 20 );
    offset_drag_rectangle = QRect( 1, 2, 9, 20 );
    k_toon = ( Ktoon * )( parent_widget -> parentWidget() -> parentWidget() -> parentWidget() );

    animation_timer = new QTimer( this );
    connect( animation_timer, SIGNAL( timeout() ), SLOT( slotStepForward() ) );
    connect( animation_timer, SIGNAL( timeout() ), SLOT( slotStopWhenReachEnd() ) );
}

//--------------- DESTRUCTOR --------------------

TLRuler::~TLRuler()
{

}

//-------------- PUBLIC MEMBERS ----------------

int TLRuler::getOffset()
{
    return offset;
}

int TLRuler::getMaxOffset()
{
    return max_offset;
}

bool TLRuler::isAnimationPlaying()
{
    return playing;
}

//-------------- SLOTS --------------------

void TLRuler::slotUpdateMaxOffset( int value )
{
    int old_max_offset = max_offset;
    max_offset = value;

    if ( max_offset < old_max_offset && offset_rectangle.x() == ( old_max_offset - 1 ) * 10 + 1 )
    {
        drawOffsetRectangle( ( max_offset - 1 ) * 10 + 1 );
	offset = max_offset;
    }

    emit offsetChanged( offset );
    emit maxOffsetChanged( max_offset );
}

void TLRuler::slotSetOffset( int offset_ )
{
    if ( offset_ > max_offset || offset_ < 1 )
        return;

    offset = offset_;
    drawOffsetRectangle( ( offset - 1 ) * 10 + 1 );

    emit offsetChanged( offset );
}

void TLRuler::slotPlay()
{
    int msec = 1000 / ( k_toon -> currentStatus() -> currentFrameRate() );
    playing = true;
    animation_timer -> start( msec, false );
}

void TLRuler::slotStop()
{
    playing = false;
    animation_timer -> stop();
}

void TLRuler::slotStopWhenReachEnd()
{
    if ( offset >= max_offset )
    {
        playing = false;
        animation_timer -> stop();
    }
}

void TLRuler::slotStepForward()
{
    slotSetOffset( offset + 1 );
}

void TLRuler::slotStepBackward()
{
    slotSetOffset( offset - 1 );
}

//-------------- EVENTS AND PROTECTED MEMBERS --------------

void TLRuler::mousePressEvent( QMouseEvent *mouse_event )
{
    dragging = true;

    int new_x = mouse_event -> x() - ( mouse_event -> x() ) % 10 + 1;

    if ( ( max_offset - 1 ) * 10 >= new_x )
    {
        drawOffsetDragRectangle( new_x );
	offset = ( ( mouse_event -> x() - ( mouse_event -> x() ) % 10 ) / 10 ) + 1;
    }
    else
    {
        drawOffsetDragRectangle( ( max_offset - 1 ) * 10 + 1 );
	offset = max_offset;
    }

    emit offsetDragged( offset );
}

void TLRuler::mouseMoveEvent( QMouseEvent *mouse_event )
{
    if ( dragging && mouse_event -> x() >= 0 )
    {
        int new_x = mouse_event -> x() - ( mouse_event -> x() ) % 10 + 1;

        if ( ( max_offset - 1 ) * 10 >= new_x )
        {
            drawOffsetDragRectangle( new_x );
	    offset = ( ( mouse_event -> x() - ( mouse_event -> x() ) % 10 ) / 10 ) + 1;
        }
        else
        {
            drawOffsetDragRectangle( ( max_offset - 1 ) * 10 + 1 );
	    offset = max_offset;
        }

	emit offsetDragged( offset );
    }
}

void TLRuler::mouseReleaseEvent( QMouseEvent *mouse_event )
{
    dragging = false;

    int new_x = mouse_event -> x() - ( mouse_event -> x() ) % 10 + 1;

    if ( mouse_event -> x() < 0 )
    {
	drawOffsetRectangle( 1 );
	offset = 1;
    }
    else if ( ( max_offset - 1 ) * 10 >= new_x )
    {
        drawOffsetRectangle( new_x );
	offset = ( ( mouse_event -> x() - ( mouse_event -> x() ) % 10 ) / 10 ) + 1;
    }
    else
    {
        drawOffsetRectangle( ( max_offset - 1 ) * 10 + 1 );
	offset = max_offset;
    }

    emit offsetChanged( offset );
}

void TLRuler::drawContents( QPainter *painter )
{
    //Variables used for iterate
    int i, j;

    painter -> setPen( QColor( 150, 150, 150 ) );

    //Draw the tiny marks at the top
    for ( i = 10; i <= width(); i = i + 10 )
    {
        QPoint origin = QPoint( i, 0 );
	QPoint destination = QPoint( i, 5 );
	painter -> drawLine( origin, destination );
    }

    //Draw the tiny marks at the bottom
    for ( i = 10; i <= width(); i = i + 10 )
    {
        QPoint origin = QPoint( i, height() );
	QPoint destination = QPoint( i, height() - 5 );
	painter -> drawLine( origin, destination );
    }

    painter -> setPen( QColor( 0, 0, 0 ) );
    painter -> setFont( QFont( "helvetica", 7 ) );

    //Print numbers, stepping 5 frames
    painter -> drawText( QPoint( 1, height() / 2 + 3 ), QString( "1" ) );
    for ( i = 40, j = 5; i <= width(); i = i + 50, j = j + 5 )
    {
        QString current_number;
	current_number.setNum( j );
        painter -> drawText( QPoint( i, height() / 2 + 3 ), current_number );
    }

    //Draw the initial offset rectangle
    drawOffsetRectangle( offset_rectangle.x() );
}

void TLRuler::drawOffsetRectangle( int x )
{
    QPainter p( this );

    //Clear the current offset rectangle
    p.eraseRect( offset_rectangle );

    //Redraw the number if the current offset rectangle erased it
    p.setFont( QFont( "helvetica", 7 ) );
    p.drawText( QPoint( 1, height() / 2 + 3 ), QString( "1" ) );
    if ( ( offset_rectangle.left() + 9 ) % 50 == 0 )
    {
        QString current_number;
	current_number.setNum( ( offset_rectangle.left() + 9 ) / 10 );
	p.setPen( QColor( 0, 0, 0 ) );
	p.drawText( QPoint( offset_rectangle.left(), height() / 2 + 3 ), current_number );
    }

    //Draw the new offset rectangle
    p.setPen( offset_outline_color );
    QBrush b( offset_fill_color, Qt::Dense4Pattern );
    p.setBrush( b );
    offset_rectangle.setRect( x, 2, 9, 20 );
    p.drawRect( offset_rectangle );
}

void TLRuler::drawOffsetDragRectangle( int x )
{
    QPainter p( this );

    //Clear the current offset rectangle
    p.eraseRect( offset_drag_rectangle );

    //Redraw the number if the current offset drag rectangle erased it
    p.setFont( QFont( "helvetica", 7 ) );
    p.drawText( QPoint( 1, height() / 2 + 3 ), QString( "1" ) );
    if ( ( offset_drag_rectangle.left() + 9 ) % 50 == 0 )
    {
        QString current_number;
	current_number.setNum( ( offset_drag_rectangle.left() + 9 ) / 10 );
	p.setPen( QColor( 0, 0, 0 ) );
	p.drawText( QPoint( offset_drag_rectangle.left(), height() / 2 + 3 ), current_number );
    }

    //Draw the new offset drag rectangle
    p.setPen( offset_drag_outline_color );
    QBrush b( offset_drag_fill_color, Qt::Dense4Pattern );
    p.setBrush( b );
    offset_drag_rectangle.setRect( x, 2, 9, 20 );
    p.drawRect( offset_drag_rectangle );
}
