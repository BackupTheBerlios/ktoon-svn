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

#include "manager.h"

#include <QFile>

#include <kcore/kmd5hash.h>
#include <kcore/kdebug.h>

#include "actionregisteruserparser.h"
#include "database.h"
#include "adduser.h"

#include "base/package.h"
#include "base/settings.h"

#include "core/connection.h"
#include "core/server.h"

#include "users/manager.h"
#include "users/user.h"

namespace Registers {

struct Manager::Private
{
    Database *db;
};

Manager::Manager() : Base::Observer(),k(new Private)
{
    k->db = new Database(Base::Settings::self()->databaseDirPath() + "/petitions.xml");
}

Manager::~Manager()
{
    delete k->db;
    delete k;
}

void Manager::handlePackage(Base::Package* const pkg)
{
    Server::TcpServer *server = pkg->source()->server();
    
    if (pkg->root() == "listregisters") {
        QString fname = k->db->fileName();
        
        QFile f(fname);
        if (f.exists()) {
            if (f.open(QIODevice::ReadOnly | QIODevice::Text))
                pkg->source()->sendToClient(f.readAll());
        }
    } else if (pkg->root() == "registeruser") {
               Parsers::ActionRegisterUserParser parser;
        
               if (parser.parse(pkg->xml())) {
                   QHash<QString, QString> data = parser.data();
                   QString email = data["email"];
            
                   data = k->db->findRegisterByEmail(email);
                   k->db->removeRegister(email);
            
                   kDebug() << data["login"];
            
                   Users::Manager *manager = server->userManager();
            
                   Users::User user;
                   user.setLogin(data["login"]);
                   user.setName(data["name"]);
                   user.setPassword(KMD5Hash::hash(""));
                   manager->addUser(user);
            
                   server->sendToAdmins(pkg->xml());
                   Packages::AddUser adduser(data["login"], data["name"]);
                   server->sendToAdmins(adduser.toString());
               }
    }
}

}

