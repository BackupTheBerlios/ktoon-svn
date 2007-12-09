/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2007 by David Cuadrado <krawek@gmail.com>               *
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

#include "ktackparser.h"

struct KTAckParser::Private
{
	QString motd;
	QString sign;
};

KTAckParser::KTAckParser() : KTXmlParserBase(), k(new Private())
{
}


KTAckParser::~KTAckParser()
{
	delete k;
}

bool KTAckParser::startTag(const QString &tag, const QXmlAttributes &atts)
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

bool KTAckParser::endTag(const QString &)
{
	return true;
}

void KTAckParser::text(const QString &msg)
{
	if( currentTag() == "motd" )
	{
		k->motd = msg;
	}
	else if ( currentTag() == "sign" )
	{
		k->sign = msg;
	}
}

QString KTAckParser::sign() const
{
	return k->sign;
}


QString KTAckParser::motd() const
{
	return k->motd;
}

