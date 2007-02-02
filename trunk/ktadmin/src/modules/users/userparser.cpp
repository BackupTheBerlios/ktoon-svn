//
// C++ Implementation: userparser
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "userparser.h"

namespace Parsers {

struct UserParser::Private
{
	QString login;
	QString name;
	QList<Users::Permission> permissions;
};

UserParser::UserParser(): KTXmlParserBase() , d(new Private)
{
	
}


UserParser::~UserParser()
{
}

bool UserParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if(root() == "user")
	{
		if(tag == "user")
		{
			d->login = atts.value("login");
			d->name = atts.value("name");
		}
		else if(tag == "perm")
		{
			d->permissions << Users::Permission(atts.value("module"), bool(atts.value("read").toInt()), bool(atts.value("write").toInt()));
		}
	}
	return true;
}

bool UserParser::endTag(const QString &)
{
	return true;
}

void UserParser::text(const QString &)
{
	
}

QString UserParser::login()
{
	return d->login;
}

QString UserParser::name()
{
	return d->name;
}

QList<Users::Permission> UserParser::permissions()
{
	return d->permissions;
}



}
