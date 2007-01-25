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

#include "chatparser.h"

namespace Parsers {

struct ChatParser::Private
{
	QString message;
};

ChatParser::ChatParser()
 : KTXmlParserBase(), d(new Private())
{
}


ChatParser::~ChatParser()
{
	delete d;
}



bool ChatParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if ( root() == "chat" )
	{
		if ( tag == "message" )
		{
			d->message = atts.value("text");
		}
	}
	
	
	return true;
}

bool ChatParser::endTag(const QString &)
{
	return true;
}

void ChatParser::text(const QString &)
{
}

QString ChatParser::message() const
{
	return d->message;
}


}