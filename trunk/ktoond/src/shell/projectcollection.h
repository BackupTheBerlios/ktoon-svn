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

#ifndef PROJECTSPROJECTCOLLECTION_H
#define PROJECTSPROJECTCOLLECTION_H

#include <QString>
#include <QDomDocument>

#include "projects.h"
#include "sproject.h"

namespace Server {
class Connection;
}

namespace Users {
	class User;
}
namespace Projects {

/**
 * @author Jorge Cuadrado \<kuadrosxx@gmail.com\>
*/
class ProjectCollection 
{
	public:
		ProjectCollection();
		~ProjectCollection();
		
		void createProject(Server::Connection *cnn, const QString& author);
		bool addProject(const QString& name, const QString& author, const QString& description, const Users::User *owner );
		
		bool openProject(Server::Connection *cnn);
		void importProject(Server::Connection *cnn, const QByteArray& data);
		
		bool handleProjectRequest(Server::Connection *cnn, const QString strRequest);
		QStringList projects() const;
		void listProjects(Server::Connection *cnn);
		void closeProject(const QString & name);
		bool saveProject(const QString & name);
		void addUser(Server::Connection *cnn, const QString & login, SProject::UserType type  );
		
		void removeConnection(Server::Connection *cnn);
		void sendToProjectMembers(Server::Connection *cnn, QDomDocument &doc);
		
	private:
		struct Private;
		Private *const d;
		
};

}
#endif
