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

#include <qgl.h>
#include <qfont.h>
#include <GL/glx.h>

#include "glgrid.h"
#include "drawingarea.h"

#define GRID_FONT "fixed"

GLGrid::GLGrid( QGLWidget *parent, const GLuint & max_width, const GLuint & max_height, const GLuint & number_lines, const bool & zoom_lines, const bool & ntsc_zone ) : max_width ( max_width ), max_height ( max_height ), number_lines ( number_lines ), zoom_lines( zoom_lines ), ntsc_zone( ntsc_zone )
{
    parent_widget = parent;
    grid_color = QColor( 210, 210, 210 );
    ntsc_color = QColor( 255, 0, 0 );
    min_width = 0;
    min_height = 0;
    id_grid = glGenLists( 11 );

    show_numbers = true;

    id_grid2 = id_grid + 1;
    id_grid12 = id_grid + 2;
    id_grid16 = id_grid + 3;
    id_sub_grid12 = id_grid + 4;
    id_sub_grid16 = id_grid + 5;
    id_ntsc = id_grid + 6;
    id_numbers2 = id_grid + 7;
    id_numbers12 = id_grid + 8;
    id_numbers16 = id_grid + 9;
    id_diagonals = id_grid + 10;
    //id_fonts = id_grid + 11;
    //id_fonts = glGenLists(1);
    id_fonts = glGenLists( 256 );

    buildGrid( 2, id_grid2 );
    buildGrid( 24, id_grid12 );
    buildGrid( 32, id_grid16 );

    buildSubGrid( 24, id_sub_grid12 );
    buildSubGrid( 32, id_sub_grid16 );

    buildNtsc();

    buildFonts();

    buildNumbers( 2, id_numbers2 );
    buildNumbers( 24, id_numbers12 );
    buildNumbers( 32, id_numbers16 );

    buildDiagonals();

    buildGrid();
}

GLGrid::~GLGrid()
{
   if( glIsList( id_grid ) )
   {
      glDeleteLists( id_grid, 11 );
      glDeleteLists( id_fonts, 256 );
   }
}


void GLGrid::draw()
{
	if( glIsList( id_grid ) )
		glCallList( id_grid );
	else
		qDebug( "invalid id_grid %d", id_grid );

}

void GLGrid::buildGrid( int nLines, GLuint list )
{
	GLfloat width  = 1.0;
	QColor color = grid_color;
	glNewList( list, GL_COMPILE );
		glColor3f( color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0 );
		glLineWidth( width );
		glBegin( GL_LINES );

		for ( int x = 0; x <= nLines; x++ )
		{
			float fraccion = ( float )x / ( float )nLines;
			if ( x == nLines / 2 )
				continue;

			if ( nLines >= 2  )
			{
					glVertex2f( 0, fraccion );
					glVertex2f( 1, fraccion );
					glVertex2f( fraccion, 0 );
					glVertex2f( fraccion, 1 );
			}
		}

		glEnd();

		// grid axes!

		if ( ( nLines % 2 == 0 ) && ( nLines > 2 ) )
		{
			float fraccion = 0.5;
			width = 2.0;
			glLineWidth( width );
			glBegin( GL_LINES );
				glVertex2f( 0, fraccion );
				glVertex2f( 1, fraccion );
				glVertex2f( fraccion, 0 );
				glVertex2f( fraccion, 1 );
			glEnd();
		}

	glEndList();
}

void GLGrid::buildSubGrid( int nLines, GLuint list )
{
	QColor color = grid_color;
	int h, s, v;
	color.getHsv( &h, &s, &v );
	if ( v <= 230 )
	    color.setRgb( color.red() + 25, color.green() + 25, color.blue() + 25 );

	glNewList( list, GL_COMPILE );
		GLfloat width = 0.5;
		glLineWidth( width );
		glColor3f( color.red()/255.0, color.green()/255.0, color.blue() / 255.0 );
		glBegin( GL_LINES );
		for ( int x = 0; x < nLines; x++ )
		{
			float fraccion = (float)x / (float)nLines;
			for ( int i = 0; i < 2; i++ )
			{
				float x2 =  fraccion + ((float)( i + 1 ) / nLines / 3.0) ;
				float y2 =  fraccion + ((float)( i + 1 ) / nLines / 3.0) ;

				glVertex2f( 0, y2 );
				glVertex2f( 1, y2 );
				glVertex2f( x2, 0 );
				glVertex2f( x2, 1 );
			}
		}
		glEnd();
	glEndList();
}

void GLGrid::buildNumbers( int nLines, GLuint list )
{
	QColor color = grid_color;

	glNewList( list, GL_COMPILE );
	for ( int x = 1; x <= nLines; x++ )
	{
		float fraccion = ( float )x / ( float )nLines;

		QString string_number = QString::number( x, 10 );
		if ( show_numbers == true )
		{
			drawNumber( -0.03, fraccion, 0, color, string_number );
			drawNumber( fraccion - 0.025, 0, 0, color, string_number );
		}
	}
	glEndList();
}

void GLGrid::buildNtsc( )
{
	QColor color = ntsc_color;
	glNewList( id_ntsc, GL_COMPILE );
		float width = 1;
		glLineWidth( width );
		glColor3f( color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0 );

		for ( int i = 1; i < 3; i++ )
		{
			float w = 1.0 / (float)( 10 * i);
			float h = 1.0 / (float)( 10 * i);

			//Inside Line

			float x2 = 1.0 - w;
			float y2 = 1.0 - h;

			glBegin( GL_LINE_LOOP );
				glVertex2f( w, h );
				glVertex2f( x2, h );
				glVertex2f( x2, y2 );
				glVertex2f( w, y2 );
			glEnd();
		}
	glEndList();
}

void GLGrid::buildDiagonals()
{
	QColor color = grid_color;
	int h, s, v;
	color.getHsv( &h, &s, &v );
	if ( v <= 235 )
	    color.setRgb( color.red() + 20, color.green() + 20, color.blue() + 20 );

	glNewList( id_diagonals, GL_COMPILE );
		GLfloat width = 1.0;
		glLineWidth( width );
		glColor3f( color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0 );
		glBegin( GL_LINES );
			glVertex2f( 1, 0 );
			glVertex2f( 0, 1 );
			glVertex2f( 0, 0 );
			glVertex2f( 1, 1 );
		glEnd();
	glEndList();
}

void GLGrid::buildGrid()
{
    //Draw the lines of the Grid
    QString string;

    //generates a display list of the Grid
    glNewList( id_grid, GL_COMPILE );
    glPushMatrix();

	glTranslatef( min_width, min_height, 0.0 );
	glScalef( max_width, max_height, 1.0 );

	switch( numberLines() )
	{
		case 2 : glCallList( id_grid2 ); break;
		case 24: glCallList( id_grid12 ); glCallList( id_diagonals ); break;
		case 32: glCallList( id_grid16 ); glCallList( id_diagonals ); break;
		default:
			qDebug("Error en el numero de lineas" );
			break;
	}

	if( zoom_lines == true && numberLines() > 2 )
	{
		switch( numberLines() )
		{
			case 24: glCallList( id_sub_grid12 ); break;
			case 32: glCallList( id_sub_grid16 ); break;
		}
	}

	if( show_numbers == true ) {
		switch( numberLines() ) {
			case 2 : glCallList( id_numbers2 ); break;
			case 24: glCallList( id_numbers12 ); break;
			case 32: glCallList( id_numbers16 ); break;
		}
	}

	if ( ntscZone() == true )
	{
		glCallList( id_ntsc );
	}

   glPopMatrix();
   glEndList();
}

void GLGrid::buildFonts()
{
    Font desc = XLoadFont( parent_widget -> x11Display(), GRID_FONT );
    glXUseXFont( desc, 0, 256, id_fonts );
}

void GLGrid::drawLine( const QPoint & initial, const QPoint & final, const QColor & color, const GLfloat & line_width )
{
     glLineWidth( line_width );
     glColor3f( color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0  );

     glBegin( GL_LINES );
       glVertex2i( initial.x(), initial.y() );
       glVertex2i( final.x(), final.y() );
     glEnd();

}

void GLGrid::drawLine( GLfloat initial_x, GLfloat initial_y, GLfloat final_x, GLfloat final_y, const QColor & color, const GLfloat & line_width )
{
     glLineWidth( line_width );
     glColor3f( color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0  );

     glBegin( GL_LINES );
       glVertex2f( initial_x, initial_y );
       glVertex2f( final_x, final_y );
     glEnd();
}

void GLGrid::drawNumber( const QPoint & initial, const QColor & color, const QString & string_number )
{
     glColor3f(  color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0  );
     parent_widget -> renderText( initial.x() - 10, initial.y() + 10, string_number, QFont( "helvetica", 12, QFont::Bold, TRUE ) );
}

void GLGrid::drawNumber( GLdouble x, GLdouble y, GLdouble z, const QColor & color, const QString & string_number ) const
{
     glColor3f( color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0 );

    glRasterPos3d(x, y, z);
    glPushAttrib( GL_LIST_BIT | GL_COLOR_BUFFER_BIT );
    glListBase( id_fonts );
    glCallLists( string_number.length(), GL_UNSIGNED_BYTE, string_number.local8Bit() );
    glPopAttrib();

}

void GLGrid::setMinWidth( const GLuint & _min_width )
{
 min_width = _min_width;
}

void GLGrid::setMinHeight( const GLuint & _min_height )
{
 min_height = _min_height;
}

void GLGrid::setMaxWidth( const GLuint & _max_width )
{
 max_width = _max_width;
  buildGrid();
}

void GLGrid::setMaxHeight( const GLuint & _max_height )
{
 max_height = _max_height;
  buildGrid();
}

void GLGrid::setNumberLines( const GLuint & _number_lines )
{
 number_lines = _number_lines;
 buildGrid();
}

void GLGrid::setShowNumbers( const bool & _show_numbers )
{
 show_numbers = _show_numbers;
}

void GLGrid::setZoomLines( const bool & _zoom_lines )
{
 zoom_lines = _zoom_lines;
 buildGrid();
}

void GLGrid::setNtscZone( const bool & _ntsc_zone )
{
 ntsc_zone = _ntsc_zone;
 buildGrid();
}

void GLGrid::setGridColor( const QColor & _grid_color )
{
    grid_color = _grid_color;
    buildGrid( 2, id_grid2 );
    buildGrid( 24, id_grid12 );
    buildGrid( 32, id_grid16 );

    buildSubGrid( 24, id_sub_grid12 );
    buildSubGrid( 32, id_sub_grid16 );

    buildFonts();

    buildNumbers( 2, id_numbers2 );
    buildNumbers( 24, id_numbers12 );
    buildNumbers( 32, id_numbers16 );

    buildDiagonals();

    buildGrid();
}

void GLGrid::setNTSCColor( const QColor & _ntsc_color )
{
    ntsc_color = _ntsc_color;
    buildNtsc();
}
GLuint GLGrid::minWidth() const
{
 return min_width;
}

GLuint GLGrid::minHeight() const
{
 return min_height;
}

GLuint GLGrid::maxWidth() const
{
 return max_width;
}

GLuint GLGrid::maxHeight() const
{
 return max_height;
}

GLuint GLGrid::numberLines() const
{
 return number_lines;
}

bool GLGrid::showNumbers() const
{
 return show_numbers;
}

bool GLGrid::zoomLines() const
{
 return zoom_lines;
}

bool GLGrid::ntscZone() const
{
 return ntsc_zone;
}

GLuint GLGrid::idGrid() const
{
 return id_grid;
}

