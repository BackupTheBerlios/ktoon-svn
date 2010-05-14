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

#include "server.h"
#include "connection.h"

#include <QHostInfo>
#include <QTimer>
#include <QQueue>
#include <QHostAddress>

#include <kcore/kdebug.h>

#include "packagehandlerbase.h"
#include "defaultpackagehandler.h"

#include "bans/banmanager.h"
#include "backups/backupmanager.h"
#include "users/manager.h"
#include "comunications/comunicationmanager.h"

#include "base/settings.h"
#include "base/package.h"
#include "base/observer.h"
#include "base/logger.h"

namespace Server{

class TcpServer::Private
{
    public:
        QList<Server::Connection *> connections;
        QList<Server::Connection *> admins;
        
        Backups::Manager *backupManager;
        Bans::Manager *banManager;
        Users::Manager *userManager;
        Comunications::Manager *comunicationManager;
        
        QList<Base::Observer *> observers;
};

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent), k(new Private)
{
    KINIT;
    k->backupManager = new Backups::Manager;
    k->observers << k->backupManager;
    
    k->banManager = new Bans::Manager;
    k->observers << k->banManager;
    
    k->userManager = new Users::Manager;
    k->observers << k->userManager;
    
    k->comunicationManager = new Comunications::Manager;
    k->observers << k->comunicationManager;
}

TcpServer::~TcpServer()
{
    KEND;
    
    Base::Logger::self()->info("Server finished");
    
    delete Base::Settings::self();
    delete Base::Logger::self();
    
    qDeleteAll(k->observers);
    
    delete k;
}

bool TcpServer::openConnection(const QString &host, int port)
{
    Base::Logger::self()->info(QObject::tr("Initialized server on %1:%2").arg(host).arg(port));
    
    QList<QHostAddress> addrs = QHostInfo::fromName(host).addresses();

    if (!addrs.isEmpty()) {
        if (! listen(QHostAddress(addrs[0]), port)) {
            kError() << "Can't connect to " << host << ":" << port << " error was: " << errorString();
            return false;
        }
    } else {
        kError() << "Error while try to resolve " << host;
        return false;
    }

    return true;
}

void TcpServer::addAdmin(Server::Connection *cnx)
{
    k->admins << cnx;
}

Backups::Manager *TcpServer::backupManager() const
{
    return k->backupManager;
}

Bans::Manager *TcpServer::banManager() const
{
    return k->banManager;
}

Users::Manager *TcpServer::userManager() const
{
    return k->userManager;
}

void TcpServer::addObserver(Base::Observer *observer)
{
    k->observers << observer;
}

bool TcpServer::removeObserver(Base::Observer *observer)
{
    return k->observers.removeAll(observer) > 0;
}

void TcpServer::incomingConnection(int socketDescriptor)
{
    SHOW_VAR(k->connections.count());
    
    Server::Connection *newConnection = new Server::Connection(socketDescriptor,this);
    
    QString ip = newConnection->client()->peerAddress().toString();
    
    k->banManager->initialize(ip);
    
    if (!k->banManager->isBanned(ip)) {
        k->banManager->unban(ip);
        
        handle(newConnection);
        k->connections << newConnection;
        newConnection->start();
    } else {
        newConnection->sendErrorPackageToClient(tr("You're banned, Please contact to server administrator if you think is an error!"), Packages::Error::Err);
        newConnection->close();
    }
}

void TcpServer::handle(Server::Connection *cnx)
{
    connect(cnx, SIGNAL(finished()), cnx, SLOT(deleteLater()));
    connect(cnx, SIGNAL(requestSendToAll(const QString& )), this, SLOT(sendToAll(const QString&)));
    
    connect(cnx, SIGNAL(packageReaded(Server::Connection*, const QString&, const QString&)), this, SLOT(handlePackage(Server::Connection*, const QString&, const QString&)));
    
    connect(cnx, SIGNAL(connectionClosed(Server::Connection*)), this, SLOT(removeConnection(Server::Connection*)));
    
}

void TcpServer::sendToAll(const QString &msg)
{
    foreach (Server::Connection *connection, k->connections)
             connection->sendToClient(msg);
}

void TcpServer::sendToAll(const QDomDocument &pkg)
{
    K_FUNCINFO;
    sendToAll(pkg.toString(0));
}

void TcpServer::sendToAdmins(const QString &str)
{
    foreach (Server::Connection *cnn, k->admins)
             cnn->sendToClient(str);
}

void TcpServer::removeConnection(Server::Connection *cnx)
{
    K_FUNCINFO;
    k->connections.removeAll(cnx);
    k->admins.removeAll(cnx);
    
    cnx->blockSignals(true);
    cnx->close();
    cnx->blockSignals(false);
    
    foreach (Base::Observer *observer, k->observers)
             observer->connectionClosed(cnx);
    
    if (!cnx->isRunning()) {
        delete cnx;
        cnx = 0;
    }
}

void TcpServer::handlePackage(Server::Connection* client, const QString &root, const QString&package)
{
    Base::Package *pkg = new Base::Package(root, package, client);
    
    foreach (Base::Observer *observer, k->observers) {
             observer->handlePackage(pkg);
        
             if (pkg->accepted())
                 break;
    }
    
    delete pkg;
}

}
