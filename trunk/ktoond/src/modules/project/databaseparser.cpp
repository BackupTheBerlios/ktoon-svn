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
#include <dcore/ddebug.h>
#include <QDir>

namespace Parsers {

DatabaseParser::DatabaseParser()
	: KTXmlParserBase(), m_projectExists(false), m_findUserProjects(false),  m_findProject(false), m_loadProject(true), m_project(0)
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
			tmpInfo.description = atts.value("description");
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
			
			tmpInfo.file = filename;
			
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
					m_project->setAuthor(tmpInfo.author);
					m_project->setDescription(tmpInfo.description);
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
		if(m_findUserProjects)
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

QList< Projects::Database::ProjectInfo > DatabaseParser::userProjects(const QString& login, const QString& db)
{
	m_condition = login;
	m_findUserProjects = true;
	parse(db);
	m_findUserProjects = false;
	return m_projectsInfo;
}

QList< Projects::Database::ProjectInfo > DatabaseParser::allProjects(const QString &db )
{
	m_condition = QString();
	m_findUserProjects = false;
	parse(db);
	
	return m_projectsInfo;
}

Projects::Database::ProjectInfo DatabaseParser::projectInfo(const QString &projectName, const QString &db)
{
	QList< Projects::Database::ProjectInfo > list = allProjects(db);
	
	foreach(Projects::Database::ProjectInfo info, list)
	{
		if( info.name == projectName)
			return info;
	}
	
	return Projects::Database::ProjectInfo();
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

void DatabaseParser::gc()
{
	D_FUNCINFO;
	delete m_project;
	m_projectExists = 0;
}

}


