/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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

#include "useractionparser.h"
#include "users/user.h"
#include "users/right.h"

namespace Parsers {

struct UserActionParser::Private
{
	Users::User user;
};

UserActionParser::UserActionParser() : KTXmlParserBase(), d(new Private)
{
}


UserActionParser::~UserActionParser()
{
}


bool UserActionParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if(root() == "adduser" || root() == "removeuser" || root() == "updateuser" || root() == "queryuser")
	{
		if(tag == "login")
		{
			setReadText(true);
		}
		else if(tag == "password")
		{
			setReadText(true);
		}
		else if(tag == "name")
		{
			setReadText(true);
		}
		else if(tag == "perm")
		{
			Users::Right *right = new Users::Right(atts.value("module"),
			bool(atts.value("read").toInt()),
			bool(atts.value("write").toInt()));
			
			d->user.addRight(right);
		}
		
	}
	return true;
}

bool UserActionParser::endTag(const QString &)
{
	return true;
}

void UserActionParser::text(const QString &text)
{
	if(currentTag() == "login")
	{
		d->user.setLogin(text);
	}
	else if(currentTag() == "password")
	{
		d->user.setPassword(text);
	}
	else if(currentTag() == "name")
	{
		d->user.setName(text);
	}
}

Users::User UserActionParser::user() const
{
	return d->user;
}

}
