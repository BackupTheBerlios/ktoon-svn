/***************************************************************************
 *   Project KTOOND: KToon Collaboration Daemon                            *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 *
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *                                                                         *
 *  License:                                                               *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *    This program is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#ifndef DOMSERVERCONNECTION_H
#define DOMSERVERCONNECTION_H

#include <QThread>
#include <QQueue>
#include <QHash>
#include <QVariant>
#include "serverclient.h"

#include "../packages/error.h"

class KTProjectRequest;

namespace Users {
    class User;
}

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
        
        void appendTextReaded(const QString &readed);
        
        void sendToClient(const QString &text) const;
        void sendToAll(const QString &text);
        
        void sendToClient(QDomDocument &doc, bool sign = true);
        void sendToAll(QDomDocument &doc, bool sign = true);
        
        void setData(int key, const QVariant &value);
        QVariant data(int key) const;
        
        Client *client() const;
        TcpServer *server() const;
        
        void setUser(Users::User *user);
        Users::User *user() const;
        
        void generateSign();
        
        void signPackage(QDomDocument &doc);
        
        QString sign() const;
        
        void setValid(bool v);
        bool isValid() const;
        
    public slots:
        void close();
        void sendErrorPackageToClient(const QString & message, Packages::Error::Level level);
        
    private slots:
        void removeConnection();
        
    signals:
        void error(QTcpSocket::SocketError socketError);
        void requestSendToAll(const QString &msg);
        void connectionClosed( Server::Connection *cnn);
        void packageReaded(Server::Connection *cnn, const QString& root,const QString & packages  );
        
    private:
        class Private;
        Private *const k;
};

}

#endif
