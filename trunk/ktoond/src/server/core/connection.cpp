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

#include "connection.h"

#include <QtNetwork>

#include <ddebug.h>

#include "server.h"
#include "ktrequestparser.h"
#include "ktcompress.h"

#include "ktprojectrequest.h"
#include "ktprojectresponse.h"
#include "ktrequestparser.h"

#include "users/user.h"

#include "dmd5hash.h"
#include "dalgorithm.h"


namespace Server {

class Connection::Private
{
	public:
		Private(TcpServer *server) : server(server), user(0)
		{
		}
		
		~Private()
		{
			delete client;
			delete server;
		}
		
		Server::Client *client;
		Server::TcpServer *server;
		bool isLogged;
		QQueue<QString> readed;
		QHash<int, QVariant> datas;
		QString sign;
		
		Users::User *user;
};

Connection::Connection(int socketDescriptor, Server::TcpServer *server) : QThread(server), d(new Private(server))
{
	d->client = new Server::Client(this);
	d->client->setSocketDescriptor(socketDescriptor);
	
	connect(d->client, SIGNAL(disconnected()), this, SLOT(removeConnection()));
}

Connection::~Connection()
{
	delete d->client;
}

void Connection::run()
{
	while(d->client->state() != QAbstractSocket::UnconnectedState)
	{
		if ( d->readed.isEmpty() ) continue;
		
		QString readed = d->readed.dequeue();
		
		dDebug("server") << "Reicieved: " << readed;
		QDomDocument doc;
		if (doc.setContent(readed.trimmed()) )
		{
			emit packageReaded(this, doc.documentElement().tagName(), readed);
		}
		else
		{
			dError("server") << "Cannot set document content!";
		}
	}
	// Finish connection
	removeConnection();
}


void Connection::removeConnection()
{
	emit connectionClosed(this);
}

void Connection::close()
{
	d->client->disconnectFromHost();
	d->client->close();
	
	d->isLogged = false;
}

bool Connection::isLogged() const
{
	return d->isLogged;
}

void Connection::appendTextReaded(const QString &readed)
{
	dDebug("server") << "Enqueing: " << readed;
	d->readed.enqueue(readed);
}

void Connection::sendToClient(const QString &text) const
{
	d->client->send(text);
}

void Connection::setData(int key, const QVariant &value)
{
	d->datas.insert(key, value);
}

QVariant Connection::data(int key) const
{
	return d->datas[key];
}

Client *Connection::client() const
{
	return d->client;
}

TcpServer *Connection::server() const
{
	return d->server;
}

void Connection::sendToAll(const QString &text)
{
	emit requestSendToAll(text);
}

void Connection::sendToClient(QDomDocument &doc)
{
	signPackage(doc);
	d->client->send(doc);
}

void Connection::sendToAll(QDomDocument &doc)
{
	signPackage(doc);
	emit requestSendToAll(doc.toString(0));
}

void Connection::signPackage(QDomDocument &doc)
{
	doc.documentElement().setAttribute("sign", d->sign);
}

void Connection::setUser(Users::User *user)
{
	d->user = user;
	generateSign();
}

Users::User *Connection::user() const
{
	return d->user;
}

void Connection::generateSign()
{
	if ( d->user )
	{
		d->sign = DMD5Hash::hash(d->user->login()+d->user->password()+DAlgorithm::randomString(DAlgorithm::random() % 10));
	}
}


}



