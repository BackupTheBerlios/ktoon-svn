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
#include "databaseparse.h"
#include <ddebug.h>
namespace Parsers {

DatabaseParser::DatabaseParser()
	: KTXmlParserBase(), m_readFilename(false)
{
}


DatabaseParser::~DatabaseParser()
{
}

bool DatabaseParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if(root() == "projects")
	{
		if(tag == "project")
		{
			QString filename = atts.value("filename");
			if(filename > m_lastFileName)
			{
				m_lastFileName = filename;
			}
			if(m_readFilename)
			{
				if(atts.value("name") == m_nameproject)
				{
					m_fileName = filename;
					m_readFilename = false;
				}
			}
		}
	}
	return true;
}

bool DatabaseParser::endTag(const QString &tag)
{
	return true;
}

void DatabaseParser::text(const QString &text)
{
	
}

QString DatabaseParser::fileName(const QString & nameproject, const QString& db)
{
	m_nameproject = nameproject;
	m_readFilename = true;
	parse(db);
	return m_fileName;
}

QString DatabaseParser::lastFileName()
{
	return m_lastFileName;
}


}
