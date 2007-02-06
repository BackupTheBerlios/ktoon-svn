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

#include "packagehandler.h"
#include "projectcollection.h"


#include "connection.h"
#include "global.h"
#include <ddebug.h>

#include "projectactionparser.h"
#include "newprojectparser.h"
#include "openprojectparser.h"
#include "importprojectparser.h"
#include "listparser.h"
#include "error.h"

#include "addbackupparser.h"

#include "backupmanager.h"

#include "listprojectsparser.h"

#include "user.h"
#include "database.h"

#include "addbackup.h"

class PackageHandler::Private
{
	public:
		Private(){}
		
		~Private() { delete projects; }
		
		Projects::ProjectCollection *projects;
};

PackageHandler::PackageHandler() : PackageHandlerBase(), d(new Private)
{
	d->projects = new Projects::ProjectCollection();
}

PackageHandler::~PackageHandler()
{
	delete d;
}

void PackageHandler::handle(Server::Connection *cnx , const QString &root, const QString &package )
{
	Server::TcpServer *server = cnx->server();
	
	if ( root == "request" )
	{
		if ( cnx->user()->canWriteOn("project") )
		{
			if(!cnx->data(Info::ProjectName).toString().isNull())
			{
				handleProjectRequest( cnx , package);
			}
			else
			{
				dWarning() << "NO PROJECT NAME!";
			}
		}
		else
		{
			cnx->sendErrorPackageToClient(QObject::tr("You doen't have rights on project."), Packages::Error::Warning );
		}
	}
	else if( root == "list")
	{
		//Lista de items, por ejemplo proyectos
		Parsers::ListParser parser;
		if(parser.parse(package))
		{
			
		}
		
	}
	else if( root == "openproject")
	{
		if ( cnx->user()->canReadOn("project") )
		{
			Parsers::OpenProjectParser parser;
			if(parser.parse(package))
			{
				cnx->setData(Info::ProjectName, parser.name());
				d->projects->openProject(cnx);
			}
		}
		else
		{
			cnx->sendErrorPackageToClient(QObject::tr("You doen't have rights on project."), Packages::Error::Warning );
		}
	}
	else if ( root == "newproject" )
	{
		if ( cnx->user()->canWriteOn("project") )
		{
			Parsers::NewProjectParser parser;
			if(parser.parse(package))
			{
				cnx->setData(Info::ProjectName, parser.name());
				d->projects->createProject(cnx, parser.author());
			}
			else
			{
				cnx->sendErrorPackageToClient(QObject::tr("You doen't have rights on project."), Packages::Error::Warning );
			}
		}
	}
	else if(root == "importproject")
	{
		if ( cnx->user()->canWriteOn("project") )
		{
			Parsers::ImportProjectParser parser;
			if(parser.parse(package))
			{
				d->projects->importProject(cnx, parser.data());
			}
		}
	}
	else if(root == "listprojects")
	{
		if ( cnx->user()->canReadOn("project") )
		{
			Parsers::ListProjectsParser parser;
			
			if( parser.parse(package) )
			{
				if( parser.readAll())
				{
					d->projects->listAllProjects(cnx);
				}
				else
				{
					d->projects->listUserProjects(cnx);
				}
			}
		}
		else
		{
			cnx->sendErrorPackageToClient(QObject::tr("You doen't have rights on project."), Packages::Error::Warning );
		}
	}
	else if(root == "addproject")
	{
		if ( cnx->user()->canWriteOn("project") && cnx->user()->canReadOn("admin"))
		{
			Parsers::ProjectActionParser parser;
			if(parser.parse(package))
			{
				cnx->setData(Info::ProjectName, parser.name());
				
				if(!d->projects->addProject( parser.name(), parser.author(), parser.description(), parser.users()))
				{
					cnx->sendErrorPackageToClient(QObject::tr("Cannot create project %1").arg(parser.name()), Packages::Error::Warning );
				}
				else
				{
					cnx->server()->sendToAdmins(package); //TODO: Enviar un paquete diferente
				}
				
				
				
				
			}
		}
		else
		{
			cnx->sendErrorPackageToClient(QObject::tr("You doen't have rights on project."), Packages::Error::Warning );
		}
	}
	else if ( root == "removeproject")
	{
		if ( cnx->user()->canWriteOn("project") && cnx->user()->canReadOn("admin"))
		{
			Parsers::ProjectActionParser parser;
			if(parser.parse(package))
			{
				if(!d->projects->removeProject( cnx ,parser.name()))
				{
					cnx->sendErrorPackageToClient(QObject::tr("Cannot remove project %1").arg(parser.name()), Packages::Error::Warning );
				}
			}
			cnx->server()->sendToAdmins(package);//TODO: Enviar un paquete diferente
		}
		else
		{
			cnx->sendErrorPackageToClient(QObject::tr("You doen't have rights on project."), Packages::Error::Warning );
		}
	}
	else if( root == "queryproject" )
	{
		
		if ( cnx->user()->canWriteOn("project") && cnx->user()->canReadOn("admin"))
		{
			Parsers::ProjectActionParser parser;
			if(parser.parse(package))
			{
				SProject *project = d->projects->project( parser.name() );
				if(project)
				{
					QDomDocument doc;
					QDomElement projectquery = doc.createElement("projectquery");
					
					projectquery.appendChild(project->toXml(doc));
					doc.appendChild(projectquery);
					
					cnx->sendToClient(doc.toString());
					delete project;
				}
			}
		}
		else
		{
			cnx->sendErrorPackageToClient(QObject::tr("You doen't have rights on project."), Packages::Error::Warning );
		}
	}
	else if(root == "updateproject")
	{
		if ( cnx->user()->canWriteOn("project") && cnx->user()->canReadOn("admin"))
		{
			Parsers::ProjectActionParser parser;
			if(parser.parse(package))
			{
				d->projects->updateProject(cnx, parser.name(), parser.author(), parser.description(), parser.users());
			}
		}
		else
		{
			cnx->sendErrorPackageToClient(QObject::tr("You doen't have rights on project."), Packages::Error::Warning );
		}
	}
	else if ( root == "addbackup" )
	{
		if ( cnx->user()->canWriteOn("admin") )
		{
			Parsers::AddBackupParser parser;
			if( parser.parse(package))
			{
				Packages::AddBackup pkg;
				
				Server::BackupManager *bm = server->backupManager();
				
				foreach(QString project, parser.backups())
				{
					QDateTime date = QDateTime::currentDateTime();
					
					Projects::Database::ProjectInfo info = d->projects->projectInfo(project);
					bm->makeBackup(info.file, date, info.name);
					
					pkg.addEntry(info.name, date);
				}
				
				server->sendToAdmins(pkg.toString());
			}
		}
		else
		{
			cnx->sendErrorPackageToClient(QObject::tr("Permission denied."), Packages::Error::Err);
		}
	}
}

void PackageHandler::handleProjectRequest(Server::Connection *cnn, const QString &strRequest)
{
	if ( d->projects->handleProjectRequest( cnn, strRequest) )
	{
		QDomDocument request;
		request.setContent(strRequest);
		
		d->projects->sendToProjectMembers(cnn, request);
// 		cnn->sendToAll(request); // test
	}
	else
	{
		cnn->sendErrorPackageToClient(QObject::tr("Cannot handle project request"), Packages::Error::Warning );
	}
}

void PackageHandler::connectionClosed(Server::Connection *client)
{
	d->projects->removeConnection(client);
}



