/***************************************************************************
 *   Copyright (C) 2004 by Simena Dinas                                    *
 *   dsimena@toonka.com                                                    *
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

#include "glcontrol.h"
#include "status.h"

//-------------- CONSTRUCTOR ---------------

GLControl::GLControl( QWidget *parent, const char *name, QGLWidget *share, WFlags f ) : QGLWidget( parent, name, share, f )
 {
	 qDebug("[Initializing GLControl]");
 }

 //------------- DESTRUCTOR ------------------

GLControl::~GLControl( )
 {

 }

 //------------ PUBLIC MEMBERS ---------------

void GLControl::mousePressEvent( QMouseEvent *mouse )
 {
 Q_CHECK_PTR( mouse );

 mouse -> accept();
 switch ( KTStatus -> currentCursor() )
  {
	 case Tools::NORMAL_SELECTION:
		  setNewPosition ( mouse -> pos() );
		  setOldPosition ( mouse -> pos() );
                 break;
	 case Tools::CONTOUR_SELECTION:
                  break;
	 case Tools::BRUSH:
 		  setNewPosition ( mouse -> pos() );
		  setOldPosition ( mouse -> pos() );
                 break;
	 case Tools::PENCIL:
 		  setNewPosition ( mouse -> pos() );
		  setOldPosition ( mouse -> pos() );
                  break;
	 case Tools::PEN:
 		  setNewPosition ( mouse -> pos() );
		  setOldPosition ( mouse -> pos() );
                  break;
	 case Tools::LINE:
 		  setNewPosition ( mouse -> pos() );
		  setOldPosition ( mouse -> pos() );
                 break;
	 case Tools::RECTANGLE:
 		  setNewPosition ( mouse -> pos() );
		  setOldPosition ( mouse -> pos() );
                  break;
	 case Tools::ELLIPSE:
 		  setNewPosition ( mouse -> pos() );
		  setOldPosition ( mouse -> pos() );
                 break;
	 case Tools::CONTOUR_FILL:
		  setNewPosition ( mouse -> pos() );
		  setOldPosition ( mouse -> pos() );
                  break;
	 case Tools::FILL:
		  setNewPosition ( mouse -> pos() );
		  setOldPosition ( mouse -> pos() );
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
		  setNewPosition ( mouse -> pos() );
		  setOldPosition ( mouse -> pos() );
                 break;
	 case Tools::HAND:
		  setNewPosition ( mouse -> pos() );
                 break;
    	 default: break;
	}
 QWidget::mousePressEvent( mouse );
 updateGL();
}

void GLControl::mouseReleaseEvent( QMouseEvent *mouse )
 {
 Q_CHECK_PTR( mouse );

  mouse -> accept();
  switch ( KTStatus -> currentCursor() )
  {
	 case Tools::NORMAL_SELECTION:
		  setNewPosition ( mouse -> pos() );
                  break;
	 case Tools::CONTOUR_SELECTION:
                  break;
	 case Tools::BRUSH:
		  setNewPosition ( mouse -> pos() );
                  break;
	 case Tools::PENCIL:
		  setNewPosition ( mouse -> pos() );
                  break;
	 case Tools::PEN:
		  setNewPosition ( mouse -> pos() );
                  break;
	 case Tools::LINE:
		  setNewPosition ( mouse -> pos() );
                 break;
	 case Tools::RECTANGLE:
		  setNewPosition ( mouse -> pos() );
                 break;
	 case Tools::ELLIPSE:
		  setNewPosition ( mouse -> pos() );
                  break;
	 case Tools::CONTOUR_FILL:
		  setNewPosition ( mouse -> pos() );
                 break;
	 case Tools::FILL:
		  setNewPosition ( mouse -> pos() );
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
		  setNewPosition ( mouse -> pos() );
                 break;
	 case Tools::HAND:
		  setNewPosition ( mouse -> pos() );
                  break;
    	 default: break;
	}
  QWidget::mouseReleaseEvent( mouse );
 }

 void GLControl::mouseMoveEvent( QMouseEvent *mouse )
 {
 Q_CHECK_PTR( mouse );

  mouse -> accept();
  switch ( KTStatus -> currentCursor() )
  {
	 case Tools::NORMAL_SELECTION:
		  setNewPosition ( mouse -> pos() );
                  break;
	 case Tools::CONTOUR_SELECTION:
                  break;
	 case Tools::BRUSH:
		  setOldPosition ( newPosition() );
		  setNewPosition ( mouse -> pos() );
                  break;
	 case Tools::PENCIL:
		  setOldPosition ( newPosition() );
		  setNewPosition ( mouse -> pos() );
                  break;
	 case Tools::PEN:
		  setNewPosition ( mouse -> pos() );  //Con Drag hace Bezier!
                  break;
	 case Tools::LINE:
		  setNewPosition ( mouse -> pos() );
                  break;
	 case Tools::RECTANGLE:
		  setNewPosition ( mouse -> pos() );
                  break;
	 case Tools::ELLIPSE:
		  setNewPosition ( mouse -> pos() );
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
		  setOldPosition ( newPosition() );
		  setNewPosition ( mouse -> pos() );
                  break;
    	 default: break;
	}
  QWidget::mouseMoveEvent( mouse );
}

   void GLControl::mouseDoubleClickEvent( QMouseEvent *mouse )
 {
   Q_CHECK_PTR( mouse );

   mouse -> accept();
   setOldPosition ( mouse -> pos() );
 }

void GLControl::tabletEvent( QTabletEvent *tablet )
 {
  Q_CHECK_PTR( tablet );

  tablet -> accept();

  setOldPosition ( newPosition() );
  setNewPosition ( tablet -> pos() );
  setPressureTablet( ( int ) tablet -> pressure() );
  QWidget::setEnabled( true );
  //qDebug( "Tablet Move New %d, %d", ( int )tablet -> x(), ( int )tablet -> y() );
  setOldPosition ( tablet -> pos() );
 }

void GLControl::setNewPosition( const QPoint & _new_position )
{
 new_position = _new_position;
}

void GLControl::setOldPosition( const QPoint & _old_position )
{
 old_position = _old_position;
}

void GLControl::setPressureTablet( const int & _pressure )
{
 pressure_tablet = _pressure;
}

QPoint GLControl::newPosition() const
{
 return new_position;
}

QPoint GLControl::oldPosition() const
{
 return old_position;
}

int GLControl::pressureTablet() const
{
 return pressure_tablet;
}

