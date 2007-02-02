//
// C++ Implementation: permission
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
