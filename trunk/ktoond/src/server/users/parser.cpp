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

#include "parser.h"
#include "user.h"

#include <QtDebug>

namespace Users {

struct Parser::Private {
	QString dbfile;
	QList<User *> users;
};

Parser::Parser(const QString &dbfile) : KTXmlParserBase(), d(new Private)
{
	if ( !QFile::exists(dbfile) )
	{
		qWarning() << "Cannot read users database from: " << dbfile;
	}
	
	d->dbfile = dbfile;
}


Parser::~Parser()
{
	delete d;
}

bool Parser::startTag(const QString& tag, const QXmlAttributes& atts)
{
	if ( tag == "user" )
	{
		User *u = new User;
		u->setLogin( atts.value("login") );
		u->setPassword( atts.value("password") );
		u->setName( atts.value("name") );
		d->users << u;
	}
	else if ( tag == "perm" )
	{
		Right *right = new Right(atts.value("module"), atts.value("read").toInt(), atts.value("write").toInt());
		d->users.last()->addRight(right);
	}
	
	return true;
}


bool Parser::endTag(const QString& /*tag*/)
{
	
	return true;
}


void Parser::text(const QString & /*ch*/ )
{
}


User *Parser::user(const QString &login)
{
	QFile file(d->dbfile);
	
	if ( parse(&file) )
	{
		foreach(User * user, d->users)
		{
			if(user->login() == login)
			{
				d->users.clear();
				return user;
			}
		}
	}
	return 0;
}

QList<User *> Parser::listUsers()
{
	d->users.clear();
	QFile file(d->dbfile);
	if ( parse(&file) )
		return d->users;
	return QList<User *>();
	
}

}


