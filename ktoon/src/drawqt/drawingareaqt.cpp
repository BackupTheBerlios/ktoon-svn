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

#include "drawingareaqt.h"
#include "qtgrid.h"
#include "qtline.h"
#include "qtrectangle.h"
#include "qtellipse.h"
#include "qtpencil.h"
#include "qtbrush.h"
#include "ktoon.h"

//--------------- CONSTRUCTOR --------------

DrawingAreaQt::DrawingAreaQt( QWidget *parent, QWidget *grandparent, const QString &name ) 
    : QCanvasView( parent )
{
    parent_widget = parent;
    grandparent_widget = grandparent;
    setMinimumSize( 160, 120 );
    k_toon = ( Ktoon * )grandparent_widget;
    dragging = false;
    max_z = 0.0;
    
    canvas = new QCanvas( 637, 477 );
    setCanvas( canvas );
    setCaption( name );
    
    canvas_grid = new QtGrid( canvas, QtGrid::GRID12 );
    canvas_grid -> move( 20.0, 20.0 );
    canvas_grid -> setZ( -1000.0 );
    canvas_grid -> show();
    
    current_graphic = NULL;
}

//--------------- DESTRUCTOR ---------------

DrawingAreaQt::~DrawingAreaQt()
{
    delete canvas_grid;
    delete canvas;
}

//--------------- PUBLIC MEMBERS --------------

//---------------- SLOTS -----------------

//-------------- EVENTS AND PROTECTED MEMBERS -------------

void DrawingAreaQt::contentsMousePressEvent( QMouseEvent *mouse_event )
{
    mouse_event -> accept();
    dragging = true;
    
    int current_cursor = k_toon -> currentStatus() -> currentCursor();
    Color *current_outline_color = k_toon -> currentStatus() -> currentOutlineColor();
    Color *current_fill_color = k_toon -> currentStatus() -> currentFillColor();
    QColor co_color = QColor( ( int )( current_outline_color -> colorRed() * 255.0 ), ( int )( current_outline_color -> colorGreen() * 255.0 ), ( int )( current_outline_color -> colorBlue() * 255.0 ) );
    QColor cf_color = QColor( ( int )( current_fill_color -> colorRed() * 255.0 ), ( int )( current_fill_color -> colorGreen() * 255.0 ), ( int )( current_fill_color -> colorBlue() * 255.0 ) );
    Brush *current_brush = k_toon -> currentStatus() -> currentBrush();
    QPoint mapped_point = inverseWorldMatrix().map( mouse_event -> pos() );
    
    switch( current_cursor )
    {
        case Tools::NORMAL_SELECTION:
	{
	    QCanvasItemList touched_objects = canvas -> collisions( mapped_point );
	    if ( touched_objects.count() > 0 && touched_objects.first() != canvas_grid )
	    {
	        current_graphic = touched_objects.first();
		moving_start = mapped_point;
		//qDebug( QString::number( current_graphic -> z() ) + "--" + QString::number( current_graphic -> rtti() ) );
	    }
	    else
	        current_graphic = NULL;
	}
	    break;
        case Tools::CONTOUR_SELECTION:
	    break;
        case Tools::BRUSH:
	    current_graphic = new QtBrush( canvas );
	    ( ( QCanvasPolygonalItem * )current_graphic ) -> setPen( QPen( co_color, current_brush -> thicknessMinBrush() ) );
	    ( ( QtBrush * )current_graphic ) -> addPoint( mapped_point );
	    current_graphic -> setZ( max_z );
	    max_z += 0.1;
	    current_graphic -> show();
	    break;
        case Tools::PENCIL:
	    current_graphic = new QtPencil( canvas );
	    ( ( QCanvasPolygonalItem * )current_graphic ) -> setPen( QPen( co_color, current_brush -> thicknessMinBrush() ) );
	    ( ( QtPencil * )current_graphic ) -> addPoint( mapped_point );
	    current_graphic -> setZ( max_z );
	    max_z += 0.1;
	    current_graphic -> show();
	    break;
        case Tools::PEN:
	    break;
        case Tools::LINE:
	    current_graphic = new QtLine( canvas );
	    ( ( QCanvasLine * )current_graphic ) -> setPen( QPen( co_color, current_brush -> thicknessMinBrush() ) );
	    ( ( QtLine * )current_graphic ) -> setPoints( mapped_point.x(), mapped_point.y(), 
	    				  		  mapped_point.x(), mapped_point.y() );
	    current_graphic -> setZ( max_z );
	    max_z += 0.1;
	    current_graphic -> show();
	    break;
        case Tools::RECTANGLE:
	    current_graphic = new QtRectangle( canvas );
	    ( ( QCanvasRectangle * )current_graphic ) -> setPen( QPen( co_color, current_brush -> thicknessMinBrush() ) );
	    ( ( QCanvasRectangle * )current_graphic ) -> setBrush( QBrush( cf_color ) );
	    current_graphic -> move( mapped_point.x(), mapped_point.y() );
	    ( ( QtRectangle * )current_graphic ) -> setSize( 1, 1 );
	    current_graphic -> setZ( max_z );
	    max_z += 0.1;
	    current_graphic -> show();
	    break;
        case Tools::ELLIPSE:
	    current_graphic = new QtEllipse( canvas );
	    ( ( QCanvasEllipse * )current_graphic ) -> setPen( QPen( co_color, current_brush -> thicknessMinBrush() ) );
	    ( ( QCanvasEllipse * )current_graphic ) -> setBrush( QBrush( cf_color ) );
	    current_graphic -> move( mapped_point.x(), mapped_point.y() );
	    ( ( QtEllipse * )current_graphic ) -> setSize( 1, 1 );
	    current_graphic -> setZ( max_z );
	    max_z += 0.1;
	    current_graphic -> show();
	    break;
        case Tools::CONTOUR_FILL:
	    break;
        case Tools::FILL:
	    break;
        case Tools::REMOVE_FILL:
	    break;
        case Tools::DROPPER:
	    break;
        case Tools::ERASER:
	    break;
        case Tools::SLICER:
	    break;
        case Tools::MAGNIFYING_GLASS:
	    break;
        case Tools::HAND:
	    moving_start = mapped_point;
	    break;
	default: break;
    }
    canvas -> setChanged( canvas -> rect() );
    canvas -> update();
}

void DrawingAreaQt::contentsMouseMoveEvent( QMouseEvent *mouse_event )
{
    mouse_event -> accept();
    
    if ( dragging )
    {
    	int current_cursor = k_toon -> currentStatus() -> currentCursor();
	QPoint mapped_point = inverseWorldMatrix().map( mouse_event -> pos() );    
	
    	switch( current_cursor )
    	{
            case Tools::NORMAL_SELECTION:
	        if ( current_graphic )
		{
		    current_graphic -> moveBy( mapped_point.x() - moving_start.x(),
		    			       mapped_point.y() - moving_start.y() );
		    moving_start = mapped_point;
		}
	    	break;
            case Tools::CONTOUR_SELECTION:
	    	break;
            case Tools::BRUSH:
	        ( ( QtBrush * )current_graphic ) -> addPoint( mapped_point );
	    	break;
            case Tools::PENCIL:
	        ( ( QtPencil * )current_graphic ) -> addPoint( mapped_point );
	    	break;
            case Tools::PEN:
	    	break;
            case Tools::LINE:
	    {
	        QPoint start = inverseWorldMatrix().map( ( ( QtLine * )current_graphic ) -> startPoint() );
	        ( ( QtLine * )current_graphic ) -> setPoints( start.x(), start.y(),
					      		      mapped_point.x(), mapped_point.y() );
	    }
	    	break;
            case Tools::RECTANGLE:
	        ( ( QtRectangle * )current_graphic ) -> setSize( mapped_point.x() - ( int )( current_graphic -> x() ),
								 mapped_point.y() - ( int )( current_graphic -> y() ) );
	    	break;
            case Tools::ELLIPSE:
	        ( ( QtEllipse * )current_graphic ) -> setSize( mapped_point.x() - ( int )( current_graphic -> x() ),
							       mapped_point.y() - ( int )( current_graphic -> y() ) );
	    	break;
            case Tools::CONTOUR_FILL:
	    	break;
            case Tools::FILL:
	    	break;
            case Tools::REMOVE_FILL:
	    	break;
            case Tools::DROPPER:
	    	break;
            case Tools::ERASER:
	    	break;
            case Tools::SLICER:
	    	break;
            case Tools::MAGNIFYING_GLASS:
	    	break;
            case Tools::HAND:
	    {
		QCanvasItemList items = canvas -> allItems();
		for ( QCanvasItemList::iterator it = items.begin(); it != items.end(); ++it )
		    ( *it ) -> moveBy( mapped_point.x() - moving_start.x(), mapped_point.y() - moving_start.y() );
		moving_start = mapped_point;
	    }
	    	break;
	    default: break;
    	}
        canvas -> setChanged( canvas -> rect() );
        canvas -> update();
    }
}

void DrawingAreaQt::contentsMouseReleaseEvent( QMouseEvent *mouse_event )
{
    mouse_event -> accept();
    dragging = false;
    canvas -> setChanged( canvas -> rect() );
    canvas -> update();
}

void DrawingAreaQt::resizeEvent( QResizeEvent *resize_event )
{
    QSize new_size = resize_event -> size();
    canvas -> resize( new_size.width() - 3, new_size.height() - 3 );
    
    if ( new_size.width() / 4 == new_size.height() / 3 )
        canvas_grid -> resize( new_size.width() - 40, new_size.height() - 40 );
    else if ( new_size.width() / 4 > new_size.height() / 3 )
        canvas_grid -> resize( ( int )( ( ( float )new_size.height() * 4.0 ) / 3.0 ) - 40, new_size.height() - 40 );
    else if ( new_size.width() / 4 < new_size.height() / 3 )
        canvas_grid -> resize( new_size.width() - 40, ( int )( ( ( float )new_size.width() * 3.0 ) / 4.0 ) - 40 );
    canvas_grid -> move( ( int )( ( ( float )canvas -> width() - ( float )canvas_grid -> width() ) / 2.0 ), ( int )( ( ( float )canvas -> height() - ( float )canvas_grid -> height() ) / 2.0 ) );
	    
    canvas -> update();
}

void DrawingAreaQt::paintEvent( QPaintEvent *paint_event )
{
   if ( paint_event -> erased() )
       updateContents( paint_event -> rect() );
}

void DrawingAreaQt::keyPressEvent( QKeyEvent *key_event )
{
    key_event -> accept();
}
