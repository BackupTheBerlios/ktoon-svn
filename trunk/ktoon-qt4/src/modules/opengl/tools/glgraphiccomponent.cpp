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

#include "glgraphiccomponent.h"
#include "glline.h"
#include "glellipse.h"
#include "glrectangle.h"
#include "glpen.h"
#include "glpencil.h"
#include "glbrush.h"
#include "glimage.h"
#include "drawingarea.h"

#include <cmath>
//Added by qt3to4:
#include <QKeyEvent>

#define sqr( x ) ( ( x ) * ( x ) )

GLuint GLGraphicComponent::selection_name = 1;

//-------------- CONSTRUCTOR ---------------

GLGraphicComponent::GLGraphicComponent( QGLWidget *parent, const QPoint & _origin, const KTColor & _color, const Brush & _width ) : origin(_origin), color(_color), width(_width)
{
  Q_CHECK_PTR( parent );

  kind_graphic = -1;
  fill_graphic = true;
  horizontal_flip = false;
  vertical_flip = false;
  shift_pressed = false;
  control_pressed = false;
  parent_widget = parent;
}

//------------- DESTRUCTOR ------------------

GLGraphicComponent::~GLGraphicComponent()
{
   if( glIsList( id_graphic_component ) )
    {
      glDeleteLists( id_graphic_component, 1 );
    }
}

//------------ PUBLIC MEMBERS ---------------

GLuint GLGraphicComponent::idGraphicComponent() const
{
 return id_graphic_component;
}

void GLGraphicComponent::setOriginPoint( const QPoint & _origin )
{
 origin = _origin;
}

void GLGraphicComponent::setOutlineColor( const KTColor & _color )
{
 color = _color;
 buildList();
}

void GLGraphicComponent::setWidthPoint( const Brush & _width )
{
  width = _width;
  buildList();
}

void GLGraphicComponent::setKindGraphic( int kind )
{
  kind_graphic = kind;
}

void GLGraphicComponent::draw()
{
	if( ! glIsList( id_graphic_component ) )
	{
		id_graphic_component = glGenLists( 1 );
		buildList();
	}
	
	glPushMatrix();
	glTranslatef( translate.x(), translate.y(), getZ() );

	rotateGraphic( ( topLeft().x() + bottomRight().x() ) / 2,
		( topLeft().y() + bottomRight().y() ) / 2,
		rotationAngle() );
	
	flipGraphic( ( topLeft().x() + bottomRight().x() ) / 2,
		( topLeft().y() + bottomRight().y() ) / 2 );

	glPushName( local_selection_name );
	glCallList( idGraphicComponent() );
	glPopName();
	
	glPopMatrix();
}

void GLGraphicComponent::setFill( bool fill )
{
   fill_graphic = fill;
   buildList();
}

void GLGraphicComponent::setStipplePattern( GLushort pattern )
{
   stipple_pattern = pattern;
   buildList();
}

void GLGraphicComponent::setStippleFactor( GLushort factor )
{
   stipple_factor = factor;
   buildList();
}

void GLGraphicComponent::setZ( GLfloat _z )
{
   z = _z;
   buildList();
}

void GLGraphicComponent::setRotationAngle( GLfloat angle )
{
   rotation_angle += angle;
   buildList();
}

void GLGraphicComponent::setTopLeft( const QPoint & _top_left )
{
  top_left = _top_left;
}

void GLGraphicComponent::setBottomRight( const QPoint & _bottom_right )
{
   bottom_right = _bottom_right;
}

void GLGraphicComponent::translateGraphic( const QPoint & _origin, const QPoint & _end )
{
  switch( kindGraphic() )
	{
	 case GC_BRUSH:
	 case GC_PENCIL:
	 case GC_PEN:
  		translate.setX( translate.x() + ( _origin.x() - _end.x() ) );
  		translate.setY( translate.y() + ( _origin.y() - _end.y() ) );
	      break;
	 case GC_LINE:
  		setOriginPoint( origin + QPoint( ( _origin.x() - _end.x() ), ( _origin.y() - _end.y() ) ) );
	 	( ( GLLine * ) ( this ) ) -> setEndLine( ( ( GLLine * ) ( this ) ) -> endLine() + QPoint( ( _origin.x() - _end.x() ), ( _origin.y() - _end.y() ) ) );
	      break;
	 case GC_RECTANGLE:
  		setOriginPoint( origin + QPoint( ( _origin.x() - _end.x() ), ( _origin.y() - _end.y() ) ) );
	 	( ( GLRectangle * ) ( this ) ) -> setEndRectangle( ( ( GLRectangle * ) ( this ) ) -> endRectangle() + QPoint( ( _origin.x() - _end.x() ), ( _origin.y() - _end.y() ) ) );
	      break;
	 case GC_ELLIPSE:
  		setOriginPoint( origin + QPoint( ( _origin.x() - _end.x() ), ( _origin.y() - _end.y() ) ) );
	 	( ( GLEllipse * ) ( this ) ) -> setRadiusEllipse( ( ( GLEllipse * ) ( this ) ) -> radiusEllipse() + QPoint( ( _origin.x() - _end.x() ), ( _origin.y() - _end.y() ) ) );
	      break;
	 case GC_IMAGE:
  		setOriginPoint( origin + QPoint( ( _origin.x() - _end.x() ), ( _origin.y() - _end.y() ) ) );
	 	( ( GLImage * ) ( this ) ) -> setEndImage( ( ( GLImage * ) ( this ) ) -> endImage() + QPoint( ( _origin.x() - _end.x() ), ( _origin.y() - _end.y() ) ) );
	      break;
	 default:
	      break;
	}

  buildList();
}

void GLGraphicComponent::translateGraphic( int x, int y )
{
  translate.setX( translate.x() + x );
  translate.setY( translate.y() + y );
  buildList();
}

void GLGraphicComponent::setSpinPoint( const QPoint & spin )
{
   spin_point = spin;
   buildList();
}

void GLGraphicComponent::rotateGraphic( const QPoint & spin, float angle )
{
	float x = spin.x();
	float y = spin.y();
	rotateGraphic( x, y, angle );
}

void GLGraphicComponent::rotateGraphic( float x, float y, float angle )
{
	if( angle != 0.0 ) {
		glTranslatef( x, y, 0 );
		glRotatef( angle, 0.0, 0.0, 1.0 );
		glTranslatef( ( -1 ) * x, ( -1 ) * y, 0 );
	}
}

void GLGraphicComponent::flipGraphic( const QPoint & flip )
{
	float x = flip.x();
	float y = flip.y();
	flipGraphic( x, y );
}

void GLGraphicComponent::flipGraphic( float x, float y )
{
	if( horizontalFlip() || verticalFlip() ) {
		glTranslatef( x, y, 0 );
		if ( horizontalFlip() )
			glScalef( -1.0, 1.0, 1.0 );
		if ( verticalFlip() )
			glScalef( 1.0, -1.0, 1.0 );
		glTranslatef( ( -1 ) * x, ( -1 ) * y, 0 );
	}
}

void GLGraphicComponent::toggleHorizontalFlip()
{
   horizontal_flip = !horizontal_flip;
   buildList();
}

void GLGraphicComponent::toggleVerticalFlip()
{
   vertical_flip = !vertical_flip;
   buildList();
}

QPoint GLGraphicComponent::originPoint() const
{
 return origin;
}

const KTColor & GLGraphicComponent::outlineColor() const
{
 return color;
}

const Brush & GLGraphicComponent::widthPoint() const
{
 return width;
}

int GLGraphicComponent::kindGraphic() const
{
  return kind_graphic;
}

bool GLGraphicComponent::fill() const
{
  return fill_graphic;
}

GLushort GLGraphicComponent::stipplePattern() const
{
  return stipple_pattern;
}

GLushort GLGraphicComponent::stippleFactor() const
{
  return stipple_factor;
}

GLfloat GLGraphicComponent::getZ() const
{
  return z;
}

GLfloat GLGraphicComponent::rotationAngle() const
{
  return rotation_angle;
}

QPoint GLGraphicComponent::topLeft() const
{
  return top_left;
}

QPoint GLGraphicComponent::bottomRight() const
{
   return bottom_right;
}

QPoint GLGraphicComponent::spinPoint()
{
 return spin_point;
}

QPoint GLGraphicComponent::getTranslate()
{
 return translate;
}

bool GLGraphicComponent::horizontalFlip()
{
  return horizontal_flip;
}

bool GLGraphicComponent::verticalFlip()
{
  return vertical_flip;
}

bool GLGraphicComponent::graphicIncluded( QPoint mouse_press, QPoint mouse_release )
{
//Supongo que press es menor que release. Falta hacer la conersion de los valores, para que funcione siempre
  QPoint aux;

  mouse_press.x() < mouse_release.x() ? aux.setX( mouse_press.x() ) : aux.setX( mouse_release.x() );
  mouse_press.y() < mouse_release.y() ? aux.setY( mouse_press.y() ) : aux.setY( mouse_release.y() );

  mouse_press.x() > mouse_release.x() ? mouse_release.setX( mouse_press.x() ) : mouse_release.setX( mouse_release.x() );
  mouse_press.y() > mouse_release.y() ? mouse_release.setY( mouse_press.y() ) : mouse_release.setY( mouse_release.y() );

  mouse_press.setX( aux.x() );
  mouse_press.setY( aux.y() );

  this -> calculateTopLeft();
  this -> calculateBottomRight();
  QPoint topLeft = this -> topLeft();
  QPoint bottomRight = this -> bottomRight();

  if ( topLeft.x() + translate.x() >= mouse_press.x() && bottomRight.x() + translate.x() <= mouse_release.x() &&
       topLeft.y() + translate.y() >= mouse_press.y() && bottomRight.y() + translate.y() <= mouse_release.y() )
    return true;
  else
    return false;
}

int GLGraphicComponent::translationX()
{
    return translate.x();
}

int GLGraphicComponent::translationY()
{
    return translate.y();
}

GLuint GLGraphicComponent::selectionName() const
{
    return local_selection_name;
}

void GLGraphicComponent::contourSelectionPressOption()
{
  switch( kindGraphic() )
	{
	 case GC_PEN:
	      ( ( GLPen * )( this ) ) -> setShowBezierPoints( true );
	     break;
	 default:
	     break;
	}
}

void GLGraphicComponent::fillPressOption( const KTColor & fill_color )
{
   switch( kindGraphic() )
	{
	 case GC_RECTANGLE:
	      setFill( true );
	      ( ( GLRectangle * ) ( this ) ) -> setFillColor( fill_color );
	      break;
	 case GC_ELLIPSE:
	      setFill( true );
	      ( ( GLEllipse * ) ( this ) ) -> setFillColor( fill_color );
	      break;
	 case GC_IMAGE:
	      setFill( true );
	      ( ( GLImage * ) ( this ) ) -> setFillColor( fill_color );
	      break;
	 default:
	      break;
	}
}

void GLGraphicComponent::removeFillPressOption()
{
   switch( kindGraphic() )
	{
	 case GC_RECTANGLE:
	 case GC_ELLIPSE:
	      setFill( false );
	      break;
	 default:
	      break;
	}
}

void GLGraphicComponent::contourSelectionMoveOption( const QPoint & point )
{
  switch( kindGraphic() )
	{
	 case GC_PEN:
	      ( ( GLPen * )( this ) ) -> moveBezierPoint( point );
	     break;
	 default:
	     break;
	}
}

void GLGraphicComponent::brushMoveOption( const QPoint & point )
{
	qDebug("Brush move option");
  QPoint new_point( point.x(), point.y() );
  if ( !control_pressed && shift_pressed )
     ( ( GLBrush * )( this ) ) -> angleBrush( new_point );
  else if ( control_pressed && !shift_pressed )
     ( ( GLBrush * )( this ) ) -> lineBrush( new_point );
  else if ( !control_pressed && !shift_pressed )
     ( ( GLBrush * )( this ) ) -> setEndBrush( new_point );
}

void GLGraphicComponent::pencilMoveOption( const QPoint & point )
{
  if ( !control_pressed && shift_pressed )
     ( ( GLPencil * )( this ) ) -> anglePencil( point );
  else if ( control_pressed && !shift_pressed )
     ( ( GLPencil * )( this ) ) -> linePencil( point );
  else if ( !control_pressed && !shift_pressed )
     ( ( GLPencil * )( this ) ) -> setEndPencil( point );
}

void GLGraphicComponent::penMoveOption( const QPoint & point )
{
    if ( !control_pressed && shift_pressed )
       ( ( GLPen * )( this ) ) -> angleControlPen( point );
    else if ( control_pressed && !shift_pressed )
            ( ( GLPen * )( this ) ) -> moveControlPen( point );
    else if ( control_pressed && shift_pressed )
            ( ( GLPen * )( this ) ) -> lastControlPen( point );
    else if ( !control_pressed && !shift_pressed )
            ( ( GLPen * )( this ) ) -> replaceControlPen( point );

}

void GLGraphicComponent::lineMoveOption( const QPoint & point )
{
    if ( shift_pressed )
       ( ( GLLine * )( this ) ) -> angleLine( point );
    else
       ( ( GLLine * )( this ) ) -> setEndLine( point );

}

void GLGraphicComponent::rectangleMoveOption( const QPoint & point )
{
   if ( shift_pressed )
      ( ( GLRectangle * )( this ) ) -> setPerfectRectangle( true );
   else
      ( ( GLRectangle * )( this ) ) -> setPerfectRectangle( false );
   ( ( GLRectangle * )( this ) ) -> setEndRectangle( point );
}

void GLGraphicComponent::ellipseMoveOption( const QPoint & point )
{
	if ( shift_pressed )
		( ( GLEllipse * )( this ) ) -> setPerfectEllipse( true );
	else
		( ( GLEllipse * )( this ) ) -> setPerfectEllipse( false );
	( ( GLEllipse * )( this ) ) -> setRadiusEllipse( point );
}

void GLGraphicComponent::keyPressEvent( QKeyEvent * key_event )
{
	switch ( key_event -> key() )
	{
		case Qt::Key_Shift:
			shift_pressed = true;
			key_event -> accept();
			break;
		case Qt::Key_Control:
			control_pressed = true;
			key_event -> accept();
			break;
			case Qt::Key_Left: // Thanks to lauxa ;)
			if ( key_event -> state() == Qt::ShiftButton )
			{
				translateGraphic( originPoint(), QPoint ( originPoint().x() + 10, originPoint().y() ) ); 
				//translateGraphic( -10, 0 );
			}
			else
			{
				translateGraphic( originPoint(), QPoint ( originPoint().x() + 1, originPoint().y() ));
				//translateGraphic( -1, 0 );
			}
			key_event -> accept();
			break;
		case Qt::Key_Up:
			if ( key_event -> state() == Qt::ShiftButton )
			{
				translateGraphic( originPoint(), QPoint ( originPoint().x(), originPoint().y() +10 ) );
				//translateGraphic( 0, -10 );
			}
			else
			{
				//translateGraphic( 0, -1 );
				translateGraphic( originPoint(), QPoint ( originPoint().x(), originPoint().y()+1 ));
			}
			key_event -> accept();
			break;
		case Qt::Key_Right:
			if ( key_event -> state() == Qt::ShiftButton )
			{
				translateGraphic( originPoint(), QPoint ( originPoint().x() - 10, originPoint().y() ) );
// 				translateGraphic( 10, 0 );
			}
			else
			{
				translateGraphic( originPoint(), QPoint ( originPoint().x() - 1, originPoint().y() ));
				//translateGraphic( 1, 0 );
			}
			key_event -> accept();
			break;
		case Qt::Key_Down:
			if ( key_event -> state() == Qt::ShiftButton )
			{
				translateGraphic( originPoint(), QPoint ( originPoint().x() , originPoint().y() -10) );
				//translateGraphic( 0, 10 );
			}
			else
			{
				translateGraphic( originPoint(), QPoint ( originPoint().x(), originPoint().y()-1 ));
				//translateGraphic( 0, 1 );
			}
			key_event -> accept();
			break;
		case Qt::Key_F8:
			( ( DrawingArea * ) parent_widget ) -> slotAddToLibrary();
			key_event -> accept();
			break;
		default:
			key_event -> ignore();
			break;
	}
}

void GLGraphicComponent::keyReleaseEvent( QKeyEvent * key_event )
{
    switch ( key_event -> key() )
    {
     case Qt::Key_Shift:
        shift_pressed = false;
	key_event -> accept();
	break;
     case Qt::Key_Control:
        control_pressed = false;
	key_event -> accept();
	break;
     default:
        key_event -> ignore();
	break;
     }

}

void GLGraphicComponent::lineImpl( const QPoint & origin, const QPoint & end, int line_width, int stipple_factor, const KTColor & outline_color )
{
	GLfloat angulo, longitud;
	GLfloat xi, yi, xf, yf;
	xi = origin.x();
	yi = origin.y();
	xf = end.x();
	yf = end.y();

	longitud = sqrt(  sqr( xi - xf ) + sqr( yi - yf ) );
	angulo = atan2( yf - yi, xf - xi ) * 180.0 / M_PI;

	if( outline_color.colorAlpha() == 255 )
		glDisable( GL_BLEND );

	glPushMatrix();

	glTranslatef( ( xi + xf ) / 2.0, ( yi + yf ) / 2.0, 0.0 );
	glRotatef( angulo, 0, 0, 1.0 );

	GLfloat x1, y1, x2, y2;
	x1 = -0.5 * longitud;
	y1 = -0.5 * line_width;
	x2 = 0.5 * longitud;
	y2 = 0.5 * line_width;

	glBegin( GL_QUADS );
		glTexCoord1f( 0.0 );
		glVertex2f( x1, y2 );
		glVertex2f( x1, y1 );
		glTexCoord1f( longitud / 16.0 / ( GLfloat )stipple_factor );
		glVertex2f (x2, y1 );
		glVertex2f( x2, y2 );
	glEnd();

	if( outline_color.colorAlpha() == 255 )
		glEnable( GL_BLEND );
	else 	//it is done antialiasing lines with a more transparent color
		glColor4f( outline_color.colorRed(), outline_color.colorGreen(),
			   outline_color.colorBlue(), outline_color.colorAlpha() / 2.0 );

	glLineWidth(1.0);
	glBegin( GL_LINE_LOOP );
		glTexCoord1f( 0.0 );
		glVertex2f( x1, y2 );
		glVertex2f( x1, y1 );
		glTexCoord1f( longitud / 16.0 / ( GLfloat )stipple_factor );
		glVertex2f( x2, y1 );
		glVertex2f( x2, y2 );
	glEnd();

	glPopMatrix();
}

// line implementation, when the stipple pattern is 0xFFFF
void GLGraphicComponent::lineImplFast( const QPoint & origin, const QPoint & end, int line_width, const KTColor & outlineColor )
{
	GLfloat angulo, longitud;
	GLfloat xi, yi, xf, yf;
	xi = origin.x();
	yi = origin.y();
	xf = end.x();
	yf = end.y();

	longitud = sqrt(  sqr(xi-xf) + sqr(yi-yf ) );
	angulo = atan2( yf-yi, xf-xi ) * 180.0 / M_PI;

	if( outlineColor.colorAlpha() == 255 )
		glDisable( GL_BLEND );

	glPushMatrix();

	glTranslatef( ( xi + xf ) / 2.0, ( yi + yf ) / 2.0, 0.0 );
	glRotatef( angulo, 0, 0, 1.0 );

	GLfloat x1, y1, x2, y2;
	x1 = -0.5 * longitud;
	y1 = -0.5 * line_width;
	x2 = 0.5 * longitud;
	y2 = 0.5 * line_width;

	glRectf( x1, y1, x2, y2 );

	if( outlineColor.colorAlpha() == 255 )
		glEnable( GL_BLEND );
	else    //it is done antialiasing lines with a more transparent color
		glColor4f( outlineColor.colorRed(), outlineColor.colorGreen(),
			   outlineColor.colorBlue(), outlineColor.colorAlpha()/2.0 );

	glLineWidth( 1.0 );
	glBegin( GL_LINE_LOOP );
		glVertex2f( x1, y2 );
		glVertex2f( x1, y1 );
		glVertex2f( x2, y1 );
		glVertex2f( x2, y2 );
	glEnd();

	glPopMatrix();
}

QGLWidget *GLGraphicComponent::parentWidget() const
{
  Q_CHECK_PTR( parent_widget );
  return parent_widget;
}

