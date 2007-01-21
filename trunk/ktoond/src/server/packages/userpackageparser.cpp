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

#include "userpackageparser.h"

namespace Parsers {

class UserPackageParser::Private
{
	public:
		QString login;
		QString password;
};

UserPackageParser::UserPackageParser() : KTXmlParserBase(), d(new Private)
{
}


UserPackageParser::~UserPackageParser()
{
	delete d;
}

bool UserPackageParser::startTag(const QString &tag, const QXmlAttributes &)
{
	if ( root() == "connect" )
	{
		if ( tag == "login" )
		{
			setReadText(true);
		}
		else if ( tag == "password")
		{
			
		}
	}
	
	
	return true;
}

bool UserPackageParser::endTag(const QString &)
{
	return true;
}

void UserPackageParser::text(const QString &text)
{
	if ( currentTag() == "login" )
	{
		d->login = text;
	}
	else if ( currentTag() == "password")
	{
		d->password = text;
	}
}


QString UserPackageParser::login() const
{
	return d->login;
}

QString UserPackageParser::password() const
{
	return d->password;
}

}


