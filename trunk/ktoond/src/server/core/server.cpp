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

#include "packagehandlerbase.h"
#include "defaultpackagehandler.h"

#include "bans/banmanager.h"
#include "backups/backupmanager.h"
#include "users/manager.h"
#include "comunications/comunicationmanager.h"

#include "base/settings.h"
#include "base/package.h"
#include "base/observer.h"
#include "base/logger.h"

namespace Server{

class TcpServer::Private
{
	public:
		QList<Server::Connection *> connections;
		QList<Server::Connection *> admins;
		
		Backups::Manager *backupManager;
		Bans::Manager *banManager;
		Users::Manager *userManager;
		Comunications::Manager *comunicationManager;
		
		QList<Base::Observer *> observers;
};

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent), d(new Private)
{
	DINIT;
	d->backupManager = new Backups::Manager;
	d->observers << d->backupManager;
	
	d->banManager = new Bans::Manager;
	d->observers << d->banManager;
	
	d->userManager = new Users::Manager;
	d->observers << d->userManager;
	
	d->comunicationManager = new Comunications::Manager;
	d->observers << d->comunicationManager;
	
	
}


TcpServer::~TcpServer()
{
	DEND;
	
	Base::Logger::self()->info("Server finished");
	
	delete Base::Settings::self();
	delete Base::Logger::self();
	
	qDeleteAll(d->observers);
	
	
	delete d;
}

bool TcpServer::openConnection(const QString &host, int port)
{
	Base::Logger::self()->info(QObject::tr("Initialized server on %1:%2").arg(host).arg(port));
	
	QList<QHostAddress> addrs = QHostInfo::fromName(host).addresses();
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

void TcpServer::addAdmin(Server::Connection *cnx)
{
	d->admins << cnx;
}

Backups::Manager *TcpServer::backupManager() const
{
	return d->backupManager;
}

Bans::Manager *TcpServer::banManager() const
{
	return d->banManager;
}

Users::Manager *TcpServer::userManager() const
{
	return d->userManager;
}

void TcpServer::addObserver(Base::Observer *observer)
{
	d->observers << observer;
}

bool TcpServer::removeObserver(Base::Observer *observer)
{
	return d->observers.removeAll(observer) > 0;
}

void TcpServer::incomingConnection(int socketDescriptor)
{
	SHOW_VAR(d->connections.count());
	
	Server::Connection *newConnection = new Server::Connection(socketDescriptor,this);
	
	QString ip = newConnection->client()->peerAddress().toString();
	
	d->banManager->initialize(ip);
	
	if ( !d->banManager->isBanned(ip) )
	{
		d->banManager->unban(ip);
		
		handle(newConnection);
		d->connections << newConnection;
		newConnection->start();
	}
	else
	{
		newConnection->sendErrorPackageToClient(tr("You're banned, Please contact to server administrator if you think is an error!"), Packages::Error::Err);
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

void TcpServer::sendToAdmins(const QString &str)
{
	foreach(Server::Connection *cnn, d->admins)
	{
		cnn->sendToClient(str);
	}
}

void TcpServer::removeConnection(Server::Connection *cnx)
{
        D_FUNCINFO;
	
        d->connections.removeAll(cnx);
	d->admins.removeAll(cnx);
	
	cnx->blockSignals(true);
	cnx->close();
	cnx->blockSignals(false);
	
	
	foreach(Base::Observer *observer, d->observers)
	{
		observer->connectionClosed(cnx);
	}
	
	if( !cnx->isRunning())
	{
		delete cnx;
		cnx = 0;
	}
}


void TcpServer::handlePackage(Server::Connection* client, const QString &root, const QString&package)
{
	Base::Package *pkg = new Base::Package(root, package, client);
	
	foreach(Base::Observer *observer, d->observers)
	{
		observer->handlePackage(pkg);
		
		if ( pkg->accepted() )
			break;
	}
	
	delete pkg;
}


}


