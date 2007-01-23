/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosxx@gmail.com                                                   *
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
#include "items.h"
#include <QStringList>
/*
<!-- Respuesta a una peticion <list> -->
<items version="0">
        <item>proyecto1.ktn</item>
        <item>proyecto2.ktn</item>
        <item>proyecto3.ktn</item>
        <item>proyecto4.ktn</item>
</items>
*/


namespace Packages
{
	
Items::Items()
 : Package()
{
	m_root = createElement ( "items" );
	m_root.setAttribute ( "version",  "0" );
	appendChild(m_root);
}


Items::~Items()
{
	
}


void Items::addItem(const QString& item )
{
	m_root.appendChild(createElement("item")).appendChild( createTextNode ( item ));
}

void Items::addItems(const QStringList& items)
{
	foreach(const QString item, items)
	{
		addItem( item );
	}
}

void Items::setItems( const QStringList& items)
{
	removeChild (m_root);
	m_root = createElement ( "items" );
	m_root.setAttribute ( "version",  "0" );
	appendChild(m_root);
	
	addItems(items);
}

}

