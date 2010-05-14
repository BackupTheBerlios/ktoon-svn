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

#include "comunicationmanager.h"
//base
#include "base/package.h"
#include "base/settings.h"
#include "base/logger.h"

//Qt
#include <QDomDocument>

//server/core
#include "core/server.h"
#include "core/connection.h"

//server/users
#include "users/user.h"

namespace Comunications {
    
Manager::Manager() : Base::Observer()
{
}

Manager::~Manager()
{
}

void Manager::handlePackage(Base::Package *const pkg)
{
    Server::Connection *cnn = pkg->source();
    // Server::TcpServer *server = cnn->server();
    
    if (pkg->root() == "chat") {

        QDomDocument doc;
        doc.setContent(pkg->xml());
        
        QDomElement element = doc.firstChild().firstChildElement("message");
        element.setAttribute("from", cnn->user()->login());
        cnn->sendToAll(doc);
        pkg->accept();

    } else if (pkg->root() == "notice") {

        QDomDocument doc;
        doc.setContent(pkg->xml());
        
        QDomElement element = doc.firstChild().firstChildElement("message");
        element.setAttribute("from", cnn->user()->login());
        cnn->sendToAll(doc);//TODO: enviar a todos los clientes del proyecto
        pkg->accept();

    } else if (pkg->root() == "wall") {

        QDomDocument doc;
        doc.setContent(pkg->xml());
        QDomElement element = doc.firstChild().firstChildElement("message");
        element.setAttribute("from", cnn->user()->login());
        cnn->sendToAll(doc);
        pkg->accept();

    }
}

}

