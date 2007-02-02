//
// C++ Implementation: useractionparser
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "useractionparser.h"
#include "../users/user.h"
#include "../users/right.h"

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

bool UserActionParser::endTag(const QString &tag)
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

Users::User UserActionParser::user()
{
	return d->user;
}

}
