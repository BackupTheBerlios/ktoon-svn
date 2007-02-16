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

#include <QFile>

#include <dcore/dmd5hash.h>
#include <dcore/ddebug.h>

#include "base/package.h"
#include "base/settings.h"

#include "core/connection.h"
#include "core/server.h"

#include "actionregisteruserparser.h"
#include "database.h"

#include "users/manager.h"
#include "users/user.h"

#include "adduser.h"


namespace Registers {

struct Manager::Private
{
	Database *db;
};

Manager::Manager() : Base::Observer(),d( new Private)
{
	d->db = new Database(Base::Settings::self()->databaseDirPath() + "/petitions.xml");
}


Manager::~Manager()
{
	delete d->db;
	delete d;
}


void Manager::handlePackage(Base::Package* const pkg)
{
	Server::TcpServer *server = pkg->source()->server();
	
	if( pkg->root() == "listregisters" )
	{
		QString fname = d->db->fileName();
		
		QFile f(fname);
		if( f.exists() )
		{
			if( f.open(QIODevice::ReadOnly | QIODevice::Text) )
			{
				pkg->source()->sendToClient(f.readAll());
			}
		}
	}
	else if ( pkg->root() == "registeruser" )
	{
		Parsers::ActionRegisterUserParser parser;
		
		if( parser.parse(pkg->xml()))
		{
			QHash<QString, QString> data = parser.data();
			QString email = data["email"];
			
			data = d->db->findRegisterByEmail(email);
			d->db->removeRegister(email);
			
			dDebug() << data["login"];
			
			Users::Manager *manager = server->userManager();
			
			Users::User user;
			user.setLogin(data["login"]);
			user.setName(data["name"]);
			user.setPassword(DMD5Hash::hash(""));
			manager->addUser(user);
			
			server->sendToAdmins(pkg->xml());
			Packages::AddUser adduser(data["login"], data["name"]);
			server->sendToAdmins(adduser.toString());
		}
	}
}

}

