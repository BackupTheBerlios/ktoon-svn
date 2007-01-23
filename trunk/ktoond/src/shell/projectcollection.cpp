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

ProjectCollection::ProjectCollection()
{
}


ProjectCollection::~ProjectCollection()
{
}

void ProjectCollection::createProject( Server::Connection *cnn, const QString &author )
{
	QString projectName = cnn->data(Info::ProjectName).toString();
	if(!m_projects.contains( projectName ))
	{
		SProject *project = new SProject;
		
		project->setProjectName(projectName);
		m_projects.insert(projectName, project);
		
		saveProject(projectName);
	}
	else
	{
		//TODO: enviar paquete de error
	}
}


void ProjectCollection::openProject( Server::Connection *cnn )
{
 	QString projectName = cnn->data(Info::ProjectName).toString();
	if(!m_projects.contains( projectName ))
	{
		KTSaveProject *loader = new KTSaveProject;
		SProject *project = 0;
		
		loader->load(dAppProp->cacheDir() + "/" + projectName, project);
		
		delete loader;
		if(project)
		{
			m_projects.insert(projectName, project);
		}
	}
	
	QString fileName = dAppProp->cacheDir() + "/" + projectName ;
	if ( !fileName.endsWith(".ktn") )
	{
		fileName += ".ktn";
	}
	
	Packages::Project project(fileName);
	cnn->sendToClient(project.toString());
	
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
	delete m_projects.take(name);
	
	
}

void ProjectCollection::saveProject(const QString & name)
{
	if(m_projects.contains( name ))
	{
		m_projects[name]->save();
	}
}

bool ProjectCollection::handleProjectRequest(Server::Connection *cnn, const QString strRequest)
{
	QString projectName = cnn->data(Info::ProjectName).toString();
	SHOW_VAR(projectName);
	KTRequestParser parser;
	if ( parser.parse(strRequest) )
	{
		KTProject *project = m_projects[projectName];
		if(project)
		{
			KTCommandExecutor * commandExecutor = new KTCommandExecutor(project);
			KTProjectCommand command(commandExecutor, parser.response());
			command.redo();
			delete commandExecutor;
			m_projects[projectName]->resetTimer();
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
