/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include "ktnetprojectmanagerhandler.h"
#include "ktnetprojectmanagerparams.h"

#include "ktprojectresponse.h"


#include <ddebug.h>
#include <dosd.h>


#include "ktprojectcommand.h"
#include "ktcommandexecutor.h"

#include "ktnetsocket.h"

#include "ktprojectrequest.h"


#include "ktnewprojectpackage.h"
#include "ktconnectpackage.h"

#include "ktsavenetproject.h"
#include "ktopenpackage.h"

#include "kterrorparser.h"
#include "ktprojectsparser.h"
#include "ktprojectparser.h"
#include "ktrequestparser.h"
#include "ktrequestbuilder.h"

#include "ktackparser.h"

#include "ktlistprojectdialog.h"

#include "ktproject.h"

#include <QTemporaryFile>

struct KTNetProjectManagerHandler::Private
{
	KTNetSocket *socket;
	QString projectName, author;
	
	KTNetProjectManagerParams *params;
	KTProject *project;
	
	QString sign;
	bool ownPackage;
};

KTNetProjectManagerHandler::KTNetProjectManagerHandler(QObject *parent) : KTAbstractProjectHandler(parent), d(new Private)
{
	d->socket = new KTNetSocket(this);
	d->project = 0;
	d->params = 0;
	d->ownPackage = false;
}


KTNetProjectManagerHandler::~KTNetProjectManagerHandler()
{
	delete d;
}


void KTNetProjectManagerHandler::handleProjectRequest(const KTProjectRequest* request)
{
	// Esto llega desde el proyecto antes de ejecutar el comando!
	
	// TODO: Guardar una copia de los eventos o paquetes en una cola y reenviar a la GUI cuando llegue el paquete de que todo va bien desde el servidor!
	
	if ( d->socket->state() == QAbstractSocket::ConnectedState )
	{
		dDebug("net") << "SENDING: " << request->xml();
		d->socket->send( request->xml() );
	}
}


bool KTNetProjectManagerHandler::commandExecuted(KTProjectResponse *response)
{
	if ( response->mode() == KTProjectResponse::Do || d->ownPackage /*FIXME*/) return true;
	
	
	KTProjectRequest request = KTRequestBuilder::fromResponse(response);
	handleProjectRequest( &request );
	
	return false;
}

bool KTNetProjectManagerHandler::saveProject(const QString &fileName, const KTProject *project)
{
	KTSaveNetProject saver(d->params->server(), d->params->port());
	
	return saver.save(fileName, project);
}

bool KTNetProjectManagerHandler::loadProject(const QString &fileName, KTProject *project)
{
	if ( d->socket->state() != QAbstractSocket::ConnectedState  )
		return false;
	
	return loadProjectFromServer(d->params->projectName());
}


bool KTNetProjectManagerHandler::loadProjectFromServer(const QString &name)
{
	KTOpenPackage package(name);
	d->socket->send(package);
	return true;
}

bool KTNetProjectManagerHandler::initialize(KTProjectManagerParams *params)
{
	KTNetProjectManagerParams *netparams = dynamic_cast<KTNetProjectManagerParams*>(params);
	if ( ! netparams ) return false;
	
	d->params = netparams;
	
	dDebug("net") << "Connecting to " << netparams->server() << ":" << netparams->port();
	
	d->socket->connectToHost(d->params->server(), d->params->port());
	
	bool connected = d->socket->waitForConnected(1000);
	if(connected)
	{
		KTConnectPackage connectPackage( d->params->login(), d->params->password());
		d->socket->send( connectPackage );
	}
	
	return connected;
}

bool KTNetProjectManagerHandler::setupNewProject(KTProjectManagerParams *params)
{
	KTNetProjectManagerParams *netparams = dynamic_cast<KTNetProjectManagerParams*>(params);
	
	if ( ! netparams ) return false;
	
	SHOW_VAR(netparams->projectName());
	d->projectName = netparams->projectName();
// 	d->author = netparams->author();
	
	if ( ! d->socket->isOpen() )
	{
		bool connected = initialize(params);
		if ( !connected ) return false;
	}
	
	
	// TODO: enviar paquete de crear proyecto
	
	KTNewProjectPackage newProjectPackage(netparams->projectName(), "");
	d->socket->send(newProjectPackage);
	
	return true;
}



bool KTNetProjectManagerHandler::closeProject()
{
	return KTAbstractProjectHandler::closeProject();
}


void KTNetProjectManagerHandler::emitRequest(KTProjectRequest *request)
{
	emit sendCommand( request, true );
}

void KTNetProjectManagerHandler::handlePackage(const QString &root ,const QString &package )
{
	D_FUNCINFOX("net");
	if ( root == "request" )
	{
		KTRequestParser parser;
		if ( parser.parse(package) )
		{
			if ( parser.sign() == d->sign )
			{
				d->ownPackage = true;
			}
			else
			{
				d->ownPackage = false;
			}
			
			KTProjectRequest request = KTRequestBuilder::fromResponse(parser.response() );
			emitRequest(&request);
			
		}
		else // TODO: mostrar error
		{
			dError() << "Error parsing";
		}
	}
	else if( root == "ack")
	{
		//analizar el paquete
		KTAckParser parser;
		if ( parser.parse(package) )
		{
			d->sign = parser.sign();
		}
		
	}
	else if(root == "error")
	{
		KTErrorParser parser;
		if(parser.parse(package) )
		{
			DOsd::self()->display(parser.error().message, DOsd::Level(parser.error().level));
		}
	}
	else if(root == "project")
	{
		KTProjectParser parser;
		if(parser.parse(package) )
		{
			QTemporaryFile file;
			if(file.open ())
			{
				file.write(parser.data() );
				file.flush();
				
				
				if(d->project)
				{
					KTSaveProject *loader = 0;
					loader = new KTSaveProject;
					loader->load(file.fileName (), d->project);
					d->project->setOpen(true);
					emit openNewArea(d->project->projectName());
					delete loader;
				}
			}
		}
	}
	else if(root == "projects")
	{
		KTProjectsParser parser;
		if(parser.parse(package) )
		{
			KTListProjectDialog dialog;
			foreach(KTProjectsParser::ProjectInfo info, parser.projectsInfo())
			{
				dialog.addProject( info.name, info.author, info.description);
			}
			if(dialog.exec () == QDialog::Accepted && !dialog.currentProject().isEmpty())
			{
				dDebug() << "opening " << dialog.currentProject() << "project";
				loadProjectFromServer(dialog.currentProject() );
			}
		}
	}
	else
	{
		dDebug("net") << "Unknown package: " << root;
	}
}


bool KTNetProjectManagerHandler::isValid() const
{
	return d->socket->state() == QAbstractSocket::ConnectedState;
}

void KTNetProjectManagerHandler::sendPackage(const QDomDocument &doc)
{
	d->socket->send(doc);
}

void KTNetProjectManagerHandler::setProject(KTProject *project)
{
	d->project = project;
}

