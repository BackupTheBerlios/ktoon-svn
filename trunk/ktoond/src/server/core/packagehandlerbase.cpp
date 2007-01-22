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
 
#include "packagehandlerbase.h"
#include <ddebug.h>

#include "users/manager.h"
#include "packages/userpackageparser.h"

#include "connection.h"
#include "dapplicationproperties.h"

namespace Server {

struct PackageHandlerBase::Private
{
	Users::Manager *manager;
};

PackageHandlerBase::PackageHandlerBase() : d(new Private())
{
	d->manager = 0;
}

PackageHandlerBase::~PackageHandlerBase()
{
	delete d->manager;
	delete d;
}

void PackageHandlerBase::handlePackage(Server::Connection *client, const QString &root, const QString &package )
{
	dWarning("server") << "PACKAGE: " << package;
	
	if ( root == "connect" )
	{
		if ( !d->manager )
		{
			d->manager = new Users::Manager(client->server()->databaseDirPath()+"/users.xml" );
		}
		
		Parsers::UserPackageParser parser;
		if ( parser.parse(package) )
		{
			if ( d->manager->auth(parser.login(), parser.password()) )
			{
				// TODO: Enviar paquete de reconocimiento
			}
		}
		else
		{
			dError() << "ERROR PARSING CONNECT PACKAGE!";
		}
		
	}
	else if ( root == "chat" )
	{
		
	}
	else if ( root == "notice" )
	{
	}
	else if ( root == "wall" )
	{
	}
	else
	{
		handle(client, root, package);
	}
}

}

