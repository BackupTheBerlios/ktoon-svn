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

#ifndef HANDLERPACKGES_H
#define HANDLERPACKGES_H

#include <QString>

#include "core/packagehandlerbase.h"
#include "core/connection.h"

namespace Projects 
{
class ProjectCollection; 
}

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
 * @TODO Remove me when packagehandlerbase is ported to new architecture
 */
class PackageHandler : public Server::PackageHandlerBase
{
    public:
        PackageHandler();
        ~PackageHandler();
        
        void handle(Server::Connection *, const QString &root, const QString &package);
        void connectionClosed(Server::Connection *) {}
        
    private:
        class Private;
        Private *const k;
};

#endif
