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
#include "qtfill.h"
#include "qtpen.h"
#include "status.h"

//--------------- CONSTRUCTOR --------------

DrawingAreaQt::DrawingAreaQt( QWidget *parent, const QString &name ) 
	: QCanvasView( parent )
{
    parent_widget = parent;
    setMinimumSize( 160, 120 );

    dragging = false;
    drawing_pen = false;
    applying_bezier = false;
    max_z = 0.0;
    
    _canvas = new QCanvas( 637, 477 );
    setCanvas( _canvas );
    setCaption( name );
    
    canvas_grid = new QtGrid( _canvas, QtGrid::GRID12 );
    canvas_grid -> move( 20.0, 20.0 );
    canvas_grid -> setZ( -1000.0 );
    canvas_grid -> show();
    
    current_graphic = NULL;
    zoom_rectangle = NULL;
    selection_rectangle = NULL;
}

//--------------- DESTRUCTOR ---------------

DrawingAreaQt::~DrawingAreaQt()
{
    delete canvas_grid;
    delete _canvas;
}

//--------------- PUBLIC MEMBERS --------------

//---------------- SLOTS -----------------

//-------------- EVENTS AND PROTECTED MEMBERS -------------

void DrawingAreaQt::contentsMousePressEvent( QMouseEvent *mouse_event )
{
    mouse_event -> accept();
    dragging = true;
    
    int current_cursor = KTStatus-> currentCursor();
    KTColor *current_outline_color = KTStatus-> currentOutlineColor();
    KTColor *current_fill_color = KTStatus-> currentFillColor();
    QColor co_color = QColor( ( int )( current_outline_color -> colorRed() * 255.0 ), ( int )( current_outline_color -> colorGreen() * 255.0 ), ( int )( current_outline_color -> colorBlue() * 255.0 ) );
    QColor cf_color = QColor( ( int )( current_fill_color -> colorRed() * 255.0 ), ( int )( current_fill_color -> colorGreen() * 255.0 ), ( int )( current_fill_color -> colorBlue() * 255.0 ) );
    Brush *current_brush = KTStatus-> currentBrush();
    QPoint mapped_point = inverseWorldMatrix().map( mouse_event -> pos() );
    
    switch( current_cursor )
    {
        case Tools::NORMAL_SELECTION:
	{
	    QCanvasItemList touched_objects = _canvas -> collisions( mapped_point );
	    if ( touched_objects.count() > 0 && touched_objects.first() != canvas_grid )
	    {
	        current_graphic = touched_objects.first();
		if ( current_graphic -> rtti() == QtBrush::RTTI )
		{
		    if ( !( ( QtBrush * )current_graphic ) -> hit( mapped_point ) )
		    {
		        current_graphic = NULL;
			return;
		    }
		}
		else if ( current_graphic -> rtti() == QtPencil::RTTI )
		{
		    if ( !( ( QtPencil * )current_graphic ) -> hit( mapped_point ) )
		    {
		        current_graphic = NULL;
			return;
		    }
		}
		moving_start = mapped_point;
	    }
	    else
	    {
	        current_graphic = NULL;
		applying_bezier = false;
		drawing_pen = false;
	    	selection_rectangle = new QCanvasRectangle( _canvas );
	    	selection_rectangle -> setPen( QPen( QColor( 0, 0, 0 ), 1, Qt::DashLine ) );
	    	selection_rectangle -> setBrush( QBrush( Qt::NoBrush ) );
	    	selection_rectangle -> move( mapped_point.x(), mapped_point.y() );
	    	selection_rectangle -> setSize( 1, 1 );
	    	selection_rectangle -> setZ( max_z );
	    	selection_rectangle -> show();
	    }
	}
	    break;
        case Tools::CONTOUR_SELECTION:
	    break;
        case Tools::BRUSH:
	{
	    current_graphic = new QtBrush( _canvas );
	    QPen pen = QPen( co_color, current_brush -> thicknessMinBrush() );
	    pen.setJoinStyle( Qt::RoundJoin );
	    ( ( QCanvasPolygonalItem * )current_graphic ) -> setPen( pen );
	    current_graphic -> move( ( double )( mapped_point.x() ), ( double )( mapped_point.y() ) );
	    ( ( QtBrush * )current_graphic ) -> addPoint( mapped_point );
	    current_graphic -> setZ( max_z );
	    max_z += 0.1;
	    current_graphic -> show();
	}
	    break;
        case Tools::PENCIL:
	{
	    current_graphic = new QtPencil( _canvas );
	    QPen pen = QPen( co_color, current_brush -> thicknessMinBrush() );
	    pen.setJoinStyle( Qt::RoundJoin );
	    ( ( QCanvasPolygonalItem * )current_graphic ) -> setPen( pen );
	    current_graphic -> move( ( double )( mapped_point.x() ), ( double )( mapped_point.y() ) );
	    ( ( QtPencil * )current_graphic ) -> addPoint( mapped_point );
	    current_graphic -> setZ( max_z );
	    max_z += 0.1;
	    current_graphic -> show();
	}
	    break;
        case Tools::PEN:
	{
	    if ( !drawing_pen )
	    {
	        current_graphic = new QtPen( _canvas );
	        QPen pen = QPen( co_color, current_brush -> thicknessMinBrush() );
	        pen.setJoinStyle( Qt::RoundJoin );
	        ( ( QCanvasPolygonalItem * )current_graphic ) -> setPen( pen );
	    	current_graphic -> move( ( double )( mapped_point.x() ), ( double )( mapped_point.y() ) );
	    	( ( QtPen * )current_graphic ) -> addPoint( mapped_point );
	    	current_graphic -> setZ( max_z );
	    	max_z += 0.1;
	    	current_graphic -> show();
		drawing_pen = true;
		moving_start = mapped_point;
	    }
	    else
	    {
	        ( ( QtPen * )current_graphic ) -> addPoint( mapped_point );
		moving_start = mapped_point;
	    }
	}
	    break;
        case Tools::LINE:
	    current_graphic = new QtLine( _canvas );
	    ( ( QCanvasLine * )current_graphic ) -> setPen( QPen( co_color, current_brush -> thicknessMinBrush() ) );
	    ( ( QtLine * )current_graphic ) -> setPoints( mapped_point.x(), mapped_point.y(), 
	    				  		  mapped_point.x(), mapped_point.y() );
	    current_graphic -> setZ( max_z );
	    max_z += 0.1;
	    current_graphic -> show();
	    break;
        case Tools::RECTANGLE:
	    current_graphic = new QtRectangle( _canvas );
	    ( ( QCanvasRectangle * )current_graphic ) -> setPen( QPen( co_color, current_brush -> thicknessMinBrush() ) );
	    ( ( QCanvasRectangle * )current_graphic ) -> setBrush( QBrush( cf_color ) );
	    current_graphic -> move( mapped_point.x(), mapped_point.y() );
	    ( ( QtRectangle * )current_graphic ) -> setSize( 1, 1 );
	    current_graphic -> setZ( max_z );
	    max_z += 0.1;
	    current_graphic -> show();
	    break;
        case Tools::ELLIPSE:
	    current_graphic = new QtEllipse( _canvas );
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
	{
	    QCanvasItemList touched_objects = _canvas -> collisions( mapped_point );
	    if ( touched_objects.count() > 0 )
	    {
	        QCanvasItem *filled = touched_objects.first();
	        if ( filled -> rtti() == QtRectangle::RTTI || filled -> rtti() == QtEllipse::RTTI )
		    ( ( QCanvasPolygonalItem * )filled ) -> setBrush( cf_color );
	    }
	    else
	    {
	        /*current_graphic = new QtFill( _canvas );
	        ( ( QCanvasPolygon * )current_graphic ) -> setBrush( QBrush( cf_color ) );
	        current_graphic -> move( mapped_point.x(), mapped_point.y() );
	        current_graphic -> setZ( max_z );
	        max_z += 0.1;
	        current_graphic -> show();*/
	    }
	}
	    break;
        case Tools::REMOVE_FILL:
	{
	    QCanvasItemList touched_objects = _canvas -> collisions( mapped_point );
	    if ( touched_objects.count() > 0 )
	    {
	        QCanvasItem *filled = touched_objects.first();
	        if ( filled -> rtti() == QtRectangle::RTTI || filled -> rtti() == QtEllipse::RTTI )
		    ( ( QCanvasPolygonalItem * )filled ) -> setBrush( Qt::NoBrush );
	    }
	}
	    break;
        case Tools::DROPPER:
	    break;
        case Tools::ERASER:
	    break;
        case Tools::SLICER:
	    break;
        case Tools::MAGNIFYING_GLASS:
	{
	    zoom_rectangle = new QCanvasRectangle( _canvas );
	    zoom_rectangle -> setPen( QPen( QColor( 0, 0, 0 ), 1, Qt::DotLine ) );
	    zoom_rectangle -> setBrush( QBrush( Qt::NoBrush ) );
	    zoom_rectangle -> move( mapped_point.x(), mapped_point.y() );
	    zoom_rectangle -> setSize( 1, 1 );
	    zoom_rectangle -> setZ( max_z );
	    zoom_rectangle -> show();
	}
	    break;
        case Tools::HAND:
	    moving_start = mapped_point;
	    break;
	default: break;
    }
    _canvas -> setChanged( _canvas -> rect() );
    _canvas -> update();
}

void DrawingAreaQt::contentsMouseMoveEvent( QMouseEvent *mouse_event )
{
    mouse_event -> accept();
    int current_cursor = KTStatus-> currentCursor();
    
    if ( dragging )
    {
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
		else
                    selection_rectangle -> setSize( mapped_point.x() - ( int )( selection_rectangle -> x() ),
			 		   	    mapped_point.y() - ( int )( selection_rectangle -> y() ) );
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
	        if ( !applying_bezier )
		{
	            ( ( QtPen * )current_graphic ) -> addBezier( mapped_point, moving_start );
		    applying_bezier = true;
		}
		else
	            ( ( QtPen * )current_graphic ) -> replaceBezier( mapped_point, moving_start );
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
            case Tools::ERASER:
	    	break;
            case Tools::SLICER:
	    	break;
            case Tools::MAGNIFYING_GLASS:
                zoom_rectangle -> setSize( mapped_point.x() - ( int )( zoom_rectangle -> x() ),
					   mapped_point.y() - ( int )( zoom_rectangle -> y() ) );
		break;
            case Tools::HAND:
	    {
		QCanvasItemList items = _canvas -> allItems();
		for ( QCanvasItemList::iterator it = items.begin(); it != items.end(); ++it )
		    ( *it ) -> moveBy( mapped_point.x() - moving_start.x(), mapped_point.y() - moving_start.y() );
		moving_start = mapped_point;
	    }
	    	break;
	    default: break;
    	}
        _canvas -> setChanged( _canvas -> rect() );
        _canvas -> update();
    }
}

void DrawingAreaQt::contentsMouseReleaseEvent( QMouseEvent *mouse_event )
{
    mouse_event -> accept();
    dragging = false;
    
    int current_cursor = KTStatus-> currentCursor();
    
    switch ( current_cursor )
    {
        case Tools::NORMAL_SELECTION:
	{
	    if ( selection_rectangle )
	        delete selection_rectangle;
	}
	    break;
        case Tools::PEN:
	    applying_bezier = false;
	    break;
        case Tools::MAGNIFYING_GLASS:
	{
	    if ( zoom_rectangle )
                delete zoom_rectangle;
    	    //TODO: Apply the zoom
	}
	    break;
	default: break;
    }
        
    _canvas -> setChanged( _canvas -> rect() );
    _canvas -> update();
}

void DrawingAreaQt::contentsMouseDoubleClickEvent( QMouseEvent *mouse_event )
{
    mouse_event -> accept();
    int current_cursor = KTStatus-> currentCursor();

    if ( current_cursor == Tools::PEN )
    {
        drawing_pen = false;
	applying_bezier = false;
    }
}

void DrawingAreaQt::resizeEvent( QResizeEvent *resize_event )
{
    QSize new_size = resize_event -> size();
    _canvas -> resize( new_size.width() - 3, new_size.height() - 3 );
    
    if ( new_size.width() / 4 == new_size.height() / 3 )
        canvas_grid -> resize( new_size.width() - 40, new_size.height() - 40 );
    else if ( new_size.width() / 4 > new_size.height() / 3 )
        canvas_grid -> resize( ( int )( ( ( float )new_size.height() * 4.0 ) / 3.0 ) - 40, new_size.height() - 40 );
    else if ( new_size.width() / 4 < new_size.height() / 3 )
        canvas_grid -> resize( new_size.width() - 40, ( int )( ( ( float )new_size.width() * 3.0 ) / 4.0 ) - 40 );
    canvas_grid -> move( ( int )( ( ( float )_canvas -> width() - ( float )canvas_grid -> width() ) / 2.0 ), ( int )( ( ( float )_canvas -> height() - ( float )canvas_grid -> height() ) / 2.0 ) );
    
    _canvas -> update();
}

void DrawingAreaQt::paintEvent( QPaintEvent *paint_event )
{
   if ( paint_event -> erased() )
       updateContents( paint_event -> rect() );
}

void DrawingAreaQt::keyPressEvent( QKeyEvent *key_event )
{
    key_event -> accept();
    
    switch ( key_event -> key() )
    {
        case Qt::Key_Plus:
	{
	    QWMatrix matrix = worldMatrix();
	    matrix.scale( 1.5, 1.5 );
	    setWorldMatrix( matrix );
	}
	    break;
        case Qt::Key_Minus:
	{
	    QWMatrix matrix = worldMatrix();
	    matrix.scale( 0.75, 0.75 );
	    setWorldMatrix( matrix );
	}
	    break;
	default: break;
    }
    
    _canvas -> update();
}
