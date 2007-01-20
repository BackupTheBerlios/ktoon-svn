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

#include "dcore/dmd5hash.h"

namespace Users {

struct Manager::Private 
{
	Parser *parser;
	QHash<QString, User *> users;
};

Manager::Manager() : d(new Private)
{
	d->parser = new Parser("users.xml"); // FIXME: path
}


Manager::~Manager()
{
	delete d;
}

bool Manager::auth(const QString &login, const QString &password)
{
	if ( d->users.contains(login) ) return true;
	
	if ( User *user = d->parser->user(login) )
	{
		if( user->password() == DMD5Hash::hash(password) )
		{
			d->users.insert(login, user);
			return true;
		}
		
		delete user;
		return false;
	}
	
	return false;
}




}




