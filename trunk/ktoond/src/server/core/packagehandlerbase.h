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

#ifndef ABSTRACTHANDLERPACKAGES_H
#define ABSTRACTHANDLERPACKAGES_H

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
*/

#include <QDomDocument>

#include "base/observer.h"
#include "server.h"

namespace Server {

class Connection;

class PackageHandlerBase : public Base::Observer
{
    public:
        PackageHandlerBase();
        
        virtual ~PackageHandlerBase();
        void handlePackage(Base::Package *const pkg);
        virtual void handle(Server::Connection *client, const QString &root, const QString &package ) = 0;
        virtual void connectionClosed(Server::Connection *client) = 0;
        
    private:
        struct Private;
        Private *const k;
};

}

#endif
