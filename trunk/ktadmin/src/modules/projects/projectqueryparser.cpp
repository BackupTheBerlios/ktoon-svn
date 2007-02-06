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
 
#include "projectqueryparser.h"

namespace Projects {

struct ProjectQueryParser::Private
{
	QString name;
	QString author;
	QString description;
	QMultiHash<UserSelector::UserType, QString> users;
	UserSelector::UserType tmpType;
};

ProjectQueryParser::ProjectQueryParser() : KTXmlParserBase(), d(new Private)
{
}


ProjectQueryParser::~ProjectQueryParser()
{
}

bool ProjectQueryParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if(root() == "projectquery")
	{
		if(tag == "project")
		{
			d->name = atts.value("name");
			d->author = atts.value("author");
			d->description = atts.value("description");
		}
		else if(tag == "user")
		{
			d->tmpType = UserSelector::UserType(atts.value("type").toInt());
			setReadText(true);
		}
	}
	return true;
}

bool ProjectQueryParser::endTag(const QString &)
{
	return true;
}

void ProjectQueryParser::text(const QString &text)
{
	d->users.insert(d->tmpType, text);
}


QString ProjectQueryParser::name()
{
	return d->name;
}

QString ProjectQueryParser::author()
{
	return d->author;
}

QString ProjectQueryParser::description()
{
	return d->description;
}

QMultiHash<UserSelector::UserType, QString> ProjectQueryParser::users()
{
	return d->users;
}


}
