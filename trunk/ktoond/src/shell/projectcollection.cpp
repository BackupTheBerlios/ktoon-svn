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


namespace Projects {

struct ProjectCollection::Private
{
	QHash<QString, SProject * > projects;
	Database *db;
	QHash<QString, QList<Server::Connection *> > connections;
};

ProjectCollection::ProjectCollection() : d(new Private())
{
	d->db = new Database(dAppProp->configDir() + "/database/projects.xml");
}


ProjectCollection::~ProjectCollection()
{
	delete d->db;
	delete d;
}

void ProjectCollection::createProject( Server::Connection *cnn, const QString &author )
{
	QString projectName = cnn->data(Info::ProjectName).toString();
	if(d->db->exists(projectName))
	{
		cnn->sendErrorPackageToClient(QObject::tr("Project %1 exists").arg(projectName), Packages::Error::Warning );
		
		cnn->removeConnection();
	}
	else
	{
		if(!d->projects.contains( projectName ) )
		{
			SProject *project = new SProject( dAppProp->cacheDir() +"/"+ d->db->nextFileName());
			QObject::connect(project, SIGNAL(requestSendErrorMessage(const QString&, Packages::Error::Level)), cnn, SLOT(sendErrorPackageToClient(const QString&, Packages::Error::Level)));
			project->setProjectName(projectName);
			d->projects.insert(projectName, project);
			d->db->addProject(project);
			saveProject(projectName);
			d->connections.insert(projectName, QList<Server::Connection *>() << cnn);
		}
	}
}


void ProjectCollection::openProject( Server::Connection *cnn )
{
 	QString projectName = cnn->data(Info::ProjectName).toString();
	
	QString fileName = d->db->fileName(projectName);
	
	if(fileName.isEmpty())
	{
		cnn->sendErrorPackageToClient(QObject::tr("Project %1 not exists").arg(projectName), Packages::Error::Err);
		cnn->close();
		return;
	}
	
	if(!d->projects.contains( projectName ) )
	{
		KTSaveProject *loader = new KTSaveProject;
		SProject *project = new SProject(fileName);
		QObject::connect(project, SIGNAL(requestSendErrorMessage(const QString&, Packages::Error::Level)), cnn, SLOT(sendErrorPackageToClient(const QString&, Packages::Error::Level)));
		bool ok  =loader->load(fileName, project);
		delete loader;
		if(!ok)
		{
			cnn->sendErrorPackageToClient(QObject::tr("Project %1 not exists").arg(projectName), Packages::Error::Err);
			return;
		}
		if(project)
		{
			d->projects.insert(projectName, project);
		}
	}
	else
	{
		d->projects[projectName]->save();
		QObject::connect(d->projects[projectName], SIGNAL(requestSendErrorMessage(const QString&, int)), cnn, SLOT(sendErrorPackageToClient(const QString&, int)));
	}
	
	Packages::Project project(fileName);
	cnn->sendToClient(project.toString());
	
	d->connections[projectName].append( cnn );
	
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

void ProjectCollection::saveProject(const QString & name)
{
	if(d->projects[name])
	{
		d->projects[name]->save();
	}
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

void ProjectCollection::listProjects(Server::Connection *cnn)
{
	Packages::Projects list;
	foreach(Database::ProjectInfo info, d->db->projectsInfoOfUser(cnn->user()->login()))
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
		cnn->sendToClient(doc.toString(0));
	}
}

}
