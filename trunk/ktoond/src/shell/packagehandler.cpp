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


#include "newprojectparser.h"
#include "openprojectparser.h"
#include "importprojectparser.h"
#include "listparser.h"
#include "error.h"

#include "user.h"

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
			d->projects->listProjects(cnx);
		}
		else
		{
			cnx->sendErrorPackageToClient(QObject::tr("You doen't have rights on project."), Packages::Error::Warning );
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
		// TODO: enviar error
		
		cnn->sendErrorPackageToClient(QObject::tr("Cannot handle project request"), Packages::Error::Warning );
	}
}

void PackageHandler::connectionClosed(Server::Connection *client)
{
	d->projects->removeConnection(client);
}



