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
#include <qstring.h>
//Added by qt3to4:
#include <Q3PtrList>

#include "folder.h"

Folder::Folder() : Item()
{
    item_kind = ITEM_FOLDER;
}

Folder::~Folder()
{
    items.setAutoDelete( true );
    items.clear();
    items.setAutoDelete( false );
}

void Folder::setItems( Q3PtrList<Item> new_items )
{
    items = new_items;
}

Q3PtrList<Item> Folder::getItems() const
{
    return items;
}

QDomElement Folder::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Folder" );

    e.setAttribute( "Name", name );

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
