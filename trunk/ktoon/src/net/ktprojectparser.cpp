/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by Jorge Cuadrado <kuadrosx@toonka.com>            *
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

#include "ktprojectparser.h"
#include <kcore/kdebug.h>

struct KTProjectParser::Private
{
	QByteArray data;
};

KTProjectParser::KTProjectParser(): KTXmlParserBase() , k( new Private())
{
}


KTProjectParser::~KTProjectParser()
{
}


bool KTProjectParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if(root() == "project")
	{
		if(tag == "data")
		{
			setReadText(true);
		}
	}
	return true;
}

bool KTProjectParser::endTag(const QString &tag)
{
	Q_UNUSED(tag);
	return true;
	
}

void KTProjectParser::text(const QString &text)
{
	if ( currentTag() == "data" )
	{
		k->data = QByteArray::fromBase64(text.toLocal8Bit());
	}
}

QByteArray KTProjectParser::data()
{
	return k->data;
}
