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

 #include "glbitmap.h"

GLBitmap::GLBitmap( QGLWidget *parent, const QPoint &  _origin, const Color & _color, const Brush & _width, const QPoint & _end ) : GLGraphicComponent( parent, _origin, _color, _width ), end( _end )
{
	 setKindGraphic( GC_BITMAP );
         id_graphic_component = glGenLists(1);
         buildList();
}

GLBitmap::~GLBitmap( )
{

}
 
void GLBitmap::buildList( )
{
	 glNewList( id_graphic_component, GL_COMPILE );
	 glPushName( idGraphicComponent() );
	   glLineWidth( widthPoint().thicknessMaxBrush() );
	   glColor4f( outlineColor().colorRed(), outlineColor().colorGreen(), outlineColor().colorBlue(), outlineColor().colorAlpha() );
           glBegin( GL_LINES );
       	      glVertex2i( origin.x(), origin.y() );
       	      glVertex2i( end.x(), end.y() );
           glEnd();
	 glPopName();
         glEndList();
}

