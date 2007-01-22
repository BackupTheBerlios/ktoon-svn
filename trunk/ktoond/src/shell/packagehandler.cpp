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


class PackageHandler::Private
{
	public:
		Private()
		{
		}
		
		~Private()
		{
			delete projects;
		}
		
		ProjectCollection *projects;
};

PackageHandler::PackageHandler() : PackageHandlerBase(), d(new Private)
{
	d->projects = new ProjectCollection();
}

PackageHandler::~PackageHandler()
{
	delete d;
}

void PackageHandler::handle(Server::Connection *cnx , const QString &root, const QString &package )
{
	if ( root == "request" )
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
	else if( root == "connect" )
	{
		//Peticion de conexion al servidor
	}
	else if( root == "list")
	{
		//Lista de items, por ejemplo proyectos
	}
	else if( root == "open")
	{
		cnx->setData(Info::ProjectName, "name");
// 		Abrir proyecto
	}
}

void PackageHandler::handleProjectRequest(Server::Connection *cnn, const QString &strRequest)
{
	if ( d->projects->handleProjectRequest( cnn , strRequest) )
	{
		cnn->server()->sendToAll(strRequest);
	}
	else
	{
		// TODO: error
		dWarning() << "CANNOT HANDLE PROJECT REQUEST";
	}
}

