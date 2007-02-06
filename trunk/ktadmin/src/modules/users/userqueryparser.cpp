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
 
#include "userqueryparser.h"

namespace Users {

struct UserQueryParser::Private
{
	QString login;
	QString name;
	QList<Users::Permission> permissions;
};

UserQueryParser::UserQueryParser(): KTXmlParserBase() , d(new Private)
{
	
}


UserQueryParser::~UserQueryParser()
{
}

bool UserQueryParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if(root() == "userquery")
	{
		if(tag == "login" || tag == "name" )
		{
			setReadText(true);
		}
		else if(tag == "perm")
		{
			d->permissions << Users::Permission(atts.value("module"), bool(atts.value("read").toInt()), bool(atts.value("write").toInt()));
		}
	}
	return true;
}

bool UserQueryParser::endTag(const QString &)
{
	return true;
}

void UserQueryParser::text(const QString &text)
{
	if(currentTag() == "login")
	{
		d->login = text;
	}
	else if(currentTag() == "name")
	{
		d->name = text;
	}
}

QString UserQueryParser::login()
{
	return d->login;
}

QString UserQueryParser::name()
{
	return d->name;
}

QList<Users::Permission> UserQueryParser::permissions()
{
	return d->permissions;
}



}
