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
#include "listprojectsparser.h"

namespace Parsers {

struct ListProjectsParser::Private
{
	bool readAll;
};

ListProjectsParser::ListProjectsParser()
 : KTXmlParserBase(), d(new Private)
{
}


ListProjectsParser::~ListProjectsParser()
{
	delete d;
}

bool ListProjectsParser::startTag(const QString &tag, const QXmlAttributes &)
{
	if( tag == "listprojects")
	{
		d->readAll = false;
	}
	else if( tag == "option" )
	{
		setReadText(true);
	}
	
	return true;
}
bool ListProjectsParser::endTag(const QString &)
{
	return true;
}
void ListProjectsParser::text(const QString &text)
{
	if ( currentTag() == "option" )
	{
		if ( text == "all" )
		{
			d->readAll = true;
		}
	}
}

bool ListProjectsParser::readAll() const
{
	return d->readAll;
}

}
