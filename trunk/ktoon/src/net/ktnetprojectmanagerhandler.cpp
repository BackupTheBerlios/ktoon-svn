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

#include "ktprojectrequest.h"
#include "ktprojectcommand.h"
#include "ktcommandexecutor.h"

#include "ktnetsocket.h"

// #include "ktrequestpackage.h"

#include <ddebug.h>

KTNetProjectManagerHandler::KTNetProjectManagerHandler(QObject *parent) : KTAbstractProjectHandler(parent)
{
	m_socket = new KTNetSocket(this);
	
	connect(m_socket, SIGNAL(connected()), this, SLOT(sendHello()));
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
		m_socket->sendToServer( request->xml() );
	}
}


bool KTNetProjectManagerHandler::commandExecuted(KTProjectResponse *response, int state)
{
	if ( state == KTCommandExecutor::Do ) return true;
	
	// FIXME: ESTO LLEGA DESDE EL SERVIDOR!
	
	return false;
}

bool KTNetProjectManagerHandler::saveProject(const QString &fileName, const KTProject *project)
{
	qFatal("ktnetprojectmanagerhandler.h: Can't save");
}

bool KTNetProjectManagerHandler::loadProject(const QString &fileName, KTProject *project)
{
	qFatal("ktnetprojectmanagerhandler.h: Can't load");
}

bool KTNetProjectManagerHandler::setupNewProject(KTProjectManagerParams *params)
{
	KTNetProjectManagerParams *netparams = dynamic_cast<KTNetProjectManagerParams*>(params);
	
	if ( ! netparams ) return false;
	
	SHOW_VAR(netparams->projectName());
	
	dDebug("net") << "Connecting to " << netparams->server() << ":" << netparams->port();
	
	m_socket->connectToHost(netparams->server(), netparams->port());
	return m_socket->waitForConnected(1000);
}

bool KTNetProjectManagerHandler::closeProject()
{
	return KTAbstractProjectHandler::closeProject();
}

void KTNetProjectManagerHandler::sendHello()
{
	m_socket->sendToServer( "<cnx>helo ktoon server!</cnx>\n");
}

void KTNetProjectManagerHandler::emitRequest(KTProjectRequest *request)
{
	emit sendCommand( request, true );
}




