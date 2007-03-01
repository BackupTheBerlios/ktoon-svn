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

#include "ktnoticeparser.h"

struct KTNoticeParser::Private
{
	QString message, from;
};

KTNoticeParser::KTNoticeParser() : KTXmlParserBase(), d(new Private)
{
}


KTNoticeParser::~KTNoticeParser()
{
	
}

bool KTNoticeParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if( root() == "notice")
	{
		if(tag == "message")
		{
			d->message = atts.value("text");
			d->from = atts.value("from");
		}
	}
	return true;
}

bool KTNoticeParser::endTag(const QString &tag)
{
	Q_UNUSED(tag);
	return true;
}

void KTNoticeParser::text(const QString &msg)
{
	Q_UNUSED(msg);
}


QString KTNoticeParser::message() const
{
	return d->message;
}

QString KTNoticeParser::from() const
{
	return d->from;
}
