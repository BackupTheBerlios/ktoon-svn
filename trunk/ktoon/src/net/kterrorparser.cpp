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
 
#include "kterrorparser.h"

KTErrorParser::KTErrorParser(): KTXmlParserBase()
{
}


KTErrorParser::~KTErrorParser()
{
}


bool KTErrorParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if(root() == "error")
	{
		if(tag == "message")
		{
			m_error.level = atts.value("level").toInt();
			setReadText(true);
		}
	}
	return true;
}

bool KTErrorParser::endTag(const QString &tag)
{
	return true;
}

void KTErrorParser::text(const QString &text)
{
	if(currentTag() == "message")
	{
		m_error.message = text;
	}
}

KTErrorParser::Error KTErrorParser::error()
{
	return m_error;
}

