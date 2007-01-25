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

#include "ktprojectcommand.h"
#include "ktcommandexecutor.h"

#include "ktnetsocket.h"

#include "ktprojectrequest.h"


#include "ktnewprojectpackage.h"
#include "ktconnectpackage.h"

#include "ktsavenetproject.h"
#include "ktopenpackage.h"

#include "ktprojectparser.h"
#include "ktrequestparser.h"
#include "ktrequestbuilder.h"

#include <QTemporaryFile>

KTNetProjectManagerHandler::KTNetProjectManagerHandler(QObject *parent) : KTAbstractProjectHandler(parent)
{
	m_socket = new KTNetSocket(this);
}


KTNetProjectManagerHandler::~KTNetProjectManagerHandler()
{
}


void KTNetProjectManagerHandler::handleProjectRequest(const KTProjectRequest* request)
{
	// Esto llega desde el proyecto antes de ejecutar el comando!
	
	// TODO: Guardar una copia de los eventos o paquetes en una cola y reenviar a la GUI cuando llegue el paquete de que todo va bien desde el servidor!
	
	if ( m_socket->state() == QAbstractSocket::ConnectedState )
	{
		dDebug("net") << "SENDING: " << request->xml();
		m_socket->send( request->xml() );
	}
}


bool KTNetProjectManagerHandler::commandExecuted(KTProjectResponse *response)
{
	dFatal() << "commandExecuted";
	if ( response->mode() == KTProjectResponse::Do ) return true;
	/*
	*/
	
	// FIXME: ESTO LLEGA DESDE EL SERVIDOR!
	
	return false;
}

bool KTNetProjectManagerHandler::saveProject(const QString &fileName, const KTProject *project)
{
	KTSaveNetProject saver(m_params->server(), m_params->port());
	
	return saver.save(fileName, project);
}

bool KTNetProjectManagerHandler::loadProject(const QString &fileName, KTProject *project)
{
	if ( m_socket->state() != QAbstractSocket::ConnectedState  )
		return false;
	
	KTOpenPackage package(m_params->projectName());
	
	m_socket->send(package);
	m_project = project;
			
	return true;
}

bool KTNetProjectManagerHandler::initialize(KTProjectManagerParams *params)
{
	KTNetProjectManagerParams *netparams = dynamic_cast<KTNetProjectManagerParams*>(params);
	if ( ! netparams ) return false;
	
	m_params = netparams;
	
	dDebug("net") << "Connecting to " << netparams->server() << ":" << netparams->port();
	
	m_socket->connectToHost(m_params->server(), m_params->port());
	
	bool connected = m_socket->waitForConnected(1000);
	if(connected)
	{
		KTConnectPackage connectPackage( m_params->login(), m_params->password());
		m_socket->send( connectPackage );
	}
	
	return connected;
}

bool KTNetProjectManagerHandler::setupNewProject(KTProjectManagerParams *params)
{
	KTNetProjectManagerParams *netparams = dynamic_cast<KTNetProjectManagerParams*>(params);
	
	if ( ! netparams ) return false;
	
	SHOW_VAR(netparams->projectName());
	m_projectName = netparams->projectName();
// 	m_author = netparams->author();
	
	if ( ! m_socket->isOpen() )
	{
		bool connected = initialize(params);
		if ( !connected ) return false;
	}
	
	
	// TODO: enviar paquete de crear proyecto
	
	KTNewProjectPackage newProjectPackage(netparams->projectName(), "");
	m_socket->send(newProjectPackage);
	
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
				
				KTSaveProject *loader = 0;
				
				loader = new KTSaveProject;
				loader->load(file.fileName (), m_project);
				delete loader;
				
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
	return m_socket->isOpen();
}
