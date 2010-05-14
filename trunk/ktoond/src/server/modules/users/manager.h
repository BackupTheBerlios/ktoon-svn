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

#ifndef USERMANAGER_H_
#define USERMANAGER_H_

#include <QString>

#include "base/observer.h"

namespace Base {
    class Package;
}

namespace Users {
class User;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class Manager : public Base::Observer
{
    public:
        explicit Manager();
        ~Manager();
        
        bool auth(const QString &login, const QString &password);
        User *user(const QString &login);
        
        bool addUser(const User & user);
        bool updateUser(const User & user);
        bool removeUser(const QString &login);
        
        QList<User*> listUsers();
        
        void handlePackage(Base::Package *const pkg);
        
    private:
        struct Private;
        Private *const k;
};

}

#endif
