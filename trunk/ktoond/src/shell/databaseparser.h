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
#ifndef PARSERSDATABASEPARSE_H
#define PARSERSDATABASEPARSE_H

#include <ktxmlparserbase.h>
#include <QHash>
#include "database.h"

namespace Parsers
{

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/
class DatabaseParser : public KTXmlParserBase
{
	public:
		DatabaseParser();
		~DatabaseParser();
		
		virtual bool startTag(const QString &tag, const QXmlAttributes &atts);
		virtual bool endTag(const QString &tag);
		virtual void text(const QString &text);
		
		QString lastFileName();
		
		QList< Projects::Database::ProjectInfo > userProjects(const QString & login, const QString &db );
		QList< Projects::Database::ProjectInfo > allProjects(const QString &db );
		
		bool exists(const QString & nameProject, const QString &db );
		
		SProject *loadProject(const QString &projectName, const QString &db);
		
		Projects::Database::ProjectInfo projectInfo(const QString &projectName, const QString &db);
		
	private:
		// TODO: Poner esto en un struct Private
		QString m_lastFileName;
		
		QString m_condition;
		bool m_projectExists;
		QStringList  m_users;
		
		bool m_findUserProjects, m_findProject, m_loadProject;
		QList< Projects::Database::ProjectInfo > m_projectsInfo;
		
		SProject *m_project;
		
		Projects::Database::ProjectInfo tmpInfo; // ????
		SProject::UserType m_typeUser;
		
		
		
};

}

#endif
