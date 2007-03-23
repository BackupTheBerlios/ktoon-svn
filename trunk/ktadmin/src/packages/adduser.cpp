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
 
#include "adduser.h"

#include <dcore/dmd5hash.h>

namespace Packages {

struct AddUser::Private
{
	QDomText login;
	QDomText password;
	QDomText name;
	QDomElement premissions;
};

AddUser::AddUser(const QString& login, const QString& password, const QString&name): QDomDocument(), d(new Private)
{
	QDomElement root = createElement( "adduser" );
	root.setAttribute( "version",  "0" );
	appendChild(root);
	d->login = createTextNode(login);
	root.appendChild(createElement("login")).appendChild(d->login);
	
	d->password = createTextNode(DMD5Hash::hash(password));
	root.appendChild(createElement("password")).appendChild(d->password);
	
	d->name = createTextNode(name);
	root.appendChild(createElement("name")).appendChild(d->name);
	
	d->premissions = createElement("permissions");
	root.appendChild(d->premissions);
}


AddUser::~AddUser()
{
}

void AddUser::setLogin(const QString& login)
{
	d->login.setData(login);
}

void AddUser::setPassword(const QString& password)
{
	d->password.setData(password);
}

void AddUser::setName(const QString& name)
{
	d->name.setData(name);
}

void  AddUser::addPermission(const QString &module, bool read, bool write)
{
	QDomElement perm = createElement("perm");
	perm.setAttribute("module", module);
	perm.setAttribute("read", read);
	perm.setAttribute("write", write);
	d->premissions.appendChild(perm);
	
}
	
}
