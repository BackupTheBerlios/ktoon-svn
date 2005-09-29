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

 #include "glrectangle.h"

//-------------- CONSTRUCTOR ---------------

GLRectangle::GLRectangle( QGLWidget *parent, const QPoint & _origin, const KTColor & outline_color, const Brush & _width, const QPoint & _end, KTColor & _fill_color ) : GLGraphicComponent( parent, _origin, outline_color, _width ), end( _end ), fill_color( _fill_color)
{
         Q_CHECK_PTR( parent );

	 setKindGraphic( GC_RECTANGLE );
         id_graphic_component = glGenLists( 1 );
	 setPerfectRectangle( false );
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

GLRectangle::GLRectangle( const GLRectangle & rectangle ) : GLGraphicComponent( rectangle.parentWidget(), rectangle.originPoint(), rectangle.outlineColor(), rectangle.widthPoint() ), end( rectangle.endRectangle() )
{
	 setKindGraphic( GC_RECTANGLE );
         id_graphic_component = glGenLists( 1 );
 	 stipple_factor = rectangle.stippleFactor();
  	 stipple_pattern = rectangle.stipplePattern();
	 rotation_angle = rectangle.rotationAngle();
	 translate.setX( 0 );
	 translate.setY( 0 );
	 local_selection_name = selection_name;
	 selection_name++;
	 perfect_rectangle = rectangle.perfectRectangle();
	 fill_color = rectangle.fillColor();
         buildList();
}

//------------- DESTRUCTOR ------------------

GLRectangle::~GLRectangle( )
{

}

//------------ PUBLIC MEMBERS ---------------

//implementation of the virtual method buildList for rectangle
void GLRectangle::buildList( )
{
         if ( perfectRectangle() == true )
          {
	   	setPerfectRectangle( false );
		if ( originPoint().x() <= endRectangle().x() && originPoint().y() < endRectangle().y() )
		   {
		     if ( abs( originPoint().x() - endRectangle().x() ) > abs( originPoint().y() - endRectangle().y() ) )
		       	{
		 	    QPoint *aux = new QPoint( originPoint().x() + abs( originPoint().x() - endRectangle().x() ),
			                              originPoint().y() + abs( originPoint().x()  - endRectangle().x() ) );
			    setEndRectangle ( *aux );
		 	}
		     else
		       	{
		 	    QPoint *aux = new QPoint( originPoint().x() + abs( originPoint().y() - endRectangle().y() ),
			                              originPoint().y() + abs( originPoint().y()  - endRectangle().y() ) );
			    setEndRectangle ( *aux );
		 	}
		   }
		else if ( originPoint().x() >= endRectangle().x() && originPoint().y() < endRectangle().y()  )
		   {
		     if ( abs( originPoint().x() - endRectangle().x() ) > abs( originPoint().y() - endRectangle().y() ) )
		       	{
		 	    QPoint *aux = new QPoint( originPoint().x() - abs( originPoint().x() - endRectangle().x() ),
			                              originPoint().y() + abs( originPoint().x()  - endRectangle().x() ) );
			    setEndRectangle ( *aux );
		 	}
		     else
		       	{
		 	    QPoint *aux = new QPoint( originPoint().x() - abs( originPoint().y() - endRectangle().y() ),
			                              originPoint().y() + abs( originPoint().y()  - endRectangle().y() ) );
			    setEndRectangle ( *aux );
		 	}
		   }
		else if ( originPoint().x() < endRectangle().x() && originPoint().y() >= endRectangle().y() )
		   {
		     if ( abs( originPoint().x() - endRectangle().x() ) > abs( originPoint().y() - endRectangle().y() ) )
		       	{
		 	    QPoint *aux = new QPoint( originPoint().x() + abs( originPoint().x() - endRectangle().x() ),
			                              originPoint().y() - abs( originPoint().x()  - endRectangle().x() ) );
			    setEndRectangle ( *aux );
		 	}
		     else
		       	{
		 	    QPoint *aux = new QPoint( originPoint().x() + abs( originPoint().y() - endRectangle().y() ),
			                              originPoint().y() - abs( originPoint().y()  - endRectangle().y() ) );
			    setEndRectangle ( *aux );
		 	}
		   }
		else if ( originPoint().x() > endRectangle().x() && originPoint().y() >= endRectangle().y() )
		   {
		     if ( abs( originPoint().x() - endRectangle().x() ) > abs( originPoint().y() - endRectangle().y() ) )
		       	{
		 	    QPoint *aux = new QPoint( originPoint().x() - abs( originPoint().x() - endRectangle().x() ),
			                              originPoint().y() - abs( originPoint().x()  - endRectangle().x() ) );
			    setEndRectangle ( *aux );
		 	}
		     else
		       	{
		 	    QPoint *aux = new QPoint( originPoint().x() - abs( originPoint().y() - endRectangle().y() ),
			                              originPoint().y() - abs( originPoint().y()  - endRectangle().y() ) );
			    setEndRectangle ( *aux );
		 	}
		   }
          }
	 glNewList( id_graphic_component, GL_COMPILE );
	   glLineWidth( widthPoint().thicknessMinBrush() );
	   //Draw fill rectangle
   	   glPushMatrix();

	   if( fill() == true )
	     {
		glColor4f( fillColor().colorRed(), fillColor().colorGreen(),
		           fillColor().colorBlue(), fillColor().colorAlpha() );
            	glRectf( originPoint().x(), originPoint().y(), endRectangle().x(), endRectangle().y() );
	     }
	    //Draw Contour Rectangle
	   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	   glEnable( GL_LINE_STIPPLE );
           glLineStipple( stippleFactor(), stipplePattern() );
	   glColor4f( outlineColor().colorRed(), outlineColor().colorGreen(),
	              outlineColor().colorBlue(), outlineColor().colorAlpha() );
	   glBegin( GL_POLYGON );
            	   glVertex2f( originPoint().x(), originPoint().y() );
           	   glVertex2f( endRectangle().x(), originPoint().y() );
           	   glVertex2f( endRectangle().x(), endRectangle().y() );
           	   glVertex2f( originPoint().x(), endRectangle().y() );
    	   glEnd();

	   glDisable( GL_LINE_STIPPLE );
	   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   	   glPopMatrix();
	 glEndList();
}

void GLRectangle::setEndRectangle( const QPoint & _end )
{
  end = _end;
  buildList();
}

void GLRectangle::setFillColor( const KTColor & _fill_color )
{
  fill_color = _fill_color;
  buildList();
}

void GLRectangle::setPerfectRectangle( const bool & perfect )
{
  perfect_rectangle = perfect;
}

void GLRectangle::calculateTopLeft()
{
   if ( originPoint().x() < endRectangle().x() )
     {
       	if ( originPoint().y() < endRectangle().y() )
            setTopLeft( originPoint() );
	else
	    setTopLeft( QPoint( originPoint().x(), endRectangle().y() ) );
     }
   else
     {
       	if ( originPoint().y() > endRectangle().y() )
	    setTopLeft( endRectangle() );
	else
            setTopLeft( QPoint( endRectangle().x(), originPoint().y() ) );
     }
}

void GLRectangle::calculateBottomRight()
{
   if ( originPoint().x() > endRectangle().x() )
     {
       	if ( originPoint().y() > endRectangle().y() )
            setBottomRight( originPoint() );
	else
	    setBottomRight( QPoint( originPoint().x(), endRectangle().y()) );
     }
   else
     {
       	if ( originPoint().y() < endRectangle().y() )
 	    setBottomRight( endRectangle() );
 	else
            setBottomRight( QPoint( endRectangle().x(), originPoint().y() ) );
     }
}

QPoint GLRectangle::endRectangle() const
{
  return end;
}

KTColor GLRectangle::fillColor() const
{
  return fill_color;
}

bool GLRectangle::perfectRectangle() const
{
  return perfect_rectangle;
}

QString GLRectangle::clipboardGraphic()
{
  QString graphic;
  graphic = ( graphic.setNum( kindGraphic() ) + " " + graphic.setNum( originPoint().x() ) + " " +
              graphic.setNum( originPoint().y() ) + " " + graphic.setNum( endRectangle().x() ) + " " +
	      graphic.setNum( endRectangle().y() ) + " " + graphic.setNum( outlineColor().colorRed() ) + " " +
	      graphic.setNum( outlineColor().colorGreen() ) + " " + graphic.setNum( outlineColor().colorBlue() ) + " " +
	      graphic.setNum( outlineColor().colorAlpha() ) + " " + graphic.setNum( fillColor().colorRed() ) + " " +
	      graphic.setNum( fillColor().colorGreen() ) + " " + graphic.setNum( fillColor().colorBlue() ) + " " +
	      graphic.setNum( fillColor().colorAlpha() ) + " " + graphic.setNum( widthPoint().thicknessMinBrush() ) + " " +
	      graphic.setNum( widthPoint().thicknessMaxBrush() ) + " " + graphic.setNum( widthPoint().smoothnessBrush() ) + " " +
	      graphic.setNum( stipplePattern() ) + " " + graphic.setNum( stippleFactor() ) + " " +
	      graphic.setNum( perfectRectangle() ) + " " + graphic.setNum( rotationAngle() ) + " " +
	      graphic.setNum( translate.x() ) + " " + graphic.setNum( translate.y() ) );
  return graphic;
}

QDomElement GLRectangle::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Rectangle" );

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
    if ( perfect_rectangle )
       bool_str2 = "true";
    else
       bool_str2 = "false";

    e.setAttribute( "Perfect", bool_str2 );

    QDomElement end_tag = doc.createElement( "End" );
    end_tag.setAttribute( "X", end.x() );
    end_tag.setAttribute( "Y", end.y() );
    e.appendChild( end_tag );

    QDomElement color_tag2 = fill_color.createXML( doc );
    color_tag2.setAttribute( "Role", "Fill" );
    e.appendChild( color_tag2 );

    return e;
}
