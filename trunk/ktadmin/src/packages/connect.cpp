/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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

#include "connect.h"

#include <dcore/dmd5hash.h>
/*
<connect version="0" >
        <login>the_login</login>
        <password>the_hashed_password_md5</password>
</connect>
*/

namespace Packages {

Connect::Connect(const QString & login, const QString& passwd)
 : QDomDocument()
{
	QDomElement root = createElement("connect");
	root.setAttribute("version", "0");
	appendChild(root);
	
	QDomElement client = createElement("client");
	client.setAttribute("type", 1);
	root.appendChild(client);
	
	root.appendChild( createElement("login") ).appendChild(createTextNode(login));
	root.appendChild( createElement("password")).appendChild(createTextNode(DMD5Hash::hash( passwd)));
	
}


Connect::~Connect()
{
}


}

