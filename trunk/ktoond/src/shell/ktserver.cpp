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

#include "abstracthandlerpackages.h"
#include "defaulthandlerpackages.h"

class KTServer::Private
{
	public:
		QList<KTServerConnection *> connections;
		AbstractHandlerPackages *handler;

};

KTServer::KTServer(QObject *parent) : QTcpServer(parent), d(new Private)
{
	DINIT;
	d->handler = new DefaultHandlerPackages(this);
}


KTServer::~KTServer()
{
	DEND;
	delete d;
}

bool KTServer::openConnection(const QString &host, int port)
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

void KTServer::setHandler( AbstractHandlerPackages *handler )
{
	d->handler = handler;
	handler->setServer(this);
}

void KTServer::incomingConnection(int socketDescriptor)
{
	SHOW_VAR(d->connections.count());
	
	KTServerConnection *newConnection = new KTServerConnection(socketDescriptor,this);
	handle(newConnection);
	d->connections << newConnection;
	newConnection->start();
}

void KTServer::handle(const KTServerConnection *cnx)
{
	connect(cnx, SIGNAL(finished()), cnx, SLOT(deleteLater()));
	connect(cnx, SIGNAL(requestSendToAll( const QString& )), this, SLOT(sendToAll( const QString& )));
	connect(cnx, SIGNAL(packagesReaded(const KTServerConnection*, const QString&)), this, SLOT(handlerPackages(const KTServerConnection*, const QString&)));
	connect(cnx, SIGNAL(connectionClosed(KTServerConnection*)), this, SLOT(removeConnection(KTServerConnection*)));
	
	
// 	d->projects.createProject(cnx);
	
}


void KTServer::sendToAll(const QString &msg)
{
	dDebug("server") << "SENDING TO ALL: " << msg;
	foreach(KTServerConnection *connection, d->connections)
	{
		connection->sendToClient(msg);
	}
}

void KTServer::sendToAll(const QDomDocument &pkg)
{
	D_FUNCINFO;
	QString doc = pkg.toString(0);
	
	foreach(KTServerConnection *connection, d->connections)
	{
		connection->sendToClient(doc);
	}
}

void KTServer::removeConnection(KTServerConnection *cnx)
{
        D_FUNCINFO;
        cnx->close();
//      cnx->setLogin(0);
        d->connections.removeAll(cnx);
}


void KTServer::handlerPackages(const KTServerConnection* cnx, const QString&package)
{
	d->handler->handle(cnx, package);
}


