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
#include "newprojectparser.h"

namespace Parsers {

struct NewProjectParser::Private
{
	QString author;
	QString name;
	QString description;
};

NewProjectParser::NewProjectParser()
 : KTXmlParserBase(), d( new Private())
{
}


NewProjectParser::~NewProjectParser()
{
	delete d;
}




bool NewProjectParser::startTag(const QString &tag, const QXmlAttributes &)
{
	if ( root() == "newproject" )
	{
		if ( tag == "author" )
		{
			setReadText(true);
		}
		else if ( tag == "name")
		{
			setReadText(true);
		}
		else if (tag == "description")
		{
			setReadText(true);
		}
	}
	
	
	return true;
}

bool NewProjectParser::endTag(const QString &)
{
	return true;
}

void NewProjectParser::text(const QString &text)
{
	if ( currentTag() == "name" )
	{
		d->name = text;
	}
	else if ( currentTag() == "author")
	{
		d->author = text;
	}
	else if (currentTag() == "description")
	{
		d->description = text;
	}
}

QString NewProjectParser::author() const
{
	return d->author;
}

QString NewProjectParser::name() const
{
	return d->name;
}

QString NewProjectParser::description() const
{
	return d->description;
}

}
