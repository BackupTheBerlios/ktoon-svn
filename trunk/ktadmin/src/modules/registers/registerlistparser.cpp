/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#include "registerlistparser.h"

#include <QList>
#include <QHash>

/*<register>
	<login value='[login]'/>
	<name value='[name]'/>
	<email value='[email]'/>
	<comments value='[comments]'/>
</register>*/

namespace Packages {

struct RegisterListParser::Private
{
	QList<QHash<QString, QString> > registers;
};

RegisterListParser::RegisterListParser() : KTXmlParserBase(), d(new Private)
{
}


RegisterListParser::~RegisterListParser()
{
	delete d;
}

bool RegisterListParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if( tag == "login" )
	{
		d->registers.last()["login"] = atts.value("value");
	}
	else if( tag == "name" )
	{
		d->registers.last()["name"] = atts.value("value");
	}
	else if( tag == "email" )
	{
		d->registers.last()["email"] = atts.value("value");
	}
	else if( tag == "comments" )
	{
		d->registers.last()["comments"] = atts.value("value");
	}
	else if(tag == "register" )
	{
		d->registers << QHash<QString, QString>();
	}
	
	return true;
}

bool RegisterListParser::endTag(const QString &)
{
	return true;
}

void RegisterListParser::text(const QString &)
{
}

QList<QHash<QString, QString> > RegisterListParser::registers() const
{
	return d->registers;
}

}

