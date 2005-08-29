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

#define DEBUG_GLBRUSH 1

#include <cmath>
#include <cstdlib>

#ifdef Q_WS_MAC
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "glbrush.h"

#include <new>

#include <iostream>

#define sqr( x ) ( ( x ) * ( x ) )

//-------------- CONSTRUCTOR ---------------

GLBrush::GLBrush( QGLWidget *parent, const QPoint & _origin, const KTColor & _color, const Brush & _width, const QPoint & _end ) : GLGraphicComponent( parent, _origin, _color, _width ), end( _end )
{
        Q_CHECK_PTR( parent );
	
	setKindGraphic( GC_BRUSH );
        id_graphic_component = glGenLists( 1 );
	QPoint * origin_point = new QPoint( origin.x(), origin.y() );
	QPoint * end_point = new( std::nothrow ) QPoint( end.x(), end.y() );
	if( !end_point )
	    {
	    delete origin_point;
	    throw std::bad_alloc();
	    }
	try {
	    points.append( origin_point );
	    points.append( end_point );
	    }
	catch(...)
	    {
	    delete end_point;
	    delete origin_point;
	    throw;
	    }
	    
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

GLBrush::GLBrush( const GLBrush & brush ) : GLGraphicComponent( brush.parentWidget(), brush.originPoint(), brush.outlineColor(), brush.widthPoint() ), end( brush.endBrush() )
{
	 setKindGraphic( GC_BRUSH );
         id_graphic_component = glGenLists( 1 );
 	 stipple_factor = brush.stippleFactor();
  	 stipple_pattern = brush.stipplePattern();
	 rotation_angle = brush.rotationAngle();
	 translate.setX( 0 );
	 translate.setY( 0 );
	 local_selection_name = selection_name;
	 selection_name++;
	 points = brush.getPoints();
	 pressure = brush.pressureBrush();
         buildList();
}

//------------- DESTRUCTOR ------------------

GLBrush::~GLBrush()
{

}

//extern void lineImpl( const QPoint & origin, const QPoint & end, int lw, int stippleFactor, const KTColor & outlineColor );
//extern void lineImplFast( const QPoint & origin, const QPoint & end, int lw, const KTColor & outlineColor );

//------------ PUBLIC MEMBERS ---------------

//implementation of the virtual method buildList to Bezier curve
void GLBrush::buildList()
{
	QPoint *initial_point, *final_point;
	glNewList( id_graphic_component, GL_COMPILE );
	glPushMatrix();
	      
// 	glDisable( GL_LINE_STIPPLE );
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
		for ( unsigned int i = 0; i < points.count()-1 ;  i++ )
		{
			initial_point = points.at( i );
			final_point = points.at( i+1 );
			glColor4f( outlineColor().colorRed(), outlineColor().colorGreen(),
				   outlineColor().colorBlue(), outlineColor().colorAlpha() );

			lineImpl( *initial_point, *final_point, 
				   widthPoint().thicknessMinBrush(), stippleFactor(), outlineColor() );
		}
		
		glDisable( GL_TEXTURE_1D );
		glPopAttrib();
	}
	else
	{
		for ( unsigned int i = 0; i < points.count()-1 ;  i++ )
		{
			initial_point = points.at( i );
			final_point = points.at( i+1 );
			
			glColor4f( outlineColor().colorRed(), outlineColor().colorGreen(),
				   outlineColor().colorBlue(), outlineColor().colorAlpha() );

			lineImplFast( *initial_point, *final_point, 
				       widthPoint().thicknessMinBrush(), outlineColor() );
		}
	}

	glColor4f( outlineColor().colorRed(), outlineColor().colorGreen(),
		   outlineColor().colorBlue(), outlineColor().colorAlpha() );

	if ( stipplePattern() == 0xFFFF )
	{
	        // TODO: change the dots to quads with a circle texture with alpha 
		glPointSize( widthPoint().thicknessMinBrush() );
		glBegin( GL_POINTS );
		for ( unsigned int i = 1; i < points.count() - 1;  i++ )
		{
			initial_point = points.at( i );
			glVertex2f( initial_point -> x(), initial_point -> y() );
		}
		glEnd();
	}

	glPopMatrix();
	glEndList();
}

/*
//implementation of the virtual method buildList to Bezier curve
void GLBrush::buildList()
{
  	QPoint *initial_point;
	glNewList( id_graphic_component, GL_COMPILE );
   	   glPushMatrix();
	   glLineWidth( widthPoint().thicknessMinBrush() );
	   glColor4f( outlineColor().colorRed(), outlineColor().colorGreen(),
	              outlineColor().colorBlue(), outlineColor().colorAlpha() );
	   if( stipplePattern() != 0xFFFF )
	   {
		glEnable( GL_LINE_STIPPLE );
		glLineStipple( stippleFactor(), stipplePattern() );
	   }
           glBegin( GL_LINE_STRIP );
	   for ( unsigned int i = 0; i < points.count() ;  i++ )
	     {
	        initial_point = points.at( i );
       	        glVertex2f( initial_point -> x(), initial_point -> y() );
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
       	        	glVertex2f( initial_point -> x(), initial_point -> y() );
	     	    }
           	glEnd();
	     }

   	   glPopMatrix();
       glEndList();
}
*/

void GLBrush::angleBrush( const QPoint & _end )
{
	qDebug("ANGLE");
	QPoint * point = new QPoint( _end.x(), _end.y() );
	double theta = asin( ( originPoint().y() - _end.y() ) / sqrt( pow ( (originPoint().x() - _end.x() ), 2 ) +
				pow ( ( originPoint().y() - _end.y() ), 2 ) ) );

	if ( ( fabs( fmod( ( ( 180.0 * theta ) / 3.14159265358979323846 ), 45 ) == 0.0 ) ) )
	{
		points.replace( points.count() - 1, point );
	}
	buildList();
}

void GLBrush::lineBrush( const QPoint & _end )
{
     QPoint * point = new QPoint( _end.x(), _end.y() );
     points.replace( points.count() - 1, point );
     buildList();
}

GLfloat dist( const QPoint &p1, const QPoint &p2 ) 
{
	return sqrt( sqr( p1.x() - p2.x() ) + sqr( p1.y() - p2.y() ) );
}

void GLBrush::setEndBrush( const QPoint & _end )
{
	qDebug("setEndBrush");
	if ( abs( _end.x() - points.last() -> x() ) <= 2 && abs( _end.y() - points.last() -> y() ) <= 2 )
		return;
	if ( points.count() > 2 )
	{

		QPoint *point1 = points.at( points.count() - 1 );
		QPoint *point2 = points.last();
	
		if( dist( *point1, *point2 ) < dist( *point1, _end ) )
		{
	
			if( point1 -> x() == point2 -> x() )
			{
				if( point2 -> x() == _end.x() )
				{
					// estan alineados al eje X
					point2 -> setX( _end.x() );
					point2 -> setY( _end.y() );
					buildList();
					return;
				}
			}
			else if( point1 -> y() == point2 -> y() )
			{
				if( point2 -> y() == _end.y() )
				{
					// estan alineados al eje Y
					point2 -> setX( _end.x() );
					point2 -> setY( _end.y() );
					buildList();
					return;
				}
			}
			else if( ( ( point2 -> x() - point1 -> x() ) * ( _end.y() - point2 -> y() ) ) == ( ( point2 -> y() - point1 -> y() ) * ( _end.x() - point2 -> x() ) ) )
			{
				point2 -> setX( _end.x() );
				point2 -> setY( _end.y() );
				buildList();
				return;
			}
		}
	}
	QPoint * point = new QPoint( _end.x(), _end.y() );
	try {
	    points.append( point );
	    }
	catch(...)
	    {
	    delete point;
	    throw;
	    }
	buildList();
}

void GLBrush::smoothnessBrush( int smooth )
{
#ifdef DEBUG_GLBRUSH
	std::cout << "smooth= " << smooth << std::endl;
#endif
	if( points.count()  < 3)
		return;
	
	QPoint *p1, *p2, *p3;
	double theta1, theta2;
	double m1, m2;
	QPtrList<QPoint> aux;
	int firstDummy = 0;
	
	// Improving excpetion-safety of this function whithout making design changes with regard
	// to other code was a relative complex task. The main for-loop in this function inserts
	// intermixed items into the QPtrList "aux", some created via new and some taken from points.
	// Later aux is assigned to points.
	// In case of an exception occuring, we must only delete the newly created objects, so we must 
	// somehow keep track of them without introducing to much burden on efficiency. I tried to
	// solve that by looking up which objects were in points previously until the assignment takes
	// place. Then I changed the simple assignment to a hand-written loop which replaces the items
	// in points one-by-one, so that no exception is thrown.
	
	QPoint *dummy = new QPoint();
	
	try 
	{
		//  for ( int j = 0; j < smooth; j++ )
		//    {
		for ( unsigned int i = 0; i < points.count() - 2; i +=2 )
		{
			p1 = points.at( i );
			p2 = points.at( i + 1 );
			p3 = points.at( i + 2 );
		
			theta1 = asin( ( p1 -> y() - p2 -> y() ) / sqrt( pow ( ( p1 -> x() - p2 -> x() ), 2 ) +
					pow ( ( p1 -> y() - p2 -> y() ), 2 ) ) );
			theta2 = asin( ( p2 -> y() - p3 -> y() ) / sqrt( pow ( ( p2 -> x() - p3 -> x() ), 2 ) +
					pow ( ( p2 -> y() - p3 -> y() ), 2 ) ) );
		
			m1 = fmod( ( ( 180.0 * theta1 ) / 3.14159265358979323846 ), 45 );
			m2 = fmod( ( ( 180.0 * theta2 ) / 3.14159265358979323846 ), 45 );
		
			if ( ( m1 + m2 ) != 180 )
			{
// 				std::cerr << "Append to aux p1" << std::endl;
				aux.append( p1 );
				QPoint *aux1 = new QPoint( ( int ) ( ( p1 -> x() + p2 -> x() ) / 2 ),
							( int ) ( ( p1 -> y() + p2 -> y() ) / 2 ) );
				try 
				{
// 					std::cerr << "Append to aux aux1" << std::endl;
					aux.append( aux1 );
				}
				catch(...)
				{
					delete aux1;
					throw;
				}
				QPoint *aux2 = new QPoint( ( int ) ( ( p2 -> x() + p3 -> x() ) / 2 ),
							( int ) ( ( p2 -> y() + p3 -> y() ) / 2 ) );
				try 
				{
// 					std::cerr << "Append to aux aux2" << std::endl;
					aux.append( aux2 );
				}
				catch(...)
				{
					delete aux2;
					throw;
				}
			}
			else
			{
// 				std::cerr << "Append to aux p1 and p2" << std::endl;
				aux.append( p1 );
				aux.append( p2 );
			}
		}
		//   }
		
		// This and the similar if-block down ensure that both lists have the same size. This is
		// needed for the special assignment later.
#ifdef DEBUG_GLBRUSH
		std::cout << "Points size: " << points.count() << std::endl;
		std::cout << "Aux Size: " << aux.count() << std::endl;	
#endif
		if(points.count() < aux.count() )
		{
#ifdef DEBUG_GLBRUSH
			std::cerr << "\npoints.count() < aux.count()\n";
#endif
			int stupid = 0;
			firstDummy = points.count();
			
			std::cout << "Size points after build: " << points.count() << std::endl;
			for( uint c_diff = points.count(); c_diff != aux.count(); ++c_diff )
			{
				if ( stupid > 300 )
				{
#ifdef DEBUG_GLBRUSH
					std::cerr << "Oops! an error has ocurred!" << std::endl;
#endif
					//aux.clear();
					break;
				}
				stupid++;
				points.append( dummy );
			}
			std::cout << "Size points before build: " << points.count() << std::endl;
		}
	} // outer try block
	catch(...)
	{
#ifdef DEBUG_GLBRUSH
		std::cerr << "Catched!" << std::endl;
#endif
		if( !aux.isEmpty() )
		{
			QPtrListIterator<QPoint> iter( aux );
			do
			{
				if(!points.containsRef( *iter ) ) // Only delete items which are not in "points"
				{
					delete *iter;
				}
		
				++iter;
			} while( !iter.atLast() );
		} // if(!aux.isEmpty() )
	
		throw;
	}
	
	//points = aux;

	if( points.count() > aux.count() )
	{
		std::cerr << "\npoints.count() > aux.count()\n";
		
#ifdef DEBUG_GLBRUSH
		std::cout << "Points size: " << points.count() << std::endl;
		std::cout << "Aux Size: " << aux.count() << std::endl;	
#endif
		for ( uint c_diff = aux.count(); c_diff != points.count(); ++c_diff )
		{
			points.removeLast();
		}
	}
	
	QPtrListIterator<QPoint> iter( aux );
	for( uint c = 0; c != points.count(); ++c )
	{
		//points.at(c);
		points.replace( c, *iter );
		
		++iter;
	}
	
	buildList();

	
	for (uint i = firstDummy; i < points.count(); i++)
	{
		points.remove(i);
	}
	
#ifdef DEBUG_GLBRUSH
	std::cout << "End!" << std::endl;
#endif
}

void GLBrush::setPoints( QPtrList<QPoint> _points )
{
   #if 1
   points = _points;
   #else
  QPoint *point;
  for ( point = _points.first(); point; point = _points.next() )
   {
      setEndBrush( *point );
   }
   #endif
}

void GLBrush::setPressureBrush( int _pressure )
{
    pressure.append( &_pressure );
}

void GLBrush::calculateTopLeft()
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

void GLBrush::calculateBottomRight()
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

QPoint GLBrush::endBrush() const
{
 return end;
}

QString GLBrush::clipboardGraphic()
{
  QString graphic;
  QPoint *point;
  graphic = ( graphic.setNum( kindGraphic() ) + " " + graphic.setNum( originPoint().x() ) + " " +
              graphic.setNum( originPoint().y() ) + " " + graphic.setNum( endBrush().x() ) + " " +
	      graphic.setNum( endBrush().y() ) + " " + graphic.setNum( outlineColor().colorRed() ) + " " +
	      graphic.setNum( outlineColor().colorGreen() ) + " " + graphic.setNum( outlineColor().colorBlue() ) + " " +
	      graphic.setNum( outlineColor().colorAlpha() ) + " " + graphic.setNum( widthPoint().thicknessMinBrush() ) + " " +
	      graphic.setNum( widthPoint().thicknessMaxBrush() ) + " " + graphic.setNum( widthPoint().smoothnessBrush() ) + " " +
	      graphic.setNum( stipplePattern() ) + " " + graphic.setNum( stippleFactor() ) + " " +
	      graphic.setNum( rotationAngle() ) + " " + graphic.setNum( translate.x() ) + " " +
	      graphic.setNum( translate.y() )+ " " + graphic.setNum( points.count() ) );

  for ( point = points.first(); point; point = points.next() )
    {
       graphic = graphic + " " + ( graphic.setNum( point -> x() ) + " " +  graphic.setNum( point -> y() ) );
    }
  return graphic;
}

QPtrList<QPoint> GLBrush::getPoints() const
{
    return points;
}

QPtrList<int> GLBrush::pressureBrush() const
{
    return pressure;
}

QDomElement GLBrush::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Brush" );

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


