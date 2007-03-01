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

#include "manager.h"

#include <QHash>
#include <QDomDocument>

#include "parser.h"
#include "user.h"
#include "right.h"
#include "database.h"

#include <dcore/dmd5hash.h>
#include <dcore/ddebug.h>
#include <dcore/dfortunegenerator.h>

#include "base/package.h"
#include "base/settings.h"
#include "base/logger.h"

#include "core/server.h"
#include "core/connection.h"

#include "useractionparser.h"
#include "userlist.h"

#include "connectparser.h"
#include "ack.h"

#include "bans/banmanager.h"

namespace Users {

class Manager::Private
{
	public:
		Private()
		{
		}
		
		~Private()
		{
			delete parser;
			delete database;
		}
		
		Parser *parser;
		Database *database;
};

Manager::Manager() : d(new Private())
{
	QString dbfile = Base::Settings::self()->databaseDirPath()+"/users.xml";
	d->database = new Database(dbfile);
	d->parser = new Parser(dbfile);
}


Manager::~Manager()
{
	delete d;
}


bool Manager::auth(const QString &login, const QString &password)
{
	if ( User *user = d->parser->user(login) )
	{
		dDebug() << "login: " << user->login() << " pass: " << user->password();
		dDebug() << "comp: " << password;
		if( user->password() == password )
		{
			return true;
		}
		
		delete user;
		return false;
	}
	else
	{
		dWarning() << "ERROR PARSING USER DATABASE";
	}
	
	return false;
}

User *Manager::user(const QString &login)
{
	return d->parser->user(login);
}

bool Manager::addUser( const User & user )
{
	if(!d->parser->user( user.login() ))
	{
		return d->database->addUser(user);
	}
	return false;
}

bool Manager::updateUser( const User & user )
{
	return d->database->updateUser(user);
}

bool Manager::removeUser(const QString &login)
{
	return d->database->removeUser(login);
}



QList<User *> Manager::listUsers()
{
	return d->parser->listUsers();
}

void Manager::handlePackage(Base::Package *const pkg)
{
	QString root = pkg->root();
	QString package = pkg->xml();
	
	Server::Connection *cnn = pkg->source();
	Server::TcpServer *server = cnn->server();
	
	if ( root == "connect" )
	{
		cnn->setValid(false);
		
		Parsers::ConnectParser parser;
		if ( parser.parse(package) )
		{
			if ( this->auth(parser.login(), parser.password()) )
			{
				Users::User *user = this->user(parser.login());
				
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
				
				server->banManager()->failed(cnn->client()->peerAddress().toString());
				Base::Logger::self()->error(QObject::tr("Invalid login or password"));
				
				cnn->close();
			}
		}
		else
		{
			dError() << "ERROR PARSING CONNECT PACKAGE!";
		}
		
	}
	else if(root == "adduser")
	{
		if ( cnn->user()->canWriteOn("admin") )
		{
			Parsers::UserActionParser parser;
			if(parser.parse(package))
			{
				if( this->addUser(parser.user()) )
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
				if ( this->removeUser(parser.user().login()) )
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
				Users::User * user = this->user(parser.user().login());
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
				if ( this->updateUser(parser.user()) )
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
			foreach(Users::User *u, this->listUsers())
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
}

}



