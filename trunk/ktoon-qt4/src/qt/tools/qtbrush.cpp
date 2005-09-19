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

#include "qtbrush.h"

#include <qpainter.h>
//Added by qt3to4:
#include <Q3PointArray>

//--------------- CONSTRUCTOR --------------

QtBrush::QtBrush( Q3Canvas *canvas ) : Q3CanvasPolygonalItem( canvas ), point_array( 0 )
{
    
}

//--------------- DESTRUCTOR ---------------

QtBrush::~QtBrush()
{
    hide();
}

//--------------- PUBLIC MEMBERS --------------

int QtBrush::rtti() const
{
    return RTTI;
}

Q3PointArray QtBrush::areaPoints() const
{
    Q3PointArray point_array_tmp( 4 );
    QRect bounding_rectangle = point_array.boundingRect();
    point_array_tmp.putPoints( 0, 4, ( int )x() + bounding_rectangle.left() - 1, ( int )y() + bounding_rectangle.top() - 1, 
    			       ( int )x() + bounding_rectangle.right() + 1, ( int )y() + bounding_rectangle.top() - 1,
			       ( int )x() + bounding_rectangle.right() + 1, ( int )y() + bounding_rectangle.bottom() + 1,
			       ( int )x() + bounding_rectangle.left() - 1, ( int )y() + bounding_rectangle.bottom() + 1 );
    return point_array_tmp;
}

QPoint QtBrush::mapToLocal( const QPoint &point ) const
{
    if ( point_array.count() > 0 )
    {
        QPoint to_return = QPoint( point.x() - ( int )x(), point.y() - ( int )y() );
        return to_return;
    }
    else
        return QPoint( 0, 0 );
}

bool QtBrush::hit( const QPoint &point )
{
    for ( unsigned int i = 0; i < point_array.count(); i++ )
    {
	if ( ( mapToLocal( point ).x() <= point_array[i].x() + ( int )pen().width() + 3 &&
	       mapToLocal( point ).x() >= point_array[i].x() - ( int )pen().width() - 3 ) &&
	     ( mapToLocal( point ).y() <= point_array[i].y() + ( int )pen().width() + 3 &&
	       mapToLocal( point ).y() >= point_array[i].y() - ( int )pen().width() - 3 ) )
	    return true;
    }
    return false;
}

void QtBrush::addPoint( const QPoint &point )
{
    QPoint mapped = mapToLocal( point );
    point_array.putPoints( point_array.count(), 1, mapped.x(), mapped.y() );
    update();
}

//-------------- EVENTS AND PROTECTED MEMBERS -------------

void QtBrush::drawShape( QPainter &painter )
{
    if ( point_array.count() > 1 )
    {
        Q3PointArray translated_point_array( point_array.count() );
	for ( unsigned int i = 0; i < point_array.count(); i++ )
	    translated_point_array[i] = QPoint( ( int )x() + point_array[i].x(), ( int )y() + point_array[i].y() );
        painter.drawPolyline( translated_point_array );
    }
}
