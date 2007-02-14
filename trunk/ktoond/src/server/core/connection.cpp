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
#include "base/logger.h"

#include "ktrequestparser.h"
#include "ktcompress.h"

#include "ktprojectrequest.h"
#include "ktprojectresponse.h"
#include "ktrequestparser.h"

#include "users/user.h"

#include "dmd5hash.h"
#include "dalgorithm.h"

#include "packages/error.h"


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
			delete user;
		}
		
		Server::Client *client;
		Server::TcpServer *server;
		bool isValid;
		QQueue<QString> readed;
		QHash<int, QVariant> datas;
		QString sign;
		
		Users::User *user;
};

Connection::Connection(int socketDescriptor, Server::TcpServer *server) : QThread(server), d(new Private(server))
{
	DINIT;
	d->client = new Server::Client(this);
	d->client->setSocketDescriptor(socketDescriptor);
	d->isValid = true;
}

Connection::~Connection()
{
	DEND;
	delete d;
}

void Connection::run()
{
	while(d->client->state() != QAbstractSocket::UnconnectedState)
	{
		if ( d->readed.isEmpty() || !d->isValid ) continue;
		
		if( !d->user )
			d->isValid = false;
		
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
	
	removeConnection();
}


void Connection::removeConnection()
{
	emit connectionClosed(this);
}

void Connection::close()
{
	d->isValid = false;
	
	d->readed.clear();
	if ( d->client->state() != QAbstractSocket::UnconnectedState )
	{
		d->client->flush();
		
		d->client->disconnectFromHost();
		d->client->waitForDisconnected();
		d->client->close();
	}
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

void Connection::sendToClient(QDomDocument &doc, bool sign)
{
	if ( sign)
		signPackage(doc);
	
	dDebug() << "sending " << doc.toString();
	
	d->client->send(doc);
	
}

void Connection::sendToAll(QDomDocument &doc, bool sign)
{
	if( sign )
		signPackage(doc);
	emit requestSendToAll(doc.toString(0));
}

void Connection::signPackage(QDomDocument &doc)
{
	doc.documentElement().setAttribute("sign", d->sign);
}

QString Connection::sign() const
{
	return d->sign;
}

void Connection::setUser(Users::User *user)
{
	d->user = user;
	generateSign();
	
	d->isValid = true;
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


void Connection::sendErrorPackageToClient(const QString & message, Packages::Error::Level level)
{
	Packages::Error error(message, level);
	sendToClient(error);
}

void Connection::setValid(bool v)
{
	d->isValid = v;
}

bool Connection::isValid() const
{
	return d->isValid;
}

}



