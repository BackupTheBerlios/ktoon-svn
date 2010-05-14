/***************************************************************************
 *   Project KTOOND: KToon Collaboration Daemon                            *
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
 *                                                                         *
 *  License:                                                               *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *    This program is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
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
        bool updateProject(const SProject * project);
        bool removeProject(const SProject * project);
        
        void setDBFile(const QString& dbfile);
        
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
        
        bool exists(const QString &projectName);
        
        SProject *loadProject(const QString &projectName);
        
    private:
        QDomDocument loadDataBase();
        QString m_dbfile;
        QString m_lastFileName;
};

}

#endif
