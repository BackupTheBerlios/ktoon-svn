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
#ifndef PROJECTSDATABASE_H
#define PROJECTSDATABASE_H

#include <QString>

#include "sproject.h"

namespace Projects {

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/



class Database
{
	public:
		
		Database(const QString &dbfile = 0);
		~Database();
		
		bool addProject(const SProject * project);
		bool updateProject( const SProject * project);
		bool removeProject(  const SProject * project);
		
		void setDBFile( const QString& dbfile);
		
		QString nextFileName();
		
		struct ProjectInfo
		{
			QString name;
			QString author;
			QString description;
			QString file;
		};
		
		QList<Database::ProjectInfo> allProjects();
		QList<Database::ProjectInfo> userProjects(const QString& user);
		Database::ProjectInfo projectInfo(const QString &project);
		
		bool exists( const QString &projectName );
		
		SProject *loadProject(const QString &projectName );
		
		
	private:
		QDomDocument loadDataBase();
		QString m_dbfile;
		QString m_lastFileName;
};

}

#endif
