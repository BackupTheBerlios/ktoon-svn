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

#include "removebanparser.h"

namespace Bans {

RemoveBanParser::RemoveBanParser()
 : KTXmlParserBase()
{
}


RemoveBanParser::~RemoveBanParser()
{
}

bool RemoveBanParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if ( tag == "pattern" )
	{
		m_pattern = atts.value("value");
	}
	return true;
}

bool RemoveBanParser::endTag(const QString &)
{
	
	return true;
}

QString RemoveBanParser::pattern() const
{
	return m_pattern;
}


}
