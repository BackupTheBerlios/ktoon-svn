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

#include "qtpen.h"

#include <qpainter.h>
//Added by qt3to4:
#include <Q3PointArray>

//--------------- CONSTRUCTOR --------------

QtPen::QtPen( Q3Canvas *canvas ) : Q3CanvasPolygonalItem( canvas ), point_array( 0 ), control_points( 0 )
{
    
}

//--------------- DESTRUCTOR ---------------

QtPen::~QtPen()
{
    hide();
}

//--------------- PUBLIC MEMBERS --------------

int QtPen::rtti() const
{
    return RTTI;
}

Q3PointArray QtPen::areaPoints() const
{
    Q3PointArray point_array_tmp( 4 );
    QRect bounding_rectangle = point_array.boundingRect();
    point_array_tmp.putPoints( 0, 4, ( int )x() + bounding_rectangle.left() - 1, ( int )y() + bounding_rectangle.top() - 1, 
    			       ( int )x() + bounding_rectangle.right() + 1, ( int )y() + bounding_rectangle.top() - 1,
			       ( int )x() + bounding_rectangle.right() + 1, ( int )y() + bounding_rectangle.bottom() + 1,
			       ( int )x() + bounding_rectangle.left() - 1, ( int )y() + bounding_rectangle.bottom() + 1 );
    return point_array_tmp;
}

QPoint QtPen::mapToLocal( const QPoint &point ) const
{
    if ( point_array.count() > 0 )
    {
        QPoint to_return = QPoint( point.x() - ( int )x(), point.y() - ( int )y() );
        return to_return;
    }
    else
        return QPoint( 0, 0 );
}

bool QtPen::hit( const QPoint & )
{
    return false;
}

void QtPen::addPoint( const QPoint &point )
{
    QPoint mapped = mapToLocal( point );
    point_array.putPoints( point_array.count(), 1, mapped.x(), mapped.y() );
    update();
}

void QtPen::addBezier( const QPoint &point, const QPoint &spin )
{
    if ( point_array.count() >= 2 )
    {
    	QPoint mapped = mapToLocal( point );
	QPoint reflection = ( spin * 2 ) - point;
	control_points.putPoints( control_points.count(), 2, mapped.x(), mapped.y(), reflection.x(), reflection.y() );
	indexes.append( point_array.count() - 2 );
        update();
    }
}

void QtPen::replaceBezier( const QPoint &point, const QPoint &spin )
{
    if ( control_points.count() >= 2 )
    {
        QPoint mapped = mapToLocal( point );
	QPoint reflection = ( spin * 2 ) - point;
	control_points.putPoints( control_points.count() - 2, 2, mapped.x(), mapped.y(), reflection.x(), reflection.y() );
        update();
    }
}

//-------------- EVENTS AND PROTECTED MEMBERS -------------

void QtPen::drawShape( QPainter &painter )
{
    if ( point_array.count() > 1 )
    {
        int control_points_pair = 0;
        Q3PointArray translated_point_array( point_array.count() );
	Q3PointArray translated_control_points( control_points.count() );
	
	for ( unsigned int i = 0; i < point_array.count(); i++ )
	    translated_point_array[i] = QPoint( ( int )x() + point_array[i].x(), ( int )y() + point_array[i].y() );
	for ( unsigned int i = 0; i < control_points.count(); i++ )
	    translated_control_points[i] = QPoint( ( int )x() + control_points[i].x(), ( int )y() + control_points[i].y() );
	    
	for ( unsigned int i = 0; i < translated_point_array.count() - 1; i++ )
	{
	    if ( isBezierIndex( i ) )
	    {
	        //There is a bezier between points at translated_point_array[i] and translated_point_array[i+1];
		Q3PointArray cubic_bezier( 4 );
		cubic_bezier[ 0 ] = translated_point_array[ i + 1 ];
		cubic_bezier[ 1 ] = translated_control_points[ control_points_pair ];
		cubic_bezier[ 2 ] = translated_control_points[ control_points_pair + 1 ];
		cubic_bezier[ 3 ] = translated_point_array[ i ];
		painter.drawCubicBezier( cubic_bezier );
		control_points_pair += 2;
	    }
	    else
	        painter.drawLine( translated_point_array[i], translated_point_array[i+1] );
	}
    }
}

bool QtPen::isBezierIndex( unsigned int index ) const
{
    if ( indexes.contains( index ) > 0 )
        return true;
    return false;
}
