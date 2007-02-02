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
#include "packages/banlist.h"

#include "packages/useractionparser.h"
#include "packages/usersinfo.h"
#include "packages/removebanparser.h"

#include "banmanager.h"


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
				fortune.replace("\n", "<br/>");
				Packages::Ack ack(QObject::tr("<center><b>Message of the day:</b></center><br/> ")+fortune, client->sign());
				
				foreach(Users::Right *right, user->rights())
				{
					ack.addPermission(right);
				}
				
				client->sendToClient(ack, false);
				Packages::UsersInfo info;
				foreach(Users::User *u, d->manager->listUsers())
				{
					info.addUser(u);
				}
				client->sendToClient(info);
				
			}
			else
			{
				Packages::Error error(QObject::tr("Invalid login or password"), Packages::Error::Err);
				client->sendToClient(error);
				
				BanManager::self()->failed(client->client()->peerAddress().toString());
				Server::Logger::self()->error(QObject::tr("Invalid login or password"));
				
				client->close();
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
	else if(root == "adduser")
	{
		Parsers::UserActionParser parser;
		if(parser.parse(package))
		{
			d->manager->addUser(parser.user());
			client->sendToAll(package);
		}
	}
	else if(root == "removeuser")
	{
		Parsers::UserActionParser parser;
		if(parser.parse(package))
		{
			d->manager->removeUser(parser.user().login());
			client->sendToAll(package);
		}
	}
	else if(root == "queryuser")
	{
		Parsers::UserActionParser parser;
		if(parser.parse(package))
		{
			Users::User * user = d->manager->loadUser(parser.user().login());
			if(user)
			{
				QDomDocument doc;
				doc.appendChild(user->toXml(doc, false));
				client->sendToClient (doc.toString());
			}
		}
	}
	else if(root == "updateuser")
	{
		Parsers::UserActionParser parser;
		if(parser.parse(package))
		{
			d->manager->updateUser(parser.user());
			client->sendToClient(package);
		}
	}
	else if ( root == "listbans" )
	{
		if ( client->user()->canReadOn("admin") )
		{
			QStringList bans = BanManager::self()->allBanned();
			
			Packages::BanList pkg;
			pkg.setBans(bans);
			
			client->sendToClient(pkg, true);
		}
		else
		{
			client->sendErrorPackageToClient(QObject::tr("Permission denied."), Packages::Error::Err);
		}
	}
	else if ( root == "removeban" )
	{
		if( client->user()->canWriteOn("admin") )
		{
			Parsers::RemoveBanParser parser;
			if ( parser.parse(package) )
			{
				BanManager::self()->unban(parser.pattern());
				
				client->sendToClient(package);
			}
		}
		else
		{
			client->sendErrorPackageToClient(QObject::tr("Permission denied."), Packages::Error::Err);
		}
	}
	else
	{
		handle(client, root, package);
	}
}

}

