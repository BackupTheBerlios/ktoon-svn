/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosxx@gmail.com                                                   *
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
 
#include "packagehandlerbase.h"
#include "connection.h"

#include <kcore/kdebug.h>
#include <kcore/kapplicationproperties.h>

#include <QHashIterator>
#include <QHash>

#include "packages/error.h"
#include "users/user.h"
#include "base/settings.h"
#include "base/logger.h"
#include "base/package.h"
#include "bans/banmanager.h"
#include "backups/backupmanager.h"

namespace Server {

struct PackageHandlerBase::Private
{
};

PackageHandlerBase::PackageHandlerBase() : k(new Private())
{
}

PackageHandlerBase::~PackageHandlerBase()
{
    delete k;
}

void PackageHandlerBase::handlePackage(Base::Package *const pkg)
{
    Server::Connection *cnn = pkg->source();
    QString root = pkg->root();
    QString package = pkg->xml();
    
    kWarning("server") << "PACKAGE: " << package;
    
    TcpServer *server = cnn->server();
    if (!pkg->accepted())
        handle(cnn, root, package);
}

}
