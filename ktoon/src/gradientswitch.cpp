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

#include <qpointarray.h>

#include "gradientswitch.h"

//------------ CONSTRUCTOR ----------------

GradientSwitch::GradientSwitch( QWidget *parent ) : QWidget( parent )
{
    resize( 7, 10 );
    parent_widget = parent;
    border_color = QColor( 0, 0, 0 );
    current_color = QColor( 255, 255, 255 );
    click_point = 0;
    dragging = false;
    is_selected = false;
    alpha = 100;
    position_ = 0; //Bound from 0 to 168
}

//------------ DESTRUCTOR -----------------

GradientSwitch::~GradientSwitch()
{

}

//----------- PUBLIC MEMBERS --------------

void GradientSwitch::setPosition( int pos )
{
    position_ = pos;
}

void GradientSwitch::setSelected( bool in_is_selected )
{
    is_selected = in_is_selected;
    update();
}

int GradientSwitch::position()
{
    return position_;
}

bool GradientSwitch::isSelected()
{
    return is_selected;
}

QColor GradientSwitch::currentColor()
{
    return current_color;
}

int GradientSwitch::getAlpha()
{
    return alpha;
}

//----------- SLOTS ------------

void GradientSwitch::slotSetColor( const QColor &new_color )
{
    current_color = new_color;
    update();
    emit colorChanged( new_color );
}

void GradientSwitch::slotSetAlpha( int new_alpha )
{
    alpha = new_alpha;
    emit alphaChanged( new_alpha );
}

//--------- EVENTS AND PROTECTED MEMBERS -----------

void GradientSwitch::mousePressEvent( QMouseEvent *mouse_event )
{
    if ( mouse_event -> button() == Qt::LeftButton && mouse_event -> state() == Qt::ShiftButton )
    {
	emit deleted();
    }
    else
    {
    	setSelected( true );
    	click_point = mouse_event -> x();
    	emit clicked();
    	dragging = true;
    	mouse_event -> accept();
    }
}

void GradientSwitch::mouseMoveEvent( QMouseEvent *mouse_event )
{
    if ( dragging )
    {
        if ( mouse_event -> x() < click_point )
            emit dragged( DRAG_LEFT );
	else
	    emit dragged( DRAG_RIGHT );
    	mouse_event -> accept();
    }
}

void GradientSwitch::mouseReleaseEvent( QMouseEvent *mouse_event )
{
    dragging = false;
    mouse_event -> accept();
}

void GradientSwitch::paintEvent( QPaintEvent *paint_event )
{
    if ( paint_event -> erased() )
    {
        painter.begin( this );

        //Square
    	painter.setPen( border_color );
    	painter.drawRect( 0, 3, width(), width() );

    	//Triangle
    	QPointArray pa( 3 );
    	pa.putPoints( 0, 3, 3, 0, 0, 3, 6, 3 );
    	painter.setPen( border_color );
    	if ( is_selected )
            painter.setBrush( border_color );
    	else
    	    painter.setBrush( Qt::NoBrush );
    	painter.drawPolygon( pa );

    	//Color indicator
    	painter.setPen( current_color );
    	painter.setBrush( current_color );
    	painter.drawRect( 2, 5, 3, 3 );

    	painter.end();
    }
}
