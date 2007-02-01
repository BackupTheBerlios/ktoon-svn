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

#include "ackparser.h"

namespace Packages {

struct AckParser::Private
{
	QString motd;
	QString sign;
};

AckParser::AckParser() : KTXmlParserBase(), d(new Private())
{
}


AckParser::~AckParser()
{
	delete d;
}

bool AckParser::startTag(const QString &tag, const QXmlAttributes &)
{
	if( tag == "motd" )
	{
		setReadText(true);
	}
	else if ( tag == "sign" )
	{
		setReadText(true);
	}
	
	return true;
}

bool AckParser::endTag(const QString &)
{
	return true;
}

void AckParser::text(const QString &msg)
{
	if( currentTag() == "motd" )
	{
		d->motd = msg;
	}
	else if ( currentTag() == "sign" )
	{
		d->sign = msg;
	}
}

QString AckParser::sign() const
{
	return d->sign;
}


QString AckParser::motd() const
{
	return d->motd;
}

}
