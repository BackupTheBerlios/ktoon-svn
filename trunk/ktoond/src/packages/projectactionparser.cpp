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
 
#include "projectactionparser.h"

namespace Parsers {

struct ProjectActionParser::Private
{
	QString name;
	QString author;
	QString description;
	QMultiHash<int, QString> users;
	int type;
};

ProjectActionParser::ProjectActionParser() : KTXmlParserBase(), d(new Private())
{
	
}


ProjectActionParser::~ProjectActionParser()
{
}

bool ProjectActionParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if(root() == "addproject" || root() == "removeproject" || root() == "updateproject" || root() == "queryproject")
	{
		if(tag == "name" || tag == "author" || tag == "description")
		{
			setReadText(true);
		}
		else if(tag == "user")
		{
			d->type = atts.value("type").toInt();
			setReadText(true);
		}
	}
	return true;
}

bool ProjectActionParser::endTag(const QString &)
{
	return true;
}

void ProjectActionParser::text(const QString &text)
{
	if(currentTag() == "user")
	{
		d->users.insert(d->type, text);
	}
	else if( currentTag() == "name" )
	{
		d->name = text;
	}
	else if( currentTag() == "author" )
	{
		d->author = text;
	}
	else if( currentTag() == "description" )
	{
		d->description = text;
	}
}

QString ProjectActionParser::name()
{
	return d->name;
}

QString ProjectActionParser::author()
{
	return d->author;
}

QString ProjectActionParser::description()
{
	return d->description;
}

QMultiHash<int, QString> ProjectActionParser::users()
{
	return d->users;
}



}
