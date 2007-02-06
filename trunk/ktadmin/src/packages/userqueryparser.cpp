/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosxx@gmail.com                                                   *
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
 
#include "userparser.h"

namespace Packages {

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
