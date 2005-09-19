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

#include "colordisplay.h"
//Added by qt3to4:
#include <Q3Frame>

//-------------- CONSTRUCTOR --------------

ColorDisplay::ColorDisplay( QWidget *parent ) : Q3Frame( parent )
{
    Q_CHECK_PTR( parent );

    setFrameStyle( Q3Frame::Panel | Q3Frame::Sunken );
    parent_widget = parent;
    current_color = QColor( 0, 0, 0 );
    dark_block_color = QColor( 135, 135, 135 );
    light_block_color = paletteBackgroundColor();
    alpha = 1.0;
    setMaximumSize(47, 47 );
//     adjustSize();
}

//-------------- DESTRUCTOR ---------------

ColorDisplay::~ ColorDisplay()
{

}

//------------- PUBLIC MEMBERS -------------

QColor ColorDisplay::currentColor()
{
    return current_color;
}

//-------------- SLOTS ----------------

void ColorDisplay::slotSetColor( const QColor & new_color )
{
    Q_ASSERT( new_color.isValid() );
    current_color = new_color;
    update();
    emit colorChanged( current_color );
}

void ColorDisplay::slotChangeR( int value )
{
    Q_ASSERT( value > 0 );
    current_color.setRgb( value, current_color.green(), current_color.blue() );
    update();
    emit colorChanged( current_color );
}

void ColorDisplay::slotChangeG( int value )
{
    Q_ASSERT( value > 0 );
    current_color.setRgb( current_color.red(), value, current_color.blue() );
    update();
    emit colorChanged( current_color );
}

void ColorDisplay::slotChangeB( int value )
{
    Q_ASSERT( value > 0 );
    current_color.setRgb( current_color.red(), current_color.green(), value );
    update();
    emit colorChanged( current_color );
}

void ColorDisplay::slotChangeH( int value )
{
    Q_ASSERT( value > 0 );
    int c_h, c_s, c_v;
    current_color.getHsv( &c_h, &c_s, &c_v );
    current_color.setHsv( value, c_s, c_v );
    update();
    emit colorChanged( current_color );
}

void ColorDisplay::slotChangeS( int value )
{
    Q_ASSERT( value > 0 );
    int c_h, c_s, c_v;
    current_color.getHsv( &c_h, &c_s, &c_v );
    current_color.setHsv( c_h, value, c_v );
    update();
    emit colorChanged( current_color );
}

void ColorDisplay::slotChangeV( int value )
{
    Q_ASSERT( value > 0 );
    int c_h, c_s, c_v;
    current_color.getHsv( &c_h, &c_s, &c_v );
    current_color.setHsv( c_h, c_s, value );
    update();
    emit colorChanged( current_color );
}

void ColorDisplay::slotChangeA( int value )
{
    Q_ASSERT( value > 0 );
    alpha = ( float )value / 100.0;
    update();
}

//------------ EVENTS AND PROTECTED MEMBERS --------------

void ColorDisplay::drawContents( QPainter *painter )
{
    //----- Draw the current color rectangle -----

    intermediate_dark_color = QColor( ( int )( current_color.red() * alpha + dark_block_color.red() * ( 1.0 - alpha ) ),
    				      ( int )( current_color.green() * alpha + dark_block_color.green() * ( 1.0 - alpha ) ),
				      ( int )( current_color.blue() * alpha + dark_block_color.blue() * ( 1.0 - alpha ) ) );
    painter -> setPen( intermediate_dark_color );
    painter -> setBrush( intermediate_dark_color );
//     painter -> drawRect( 0, 0, 15, 15 );
//     painter -> drawRect( 30, 0, 15, 15 );
//     painter -> drawRect( 15, 15, 15, 15 );
//     painter -> drawRect( 0, 30, 15, 15 );
//     painter -> drawRect( 30, 30, 15, 15 );
    
    painter -> drawRect( 0, 0, height(), height() );
    painter -> drawRect( height()*2, 0, height(), height() );
    painter -> drawRect( height(), height(), height(), height() );
    painter -> drawRect( 0, height()*2, height(), height() );
    painter -> drawRect( height()*2, height()*2, height(), height() );

    intermediate_light_color = QColor( ( int )( current_color.red() * alpha + light_block_color.red() * ( 1.0 - alpha ) ),
    				       ( int )( current_color.green() * alpha + light_block_color.green() * ( 1.0 - alpha ) ),
				       ( int )( current_color.blue() * alpha + light_block_color.blue() * ( 1.0 - alpha ) ) );
    painter -> setPen( intermediate_light_color );
    painter -> setBrush( intermediate_light_color );
    
    painter -> drawRect( height(), 0, height(), height() );
    painter -> drawRect( 0, height(), height(), height() );
    painter -> drawRect( height()*2, height(), height(), height() );
    painter -> drawRect( height(), height()*2, height(), height() );
    
//     painter -> drawRect( 15, 0, 15, 15 );
//     painter -> drawRect( 0, 15, 15, 15 );
//     painter -> drawRect( 30, 15, 15, 15 );
//     painter -> drawRect( 15, 30, 15, 15 );
}
