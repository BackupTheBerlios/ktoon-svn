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

#include <cmath>
#include <cstdlib>

#include "glpen.h"

#include <new>

//-------------- CONSTRUCTOR ---------------

GLPen::GLPen( QGLWidget *parent, const QPoint & _origin, const Color & _color, const Brush & _width, const QPoint & _end ) : GLGraphicComponent( parent, _origin, _color, _width ), end( _end )
{
   	Q_CHECK_PTR( parent );

	setKindGraphic( GC_PEN );
        id_graphic_component = glGenLists( 1 );
	order_pen = 3;
  	stride_pen = 3;
	knots_count = 0;
	QPoint * origin_point = new QPoint( origin.x(), origin.y() );
	try {
	    points.append( origin_point );
	    }
	catch(...)
	    {
	    delete origin_point;
	    throw;
	    }
	
	QPoint * end_point = new(std::nothrow) QPoint( end.x(), end.y() );
	if(!end_point)
	    {
	    delete origin_point;
	    throw std::bad_alloc();
	    }
	
	try {
	    points.append( end_point );
	    }
	catch(...)
	    {
	    delete end_point;
	    delete origin_point;
	    throw;
	    }
	
	bezier_point = origin;
 	stipple_factor = 1;
  	stipple_pattern = 0xFFFF;
  	z = 0.0;
	rotation_angle = 0.0;
	translate.setX( 0 );
	translate.setY( 0 );
	setShowBezierPoints( false );
	local_selection_name = selection_name;
        buildList();
	selection_name++;
}

GLPen::GLPen( const GLPen & pen ) : GLGraphicComponent( pen.parentWidget(), pen.originPoint(), pen.outlineColor(), pen.widthPoint() ), end( pen.endPen() )
{
	 setKindGraphic( GC_PEN );
         id_graphic_component = glGenLists( 1 );
	 order_pen = 3;
  	 stride_pen = 3;
	 knots_count = 0;
	 
	 bezier = pen.getBezier();
	 points = pen.getPoints();
	 
	 QPoint * origin_point = new QPoint( origin.x(), origin.y() );
	 try {
	     points.append( origin_point );
	     }
	 catch(...)
	     {
	     delete origin_point;
	     throw;
	     }
	
	 QPoint * end_point = new(std::nothrow) QPoint( end.x(), end.y() );
	 if(!end_point)
	     {
	     delete origin_point;
	     throw std::bad_alloc();
	     }
	 
	 try {
	     points.append( end_point );
	     }
	 catch(...)
	     {
	     delete end_point;
	     delete origin_point;
	     throw;
	     }
	 
	 bezier_point = origin;
	 bezier_point = pen.bezierPoint();
 	 stipple_factor = pen.stippleFactor();
  	 stipple_pattern = pen.stipplePattern();
	 rotation_angle = pen.rotationAngle();
	 translate.setX( 0 );
	 translate.setY( 0 );
	 local_selection_name = selection_name;
	 selection_name++;
	 
         buildList();
}

//------------- DESTRUCTOR ------------------

GLPen::~GLPen( )
{

}

//------------ PUBLIC MEMBERS ---------------

//implementation of the virtual method buildList to Bezier curve
void GLPen::buildList( )
{
	GLfloat knots[ points.count() + orderPen() ];
	GLfloat *cknots;

	GLfloat ctrlpoints[ points.count() * 3 ];
	GLfloat *cpoints;
        QPoint *point;

	cknots = knots;
	for ( unsigned int i = 0; i < points.count() + orderPen(); i++ )
	     {
 	      *cknots = i;
	      cknots++;
	     }

	cpoints = ctrlpoints;
	for ( point = points.first(); point; point = points.next() )
	  {
	    *cpoints = ( GLfloat )point -> x();
	    cpoints++;
	    *cpoints = ( GLfloat )point -> y();
	    cpoints++;
	    *cpoints = 0.0;
	    cpoints++;
	  }

	setControlPointsPen( points.count() );
	setKnotsCount( controlPointsPen() + orderPen() );

	glNewList( id_graphic_component, GL_COMPILE );
   	   glPushMatrix();
	   glLineWidth( widthPoint().thicknessMinBrush() );
	   glColor4f( outlineColor().colorRed(), outlineColor().colorGreen(),
	              outlineColor().colorBlue(), outlineColor().colorAlpha() );
	   glEnable( GL_LINE_STIPPLE );
           glLineStipple( stippleFactor(), stipplePattern() );
	   GLUnurbs *nurb = gluNewNurbsRenderer();
	   	gluNurbsProperty( nurb, GLU_SAMPLING_TOLERANCE, 2.0 );
	   	gluBeginCurve( nurb );
			gluNurbsCurve( nurb, knotsCount(), knots, stridePen(), ctrlpoints, orderPen(), GL_MAP1_VERTEX_3 );
	   	gluEndCurve( nurb );

	   gluDeleteNurbsRenderer( nurb );
	   glDisable( GL_LINE_STIPPLE );

	   glLineWidth( 2.0 );
   	   glBegin( GL_LINES );
	        glColor3f( 0.0, 0.0, 1.0 );
        	glVertex2f( ctrlpoints[ ( points.count() - 2 ) * 3 ], ctrlpoints[ ( ( points.count() - 2 ) * 3 ) + 1 ] );
		glColor3f( 1.0, 0.0, 0.0 );
        	glVertex2f( ctrlpoints[ ( points.count() - 1 ) * 3 ], ctrlpoints[ ( ( points.count() - 1 ) * 3 ) + 1 ] );
   	   glEnd();

	   glPointSize( 5.0 );
	   if ( showBezierPoints() )
	     {
		glColor3f( 0.0, 0.0, 1.0 );
		glBegin( GL_POINTS );
	  	for ( point = bezier.first(); point; point = bezier.next() )
     		  	glVertex2f( point -> x(), point -> y() ) ;
   		glEnd();
	     }
/*
	   glColor3f( 1.0, 0.0, 0.0 );
	   glBegin( GL_POINTS );
	      for ( point = points.first(); point; point = points.next() )
     		  glVertex2f( point -> x(), point -> y() ) ;
   	   glEnd();
*/
	   glDisable( GL_LINE_STIPPLE );
   	   glPopMatrix();
        glEndList();
}

void GLPen::setEndPen( const QPoint & _end )
{
 end = _end;
 buildList();
}

void GLPen::setControlPen( const QPoint & control1, const QPoint & control2 )
{
 QPoint * control_point1 = new QPoint( control1.x(), control1.y() );
 try {
     points.append( control_point1 );
     }
 catch(...)
     {
     delete control_point1;
     throw;
     }
    
 QPoint * control_point2 = new QPoint( control2.x(), control2.y() );
 try {
 points.append( control_point2 );
     }
 catch(...)
     {
     delete control_point2;
     throw;
     }    
 
 bezier_point = control1;
 buildList();
}

void GLPen::setOneControlPen( const QPoint & _end )
{
 QPoint * end_point = new QPoint( _end.x(), _end.y() );
 try {
     points.append( end_point );
     }
 catch(...)
     {
     delete end_point;
     throw;
     }
 buildList();
}

void GLPen::replaceControlPen( const QPoint & _end )
{
 QPoint * end_point = new QPoint( _end.x(), _end.y() );
 QPoint * aux = new(std::nothrow) QPoint( ( - ( _end.x() - bezier_point.x() ) + bezier_point.x() ),
                                          ( - ( _end.y() - bezier_point.y() ) + bezier_point.y() ) );
 if(!aux)
     {
     delete end_point;
     throw;
     }

 points.replace( points.count() - 2, aux );
 points.replace( points.count() - 1, end_point );
 buildList();
}

void GLPen::replaceControlPen( const QPoint & control1, const QPoint & control2 )
{
 QPoint * control_point1 = new QPoint( control1.x(), control1.y() );
 QPoint * control_point2 = new(std::nothrow) QPoint( control2.x(), control2.y() );
 if(!control_point2)
     {
     delete control_point1;
     throw;
     }
 
 points.replace( points.count() - 2, control_point1 );
 points.replace( points.count() - 1, control_point2 );
 buildList();
}

void GLPen::lastControlPen( const QPoint & _end )
{
 QPoint * end_point = new QPoint( _end.x(), _end.y() );
 points.replace( points.count() - 1, end_point );
 buildList();
}

void GLPen::angleControlPen( const QPoint & _end )
{
 //Calcular lo del angulo de inclinacion
 QPoint * end_point = new QPoint( _end.x(), _end.y() );
 double theta = asin( ( _end.y() - bezier_point.y() ) / sqrt( pow ( (_end.x() - bezier_point.x() ), 2 ) +
                         pow ( ( _end.y() - bezier_point.y() ), 2 ) ) );

 if ( ( fabs( fmod( ( ( 180.0 * theta ) / 3.14159265358979323846 ), 45 ) == 0.0 ) ) )
 {
   QPoint * aux = new(std::nothrow) QPoint( ( - ( _end.x() - bezier_point.x() ) + bezier_point.x() ),
                                          ( - ( _end.y() - bezier_point.y() ) + bezier_point.y() ) );
   if(!aux)
       {
	 delete end_point;
	 throw;
	 }
   
   points.replace( points.count() - 2, aux );
   points.replace( points.count() - 1, end_point );
 }
 buildList();
}

void GLPen::moveControlPen( const QPoint & _end )
{
 QPoint * end_point = new QPoint( _end.x(), _end.y() );
 QPoint * last_point = points.at( points.count() - 1 );
 QPoint * reflex_point = points.at( points.count() - 2 );

 QPoint * aux = new(std::nothrow) QPoint( reflex_point -> x() + ( _end.x() - last_point -> x() ) ,
                                          reflex_point -> y() + ( _end.y() - last_point -> y() ) );
 if(!aux)
     {
     delete end_point;
     throw;
     }
 
 //calculate el new bezier point or mid point of the bezier
 bezier_point.setX( bezier_point.x() + ( _end.x() - last_point -> x() ) );
 bezier_point.setY( bezier_point.y() + ( _end.y() - last_point -> y() ) );

 points.replace( points.count() - 1, end_point );
 points.replace( points.count() - 2, aux );
 buildList();
}

void GLPen::moveControlPen( int index, const QPoint & _end )
{
 QPoint * last_point = points.at( index );
 QPoint * reflex_point = points.at( index + 1 );

 QPoint * aux1 = new QPoint( last_point -> x() + ( _end.x() - last_point -> x() ) ,
                             last_point -> y() + ( _end.y() - last_point -> y() ) );
 QPoint * aux2 = new(std::nothrow) QPoint( reflex_point -> x() + ( _end.x() - reflex_point -> x() ) ,
                                           reflex_point -> y() + ( _end.y() - reflex_point -> y() ) );
 if(!aux2)
     {
     delete aux1;
     throw;
     }

 points.replace( index, aux1 );
 points.replace( index + 1, aux2 );
 buildList();
}

void GLPen::moveBezierPoint( const QPoint & _end )
{
 QPoint * end_point = new QPoint( _end.x(), _end.y() );
 QPoint *point;
  int i = 0, j=-1;

  for ( point = bezier.first(); point; point = bezier.next() )
   {
     if ( ( abs( point -> x() - _end.x() ) <= 10 ) && ( abs( point -> y() - _end.y() ) <= 10 ) )
     {
	end_point -> setX( point -> x() + ( _end.x() - point -> x() ) );
	end_point -> setY( point -> y() + ( _end.y() - point -> y() ) );
	j = i;
	break;
     }
     i++;
   }
  if ( j != -1 )
    {
     bezier.replace( j, end_point );
     try {
         moveControlPen( j*2, _end );
	   }
     catch(...)
         {
	   bezier.remove(j);
	   delete end_point;
	   throw;
	   }
    }
 buildList();
}


void GLPen::setControlPointsPen( const int & _control_points)  //number of control points
{
 control_points = _control_points;
}

void GLPen::setKnotsCount( const int & _knots_count )
{
 knots_count = _knots_count;
}

void GLPen::setOrderPen( const int & order )
{
 order_pen = order;
}

void GLPen::setStridePen( const int & stride )
{
 stride_pen = stride;
}

void GLPen::setBezierPoint( const QPoint & _bezier )
{
  bezier.append( &_bezier );
}

void GLPen::setPoints( QPtrList<QPoint> _points )
{
    points = _points;
}

void GLPen::setBezier( QPtrList<QPoint> _bezier )
{
    bezier = _bezier;
}

void GLPen::calculateTopLeft()
{
  int xi = 1000, yi = 1000;
  QPoint *point;
  for ( point = points.first(); point; point = points.next() )
   {
    if ( point -> x() < xi )
        xi = point -> x();
    if ( point -> y() < yi )
        yi = point -> y();
   }
   setTopLeft( QPoint( xi, yi ) );
}

void GLPen::calculateBottomRight()
{
  int xf = -1000, yf = -1000;
  QPoint *point;
  for ( point = points.first(); point; point = points.next() )
   {
    if ( point -> x() > xf )
        xf = point -> x();
    if ( point -> y() > yf )
        yf = point -> y();
   }
  setBottomRight( QPoint( xf, yf ) );
}

void GLPen::clearPoints()
{
  points.clear();
}

void GLPen::setShowBezierPoints( bool show )
{
  show_bezier_point = show;
}

QPoint GLPen::endPen() const
{
 return end;
}

int GLPen::orderPen() const
{
 return order_pen;
}

int GLPen::stridePen() const
{
 return stride_pen;
}

int GLPen::controlPointsPen() const
{
 return control_points;
}

int GLPen::knotsCount() const
{
 return knots_count;
}

QPoint GLPen::bezierPoint() const
{
  //deberia retorar un valor de las lista de los puntos de bezier, no esto!
  return bezier_point;
}

QString GLPen::clipboardGraphic()
{
  QString graphic;
  QPoint *point;
  graphic = ( graphic.setNum( kindGraphic() ) + " " + graphic.setNum( originPoint().x() ) + " " +
              graphic.setNum( originPoint().y() ) + " " + graphic.setNum( endPen().x() ) + " " +
	      graphic.setNum( endPen().y() ) + " " + graphic.setNum( outlineColor().colorRed() ) + " " +
	      graphic.setNum( outlineColor().colorGreen() ) + " " + graphic.setNum( outlineColor().colorBlue() ) + " " +
	      graphic.setNum( outlineColor().colorAlpha() ) + " " + graphic.setNum( widthPoint().thicknessMinBrush() ) + " " +
	      graphic.setNum( widthPoint().thicknessMaxBrush() ) + " " + graphic.setNum( widthPoint().smoothnessBrush() ) + " " +
	      graphic.setNum( stipplePattern() ) + " " + graphic.setNum( stippleFactor() ) + " " +
	      graphic.setNum( rotationAngle() ) + " " + graphic.setNum( translate.x() ) + " " +
	      graphic.setNum( translate.y() ) + " " + graphic.setNum( points.count() ) );

  for ( point = points.first(); point; point = points.next() )
    {
       graphic = graphic + " " + ( graphic.setNum( point -> x() ) + " " +  graphic.setNum( point -> y() ) );
    }
  for ( point = bezier.first(); point; point = bezier.next() )
    {
       graphic = graphic + " " + ( graphic.setNum( point -> x() ) + " " +  graphic.setNum( point -> y() ) );
    }
  return graphic;
}

QPtrList<QPoint> GLPen::getPoints() const
{
    return points;
}

QPtrList<QPoint> GLPen::getBezier() const
{
    return bezier;
}

bool GLPen::showBezierPoints() const
{
  return show_bezier_point;
}

QDomElement GLPen::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Pen" );

    //----------- Properties inherited from GLGraphicComponent ----------

    QString bool_str;
    if ( fill_graphic )
       bool_str = "true";
    else
       bool_str = "false";

    e.setAttribute( "Kind", kind_graphic );
    e.setAttribute( "Id", id_graphic_component );
    e.setAttribute( "Fill", bool_str );
    e.setAttribute( "Pattern", stipple_pattern );
    e.setAttribute( "Factor", stipple_factor );
    e.setAttribute( "Z", z );
    e.setAttribute( "Angle", rotation_angle );
    e.setAttribute( "XTranslate", translate.x() );
    e.setAttribute( "YTranslate", translate.y() );

    QDomElement origin_tag = doc.createElement( "Origin" );
    origin_tag.setAttribute( "X", origin.x() );
    origin_tag.setAttribute( "Y", origin.y() );
    e.appendChild( origin_tag );

    QDomElement color_tag = color.createXML( doc );
    e.appendChild( color_tag );

    QDomElement width_tag = width.createXML( doc );
    e.appendChild( width_tag );

    //----------- Special Properties ----------------

    e.setAttribute( "Order", order_pen );
    e.setAttribute( "Stride", stride_pen );
    e.setAttribute( "Knots", knots_count );
    e.setAttribute( "Control", control_points );

    QDomElement end_tag = doc.createElement( "End" );
    end_tag.setAttribute( "X", end.x() );
    end_tag.setAttribute( "Y", end.y() );
    e.appendChild( end_tag );

    QDomElement points_tag = doc.createElement( "Points" );
    e.appendChild( points_tag );
    QPoint *p_it;
    for ( p_it = points.first(); p_it; p_it = points.next() )
    {
	QDomElement point_tag = doc.createElement( "Point" );

	point_tag.setAttribute( "X", p_it -> x() );
	point_tag.setAttribute( "Y", p_it -> y() );

	points_tag.appendChild( point_tag );
    }

    QDomElement beziers_tag = doc.createElement( "Beziers" );
    e.appendChild( beziers_tag );
    QPoint *b_it;
    for ( b_it = bezier.first(); b_it; b_it = bezier.next() )
    {
	QDomElement bezier_tag = doc.createElement( "Bezier" );

	bezier_tag.setAttribute( "X", b_it -> x() );
	bezier_tag.setAttribute( "Y", b_it -> y() );

	beziers_tag.appendChild( bezier_tag );
    }

    return e;
}
