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

#include "package.h"
#include "../core/connection.h"

namespace Base {

struct Package::Private
{
	Private(const QString &root, const QString &xml, Server::Connection *cnx) : root(root), xml(xml), connection(cnx), accepted(false)
	{
	}
	QString root;
	QString xml;
	Server::Connection *connection;
	
	bool accepted;
};

Package::Package(const QString &root, const QString &xml, Server::Connection *cnx) : d(new Private(root, xml, cnx))
{
}


Package::~Package()
{
	delete d;
}

QString Package::root() const
{
	return d->root;
}

QString Package::xml() const
{
	return d->xml;
}

Server::Connection *Package::source() const
{
	return d->connection;
}

bool Package::accepted() const
{
	return d->accepted;
}


void Package::accept()
{
	d->accepted = true;
}

void Package::ignore()
{
	d->accepted = false;
}

}


