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
 
#include "updateproject.h"

namespace Packages {

struct UpdateProject::Private
{
	QDomText name;
	QDomElement author;
	QDomElement description;
	QDomElement users;
};

UpdateProject::UpdateProject(const QString &name): QDomDocument(), d(new Private)
{
	QDomElement root = createElement( "removeproject" );
	root.setAttribute( "version",  "0" );
	appendChild(root);
	d->name = createTextNode(name);
	root.appendChild(createElement("name").appendChild(d->name));
}


UpdateProject::~UpdateProject()
{
}

void UpdateProject::setName(const QString &name)
{
	d->name.setData(name);
}

void UpdateProject::setAuthor(const QString &author)
{
	if(d->author.isNull())
	{
		d->author = createElement("author");
		d->author.appendChild(createTextNode(author));
	}
	else
	{
		d->author.firstChild().toText().setData(author);
	}
}

void UpdateProject::setDescription(const QString &description)
{
	if(d->description.isNull())
	{
		d->description = createElement("description");
		d->description.appendChild(createTextNode(description));
	}
	else
	{
		d->description.firstChild().toText().setData(description);
	}
}

void UpdateProject::addUser(const QString& login)
{
	if(d->users.isNull())
	{
		d->users = createElement("users");
	}
	
	QDomElement userE = createElement("user");
	userE.appendChild(createTextNode(login));
	d->users.appendChild( userE );
	
	
}

}
