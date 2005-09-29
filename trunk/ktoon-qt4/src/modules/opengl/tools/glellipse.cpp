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
#include <stdlib.h>

#include "glellipse.h"

//-------------- CONSTRUCTOR ---------------

GLEllipse::GLEllipse( QGLWidget *parent, const QPoint &  _origin, const KTColor & _color, const Brush & _width, const QPoint & _radius, KTColor & _fill_color ) : GLGraphicComponent( parent, _origin, _color, _width ), radius( _radius ), fill_color( _fill_color)
{
	 Q_CHECK_PTR( parent );

         setKindGraphic( GC_ELLIPSE );
         id_graphic_component = glGenLists( 1 );
	 setPerfectEllipse( false );
 	 stipple_factor = 1;
  	 stipple_pattern = 0xFFFF;
  	 //z = 0.0;
	 rotation_angle = 0.0;
	 translate.setX( 0 );
	 translate.setY( 0 );
	 local_selection_name = selection_name;
         buildList();
	 selection_name++;
}

GLEllipse::GLEllipse( const GLEllipse & ellipse ) : GLGraphicComponent( ellipse.parentWidget(), ellipse.originPoint(), ellipse.outlineColor(), ellipse.widthPoint() ), radius( ellipse.radiusEllipse() )
{
	 setKindGraphic( GC_ELLIPSE );
         id_graphic_component = glGenLists( 1 );
 	 stipple_factor = ellipse.stippleFactor();
  	 stipple_pattern = ellipse.stipplePattern();
	 rotation_angle = ellipse.rotationAngle();
	 translate.setX( 0 );
	 translate.setY( 0 );
	 local_selection_name = selection_name;
	 selection_name++;
	 perfect_ellipse = ellipse.perfectEllipse();
	 fill_color = ellipse.fillColor();
         buildList();
}

//------------- DESTRUCTOR ------------------

GLEllipse::~GLEllipse( )
{

}

//------------ PUBLIC MEMBERS ---------------

//implementation of the virtual method buildList to circle
void GLEllipse::buildList( )
{
 	 GLfloat DEG2RAD = M_PI / 180.0;
         GLfloat xradius;
         GLfloat yradius;
         float degInRad;

         xradius = ( float )( abs( radiusEllipse().x() - originPoint().x() ) );
         yradius = ( float )( abs( radiusEllipse().y() - originPoint().y() ) );

         if ( perfectEllipse() )
         {
            if ( xradius > yradius )
	     {
               yradius = xradius;
	       radius.setY( originPoint().y() + ( int )yradius );
	     }
            else
	     {
               xradius = yradius;
	       radius.setX( originPoint().x() + ( int )xradius );
	     }
         }

	 glNewList( id_graphic_component, GL_COMPILE );
	   glLineWidth( widthPoint().thicknessMinBrush() );
    	   glPushMatrix();
	   glTranslatef( originPoint().x(), originPoint().y(), 0 );

	   if( fill() == true )
	   	 glColor4f( fillColor().colorRed(), fillColor().colorGreen(),
		            fillColor().colorBlue(), fillColor().colorAlpha() );
	   else
	   	 glColor4f( 0.0, 0.0, 0.0, 0.0 );

     	   glBegin( GL_POLYGON );
	       for( int i = 0; i < 360; i+=2 )
	   	{
	   		degInRad = i * DEG2RAD;
            		glVertex2d( cos( degInRad ) * ( xradius + 1 ), sin( degInRad ) * ( yradius + 1 ) );
	      	}
    	   glEnd();

	   //Draw the contour ellipse

	   glColor4f( outlineColor().colorRed(), outlineColor().colorGreen(),
	              outlineColor().colorBlue(), outlineColor().colorAlpha() );
	   glEnable( GL_LINE_STIPPLE );
           glLineStipple( stippleFactor(), stipplePattern() );
	   glBegin( GL_LINE_LOOP );
	     double x1, y1;
	     for( int i = 0; i < 360; i+=2 )
	      {
	     	degInRad = i * DEG2RAD;
		x1 = cos( degInRad ) * xradius;
		y1 = sin( degInRad ) * yradius;

		glVertex2d( x1, y1 );
	      }
    	   glEnd();

	   if ( stipplePattern() == 0xFFFF )
	     {
	        glPointSize( widthPoint().thicknessMinBrush() );
	   	glBegin( GL_POINTS );
	          double x1, y1;
	     	  for( int i = 0; i < 360; i++ )
	      	     {
	     	   	degInRad = i * DEG2RAD;
		    	x1 = cos( degInRad ) * xradius;
		    	y1 = sin( degInRad ) * yradius;
		    	glVertex2d( x1, y1 );
	      	     }
    	   	glEnd();
	     }

   	   glPopMatrix();

	 glDisable( GL_LINE_STIPPLE );
	 glEndList();
}

void GLEllipse::setRadiusEllipse( const QPoint & _radius )
{
  radius = _radius;
  buildList();
}

void GLEllipse::setFillColor( const KTColor & _fill_color )
{
  fill_color = _fill_color;
  buildList();
}

void GLEllipse::setPerfectEllipse( const bool & perfect )
{
  perfect_ellipse = perfect;
  buildList();
}

void GLEllipse::calculateTopLeft()
{
   if( originPoint().x() < radiusEllipse().x() )
     {
      if( originPoint().y() < radiusEllipse().y() )
          setTopLeft( QPoint( originPoint().x() - ( radiusEllipse().x() - originPoint().x() ),
	                      originPoint().y() - ( radiusEllipse().y() - originPoint().y() ) ) );
      else
	  setTopLeft( QPoint( originPoint().x() - ( radiusEllipse().x() - originPoint().x() ), radiusEllipse().y() ) );
     }
   else
     {
      if( originPoint().y() > radiusEllipse().y() )
	  setTopLeft( QPoint( radiusEllipse() ) );
      else
          setTopLeft( QPoint( radiusEllipse().x(), originPoint().y() - ( radiusEllipse().y() - originPoint().y() ) ) );
     }
}

void GLEllipse::calculateBottomRight()
{
  if ( originPoint().x() > radiusEllipse().x() )
     {
      if ( originPoint().y() > radiusEllipse().y() )
           setBottomRight( QPoint( originPoint().x() - ( radiusEllipse().x() - originPoint().x() ),
	                           originPoint().y() - ( radiusEllipse().y() - originPoint().y() ) ) );
      else
	   setBottomRight( QPoint( originPoint().x() - ( radiusEllipse().x() - originPoint().x() ), radiusEllipse().y() ) );
     }
  else
     {
      if ( originPoint().y() < radiusEllipse().y() )
	   setBottomRight( radiusEllipse() );
      else
           setBottomRight( QPoint( radiusEllipse().x(), originPoint().y() - ( radiusEllipse().y() - originPoint().y() ) ) );
     }
}

bool GLEllipse::perfectEllipse() const
{
  return perfect_ellipse;
}

QPoint GLEllipse::radiusEllipse() const
{
  return radius;
}

KTColor GLEllipse::fillColor() const
{
  return fill_color;
}

QString GLEllipse::clipboardGraphic()
{
	QString graphic;
	graphic = ( graphic.setNum( kindGraphic() ) + " " + graphic.setNum( originPoint().x() ) + " " +
			graphic.setNum( originPoint().y() ) + " " + graphic.setNum( radiusEllipse().x() ) + " " +
			graphic.setNum( radiusEllipse().y() ) + " " + graphic.setNum( outlineColor().colorRed() ) + " " +
			graphic.setNum( outlineColor().colorGreen() ) + " " + graphic.setNum( outlineColor().colorBlue() ) + " " +
			graphic.setNum( outlineColor().colorAlpha() ) + " " + graphic.setNum( fillColor().colorRed() ) + " " +
			graphic.setNum( fillColor().colorGreen() ) + " " + graphic.setNum( fillColor().colorBlue() ) + " " +
			graphic.setNum( fillColor().colorAlpha() ) + " " + graphic.setNum( widthPoint().thicknessMinBrush() ) + " " +
			graphic.setNum( widthPoint().thicknessMaxBrush() ) + " " + graphic.setNum( widthPoint().smoothnessBrush() ) + " " +
			graphic.setNum( stipplePattern() ) + " " + graphic.setNum( stippleFactor() ) + " " +
			graphic.setNum( perfectEllipse() ) + " " + graphic.setNum( rotationAngle() ) + " " +
			graphic.setNum( translate.y() ) + " " + graphic.setNum( translate.x() ) );
	return graphic;
}

QDomElement GLEllipse::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Ellipse" );

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
    color_tag.setAttribute( "Role", "Outline" );
    e.appendChild( color_tag );

    QDomElement width_tag = width.createXML( doc );
    e.appendChild( width_tag );

    //----------- Special Properties ----------------

    QString bool_str2;
    if ( perfect_ellipse )
       bool_str2 = "true";
    else
       bool_str2 = "false";

    e.setAttribute( "Perfect", bool_str2 );

    QDomElement radius_tag = doc.createElement( "Radius" );
    radius_tag.setAttribute( "X", radius.x() );
    radius_tag.setAttribute( "Y", radius.y() );
    e.appendChild( radius_tag );

    QDomElement color_tag2 = fill_color.createXML( doc );
    color_tag2.setAttribute( "Role", "Fill" );
    e.appendChild( color_tag2 );

    return e;
}
