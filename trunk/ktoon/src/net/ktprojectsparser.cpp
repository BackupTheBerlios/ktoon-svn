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
 
#include "ktprojectsparser.h"

struct KTProjectsParser::Private
{
	QList<KTProjectsParser::ProjectInfo> projectsInfo;
};

KTProjectsParser::KTProjectsParser() : KTXmlParserBase(), k( new Private())
{
}

KTProjectsParser::~KTProjectsParser()
{
}

bool KTProjectsParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if(root() == "projectlist")
	{
		if(tag == "project")
		{
			ProjectInfo info;
			info.name = atts.value("name");
			info.author = atts.value("author");
			info.description = atts.value("description");
			k->projectsInfo << info;
		}
	}
	return true;
}

bool KTProjectsParser::endTag(const QString &tag)
{
	return true;
}

void KTProjectsParser::text(const QString &text)
{
	
}

QList<KTProjectsParser::ProjectInfo> KTProjectsParser::projectsInfo()
{
	return k->projectsInfo;
}
