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

#include <math.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "glpencil.h"

GLPencil::GLPencil( QGLWidget *parent, const QPoint & _origin, const Color & _color, const Brush & _width, const QPoint & _end ) : GLGraphicComponent( parent, _origin, _color, _width ), end( _end )
{
	setKindGraphic( GC_PENCIL );
        id_graphic_component = glGenLists( 1 );
	QPoint * origin_point = new QPoint( origin.x(), origin.y() );
	QPoint * end_point = new QPoint( end.x(), end.y() );
	points.append( origin_point );
	points.append( end_point );
 	stipple_factor = 1;
  	stipple_pattern = 0xFFFF;
  	z = 0.0;
	rotation_angle = 0.0;
	translate.setX( 0 );
	translate.setY( 0 );
	local_selection_name = selection_name;
        buildList();
	selection_name++;
}

GLPencil::GLPencil( const GLPencil & pencil ) : GLGraphicComponent( pencil.parentWidget(), pencil.originPoint(), pencil.outlineColor(), pencil.widthPoint() ), end( pencil.endPencil() )
{
	 setKindGraphic( GC_PENCIL );
         id_graphic_component = glGenLists( 1 );
 	 stipple_factor = pencil.stippleFactor();
  	 stipple_pattern = pencil.stipplePattern();
	 rotation_angle = pencil.rotationAngle();
	 translate.setX( 0 );
	 translate.setY( 0 );
	 local_selection_name = selection_name;
	 selection_name++;
	 points = pencil.getPoints();
         buildList();
}

GLPencil::~GLPencil( )
{

}

//implementation of the virtual method buildList to Bezier curve
void GLPencil::buildList( )
{
  	QPoint *initial_point;
	glNewList( id_graphic_component, GL_COMPILE );
   	   glPushMatrix();
	   glLineWidth( widthPoint().thicknessMinBrush() );
	   glColor4f( outlineColor().colorRed(), outlineColor().colorGreen(),
	              outlineColor().colorBlue(), outlineColor().colorAlpha() );
	   glEnable( GL_LINE_STIPPLE );
           glLineStipple( stippleFactor(), stipplePattern() );
           glBegin( GL_LINE_STRIP );
	   for ( unsigned int i = 0; i < points.count() ;  i++ )
	     {
	        initial_point = points.at( i );
       	        glVertex2i( initial_point -> x(), initial_point -> y() );
	     }
           glEnd();
	   glDisable( GL_LINE_STIPPLE );

	   if ( stipplePattern() == 0xFFFF )
	     {
	        glPointSize( widthPoint().thicknessMinBrush() );
	   	glBegin( GL_POINTS );
	   	for ( unsigned int i = 1; i < points.count() - 1;  i++ )
	     	    {
	        	initial_point = points.at( i );
       	        	glVertex2i( initial_point -> x(), initial_point -> y() );
	     	    }
           	glEnd();
	     }

   	   glPopMatrix();
        glEndList();
}

void GLPencil::anglePencil( const QPoint & _end )
{
 QPoint * point = new QPoint( _end.x(), _end.y() );
 double theta = asin( ( originPoint().y() - _end.y() ) / sqrt( pow ( (originPoint().x() - _end.x() ), 2 ) +
                        pow ( ( originPoint().y() - _end.y() ), 2 ) ) );

 if ( ( fabs( fmod( ( ( 180.0 * theta ) / 3.14159265358979323846 ), 45 ) == 0.0 ) ) )
     points.replace( points.count() - 1, point );
 buildList();
}

void GLPencil::linePencil( const QPoint & _end )
{
     QPoint * point = new QPoint( _end.x(), _end.y() );
     points.replace( points.count() - 1, point );
     buildList();
}

void GLPencil::setEndPencil( const QPoint & _end )
{
/* if ( abs( _end.x() - points.last() -> x() ) <= 2 && abs( _end.y() - points.last() -> y() ) <= 2 )
     return;
 if ( points.count() > 2 )
   {
	QPoint * point1 = new QPoint( *( points.at( points.count() - 1 ) ) );
	QPoint * point2 = new QPoint( *( points.last() ) );
	if ( ( ( point2 -> x() - point1 -> x() ) * ( _end.y() - point2 -> y() ) ) == ( ( point2 -> y() - point1 -> y() ) * ( _end.x() - point2 -> x() ) ) )
        {
	        point1 -> setX( _end.x() );
	        point1 -> setY( _end.y() );
		points.replace( points.count(), point1 );
	}
   }*/
 QPoint * point = new QPoint( _end.x(), _end.y() );
 points.append( point );
 buildList();
}

void GLPencil::smoothnessPencil( int smooth )
{

// buildList();
}

void GLPencil::setPoints( QPtrList<QPoint> _points )
{
    points = _points;
}

void GLPencil::calculateTopLeft()
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

void GLPencil::calculateBottomRight()
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

QPoint GLPencil::endPencil() const
{
 return end;
}

QString GLPencil::clipboardGraphic()
{
  QString graphic;
  QPoint *point;
  graphic = ( graphic.setNum( kindGraphic() ) + " " + graphic.setNum( originPoint().x() ) + " " +
              graphic.setNum( originPoint().y() ) + " " + graphic.setNum( endPencil().x() ) + " " +
	      graphic.setNum( endPencil().y() ) + " " + graphic.setNum( outlineColor().colorRed() ) + " " +
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
  return graphic;
}

QPtrList<QPoint> GLPencil::getPoints() const
{
    return points;
}

QDomElement GLPencil::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Pencil" );

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

    QDomElement end_tag = doc.createElement( "End" );
    end_tag.setAttribute( "X", end.x() );
    end_tag.setAttribute( "Y", end.y() );
    e.appendChild( end_tag );

    QPoint *p_it;
    for ( p_it = points.first(); p_it; p_it = points.next() )
    {
	QDomElement point_tag = doc.createElement( "Point" );

	point_tag.setAttribute( "X", p_it -> x() );
	point_tag.setAttribute( "Y", p_it -> y() );

	e.appendChild( point_tag );
    }

    return e;
}
