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

#include <QHashIterator>
#include <QHash>

#include "users/manager.h"
#include "users/user.h"
#include "users/right.h"

#include "packages/connectparser.h"

#include "connection.h"
#include "base/settings.h"
#include "base/logger.h"


#include "dapplicationproperties.h"

#include "packages/error.h"
#include "packages/ack.h"
#include "packages/banlist.h"

#include "packages/useractionparser.h"
#include "packages/userlist.h"
#include "packages/removebanparser.h"
#include "packages/addbanparser.h"


#include "bans/banmanager.h"
#include "backups/backupmanager.h"

#include "base/package.h"

namespace Server {

struct PackageHandlerBase::Private
{
	Users::Manager *manager;
};

PackageHandlerBase::PackageHandlerBase() : d(new Private())
{
	d->manager = new Users::Manager(Base::Settings::self()->databaseDirPath()+"/users.xml" );
}

PackageHandlerBase::~PackageHandlerBase()
{
	delete d->manager;
	delete d;
}

void PackageHandlerBase::handlePackage(Base::Package *const pkg)
{
	Server::Connection *cnn = pkg->source();
	QString root = pkg->root();
	QString package = pkg->xml();
	
	dWarning("server") << "PACKAGE: " << package;
	
	TcpServer *server = cnn->server();
	
	if ( root == "connect" )
	{
		cnn->setValid(false);
		
		Parsers::ConnectParser parser;
		if ( parser.parse(package) )
		{
			if ( d->manager->auth(parser.login(), parser.password()) )
			{
				Users::User *user = d->manager->user(parser.login());
				
				cnn->setUser(user);
				
				QString fortune = DFortuneGenerator::self()->generate();
				fortune.replace("\n", "<br/>");
				Packages::Ack ack(QObject::tr("<center><b>Message of the day:</b></center><br/> ")+fortune, cnn->sign());
				
				foreach(Users::Right *right, user->rights())
				{
					ack.addPermission(right);
				}
				
				if ( parser.client() == 1 )
				{
					server->addAdmin(cnn);
				}
				
				cnn->sendToClient(ack, false);
			}
			else
			{
				Packages::Error error(QObject::tr("Invalid login or password"), Packages::Error::Err);
				cnn->sendToClient(error);
				
				Bans::Manager::self()->failed(cnn->client()->peerAddress().toString());
				Base::Logger::self()->error(QObject::tr("Invalid login or password"));
				
				cnn->close();
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
		element.setAttribute("from", cnn->user()->login());
		
		cnn->sendToAll(doc);
	}
	else if ( root == "notice" )
	{
		QDomDocument doc;
		doc.setContent(package);
		
		QDomElement element = doc.firstChildElement("message");
		element.setAttribute("from", cnn->user()->login());
		
		cnn->sendToAll(doc);
	}
	else if ( root == "wall" )
	{
	}
	else if(root == "adduser")
	{
		if ( cnn->user()->canWriteOn("admin") )
		{
			Parsers::UserActionParser parser;
			if(parser.parse(package))
			{
				if( d->manager->addUser(parser.user()) )
				{
					QDomDocument doc;
					doc.setContent(package);
					
					doc.firstChild().removeChild( doc.firstChild().firstChildElement ("password"));
					
					server->sendToAdmins(doc.toString());
				}
				else
				{
					cnn->sendErrorPackageToClient(QObject::tr("Error adding user"), Packages::Error::Err);
				}
			}
		}
		else
		{
			cnn->sendErrorPackageToClient(QObject::tr("Permission denied."), Packages::Error::Err);
		}
	}
	else if(root == "removeuser")
	{
		if ( cnn->user()->canWriteOn("admin") )
		{
			Parsers::UserActionParser parser;
			if(parser.parse(package))
			{
				if ( d->manager->removeUser(parser.user().login()) )
					server->sendToAdmins(package);
			}
		}
		else
		{
			cnn->sendErrorPackageToClient(QObject::tr("Permission denied."), Packages::Error::Err);
		}
	}
	else if(root == "queryuser")
	{
		if ( cnn->user()->canReadOn("admin") )
		{
			Parsers::UserActionParser parser;
			if(parser.parse(package))
			{
				Users::User * user = d->manager->user(parser.user().login());
				if(user)
				{
					QDomDocument doc;
					QDomElement userquery = doc.createElement("userquery");
					
					userquery.appendChild(user->toXml(doc, false));
					doc.appendChild(userquery);
					
					cnn->sendToClient (doc.toString());
				}
			}
		}
		else
		{
			cnn->sendErrorPackageToClient(QObject::tr("Permission denied."), Packages::Error::Err);
		}
	}
	else if(root == "updateuser")
	{
		if ( cnn->user()->canWriteOn("admin") )
		{
			Parsers::UserActionParser parser;
			if(parser.parse(package))
			{
				if ( d->manager->updateUser(parser.user()) )
				{
					QDomDocument doc;
					doc.setContent(package);
					
					doc.firstChild().removeChild( doc.firstChild().firstChildElement ("password"));
					
					server->sendToAdmins(doc.toString());
				}
			}
		}
		else
		{
			cnn->sendErrorPackageToClient(QObject::tr("Permission denied."), Packages::Error::Err);
		}
	}
	else if ( root == "listusers" )
	{
		if ( cnn->user()->canReadOn("admin") )
		{
			Packages::UserList info;
			foreach(Users::User *u, d->manager->listUsers())
			{
				info.addUser(u);
			}
			cnn->sendToClient(info);
		}
		else
		{
			cnn->sendErrorPackageToClient(QObject::tr("Permission denied."), Packages::Error::Err);
		}
	}
	else if ( root == "listbans" )
	{
		if ( cnn->user()->canReadOn("admin") )
		{
			QStringList bans = Bans::Manager::self()->allBanned();
			
			Packages::BanList pkg;
			pkg.setBans(bans);
			
			cnn->sendToClient(pkg, true);
		}
		else
		{
			cnn->sendErrorPackageToClient(QObject::tr("Permission denied."), Packages::Error::Err);
		}
	}
	else if ( root == "removeban" )
	{
		if( cnn->user()->canWriteOn("admin") )
		{
			Parsers::RemoveBanParser parser;
			if ( parser.parse(package) )
			{
				Bans::Manager::self()->unban(parser.pattern());
				
				server->sendToAdmins(package);
			}
		}
		else
		{
			cnn->sendErrorPackageToClient(QObject::tr("Permission denied."), Packages::Error::Err);
		}
	}
	else if ( root == "addban" )
	{
		if( cnn->user()->canWriteOn("admin" ) )
		{
			Parsers::AddBanParser parser;
			if ( parser.parse(package) )
			{
				Bans::Manager::self()->ban(parser.pattern());
				server->sendToAdmins(package);
			}
		}
		else
		{
			cnn->sendErrorPackageToClient(QObject::tr("Permission denied."), Packages::Error::Err);
		}
	}
	else
	{
		handle(cnn, root, package);
	}
}

}

