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
 
#include "addproject.h"

namespace Packages {

struct AddProject::Private
{
	QDomElement info;
	QDomElement users;
};

AddProject::AddProject(const QString & name, const QString & author, const QString& description )
	: QDomDocument(), d(new Private())
{
	QDomElement root = createElement( "addproject" );
	root.setAttribute( "version",  "0" );
	appendChild(root);
	
	root.appendChild(createElement("name")).appendChild(createTextNode(name));
	root.appendChild(createElement("author")).appendChild(createTextNode(author));
	root.appendChild(createElement("description")).appendChild(createTextNode(description));
	
	d->users = createElement("users");
	root.appendChild(d->users);
}


void AddProject::addUser( const QString& login, int type )
{
	QDomElement user = createElement("user");
	user.setAttribute("type", type);
	user.appendChild(createTextNode(login));
	d->users.appendChild(user);
}


AddProject::~AddProject()
{
}


}
