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

#include "connectparser.h"

namespace Parsers {

class ConnectParser::Private
{
	public:
		QString login;
		QString password;
};

ConnectParser::ConnectParser() : KTXmlParserBase(), d(new Private)
{
}


ConnectParser::~ConnectParser()
{
	delete d;
}

bool ConnectParser::startTag(const QString &tag, const QXmlAttributes &)
{
	if ( root() == "connect" )
	{
		if ( tag == "login" )
		{
			setReadText(true);
		}
		else if ( tag == "password")
		{
			setReadText(true);
		}
	}
	
	
	return true;
}

bool ConnectParser::endTag(const QString &)
{
	return true;
}

void ConnectParser::text(const QString &text)
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


QString ConnectParser::login() const
{
	return d->login;
}

QString ConnectParser::password() const
{
	return d->password;
}

}


