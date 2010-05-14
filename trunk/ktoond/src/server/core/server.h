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

#ifndef _SERVER_H__
#define _SERVER_H__

#include <QStringList>
#include <QTcpServer>
#include <QDomDocument>

namespace Server {
class Connection;
class Client;
class PackageHandlerBase;
}

namespace Base {
    class Observer;
}

namespace Backups {
    class Manager;
}

namespace Bans {
    class Manager;
}

namespace Users {
    class Manager;
}

namespace Server {

/**
 * Esta es la clase controladora, esta clase representa el servidor.
 * @author David Cuadrado \<krawek@gmail.com\>
 */
class TcpServer : public QTcpServer
{
    Q_OBJECT;
    
    public:
        TcpServer(QObject *parent = 0);
        ~TcpServer();
        void sendToAll(const QDomDocument &pkg);
        bool openConnection(const QString &host, int port);
        
        void sendToAdmins(const QString &str);
        
        void addAdmin(Server::Connection *cnx);
        Backups::Manager *backupManager() const;
        Bans::Manager *banManager() const;
        Users::Manager *userManager() const;
        
        void addObserver(Base::Observer *observer);
        bool removeObserver(Base::Observer *observer);
        
    public slots:
        void sendToAll(const QString &msg);
        void removeConnection(Server::Connection *cnx);
        void handlePackage( Server::Connection *client, const QString &root, const QString & packages  );
    
    private:
        void handle(Server::Connection *cnx);
        
    protected:
        void incomingConnection(int socketDescriptor);
        
    private:
        class Private;
        Private * const k;
};

}

#endif
