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

#include "glline.h"

//-------------- CONSTRUCTOR ---------------

GLLine::GLLine( QGLWidget *parent, const QPoint & _origin, const KTColor & _color, const Brush & _width, const QPoint & _end ) : GLGraphicComponent( parent, _origin, _color, _width ), end( _end )
{
    	 Q_CHECK_PTR( parent );

	 setKindGraphic( GC_LINE );
         id_graphic_component = glGenLists( 1 );
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

GLLine::GLLine( const GLLine & line ) : GLGraphicComponent( line.parentWidget(), line.originPoint(), line.outlineColor(), line.widthPoint() ), end( line.endLine() )
{
	 setKindGraphic( GC_LINE );
         id_graphic_component = glGenLists( 1 );
 	 stipple_factor = line.stippleFactor();
  	 stipple_pattern = line.stipplePattern();
	 rotation_angle = line.rotationAngle();
	 translate.setX( 0 );
	 translate.setY( 0 );
	 local_selection_name = selection_name;
	 selection_name++;
         buildList();
}

//------------- DESTRUCTOR ------------------

GLLine::~GLLine()
{

}


//------------ PUBLIC MEMBERS ---------------

//implementation of the virtual method buildList to Lines
void GLLine::buildList()
{
   glNewList( id_graphic_component, GL_COMPILE );

	glColor4f( outlineColor().colorRed(), outlineColor().colorGreen(),
	              outlineColor().colorBlue(), outlineColor().colorAlpha() );

	if( stipplePattern() != 0xffff )
	{
		glPushAttrib( GL_ENABLE_BIT | GL_TEXTURE_BIT );
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT );

		GLubyte texture[16];

		GLushort p = stipplePattern();
		for( int i=0; i<16; i++ )
		{
			if( p & 1 )
				texture[i] = 255;
			else
				texture[i] = 0;
			p >>= 1;
		}
		glTexImage1D( GL_TEXTURE_1D, 0, GL_ALPHA, 16, 0, GL_ALPHA, GL_UNSIGNED_BYTE, (GLvoid *)texture );

		glEnable( GL_TEXTURE_1D );
		lineImpl( originPoint(), endLine(), widthPoint().thicknessMinBrush(), stippleFactor(), outlineColor() );
		glDisable( GL_TEXTURE_1D );
		glPopAttrib();
	}
	else
	{
		lineImplFast( originPoint(), endLine(), widthPoint().thicknessMinBrush(), outlineColor() );
	}

 glEndList();
}

void GLLine::angleLine( const QPoint & _end )
{
 double theta = asin( ( originPoint().y() - _end.y() ) / sqrt( pow ( ( originPoint().x() - _end.x() ), 2 ) +
                        pow ( ( originPoint().y() - _end.y() ), 2 ) ) );

 if ( fmod( ( ( 180.0 * theta ) / M_PI ), 45 ) == 0.0 )
   {
     setEndLine ( _end );
     buildList();
   }
 else if ( fmod( ( ( 180.0 * theta ) / M_PI ), 45 ) < 10.0 )
   {
/*   double m = ( ( ( 180.0 * theta ) / 3.14159265358979323846 ) / 45 );
     double new_theta = ( ( ( m * 45 ) * 3.14159265358979323846 ) / 180 );
     double x, y;
     y = _end.y();
     x = originPoint().x() - sqrt( pow ( ( originPoint().y() - y ) / new_theta, 2 )  - pow ( ( originPoint().y() - y ), 2 ) );
     qDebug("Theta : %f, New_Theta : %f, x :%f, y : %f", theta, new_theta, x, y );

     double x, y, distancia;
     distancia = sqrt( pow( ( _end.x() - originPoint().x() ), 2 ) - pow( ( _end.y() - originPoint().y() ), 2 ) );
     y = distancia * sin( ( 180.0 * theta ) / 3.14159265358979323846 );
     x = distancia * cos( ( 180.0 * theta ) / 3.14159265358979323846 );
     qDebug(" x :%f, y : %f", x, y );
     QPoint new_point( x, y );
*/
     setEndLine ( _end );
     buildList();
   }
}

void GLLine::setEndLine( const QPoint & _end )
{
 end = _end;
 buildList();
}

void GLLine::calculateTopLeft()
{
   if ( originPoint().x() < endLine().x() )
     {
       	if ( originPoint().y() < endLine().y() )
            setTopLeft( originPoint() );
	else
	    setTopLeft( QPoint( originPoint().x(), endLine().y() ) );
     }
   else
     {
       	if ( originPoint().y() > endLine().y() )
	    setTopLeft( endLine() );
	else
            setTopLeft( QPoint( endLine().x(), originPoint().y() ) );
     }
}

void GLLine::calculateBottomRight()
{
   if ( originPoint().x() > endLine().x() )
     {
       	if ( originPoint().y() > endLine().y() )
            setBottomRight( originPoint() );
	else
	    setBottomRight( QPoint( originPoint().x(), endLine().y()) );
     }
   else
     {
       	if ( originPoint().y() < endLine().y() )
 	    setBottomRight( endLine() );
 	else
            setBottomRight( QPoint( endLine().x(), originPoint().y() ) );
   }
}

QPoint GLLine::endLine() const
{
 return end;
}

QString GLLine::clipboardGraphic()
{
  QString graphic;
  graphic = ( graphic.setNum( kindGraphic() ) + " " + graphic.setNum( originPoint().x() ) + " " +
              graphic.setNum( originPoint().y() ) + " " + graphic.setNum( endLine().x() ) + " " +
	      graphic.setNum( endLine().y() ) + " " + graphic.setNum( outlineColor().colorRed() ) + " " +
	      graphic.setNum( outlineColor().colorGreen() ) + " " + graphic.setNum( outlineColor().colorBlue() ) + " " +
	      graphic.setNum( outlineColor().colorAlpha() ) + " " + graphic.setNum( widthPoint().thicknessMinBrush() ) + " " +
	      graphic.setNum( widthPoint().thicknessMaxBrush() ) + " " + graphic.setNum( widthPoint().smoothnessBrush() ) + " " +
	      graphic.setNum( stipplePattern() ) + " " + graphic.setNum( stippleFactor() ) + " " +
	      graphic.setNum( rotationAngle() ) + " " + graphic.setNum( translate.x() ) + " " +
	      graphic.setNum( translate.y() ));
  return graphic;
}

QDomElement GLLine::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Line" );

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

    return e;
}
