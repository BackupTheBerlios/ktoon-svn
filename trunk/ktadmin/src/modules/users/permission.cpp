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
#include "permission.h"

namespace Users {

struct Permission::Private
{
	QString module;
	bool read;
	bool write;
};

Permission::Permission() : d(new Private)
{
	
}


Permission::Permission(const QString &module, bool read, bool write): d(new Private())
{
	d->module = module;
	d->read = read;
	d->write = write;
}


Permission::Permission(const Permission& copy) :  d(new Private())
{
	d->module = copy.d->module;
	d->read = copy.d->read;
	d->write = copy.d->write;
}

Permission::~Permission()
{
	delete d;
}

void Permission::setModule(const QString &module)
{
	d->module = module;
}

void Permission::setRead(bool read)
{
	d->read = read;
}

void Permission::setWrite(bool write)
{
	d->write = write;
}

QString Permission::module() const
{
	return d->module;
}

bool Permission::read()
{
	return d->read;
}

bool Permission::write()
{
	return d->write;
}


Permission *Permission::operator=(const Permission &copy) 
{
	d->module = copy.d->module;
	d->read = copy.d->read;
	d->write = copy.d->write;
	return this;
}


}
