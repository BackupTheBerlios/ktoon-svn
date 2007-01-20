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


namespace Server {

Connection::Connection(int socketDescriptor, Server::TcpServer *server) : QThread(server), m_server(server)
{
	m_client = new Server::Client(this);
	m_client->setSocketDescriptor(socketDescriptor);
	
	connect(m_client, SIGNAL(disconnected()), this, SLOT(disconnect()));
	
// 	m_projectName = "prueba.ktn";
	
}

Connection::~Connection()
{
	delete m_client;
}

void Connection::run()
{
	while(m_client->state() != QAbstractSocket::UnconnectedState)
	{
		if ( m_readed.isEmpty() ) continue;
		
		QString readed = m_readed.dequeue();
		
		dDebug("server") << "Reicived: " << readed;
		QDomDocument doc;
		if (doc.setContent(readed.trimmed()) )
		{
			emit packagesReaded(this, readed);
		}
		else
		{
			dError("server") << "Cannot set document content!";
		}
	}
	// Finish connection
	disconnect();
}


void Connection::disconnect()
{
	emit connectionClosed(this);
}

void Connection::close()
{
	m_client->disconnectFromHost();
	m_client->close();
	
	m_isLogged = false;
}

bool Connection::isLogged() const
{
	return m_isLogged;
}

void Connection::appendTextReaded(const QString &readed)
{
	dDebug("server") << "Enqueing: " << readed;
	m_readed.enqueue(readed);
}

void Connection::setData(int key, const QVariant &value)
{
	m_datas.insert(key, value);
}

QVariant Connection::data(int key) const
{
	return m_datas[key];
}


}



