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
#include "actionregisteruserparser.h"

namespace Packages {

struct ActionRegisterUserParser::Private
{
	QHash<QString, QString> data;
};

ActionRegisterUserParser::ActionRegisterUserParser()
 : KTXmlParserBase(), d(new Private)
{
}


ActionRegisterUserParser::~ActionRegisterUserParser()
{
	delete d;
}

bool ActionRegisterUserParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if( tag == "login" )
	{
		setReadText(true);
	}
	else if ( tag == "email" )
	{
		setReadText(true);
	}
	
	return true;
}

bool ActionRegisterUserParser::endTag(const QString &)
{
	return true;
}

void ActionRegisterUserParser::text(const QString &txt)
{
	d->data[currentTag()] = txt;
}

QHash<QString, QString> ActionRegisterUserParser::data() const
{
	return d->data;
}

}

