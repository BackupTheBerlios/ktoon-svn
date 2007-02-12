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

#include "projectcollection.h"

#include "ktproject.h"
#include "ktrequestparser.h"
#include "ktsaveproject.h"

#include <ktcommandexecutor.h>
#include <ktprojectcommand.h>

#include "connection.h"
#include <dapplicationproperties.h>

#include <QDir>
#include "global.h"

#include <ddebug.h>

#include "project.h"
#include "user.h"


#include <QHash>
#include <sproject.h>

#include "database.h"
#include "error.h"

#include "settings.h"


namespace Projects {

struct ProjectCollection::Private
{
	QHash<QString, SProject * > projects;
	Database *db;
	QHash<QString, QList<Server::Connection *> > connections;
};

ProjectCollection::ProjectCollection() : d(new Private())
{
	d->db = new Database(Server::Settings::self()->databaseDirPath() + "/projects.xml");
}


ProjectCollection::~ProjectCollection()
{
	delete d->db;
	delete d;
}

void ProjectCollection::createProject( Server::Connection *cnn, const QString &author ) 
{
	QString projectName = cnn->data(Info::ProjectName).toString();
	
	QMultiHash<int, QString> user;
	user.insert(int(SProject::Owner), cnn->user()->login());
	
	if(!addProject(projectName, author, "new project", user))
	{
		cnn->sendErrorPackageToClient(QObject::tr("Cannot create project %1").arg(projectName), Packages::Error::Warning );
	}
	openProject( cnn );
}

bool ProjectCollection::addProject(const QString& name, const QString& author, const QString& description, const QMultiHash<int, QString> & users )
{
	if(!d->db->exists(name) )
	{
		if(!d->projects.contains( name ) )
		{
			
			QMultiHash<SProject::UserType, QString> newusers; 
			foreach(int type, users.uniqueKeys())
			{
				foreach(QString login, users.values(type))
				{
					newusers.insert( SProject::UserType(type), login);
				}
			}
			if(newusers.values(SProject::Owner).empty())
			{
				return false;
			}
			
			SProject *project = new SProject( dAppProp->cacheDir() +"/"+ d->db->nextFileName());
			project->setProjectName(name);
			project->setAuthor(author);
			project->setDescription(description);
			bool okAddProject = false;
			
			project->setUsers(newusers);
			
			bool okSaveProject = project->save();
			if(okSaveProject)
			{
				okAddProject = d->db->addProject(project);
			}
			delete project;
			return okAddProject;
		}
	}
	return false;
	
}

bool ProjectCollection::removeProject( Server::Connection *cnn, const QString& name )
{
	if(!d->projects[name])
	{
		SProject *project = d->db->loadProject(name);
		if(project)
		{
			bool ok = d->db->removeProject(project);
			delete project;
			return ok;
		}
		else
		{
			cnn->sendErrorPackageToClient(QObject::tr("project not exist"), Packages::Error::Err);
			return false;
		}
	}
	
	cnn->sendErrorPackageToClient( QObject::tr("project is busy"), Packages::Error::Err);
	return false;
}

bool ProjectCollection::updateProject( Server::Connection *cnn, const QString& name, const QString& author, const QString& description, const QMultiHash<int, QString> &users)
{
	SProject *p = d->db->loadProject(name);
	if(p)
	{
		if(!author.isNull())
		{
			p->setAuthor(author);
		}
		if(!description.isNull())
		{
			p->setDescription(description);
		}
		
		QMultiHash<SProject::UserType, QString> newusers; 
		foreach(int type, users.uniqueKeys())
		{
			foreach(QString login, users.values(type))
			{
				newusers.insert( SProject::UserType(type), login);
			}
		}
		
		p->setUsers(newusers);
		
		
		bool ok = d->db->updateProject(p);
		delete p;
		if(!ok)
		{
			cnn->sendErrorPackageToClient( QObject::tr("cannot update project"), Packages::Error::Err);
			return false;
		}
		return true;
	}
	
	cnn->sendErrorPackageToClient(QObject::tr("Project %1 not exist").arg(name), Packages::Error::Err);
	return false;
}

SProject *ProjectCollection::project(const QString &name)
{
	return d->db->loadProject(name);
}

bool ProjectCollection::openProject( Server::Connection *cnn )
{
	QString projectName = cnn->data(Info::ProjectName).toString();
	SProject *project;
	
	if(!d->projects.contains( projectName ) )
	{
		project = d->db->loadProject(projectName);
		if(project)
		{
			KTSaveProject *loader = new KTSaveProject;
			QObject::connect(project, SIGNAL(requestSendErrorMessage(const QString&, Packages::Error::Level)), cnn, SLOT(sendErrorPackageToClient(const QString&, Packages::Error::Level)));
			bool ok  = loader->load(project->fileName(), project);
			delete loader;
			if(!ok)
			{
				cnn->sendErrorPackageToClient(QObject::tr("Project %1 not exists").arg(projectName), Packages::Error::Err);
				return false;
			}
			
			d->projects.insert(projectName, project);
		}
		else
		{
			cnn->sendErrorPackageToClient(QObject::tr("Project %1 not exists").arg(projectName), Packages::Error::Err);
			cnn->close();
			return false;
		}
	}
	else
	{
		project = d->projects[projectName];
		d->projects[projectName]->save();
		QObject::connect(d->projects[projectName], SIGNAL(requestSendErrorMessage(const QString&, int)), cnn, SLOT(sendErrorPackageToClient(const QString&, int)));
	}
	
	Packages::Project projectPackage(project->fileName());
	if(!projectPackage.isValid())
	{
		return false;
	}
	cnn->sendToClient(projectPackage.toString());
	
	d->connections[projectName].append(cnn);
	return true;
}

void ProjectCollection::importProject(Server::Connection *cnn, const QByteArray& data)
{
	QString filename = d->db->nextFileName();
	QFile file(filename);
	if(file.open(QIODevice::WriteOnly))
	{
		file.write(data);
		file.close();
		SProject *project = new SProject(filename);
		QObject::connect(project, SIGNAL(requestSendErrorMessage(const QString&, Packages::Error::Level)), cnn, SLOT(sendErrorPackageToClient(const QString&, Packages::Error::Level)));
		KTSaveProject *loader = new KTSaveProject;
		bool ok  = loader->load(filename, project);
		if(ok)
		{
			cnn->setData( Info::ProjectName , project->projectName());
			d->projects.insert(project->projectName(), project);
			d->db->addProject(project);
			d->connections.insert(project->projectName(), QList<Server::Connection *>() << cnn);
			
			Packages::Project projectPackage(project->fileName());
			cnn->sendToClient(projectPackage.toString());
		}
		else
		{
			//TODO: enviar error
		}
	}
}

QStringList ProjectCollection::projects() const
{
	QDir dir(dAppProp->cacheDir());
	return dir.entryList();
}

void ProjectCollection::closeProject(const QString & name)
{
	//FIXME: verificar que no hay otros con el proyecto abierto, antes de ejecutar esta funcion
	saveProject(name);
	delete d->projects.take(name);
	
	d->connections.remove(name);
}

bool ProjectCollection::saveProject(const QString & name)
{
	if(d->projects[name])
	{
		return d->projects[name]->save();
	}
	return false;
}

void ProjectCollection::removeConnection(Server::Connection *cnn)
{
	QString projectName = cnn->data(Info::ProjectName).toString();
	
	d->connections[projectName].removeAll(cnn);
	
	if ( d->connections[projectName].isEmpty() )
	{
		closeProject(projectName);
	}
}

bool ProjectCollection::handleProjectRequest(Server::Connection *cnn, const QString strRequest)
{
	QString projectName = cnn->data(Info::ProjectName).toString();
	SHOW_VAR(projectName);
	KTRequestParser parser;
	if ( parser.parse(strRequest) )
	{
		KTProject *project = d->projects[projectName];
		if(project)
		{
			KTCommandExecutor * commandExecutor = new KTCommandExecutor(project);
			KTProjectCommand command(commandExecutor, parser.response());
			command.redo();
			delete commandExecutor;
			d->projects[projectName]->resetTimer();
			//debug
// 			saveProject(projectName);
			
			return true;
		}
		else
		{
			dWarning() << "NO PROJECT FOR: " << projectName;
		}
	}
	
	return false;
}

void ProjectCollection::listAllProjects(Server::Connection *cnn)
{
	Packages::ProjectList list;
	
	foreach(Database::ProjectInfo info, d->db->allProjects())
	{
		list.addProject(info.name, info.author, info.description);
	}
	
	cnn->sendToAll(list);
}

void ProjectCollection::listUserProjects(Server::Connection *cnn)
{
	Packages::ProjectList list;
	foreach(Database::ProjectInfo info, d->db->userProjects(cnn->user()->login()))
	{
		list.addProject(info.name, info.author, info.description);
	}
	cnn->sendToClient(list.toString());
}

void ProjectCollection::sendToProjectMembers(Server::Connection *cnn, QDomDocument &doc)
{
	QString projectName = cnn->data(Info::ProjectName).toString();
	cnn->signPackage(doc);
	
	foreach(Server::Connection *cnn, d->connections[projectName])
	{
		if( cnn->user()->canReadOn("project") )
		{
			cnn->sendToClient(doc.toString(0));
		}
	}
}

void ProjectCollection::addUser(Server::Connection *cnn, const QString & login, SProject::UserType type  )
{
	QString projectName = cnn->data(Info::ProjectName).toString();
	SProject *project = 0;
	if(d->projects.contains( projectName ))
	{
		project = d->projects[projectName];
	}

	if(project)
	{
		if(project->isOwner(cnn->user()))
		{
			project->addUser(login, type);
			d->db->updateProject(project);
		}
		else
		{
			dDebug() << cnn->user()->login()  + " no es el owner";
		}
	}
	else
	{
		dDebug() << "no cargo el proyecto";
	}
}

Database::ProjectInfo ProjectCollection::projectInfo(const QString &projectName)
{
	return d->db->projectInfo(projectName);
}

}


