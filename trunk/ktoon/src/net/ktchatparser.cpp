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
 

#include "ktchatparser.h"

struct KTChatParser::Private
{
	QString message;
	QString login;
};


KTChatParser::KTChatParser() : KTXmlParserBase(), d(new Private())
{
	
}


KTChatParser::~KTChatParser()
{
	
}


bool KTChatParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if ( root() == "chat" )
	{
		if ( tag == "message" )
		{
			d->message = atts.value("text");
			d->login = atts.value("from");
		}
	}
	
	
	return true;
}

bool KTChatParser::endTag(const QString &)
{
	return true;
}

void KTChatParser::text(const QString &)
{
}

QString KTChatParser::message() const
{
	return d->message;
}

QString KTChatParser::login() const
{
	return d->login;
}

