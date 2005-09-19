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

#include "symbol.h"
#include "glgraphiccomponent.h"

//-------------- CONSTRUCTOR ---------------

Symbol::Symbol() : Item()
{
    graphic = NULL;
    item_kind = ITEM_SYMBOL;
}

//------------- DESTRUCTOR ------------------

Symbol::~Symbol()
{

}

//------------ PUBLIC MEMBERS ---------------

void Symbol::setGraphic( GLGraphicComponent *graphic_ )
{
    Q_CHECK_PTR( graphic_ );
    graphic = graphic_;
}

GLGraphicComponent *Symbol::getGraphic() const
{
    Q_CHECK_PTR( graphic );
    return graphic;
}

QDomElement Symbol::createXML( QDomDocument &doc )
{
    QDomElement e = doc.createElement( "Symbol" );

    e.setAttribute( "Name", name );

    QDomElement graphic_tag = graphic -> createXML( doc );
    e.appendChild( graphic_tag );

    return e;
}
