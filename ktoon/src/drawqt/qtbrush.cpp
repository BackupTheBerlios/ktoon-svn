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

//--------------- CONSTRUCTOR --------------

QtBrush::QtBrush( QCanvas *canvas ) : QCanvasPolygonalItem( canvas )
{
    point_array = QPointArray( 0 );
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

QPointArray QtBrush::areaPoints() const
{
    QPointArray point_array_tmp( 4 );
    QRect bounding_rectangle = point_array.boundingRect();
    point_array_tmp.putPoints( 0, 4, bounding_rectangle.left() - 1, bounding_rectangle.top() - 1, 
    			       bounding_rectangle.right() + 1, bounding_rectangle.top() - 1,
			       bounding_rectangle.right() + 1, bounding_rectangle.bottom() + 1,
			       bounding_rectangle.left() - 1, bounding_rectangle.bottom() + 1 );
    return point_array_tmp;
}

void QtBrush::addPoint( const QPoint &point )
{
    point_array.putPoints( point_array.count(), 1, point.x(), point.y() );
    update();
}

//-------------- EVENTS AND PROTECTED MEMBERS -------------

void QtBrush::drawShape( QPainter &painter )
{
    if ( point_array.count() > 1 )
        painter.drawPolyline( point_array );
}
