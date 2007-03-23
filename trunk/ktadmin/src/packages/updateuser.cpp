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

#include "updateuser.h"
#include <dcore/dmd5hash.h>
namespace Packages{

struct UpdateUser::Private
{
	QDomText login;
	QDomElement password;
	QDomElement name;
	QDomElement premissions;
};

UpdateUser::UpdateUser(const QString& login): QDomDocument(), d(new Private)
{
	QDomElement root = createElement( "updateuser" );
	root.setAttribute( "version",  "0" );
	appendChild(root);
	d->login = createTextNode(login);
	root.appendChild(createElement("login")).appendChild(d->login);
}


UpdateUser::~UpdateUser()
{
}

void UpdateUser::setLogin(const QString& login)
{
	d->login.setData(login);
}

void UpdateUser::setPassword(const QString& password)
{
	firstChild().removeChild(d->password);
	d->password = createElement("password");
	d->password.appendChild(createTextNode(DMD5Hash::hash(password)));
	firstChild().appendChild(d->password);
	
}

void UpdateUser::setName(const QString& name)
{
	firstChild().removeChild(d->name);
	d->name = createElement("name");
	d->name.appendChild(createTextNode(name));
	firstChild().appendChild(d->name);
}

void  UpdateUser::addPermission(const QString &module, bool read, bool write)
{
	if(d->premissions.isNull())
	{
		d->premissions = createElement("permissions");
		firstChild().appendChild(d->premissions);
	}
	
	QDomElement perm = createElement("perm");
	perm.setAttribute("module", module);
	perm.setAttribute("read", read);
	perm.setAttribute("write", write);
	d->premissions.appendChild(perm);
}

}

