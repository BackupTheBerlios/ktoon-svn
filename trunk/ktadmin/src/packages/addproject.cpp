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
	d->info = createElement("info");
	d->info.setAttribute("name", name);
	d->info.setAttribute("author", author);
	d->info.setAttribute("description", description);
	d->users = createElement("users");
	root.appendChild(d->info);
}


void AddProject::addUser( const QString& login )
{
	//TODO agregar los tipos de roles sobre el proyecto Owner, Designer ...
	QDomElement user = createElement("user");
	user.appendChild(createTextNode(login));
	d->users.appendChild(user);
}


AddProject::~AddProject()
{
}


}
