/***************************************************************************
 *   Copyright (C) 2004 by Fernando Jose Roldan Correa                     *
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

#include <qdom.h>

#include "item.h"
#include "librarydata.h"
#include "glgraphiccomponent.h"
#include "symbol.h"
#include "status.h"

//-------------- CONSTRUCTOR ---------------

LibraryData::LibraryData()
{

}

//------------- DESTRUCTOR ------------------

LibraryData::~LibraryData()
{
    items.setAutoDelete( true );
    items.clear();
    items.setAutoDelete( false );
}

//------------ PUBLIC MEMBERS ---------------

void LibraryData::setItems( QPtrList<Item> new_items )
{
    items = new_items;
}

QPtrList<Item> LibraryData::getItems() const
{
    return items;
}

void LibraryData::addSymbol( GLGraphicComponent *graphic, const QString &name )
{
	Q_CHECK_PTR( graphic );
	
	QPtrList<Item> its = getItems();
    
	std::auto_ptr<Symbol> ap_new_symbol(new Symbol);
	Symbol* new_symbol = ap_new_symbol.get();
    
	new_symbol -> setName( name );

	switch ( graphic -> kindGraphic() )
	{
		case GLGraphicComponent::GC_BRUSH:
		{
			GLBrush *b = new GLBrush( *( ( GLBrush * )graphic ) );
			new_symbol -> setGraphic( b );
		}
		break;
		case GLGraphicComponent::GC_PENCIL:
		{
			GLPencil *pc = new GLPencil( *( ( GLPencil * )graphic ) );
			new_symbol -> setGraphic( pc );
		}
		break;
		case GLGraphicComponent::GC_LINE:
		{
			GLLine *l = new GLLine( *( ( GLLine * )graphic ) );
			new_symbol -> setGraphic( l );
		}
		break;
		case GLGraphicComponent::GC_PEN:
		{
			GLPen *p = new GLPen( *( ( GLPen * )graphic ) );
			new_symbol -> setGraphic( p );
		}
		break;
		case GLGraphicComponent::GC_RECTANGLE:
		{
			GLRectangle *r = new GLRectangle( *( ( GLRectangle * )graphic ) );
			new_symbol -> setGraphic( r );
		}
		break;
		case GLGraphicComponent::GC_ELLIPSE:
		{
			GLEllipse *e = new GLEllipse( *( ( GLEllipse * )graphic ) );
			new_symbol -> setGraphic( e );
		}
		break;
		default:
		new_symbol -> setGraphic( NULL );
		break;
	}

// 	KTStatus->currentDrawingArea() -> modifyDocument( true );

	its.prepend( new_symbol );
	
	setItems( its );
	ap_new_symbol.release();
}

QDomElement LibraryData::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Library" );

    QDomElement i_tag = doc.createElement( "Items" );
    e.appendChild( i_tag );
    Item *i_it;
    for ( i_it = items.first(); i_it; i_it = items.next() )
    {
	QDomElement item_tag = i_it -> createXML( doc );
	i_tag.appendChild( item_tag );
    }

    return e;
}



