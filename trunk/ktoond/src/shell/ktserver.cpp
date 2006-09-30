/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#include "ktserver.h"
#include "ktserverconnection.h"

#include <QHostInfo>

#include <ddebug.h>
#include <QDebug>

KTServer::KTServer(QObject *parent) : QTcpServer(parent)
{
	DINIT;
}

KTServer::KTServer(const QString &host, QObject *parent) : QTcpServer(parent)
{
	openConnection( host);
}

KTServer::~KTServer()
{
	DEND;
}

bool KTServer::openConnection(const QString &host)
{
	QList<QHostAddress> addrs = QHostInfo::fromName(host).addresses();
	qDebug() << addrs;
	if ( !addrs.isEmpty() )
	{
		int port = 31337;
		
		if(! listen(QHostAddress(addrs[0]), port) )
		{
			dError() << "Can't connect to " << host<<":"<<port<< " error was: " << errorString();
			return false;
		}
	}
	else
	{
		dError() << "Error while try to resolve " << host;
		return false;
	}
	return true;
}

void KTServer::incomingConnection(int socketDescriptor)
{
	SHOW_VAR(m_connections.count());
	
	KTServerConnection *newConnection = new KTServerConnection(socketDescriptor,this);
	
	handle(newConnection);
	
	m_connections << newConnection;
	
	newConnection->start();
}

void KTServer::handle(const KTServerConnection *cnx)
{
	connect(cnx, SIGNAL(finished()), cnx, SLOT(deleteLater()));
	
	connect(cnx, SIGNAL(requestSendToAll( const QString& )), this, SLOT(sendToAll( const QString& )));
// 	connect(cnx, SIGNAL(requestSendToAll( const QDomDocument& )), this, SLOT(sendToAll( const QDomDocument& )));
}


void KTServer::sendToAll(const QString &msg)
{
	foreach(KTServerConnection *connection, m_connections)
	{
		connection->sendToClient(msg);
	}
}

void KTServer::sendToAll(const QDomDocument &pkg)
{
	D_FUNCINFO;
	foreach(KTServerConnection *connection, m_connections)
	{
		connection->sendToClient(pkg);
	}
}

void KTServer::removeConnection(KTServerConnection *cnx)
{
	D_FUNCINFO;
	cnx->close();
// 	cnx->setLogin(0);
	m_connections.removeAll(cnx);
}


