/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include "user.h"
#include "right.h"

#include <QList>
#include <QDomDocument>

namespace Users {

struct User::Private
{
    QString name;
    QString login;
    QString password;
    
    QList<Right *> rights;
};

User::User() : k(new Private)
{
}

User::~User()
{
    qDeleteAll(k->rights);
    delete k;
}

void User::setName(const QString &name)
{
    k->name = name;
}

void User::setLogin(const QString &login)
{
    k->login = login;
}

void User::setPassword(const QString &password)
{
    k->password = password;
}

QString User::name() const
{
    return k->name;
}

QString User::login() const
{
    return k->login;
}

QString User::password() const
{
    return k->password;
}

bool User::canReadOn(const QString &module)
{
    foreach (Right *right, k->rights) {
             if (right->module() == module)
                 return right->read();
    }
    
    return false;
}

bool User::canWriteOn(const QString &module)
{
    foreach (Right *right, k->rights) {
             if (right->module() == module)
        	 return right->write();
    }
    
    return false;
}

void User::addRight(Right *right)
{
    k->rights << right;
}

QList<Right *> User::rights() const
{
    return k->rights;
}

QDomElement User::toXml(QDomDocument &doc, bool password) const
{
    QDomElement userE = doc.createElement("user");
    
    userE.appendChild(doc.createElement("login")).appendChild( doc.createTextNode(k->login));
    userE.appendChild(doc.createElement("name")).appendChild( doc.createTextNode(k->name));
    
    if (password)
        userE.appendChild(doc.createElement("password")).appendChild(doc.createTextNode(k->password));
    
    QDomElement permissionsE = doc.createElement("permissions");
    userE.appendChild(permissionsE);

    foreach (Right *right, k->rights) {
             QDomElement perm = doc.createElement("perm");
             perm.setAttribute("module", right->module());
             perm.setAttribute("read", right->read());
             perm.setAttribute("write", right->write());
        
             permissionsE.appendChild(perm);
    }

    return userE;
}

bool User::operator==(const User& user)
{
    return (k->login == user.k->login) && (k->password == user.k->password) && (k->name == user.k->name);
}

}
