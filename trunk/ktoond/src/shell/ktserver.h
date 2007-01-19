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

#ifndef FORTUNESERVER_H
#define FORTUNESERVER_H

#include <QStringList>
#include <QTcpServer>
#include <QDomDocument>

#include "ktprojectrequest.h"

class KTServerConnection;
class AbstractHandlerPackages;

/**
 * Esta es la clase controladora, esta clase representa el servidor.
 * @author David Cuadrado \<krawek@gmail.com\>
 */
class KTServer : public QTcpServer
{
	Q_OBJECT;
	
	public:
		KTServer(QObject *parent = 0);
		~KTServer();
		void sendToAll(const QDomDocument &pkg);
		bool openConnection(const QString &host, int port);
		void setHandler( AbstractHandlerPackages *handler );
		
		
	public slots:
		void sendToAll(const QString &msg);
		void removeConnection(KTServerConnection *cnx);
		void handlerPackages( const KTServerConnection *cnn, const QString & packages  );
	
	private:
		void handle(const KTServerConnection *cnx);
		
	protected:
		void incomingConnection(int socketDescriptor);
		
	private:
		class Private;
		Private * const d;
};

#endif


