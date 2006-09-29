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

#include "ktserverconnection.h"

#include <QtNetwork>

// #include <ddebug.h>
#include <QtDebug>

KTServerConnection::KTServerConnection(int socketDescriptor, QObject *parent) : QThread(parent)
{
	m_client = new KTServerClient(this);
	m_client->setSocketDescriptor(socketDescriptor);
}

KTServerConnection::~KTServerConnection()
{
	delete m_client;
}

void KTServerConnection::run()
{
	while(m_client->state() != QAbstractSocket::UnconnectedState)
	{
		QString readed;
		while(m_client->canReadLine())
		{
			readed += m_client->readLine();
			if ( readed.endsWith("%%\n") )
			{
				break;
			}
		}
		
		if ( !readed.isEmpty() )
		{
			readed.remove(readed.lastIndexOf("%%"), 2);
			
			qDebug() << "READED: " << readed;
		}
	}
	
	// Finish connection
// 	emit requestRemoveConnection( this );
}

void KTServerConnection::sendToClient(const QString &msg)
{
// 	dDebug() << "SENDING: " << msg;
	
	qDebug() << "sending: " << msg;
	m_client->reset();
	
	QTextStream out(m_client);
	
	QString toSend(msg);
	toSend.remove('\n');
	
	out << toSend+"%%" << endl;
	m_client->flush();
}

void KTServerConnection::sendToClient(const QDomDocument &doc)
{
	sendToClient( doc.toString(0));
}

void KTServerConnection::close()
{
// 	D_FUNCINFO;
	
	m_client->disconnectFromHost();
	m_client->close();
	
	m_isLogged = false;
}

bool KTServerConnection::isLogged() const
{
	return m_isLogged;
}

