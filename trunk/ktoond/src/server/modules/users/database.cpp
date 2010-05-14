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

#include "database.h"
#include <QDomDocument>
#include <QFile>
#include <QTextStream>

#include <kcore/kdebug.h>

namespace Users {

struct Database::Private
{
    QString dbfile;
};

Database::Database(const QString &dbfile) : k(new Private)
{
    k->dbfile = dbfile;
    
    if (!QFile::exists(k->dbfile)) {
        Users::User user;
        user.setLogin("admin");
        user.setName(QObject::tr("System administrator"));
        user.setPassword("d41d8cd98f00b204e9800998ecf8427e");
        
        user.addRight(new Right("admin", true, true));
        user.addRight(new Right("project", true, true));
        
        addUser(user);
    }
}


Database::~Database()
{
    delete k;
}

QDomDocument Database::loadDataBase()
{
    kDebug() << "loading database from " << k->dbfile;
    QFile file(k->dbfile);
    QDomDocument document;

    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream ts(&file);
            document.appendChild(document.createElement("projects"));
            
            ts << document.toString();
            file.close();
        }
    }
    
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (!document.setContent(file.readAll()))
            kWarning() << file.fileName() << " is corrupted!";

        file.close();
    }

    return document;
}

bool Database::addUser(const Users::User &user)
{
    if (user.login().isEmpty())
        return false;
    
    QDomDocument db = loadDataBase();
    QDomElement root = db.documentElement();
    
    root.appendChild(user.toXml(db));
    
    QFile file(k->dbfile);
    
    if (file.open(QIODevice::WriteOnly| QIODevice::Text)) {
        QTextStream ts(&file);
        ts << db.toString();
        file.close();
        return true;
    }
    
    return false;
}

bool Database::updateUser(const Users::User &user)
{
    QDomDocument doc = loadDataBase();
    
    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChild();

    while (!n.isNull()) {
           QDomElement e = n.toElement(); 
           if (!e.isNull()) {
               if (e.attribute("login") == user.login()) {
                   docElem.removeChild(n);
                   docElem.appendChild(user.toXml(doc));
               }
           }
           n = n.nextSibling();
    }
    
    QFile file(k->dbfile);
    
    if (file.open(QIODevice::WriteOnly| QIODevice::Text)) {
        QTextStream ts(&file);
        ts << doc.toString();
        file.close();
        
        return true;
    }
    
    return false;
}

bool Database::removeUser(const QString &login)
{
    QDomDocument doc = loadDataBase();
    
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    while (!n.isNull()) {
           QDomElement e = n.toElement(); 

           if (!e.isNull()) {
               if (e.attribute("login") == login)
                docElem.removeChild(n);
           }
           n = n.nextSibling();
    }

    QFile file(k->dbfile);
    
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream ts(&file);
        ts << doc.toString();
        file.close();

        return true;
    }
    
    return false;
}

}
