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

#include "parser.h"
#include "user.h"
#include "database.h"

#include "dcore/dmd5hash.h"
#include "dcore/ddebug.h"

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

Manager::Manager(const QString &dbfile) : d(new Private())
{
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
	return d->database->addUser(user);
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

}




