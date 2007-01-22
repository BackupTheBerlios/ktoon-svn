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

#include "server.h"
#include "connection.h"

#include <QHostInfo>

#include <ddebug.h>
#include <QDebug>

#include "packagehandlerbase.h"
#include "defaultpackagehandler.h"

namespace Server {

class TcpServer::Private
{
	public:
		QList<Server::Connection *> connections;
		QString dbdir;
};

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent), d(new Private)
{
	DINIT;
	m_handler = new Server::DefaultPackageHandler();
}


TcpServer::~TcpServer()
{
	DEND;
	delete d;
}

bool TcpServer::openConnection(const QString &host, int port)
{
	QList<QHostAddress> addrs = QHostInfo::fromName(host).addresses();
	qDebug() << addrs;
	if ( !addrs.isEmpty() )
	{
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

void TcpServer::incomingConnection(int socketDescriptor)
{
	SHOW_VAR(d->connections.count());
	
	Server::Connection *newConnection = new Server::Connection(socketDescriptor,this);
	handle(newConnection);
	d->connections << newConnection;
	newConnection->start();
}

void TcpServer::handle(Server::Connection *cnx)
{
	connect(cnx, SIGNAL(finished()), cnx, SLOT(deleteLater()));
	connect(cnx, SIGNAL(requestSendToAll( const QString& )), this, SLOT(sendToAll( const QString& )));
	
// 	connect(cnx, SIGNAL(packageReaded(Server::Connection*, const QString&)), this, SLOT(handlePackage(Server::Connection*, const QString&)));
	
	connect(cnx, SIGNAL(connectionClosed(Server::Connection*)), this, SLOT(removeConnection(Server::Connection*)));
	
	
// 	d->projects.createProject(cnx);
	
}


void TcpServer::sendToAll(const QString &msg)
{
	dDebug("server") << "SENDING TO ALL: " << msg;
	foreach(Server::Connection *connection, d->connections)
	{
		connection->sendToClient(msg);
	}
}

void TcpServer::sendToAll(const QDomDocument &pkg)
{
	D_FUNCINFO;
	QString doc = pkg.toString(0);
	
	foreach(Server::Connection *connection, d->connections)
	{
		connection->sendToClient(doc);
	}
}

void TcpServer::removeConnection(Server::Connection *cnx)
{
        D_FUNCINFO;
        cnx->close();
        d->connections.removeAll(cnx);
}


void TcpServer::handlePackage(Server::Connection* client, const QString &root, const QString&package)
{
	m_handler->handlePackage(client, root, package);
}



void TcpServer::setDatabaseDirPath(const QString &dbdir)
{
	d->dbdir = dbdir;
}


QString TcpServer::databaseDirPath() const
{
	return d->dbdir;
}

}


