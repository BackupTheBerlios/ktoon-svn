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

#include "banmanager.h"

#include <QHash>
#include <QString>
#include <QApplication>

#include <kcore/kdebug.h>

#include "base/package.h"
#include "core/connection.h"
#include "core/server.h"
#include "users/user.h"

#include "banlist.h"
#include "removebanparser.h"
#include "addbanparser.h"

namespace Bans {

struct Manager::Private
{
    int maxFails;
    QHash<QString, int> fails;
};

Manager::Manager() : k(new Private)
{
    k->maxFails = 10;
}

Manager::~Manager()
{
    delete k;
}

void Manager::initialize(const QString &pt)
{
    if (!k->fails.contains(pt))
        k->fails.insert(pt, 0);
}

bool Manager::isBanned(const QString &pt) const
{
    if (k->fails.contains(pt)) {
        if (k->fails[pt] >= k->maxFails)
            return true;
    }

    return false;
}

void Manager::failed(const QString &pt)
{
    k->fails[pt] += 1;
}

void Manager::ban(const QString &pt)
{
    k->fails[pt] = k->maxFails;
}

void Manager::unban(const QString &pt)
{
    k->fails[pt] = 0;
}

QStringList Manager::allBanned() const
{
    QStringList allBanned;
    
    QHash<QString, int>::const_iterator i = k->fails.constBegin();
    
    while (i != k->fails.constEnd()) {
           if (isBanned(i.key()))
               allBanned << i.key();
           ++i;
    }
    
    return allBanned;
}

void Manager::handlePackage(Base::Package *const pkg)
{
    QString root = pkg->root();
    QString package = pkg->xml();
    Server::Connection *cnn = pkg->source();
    Server::TcpServer *server = cnn->server();
    
    pkg->accept();
    
    if (root == "listbans") {

        if (cnn->user()->canReadOn("admin")) {
            QStringList bans = server->banManager()->allBanned();
            
            Packages::BanList pkg;
            pkg.setBans(bans);
            
            cnn->sendToClient(pkg, true);
        } else {
            cnn->sendErrorPackageToClient(QObject::tr("Permission denied."), Packages::Error::Err);
        }

    } else if (root == "removeban") {

        if (cnn->user()->canWriteOn("admin")) {
            Parsers::RemoveBanParser parser;
            if (parser.parse(package)) {
                server->banManager()->unban(parser.pattern());
                server->sendToAdmins(package);
            }
        } else {
            cnn->sendErrorPackageToClient(QObject::tr("Permission denied."), Packages::Error::Err);
        }
    } else if (root == "addban") {
               if (cnn->user()->canWriteOn("admin")) {
                   Parsers::AddBanParser parser;
                   if (parser.parse(package)) {
                       server->banManager()->ban(parser.pattern());
                       server->sendToAdmins(package);
                   }
               } else {
                   cnn->sendErrorPackageToClient(QObject::tr("Permission denied."), Packages::Error::Err);
               }
    } else {
        pkg->ignore();
    }
}

}
