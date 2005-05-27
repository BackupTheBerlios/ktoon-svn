/***************************************************************************
 *   Copyright (C) 2004 by Fernando José Roldán Correa                     *
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

#include "glimage.h"

#include <qimage.h>
#include <cmath>

//-------------- CONSTRUCTOR ---------------

GLImage::GLImage( QGLWidget *parent, const QString &_file_name, const QPoint &_origin, const QPoint &_end ) : GLGraphicComponent( parent, _origin, Color(), Brush() ), file_name( _file_name ), end( _end )
{
    Q_CHECK_PTR( parent );

    setKindGraphic( GC_IMAGE );
    rotation_angle = 0.0;
    translate.setX( 0 );
    translate.setY( 0 );
    local_selection_name = selection_name;

    glEnable( GL_TEXTURE_2D );
    id_texture = loadTexture( file_name );
    glDisable( GL_TEXTURE_2D );

    id_graphic_component = glGenLists( 1 );

    buildList();
}

//------------- DESTRUCTOR ------------------

GLImage::~GLImage()
{

}

//------------ PUBLIC MEMBERS ---------------

void GLImage::buildList()
{
	glNewList( id_graphic_component, GL_COMPILE );
		glPushName( GLGraphicComponent::selection_name++ );
 		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, id_texture );
		glColor4f( 0.0, 0.0, 0.0, 0.0 );

		glBegin( GL_QUADS );
			glTexCoord2f( 0, 1 );
			glVertex2f( origin.x(), origin.y() );
			glTexCoord2f( 1, 1 );
			glVertex2f(  end.x(), origin.y() );
			glTexCoord2f( 1, 0 );
			glVertex2f(  end.x(), end.y() );
			glTexCoord2f( 0, 0 );
			glVertex2f( origin.x(), end.y() );
		glEnd();
		glDisable( GL_TEXTURE_2D );
		glPopName();
	glEndList();
}

void GLImage::setEndImage( const QPoint &_end )
{
    end = _end;
    buildList();
}

void GLImage::setFillColor( const Color & _fill_color )
{
  fill_color = _fill_color;

//	glPushName( GLGraphicComponent::selection_name++ );
//	glDisable( GL_TEXTURE_2D );
	glEnable( GL_POLYGON_OFFSET_FILL );
	glPolygonOffset( 0, -2 );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
#ifdef GL_ARB_imaging
	glBlendEquation( GL_FUNC_ADD );
#endif
	glColor4f( fillColor().colorRed(), fillColor().colorGreen(),
		   fillColor().colorBlue(), fillColor().colorAlpha() );
	glBegin( GL_QUADS );
		glTexCoord2f( 0, 1 );
		glVertex2f( origin.x(), origin.y() );
		glTexCoord2f( 1, 1 );
		glVertex2f(  end.x(), origin.y() );
		glTexCoord2f( 1, 0 );
		glVertex2f(  end.x(), end.y() );
		glTexCoord2f( 0, 0 );
		glVertex2f( origin.x(), end.y() );
	glEnd();
//	glEnable( GL_TEXTURE_2D );
	glDisable( GL_POLYGON_OFFSET_FILL );
	glDisable( GL_BLEND );
//	glPopName();

  buildList();
}

void GLImage::calculateTopLeft()
{
   if ( originPoint().x() < endImage().x() )
     {
       	if ( originPoint().y() < endImage().y() )
            setTopLeft( originPoint() );
	else
	    setTopLeft( QPoint( originPoint().x(), endImage().y() ) );
     }
   else
     {
       	if ( originPoint().y() > endImage().y() )
	    setTopLeft( endImage() );
	else
            setTopLeft( QPoint( endImage().x(), originPoint().y() ) );
     }
}

void GLImage::calculateBottomRight()
{
   if ( originPoint().x() > endImage().x() )
     {
       	if ( originPoint().y() > endImage().y() )
            setBottomRight( originPoint() );
	else
	    setBottomRight( QPoint( originPoint().x(), endImage().y()) );
     }
   else
     {
       	if ( originPoint().y() < endImage().y() )
 	    setBottomRight( endImage() );
 	else
            setBottomRight( QPoint( endImage().x(), originPoint().y() ) );
     }
}

QPoint GLImage::endImage() const
{
    return end;
}

QString GLImage::clipboardGraphic()
{
    QString graphic;
    graphic = ( graphic.setNum( kindGraphic() ) + " " + graphic.setNum( originPoint().x() ) + " " +
                graphic.setNum( originPoint().y() ) + " " + graphic.setNum( endImage().x() ) + " " +
	        graphic.setNum( endImage().y() ) );
    return graphic;
}

Color GLImage::fillColor() const
{
  return fill_color;
}

QDomElement GLImage::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Image" );

    //----------- Properties inherited from GLGraphicComponent ----------

    e.setAttribute( "Kind", kind_graphic );
    e.setAttribute( "Id", id_graphic_component );

    QDomElement origin_tag = doc.createElement( "Origin" );
    origin_tag.setAttribute( "X", origin.x() );
    origin_tag.setAttribute( "Y", origin.y() );
    e.appendChild( origin_tag );

    //----------- Special Properties ----------------

    QDomElement end_tag = doc.createElement( "End" );
    end_tag.setAttribute( "X", end.x() );
    end_tag.setAttribute( "Y", end.y() );
    e.appendChild( end_tag );

    return e;
}

GLuint GLImage::loadTexture( const QString & filename )
{
	GLuint _id_texture;

	QImage image, image2;
	if ( image.load( filename.latin1() ) == false )
	     qDebug( "Can't load the image" + filename );

	end = image.rect().bottomRight();

	int new_w = ( int )pow( 2.0, ceil( log( ( double )end.x() ) / log( 2.0 ) ) );
	int new_h = ( int )pow( 2.0, ceil( log( ( double )end.y() ) / log( 2.0 ) ) );

	if ( ( end.x() != new_w ) || ( end.y() != new_h ) )
	{
		QImage scaled_image;
		scaled_image = image.scale( new_w, new_h );
		image = scaled_image;
	}

	image2 = QGLWidget::convertToGLFormat( image );
	QRgb *texture = ( QRgb * )( image2.bits() ); //I'd like to make this variable as a class member!!!

	glGenTextures( 1, &_id_texture );
	glBindTexture( GL_TEXTURE_2D, _id_texture );

	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, image2.width(), image2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture );

        return _id_texture;
}
