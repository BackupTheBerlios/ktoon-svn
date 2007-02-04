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
#include "databaseparser.h"
#include <ddebug.h>
#include <QDir>

namespace Parsers {

DatabaseParser::DatabaseParser()
	: KTXmlParserBase(), m_projectExists(false), m_findProjectsUser(false),  m_findProject(false), m_loadProject(true), m_project(0)
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
			if(m_findProject)
			{
				if(tmpInfo.name == m_condition)
				{
					m_projectExists = true;
				}
			}
		}
		else if(tag == "user")
		{
			m_typeUser = SProject::UserType(atts.value("type").toInt());
			setReadText(true);
		}
		else if(tag == "file")
		{
			QString filename = atts.value("name");
			
			if(filename > m_lastFileName)
			{
				m_lastFileName = filename;
			}
			
			if(m_loadProject)
			{
				if(tmpInfo.name == m_condition)
				{
					m_project = new SProject(filename);
					m_project->setProjectName(tmpInfo.name);
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
		else if(tmpInfo.name == m_condition)
		{
			m_loadProject = false;
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
	if(currentTag() == "user")
	{
		m_users << text;
		if(m_loadProject)
		{
			if(m_project)
			{
				m_project->addUser( text, m_typeUser);
			}
		}
	}
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

bool DatabaseParser::exists(const QString& projectName, const QString& db)
{
	m_condition = projectName;
	m_findProject = true;
	parse(db);
	m_findProject = false;
	return m_projectExists;
}

SProject * DatabaseParser::loadProject(const QString& projectName, const QString &db)
{
	m_condition = projectName;
	m_loadProject = true;
	parse(db);
	m_loadProject = false;
	return m_project;
}

}


