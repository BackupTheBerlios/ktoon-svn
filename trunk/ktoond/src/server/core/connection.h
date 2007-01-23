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
#include <QHash>
#include <QVariant>
#include "serverclient.h"
// #include <QDomDocument>

class KTProjectRequest;

namespace Server {
class TcpServer;

/**
 * Esta clase representa cada conexion de un cliente al servidor, es un hilo.
 * @author David Cuadrado \<krawek@gmail.com\>
 */
class Connection : public QThread
{
	Q_OBJECT;

	public:
		Connection(int socketDescriptor, Server::TcpServer *server);
		~Connection();
		
		void run();
		
		void close();
		bool isLogged() const;
		
		void appendTextReaded(const QString &readed);
		
		void sendToClient(const QString &text) const;
		
		void setData(int key, const QVariant &value);
		QVariant data(int key) const;
		
		Client *client() const;
		TcpServer *server() const; // FIXME: REMOVER, NO USAR
		
		void sendToAll(const QString &text);
		
	public slots:
		void removeConnection();
		
		
	signals:
		void error(QTcpSocket::SocketError socketError);
		void requestSendToAll(const QString &msg);
		void connectionClosed( Server::Connection *cnn);
		void packageReaded(Server::Connection *cnn, const QString& root,const QString & packages  );
		
	private:
		class Private;
		Private *const d;
};

}

#endif


