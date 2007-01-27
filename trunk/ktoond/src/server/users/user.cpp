/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include "user.h"
#include "right.h"

#include <QList>

namespace Users {

struct User::Private
{
	QString name;
	QString login;
	QString password;
	
	QList<Right> rights;
};

User::User() : d(new Private)
{
}


User::~User()
{
	delete d;
}

void User::setName(const QString &name)
{
	d->name = name;
}

void User::setLogin(const QString &login)
{
	d->login = login;
}

void User::setPassword(const QString &password)
{
	d->password = password;
}


QString User::name() const
{
	return d->name;
}

QString User::login() const
{
	return d->login;
}

QString User::password() const
{
	return d->password;
}

QList<Right> User::rights() const
{
	return d->rights;
}

}

