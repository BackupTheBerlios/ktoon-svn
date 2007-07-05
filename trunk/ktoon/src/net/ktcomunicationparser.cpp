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
 

#include "ktcomunicationparser.h"

struct KTComunicationParser::Private
{
	QString message;
	QString login;
};


KTComunicationParser::KTComunicationParser() : KTXmlParserBase(), d(new Private())
{
	
}


KTComunicationParser::~KTComunicationParser()
{
	
}


bool KTComunicationParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if ( root() == "chat" || root() == "notice" || root() == "wall" )
	{
		if ( tag == "message" )
		{
			d->message = atts.value("text");
			d->login = atts.value("from");
		}
	}
	
	
	return true;
}

bool KTComunicationParser::endTag(const QString &)
{
	return true;
}

void KTComunicationParser::text(const QString &)
{
}

QString KTComunicationParser::message() const
{
	return d->message;
}

QString KTComunicationParser::login() const
{
	return d->login;
}

