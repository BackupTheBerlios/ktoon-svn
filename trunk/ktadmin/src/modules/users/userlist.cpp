//
// C++ Implementation: usersinfoparser
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "usersinfoparser.h"

namespace Parsers {

struct UsersInfoParser::Private
{
	QList<QStringList> info;
};

UsersInfoParser::UsersInfoParser() : KTXmlParserBase() , d(new Private)
{
}


UsersInfoParser::~UsersInfoParser()
{
}

bool UsersInfoParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if(root() == "usersinfo")
	{
		if(tag == "user")
		{
			QStringList values;
			values << atts.value("login") << atts.value("name");
			d->info << values;
		}
	}
	return true;
}

bool UsersInfoParser::endTag(const QString &)
{
	return true;
}

void UsersInfoParser::text(const QString &)
{
	
}

QList<QStringList> UsersInfoParser::info()
{
	return d->info;
}

}
