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

#include "valueselector.h"
//Added by qt3to4:
#include <QMouseEvent>
#include <Q3PointArray>
#include <QPaintEvent>

//------------- CONSTRUCTOR ----------------

ValueSelector::ValueSelector( QWidget *parent ) : QWidget( parent ), Q3RangeControl( 0, 255, 1, 15, 0 )
{
    resize( 22, 95 );
    setMinimumSize(22, 95);
    Q_CHECK_PTR( parent );
    parent_widget = parent;
    dragging = false;
    highlight = QColor( 255, 255, 255 );
    shadow = QColor( 70, 70, 70 );
    triangle_color = QColor( 0, 0, 0 );
    current_color = QColor( 255, 255, 255 );
    
}

//------------- DESTRUCTOR ----------------

ValueSelector::~ ValueSelector()
{

}

//--------------- PUBLIC MEMBERS --------------

void ValueSelector::setColor( const QColor &new_color )
{
    current_color = new_color;
    update();
}
//-------------- SLOTS --------------

void ValueSelector::slotSetValue( int new_value )
{
    Q_ASSERT( new_value >= 0 && new_value <= 255 );
    setValue( new_value );
    update();
}

//------------ EVENTS AND PROTECTED MEMBERS ---------------

void ValueSelector::mousePressEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    if ( mouse_event -> y() > 91 || mouse_event -> y() < 0 )
    {
	mouse_event -> ignore();
	return;
    }

    dragging = true;
    int new_value = 255 - valueFromPoint( mouse_event -> y() );
    setValue( new_value );
    update();
    emit valueChanged( new_value );
    mouse_event -> accept();
}

void ValueSelector::mouseMoveEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    if ( mouse_event -> y() > 91 || mouse_event -> y() < 0 )
    {
	mouse_event -> ignore();
	return;
    }

    if ( dragging )
    {
    	int new_value = 255 - valueFromPoint( mouse_event -> y() );
    	setValue( new_value );
    	update();
    	emit valueChanged( new_value );
        mouse_event -> accept();
    }
}

void ValueSelector::mouseReleaseEvent( QMouseEvent *mouse_event )
{
    dragging = false;
    mouse_event -> accept();
}

void ValueSelector::paintEvent( QPaintEvent *paint_event )
{
    Q_CHECK_PTR( paint_event );
    if ( paint_event -> erased() )
    {
        painter.begin( this );

    	painter.setPen( shadow );
    	painter.drawLine( 0, 3, 0, 91 );
    	painter.drawLine( 0, 3, 17, 3 );

    	painter.setPen( highlight );
    	painter.drawLine( 17, 4, 17, 91 );
    	painter.drawLine( 1, 91, 17, 91 );

    	//Draw the triangle
    	Q3PointArray pa( 3 );
    	pa.putPoints( 0, 3, 18, 91 - pointFromValue( value() ), 21, 91 - pointFromValue( value() ) - 3, 21, 91 - pointFromValue( value() ) + 3 );
    	painter.setPen( triangle_color );
    	painter.setBrush( triangle_color );
    	painter.drawPolygon( pa );

    	int h, s, v;
    	QColor tmp_color = current_color;
    	tmp_color.getHsv( &h, &s, &v );
    	for ( int i = 0; i <= 255; i++ )
    	{
            tmp_color.setHsv( h, s, 255 - i );
	    painter.setPen( tmp_color );
	    painter.drawLine( 1, pointFromValue( i ) + 4, 16, pointFromValue( i ) + 4 );
    	}

    	painter.end();
    }
}

int ValueSelector::pointFromValue( int value )
{
    Q_ASSERT( value >= 0 && value <= 255 );
    int point = value / 3;
    return point;
}

int ValueSelector::valueFromPoint( int point )
{
    int value = point * 3;
    return value;
}
