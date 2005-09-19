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

#include "gldrawing.h"
#include "../gl/glline.h"
#include "../gl/glellipse.h"
#include "../gl/glrectangle.h"
#include "../gl/glpen.h"
#include "../gl/glpencil.h"
#include "../gl/glbrush.h"

//-------------- CONSTRUCTOR ---------------

GLDrawing::GLDrawing(QObject *parent) : QObject(parent)
{

}

GLDrawing::GLDrawing( const GLDrawing &to_copy, QObject *parent ) : QObject(parent)
{
    QPtrList<GLGraphicComponent> list_of_graphics = to_copy.graphicComponents();
    GLGraphicComponent *graphic_iterator;
    for ( graphic_iterator = list_of_graphics.first(); graphic_iterator; graphic_iterator = list_of_graphics.next() )
    {
	switch ( graphic_iterator -> kindGraphic() )
	{
	    case GLGraphicComponent::GC_BRUSH:
	    {
	        GLBrush *new_graphic = new GLBrush( *( ( GLBrush * )graphic_iterator ) );
		  try {
		  graphic_components.append( new_graphic );
		  }
		  catch(...)
		    {
		    delete new_graphic;
		    throw;
		    }
	    }
	        break;
	    case GLGraphicComponent::GC_PENCIL:
	    {
	        GLPencil *new_graphic = new GLPencil( *( ( GLPencil * )graphic_iterator ) );
		  try {
		  graphic_components.append( new_graphic );
		  }
		  catch(...)
		    {
		    delete new_graphic;
		    throw;
		    }
	    }
	        break;
	    case GLGraphicComponent::GC_PEN:
	    {
	        GLPen *new_graphic = new GLPen( *( ( GLPen * )graphic_iterator ) );
		  try {
		  graphic_components.append( new_graphic );
		  }
		  catch(...)
		    {
		    delete new_graphic;
		    throw;
		    }
	    }
	        break;
	    case GLGraphicComponent::GC_LINE:
	    {
	        GLLine *new_graphic = new GLLine( *( ( GLLine * )graphic_iterator ) );
		  try {
		  graphic_components.append( new_graphic );
		  }
		  catch(...)
		    {
		    delete new_graphic;
		    throw;
		    }
	    }
	        break;
	    case GLGraphicComponent::GC_RECTANGLE:
	    {
	        GLRectangle *new_graphic = new GLRectangle( *( ( GLRectangle * )graphic_iterator ) );
		  try {
		  graphic_components.append( new_graphic );
		  }
		  catch(...)
		    {
		    delete new_graphic;
		    throw;
		    }
	    }
	        break;
	    case GLGraphicComponent::GC_ELLIPSE:
	    {
	        GLEllipse *new_graphic = new GLEllipse( *( ( GLEllipse * )graphic_iterator ) );
		  try {
		  graphic_components.append( new_graphic );
		  }
		  catch(...)
		    {
		    delete new_graphic;
		    throw;
		    }
	    }
	        break;
	}
    }
}

//------------- DESTRUCTOR ------------------

GLDrawing::~GLDrawing()
{
    graphic_components.clear();
}

//------------ PUBLIC MEMBERS ---------------

void GLDrawing::setGraphicComponents( QPtrList<GLGraphicComponent> _graphic_components )
{
   graphic_components = _graphic_components;
}

QPtrList<GLGraphicComponent> GLDrawing::graphicComponents() const
{
 return graphic_components;
}

QDomElement GLDrawing::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Drawing" );

    GLGraphicComponent *g_it;
    for ( g_it = graphic_components.first(); g_it; g_it = graphic_components.next() )
    {
	QDomElement graphic_tag = g_it -> createXML( doc );
	e.appendChild( graphic_tag );
    }

    return e;
}
