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
	: KTXmlParserBase(), m_findFilename(false), m_findProjectsUser(false)
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
			
			
			tmpInfo.name = atts.value("name");
			tmpInfo.author = atts.value("author");
			tmpInfo.description = atts.value("descrition");
			
		}
		else if(tag == "user")
		{
			m_users << atts.value("login");
		}
		else if(tag == "file")
		{
			QString filename = atts.value("name");
			
			if(filename > m_lastFileName)
			{
				m_lastFileName = filename;
			}
			
			if(m_findFilename)
			{
				if(tmpInfo.name == m_condition)
				{
					m_fileName = filename;
					m_findFilename = false;
				}
			}
		}
	}
	return true;
}

bool DatabaseParser::endTag(const QString &tag)
{
	if(tag == "project")
	{
		if(m_findProjectsUser)
		{
			if(m_users.contains(m_condition))
			{
				m_projectsInfo <<  tmpInfo;
			}
		}
		else
		{
			m_projectsInfo <<  tmpInfo;
		}
		m_users.clear();
	}
	return true;
}

void DatabaseParser::text(const QString &text)
{
	
}

QString DatabaseParser::fileName(const QString & nameproject, const QString& db)
{
	m_condition = nameproject;
	m_findFilename = true;
	parse(db);
	return m_fileName;
}

QString DatabaseParser::lastFileName()
{
	return m_lastFileName;
}

QList< Projects::Database::ProjectInfo > DatabaseParser::projectsInfoOfUser(const QString& login, const QString& db)
{
	m_condition = login;
	m_findProjectsUser = true;
	parse(db);
	m_findProjectsUser = false;
	return m_projectsInfo;
}

}
