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
#include <dfortunegenerator.h>

#include "users/manager.h"
#include "users/user.h"
#include "users/right.h"

#include "packages/connectparser.h"

#include "connection.h"
#include "settings.h"
#include "logger.h"


#include "dapplicationproperties.h"

#include "packages/error.h"
#include "packages/ack.h"


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
		client->setValid(false);
		if ( !d->manager )
		{
			d->manager = new Users::Manager(Server::Settings::self()->databaseDirPath()+"/users.xml" );
		}
		
		Parsers::ConnectParser parser;
		if ( parser.parse(package) )
		{
			if ( d->manager->auth(parser.login(), parser.password()) )
			{
				Users::User *user = d->manager->user(parser.login());
				
				client->setUser(user);
				
				QString fortune = DFortuneGenerator::self()->generate();
				fortune.replace("\n", "</br>");
				Packages::Ack ack(QObject::tr("<center><b>Message of the day:</b></center></br> ")+fortune, client->sign());
				
				foreach(Users::Right *right, user->rights())
				{
					ack.addPermission(right);
				}
				
				client->sendToClient(ack, false);
			}
			else
			{
				Packages::Error error(QObject::tr("Invalid login or password"), Packages::Error::Err);
				client->sendToClient(error);
				client->close(true);
				
				Server::Logger::self()->error(QObject::tr("Invalid login or password"));
			}
		}
		else
		{
			dError() << "ERROR PARSING CONNECT PACKAGE!";
		}
		
	}
	else if ( root == "chat" )
	{
		QDomDocument doc;
		doc.setContent(package);
		
		QDomElement element = doc.firstChildElement("message");
		element.setAttribute("from", client->user()->login());
		
		client->sendToAll(doc);
	}
	else if ( root == "notice" )
	{
		QDomDocument doc;
		doc.setContent(package);
		
		QDomElement element = doc.firstChildElement("message");
		element.setAttribute("from", client->user()->login());
		
		client->sendToAll(doc);
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

