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

#ifndef DOMSERVERCONNECTION_H
#define DOMSERVERCONNECTION_H

#include <QThread>
#include <QQueue>

#include "ktserverclient.h"

class KTServer;

/**
 * Esta clase representa cada conexion de un cliente al servidor, es un hilo.
 * @author David Cuadrado \<krawek@gmail.com\>
 */
class KTServerConnection : public QThread
{
	Q_OBJECT;

	public:
		KTServerConnection(int socketDescriptor, KTServer *server);
		~KTServerConnection();
		
		void run();
		
		void close();
		bool isLogged() const;
		
		void appendTextReaded(const QString &readed);
		
		inline void sendToClient(const QString &text) { m_client->send(text); }
		
	public slots:
		void disconnect();
		
		
	signals:
		void error(QTcpSocket::SocketError socketError);
		void requestSendToAll(const QString &msg);
		void connectionClosed(KTServerConnection *cnn);

	private:
		KTServerClient *m_client;
		KTServer *m_server;
		bool m_isLogged;
		
		QQueue<QString> m_readed;
};

#endif


