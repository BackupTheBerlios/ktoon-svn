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

#include "banlistparser.h"

namespace Packages {

BanListParser::BanListParser() : KTXmlParserBase()
{
}


BanListParser::~BanListParser()
{
}

bool BanListParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if( tag == "entry" )
	{
		m_bans << atts.value("value");
	}
	
	return true;
}


bool BanListParser::endTag(const QString &)
{
	return true;
}

QStringList BanListParser::bans() const
{
	return m_bans;
}

}
