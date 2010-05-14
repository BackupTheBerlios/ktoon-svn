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

#include "connection.h"

#include <QtNetwork>

#include <kcore/kdebug.h>
#include <kcore/kmd5hash.h>
#include <kcore/kalgorithm.h>

#include "server.h"
#include "base/logger.h"

#include "ktrequestparser.h"
#include "ktcompress.h"
#include "ktprojectrequest.h"
#include "ktprojectresponse.h"
#include "ktrequestparser.h"

#include "users/user.h"
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

Connection::Connection(int socketDescriptor, Server::TcpServer *server) : QThread(server), k(new Private(server))
{
    KINIT;
    k->client = new Server::Client(this);
    k->client->setSocketDescriptor(socketDescriptor);
    k->isValid = true;
}

Connection::~Connection()
{
    KEND;
    delete k;
}

void Connection::run()
{
    while (k->client->state() != QAbstractSocket::UnconnectedState) {
        
           if (k->readed.isEmpty() || !k->isValid) 
               continue;
        
           if (!k->user)
               k->isValid = false;
        
           QString readed = k->readed.dequeue();
        
           kDebug("server") << "Reicieved: " << readed;
           QDomDocument doc;

           if (doc.setContent(readed.trimmed()))
               emit packageReaded(this, doc.documentElement().tagName(), readed);
           else
               kError("server") << "Cannot set document content!";
    }

    removeConnection();
}

void Connection::removeConnection()
{
    emit connectionClosed(this);
}

void Connection::close()
{
    k->isValid = false;
    
    k->readed.clear();

    if (k->client->state() != QAbstractSocket::UnconnectedState) {
        k->client->flush();
        
        k->client->disconnectFromHost();
        k->client->waitForDisconnected();
        k->client->close();
    }
}

void Connection::appendTextReaded(const QString &readed)
{
    kDebug("server") << "Enqueing: " << readed;
    k->readed.enqueue(readed);
}

void Connection::sendToClient(const QString &text) const
{
    k->client->send(text);
}

void Connection::setData(int key, const QVariant &value)
{
    k->datas.insert(key, value);
}

QVariant Connection::data(int key) const
{
    return k->datas[key];
}

Client *Connection::client() const
{
    return k->client;
}

TcpServer *Connection::server() const
{
    return k->server;
}

void Connection::sendToAll(const QString &text)
{
    emit requestSendToAll(text);
}

void Connection::sendToClient(QDomDocument &doc, bool sign)
{
    if (sign)
        signPackage(doc);
    
    kDebug() << "sending " << doc.toString();
    
    k->client->send(doc);
}

void Connection::sendToAll(QDomDocument &doc, bool sign)
{
    if (sign)
        signPackage(doc);

    emit requestSendToAll(doc.toString(0));
}

void Connection::signPackage(QDomDocument &doc)
{
    doc.documentElement().setAttribute("sign", k->sign);
}

QString Connection::sign() const
{
    return k->sign;
}

void Connection::setUser(Users::User *user)
{
    k->user = user;
    generateSign();
    
    k->isValid = true;
}

Users::User *Connection::user() const
{
    return k->user;
}

void Connection::generateSign()
{
    if (k->user)
        k->sign = KMD5Hash::hash(k->user->login() + k->user->password() + KAlgorithm::randomString(KAlgorithm::random() % 10));
}

void Connection::sendErrorPackageToClient(const QString & message, Packages::Error::Level level)
{
    Packages::Error error(message, level);
    sendToClient(error);
}

void Connection::setValid(bool v)
{
    k->isValid = v;
}

bool Connection::isValid() const
{
    return k->isValid;
}

}
