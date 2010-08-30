/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
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
