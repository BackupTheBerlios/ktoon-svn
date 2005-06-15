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

#include "qtgrid.h"

#include <qpainter.h>

//--------------- CONSTRUCTOR --------------

QtGrid::QtGrid( QCanvas *canvas, int _grid_type ) : QCanvasPolygonalItem( canvas )
{
    grid_color = QColor( 210, 210, 210 );
    ntsc_color = QColor( 255, 0, 0 );
    subgrid_color = QColor( 235, 235, 235 );
    diagonal_color = QColor( 200, 200, 200 );
    grid_type = _grid_type;
    see_ntsc = false;
    _width = 600;
    _height = 440;
    
    setPen( QPen( grid_color, 1 ) );
}

//--------------- DESTRUCTOR ---------------

QtGrid::~QtGrid()
{
    hide();
}

//--------------- PUBLIC MEMBERS --------------

int QtGrid::rtti() const
{
    return RTTI;
}

QPointArray QtGrid::areaPoints() const
{
    QPointArray point_array( 4 );
    point_array.putPoints( 0, 4, ( int )x() - 1, ( int )y() - 1, ( int )x() + _width + 1, ( int )x() - 1, 
    			   ( int )x() + _width + 1, ( int )y() + _height + 1, ( int )x() - 1, ( int )y() + _height + 1 );
    return point_array;
}

int QtGrid::width() const
{
    return _width;
}

int QtGrid::height() const
{
    return _height;
}

void QtGrid::resize( int w, int h )
{
    _width = w;
    _height = h;
    update();
}

//-------------- EVENTS AND PROTECTED MEMBERS -------------

void QtGrid::drawShape( QPainter &painter )
{
    painter.drawRect( ( int )x(), ( int )y(), _width, _height );
    switch( grid_type )
    {
    	case EMPTY_GRID: break;
	case GRID12: draw12FieldGrid( painter );
		drawDiagonals( painter );
		break;
	case GRID16: draw16FieldGrid( painter );
		drawDiagonals( painter );
		break;
	case SUBGRID12: draw12FieldSubGrid( painter );
		drawDiagonals( painter );
		break;
	case SUBGRID16: draw16FieldSubGrid( painter );
		drawDiagonals( painter );
		break;
	default: break;
    }
}

void QtGrid::draw12FieldGrid( QPainter &painter )
{
    int width_factor = _width / 24;
    int height_factor = _height / 24;
    for ( int i = 1; i <= 23; i++ )
    {
        if ( i == 12 )
	    painter.setPen( QPen( grid_color, 2 ) );
	else if ( i == 13 )
	    painter.setPen( QPen( grid_color, 1 ) );
        
	painter.drawLine( ( int )x() + i * width_factor, ( int )y(), ( int )x() + i * width_factor, ( int )y() + _height );
	painter.drawLine( ( int )x(), ( int )y() + i * height_factor, ( int )x() + _width, ( int )y() + i * height_factor );
	painter.setFont( QFont( "helvetica", 7 ) );
	painter.drawText( ( int )x() + i * width_factor - 16, ( int )y() - 4, QString::number( i ) );
	painter.drawText( ( int )x() - 12, ( int )y() + i * height_factor - 5, QString::number( i ) );
    }
    painter.drawText( ( int )x() + 24 * width_factor - 16, ( int )y() - 4, QString::number( 24 ) );
    painter.drawText( ( int )x() - 12, ( int )y() + 24 * height_factor - 5, QString::number( 24 ) );
}

void QtGrid::draw16FieldGrid( QPainter &painter )
{
    int width_factor = _width / 32;
    int height_factor = _height / 32;
    for ( int i = 1; i <= 31; i++ )
    {
        if ( i == 16 )
	    painter.setPen( QPen( grid_color, 2 ) );
	else if ( i == 17 )
	    painter.setPen( QPen( grid_color, 1 ) );
	    
        painter.drawLine( ( int )x() + i * width_factor, ( int )y(), ( int )x() + i * width_factor, ( int )y() + _height );
	painter.drawLine( ( int )x(), ( int )y() + i * height_factor, ( int )x() + _width, ( int )y() + i * height_factor );
	painter.setFont( QFont( "helvetica", 7 ) );
	painter.drawText( ( int )x() + i * width_factor - 12, ( int )y() - 4, QString::number( i ) );
	painter.drawText( ( int )x() - 12, ( int )y() + i * height_factor - 2, QString::number( i ) );
    }
    painter.drawText( ( int )x() + 32 * width_factor - 12, ( int )y() - 4, QString::number( 32 ) );
    painter.drawText( ( int )x() - 12, ( int )y() + 32 * height_factor - 2, QString::number( 32 ) );
}

void QtGrid::draw12FieldSubGrid( QPainter &painter )
{
    draw12FieldGrid( painter );
    painter.setPen( QPen( subgrid_color, 1 ) );
}

void QtGrid::draw16FieldSubGrid( QPainter &painter )
{
    draw16FieldGrid( painter );
    painter.setPen( QPen( subgrid_color, 1 ) );
}

void QtGrid::drawDiagonals( QPainter &painter )
{
    painter.setPen( QPen( diagonal_color, 1 ) );
    //painter.drawLine( ( int )x(), ( int )y(), ( int )x() + _width, ( int )y() + _height );
    //painter.drawLine( ( int )x(), ( int )y() + _height, ( int )x() + _width, ( int )y() );
}
