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
#include <QMouseEvent>
#include <QPolygon>
#include <QPaintEvent>
#include "ktdebug.h"

//------------- CONSTRUCTOR ----------------

ValueSelector::ValueSelector( QWidget *parent ) : QAbstractSlider( parent )
{
	Q_CHECK_PTR( parent );
	setRange( 0, 255 );
    
	dragging = false;
	highlight = QColor( 255, 255, 255 );
	QPalette pal = palette();
	current_color = QColor( 255, 255, 255 );
	setMinimumSize(sizeHint());
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


//------------ EVENTS AND PROTECTED MEMBERS ---------------

void ValueSelector::mousePressEvent( QMouseEvent *mouse_event )
{
	Q_CHECK_PTR( mouse_event );
	if ( mouse_event -> y() > height() || mouse_event -> y() < 0 )
	{
		mouse_event -> ignore();
		return;
	}
	setValue( mouse_event -> y() );
	update();
	dragging = true;
	update();
	mouse_event -> accept();
}

void ValueSelector::mouseMoveEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );
    if ( mouse_event ->y() > height() || mouse_event -> y() < 0 )
    {
	mouse_event -> ignore();
	return;
    }

    if ( dragging )
    {
	    
	    setValue( mouse_event -> y() );
    	update();
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

// 	painter.setPen(  palette().color( QPalette::Base) );
//     	painter.drawLine( 0, 3, 0, 91 );
//     	painter.drawLine( 0, 3, 17, 3 );
// 
//     	painter.setPen( highlight );
//     	painter.drawLine( 17, 4, 17, 91 );
//     	painter.drawLine( 1, 91, 17, 91 );

//     	Draw the triangle
    	QPolygon pa( 3 );
	pa.setPoint ( 0, 10, 255-value());
	pa.setPoint ( 2, 15, 255-value()+5);
	pa.setPoint ( 1, 15,  255-value() -5);
	
// 	pa.putPoints(
// 	pa.putPoints( 3, 3, 18, 91 - pointFromValue( value() ), 21, 91 - pointFromValue( value() ) - 3, 21, 91 - value()/*- pointFromValue( value() )*/ + 3 );
	painter.setPen( palette().color(QPalette::Foreground));
	painter.setBrush( palette().color(QPalette::Foreground));
    	painter.drawPolygon( pa );

    	int h, s, v;
    	QColor tmp_color = current_color;
// 	QColor tmp_color = Qt::red;
    	tmp_color.getHsv( &h, &s, &v );
	
    	for ( int i = 0; i <= 255; i++ )
    	{
            tmp_color.setHsv( h, s, 255 - i );
	    painter.setPen( tmp_color );
	    painter.drawLine( 1, /*pointFromValue( i )*/ i , 10, /*pointFromValue( i )*/i  );
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

QSize ValueSelector::sizeHint() const
{
	return QSize(20, 255 );
}

void ValueSelector::setValue(int value)
{
	int h, s, v;
	QColor tmpColor = current_color;
	tmpColor.getHsv( &h, &s, &v );
	tmpColor.setHsv( h, s, value );
	QAbstractSlider::setValue( 255-tmpColor.value());
	emit valueChanged(  255-tmpColor.value());
}
