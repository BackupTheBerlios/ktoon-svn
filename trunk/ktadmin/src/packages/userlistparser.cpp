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
 
#include "userlistparser.h"

namespace Packages {

struct UserListParser::Private
{
	QList<QStringList> info;
};

UserListParser::UserListParser() : KTXmlParserBase() , d(new Private)
{
}


UserListParser::~UserListParser()
{
}

bool UserListParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if(root() == "userlist")
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

bool UserListParser::endTag(const QString &)
{
	return true;
}

void UserListParser::text(const QString &)
{
	
}

QList<QStringList> UserListParser::info()
{
	return d->info;
}

}
