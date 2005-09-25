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

#include "fillcolor.h"
#include "images.h"

//Added by qt3to4:
#include <QPixmap>
#include <QMouseEvent>

//--------------- CONSTRUCTOR --------------------

FillColor::FillColor( QWidget *parent ) : QFrame( parent )
{
    Q_CHECK_PTR( parent );

    resize( 47, 30 );
    parent_widget = parent;
    icon = QPixmap( fillcolor_xpm );
    setFrameStyle( QFrame::Panel | QFrame::Raised );
    active = false;
    current_color = QColor( 255, 255, 255 );
    square_border_color = QColor( 150, 150, 150 );
    dark_color = QColor( 135, 135, 135 );
    light_color = paletteBackgroundColor();
    alpha = 100;
}

//--------------- DESTRUCTOR -------------------

FillColor::~ FillColor()
{

}

//------------- PUBLIC MEMBERS ---------------

bool FillColor::isActive()
{
    return active;
}

QColor FillColor::currentColor()
{
    return current_color;
}

short int FillColor::getAlpha()
{
    return alpha;
}

void FillColor::setActive( bool state )
{
    active = state;

    if ( active )
        setFrameStyle( QFrame::Panel | QFrame::Sunken );
    else
        setFrameStyle( QFrame::Panel | QFrame::Raised );
}

//------------ SLOTS ----------------

void FillColor::slotSetColor( const QColor &new_color )
{
    if ( active )
    {
        current_color = new_color;
        update();
    }
}

void FillColor::slotChangeAlpha( int new_alpha )
{
    if ( active )
    {
        alpha = new_alpha;
	update();
    }
}

//------------ EVENTS AND PROTECTED MEMBERS ------------

void FillColor::drawContents( QPainter *painter )
{
    Q_CHECK_PTR( painter );
    QColor intermediate_dark_color, intermediate_light_color;
    intermediate_dark_color = QColor( ( int )( current_color.red() * alpha / 100 + dark_color.red() * ( ( 100 - alpha ) / 100 ) ),
    				      ( int )( current_color.green() * alpha / 100 + dark_color.green() * ( ( 100 - alpha ) / 100 ) ),
				      ( int )( current_color.blue() * alpha / 100 + dark_color.blue() * ( ( 100 - alpha ) ) / 100 ) );
    intermediate_light_color = QColor( ( int )( current_color.red() * alpha / 100 + light_color.red() * ( 100 - alpha ) / 100 ),
    				       ( int )( current_color.green() * alpha / 100 + light_color.green() * ( 100 - alpha ) / 100 ),
				       ( int )( current_color.blue() * alpha / 100 + light_color.blue() * ( 100 - alpha ) / 100 ) );

    if ( active )
    {
        QBrush b( Qt::white, Qt::Dense5Pattern );
        painter -> setPen( Qt::white );
        painter -> setBrush( b );
        painter -> drawRect( 0, 0, width(), height() );
    }

    painter -> drawPixmap( 1, 6, icon );

    painter -> setPen( square_border_color );
    painter -> setBrush( Qt::NoBrush );
    painter -> drawRect( 22, 4, 20, 20 );
    painter -> setPen( square_border_color );
    painter -> setBrush( intermediate_light_color );
    painter -> drawRect( 24, 6, 16, 16 );
    painter -> setPen( intermediate_dark_color );
    painter -> drawLine( 27, 7, 27, 20 );
    painter -> drawLine( 31, 7, 31, 20 );
    painter -> drawLine( 35, 7, 35, 20 );
    painter -> drawLine( 25, 10, 38, 10 );
    painter -> drawLine( 25, 14, 38, 14 );
    painter -> drawLine( 25, 18, 38, 18 );
}

void FillColor::mousePressEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    setActive( true );
    emit activated();
    mouse_event -> accept();
}
