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
#include <QTimer>
#include <QQueue>
#include <QHostAddress>

#include <ddebug.h>
#include <QDebug>

#include "packagehandlerbase.h"
#include "defaultpackagehandler.h"
#include "settings.h"
#include "banmanager.h"
#include "logger.h"

namespace Server {

class TcpServer::Private
{
	public:
		QList<Server::Connection *> connections;
		BanManager *banManager;
};

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent), d(new Private)
{
	DINIT;
	m_handler = new DefaultPackageHandler();
	d->banManager = new BanManager(this);
}


TcpServer::~TcpServer()
{
	DEND;
	
	Logger::self()->info("Server finished");
	
	delete d;
	delete Settings::self();
	delete Logger::self();
}

bool TcpServer::openConnection(const QString &host, int port)
{
	Logger::self()->info(QObject::tr("Initialized server on %1:%2").arg(host).arg(port));
	
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
	
	QString ip = newConnection->client()->peerAddress().toString();
	
	d->banManager->initialize(ip);
	
	if ( !d->banManager->isBanned(ip) )
	{
		handle(newConnection);
		d->connections << newConnection;
		newConnection->start();
	}
	else
	{
		newConnection->sendErrorPackageToClient(tr("You're banned, get out!"), Packages::Error::Err);
		newConnection->close();
	}
}

void TcpServer::handle(Server::Connection *cnx)
{
	connect(cnx, SIGNAL(finished()), cnx, SLOT(deleteLater()));
	connect(cnx, SIGNAL(requestSendToAll( const QString& )), this, SLOT(sendToAll( const QString& )));
	
	connect(cnx, SIGNAL(packageReaded(Server::Connection*, const QString&, const QString&)), this, SLOT(handlePackage(Server::Connection*, const QString&, const QString&)));
	
	connect(cnx, SIGNAL(connectionClosed(Server::Connection*)), this, SLOT(removeConnection(Server::Connection*)));
	
}


void TcpServer::sendToAll(const QString &msg)
{
	foreach(Server::Connection *connection, d->connections)
	{
		connection->sendToClient(msg);
	}
}

void TcpServer::sendToAll(const QDomDocument &pkg)
{
	D_FUNCINFO;
	sendToAll(pkg.toString(0));
}

void TcpServer::removeConnection(Server::Connection *cnx)
{
        D_FUNCINFO;
        cnx->close();
	
	if ( cnx->isFault() )
		d->banManager->failed(cnx->client()->peerAddress().toString());
        d->connections.removeAll(cnx);
	m_handler->connectionClosed(cnx);
}


void TcpServer::handlePackage(Server::Connection* client, const QString &root, const QString&package)
{
	m_handler->handlePackage(client, root, package);
}


}


