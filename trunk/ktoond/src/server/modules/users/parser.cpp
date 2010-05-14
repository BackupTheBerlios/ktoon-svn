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

#include "parser.h"
#include "user.h"

#include <QtDebug>

namespace Users {

struct Parser::Private {
    QString dbfile;
    QList<User *> users;
};

Parser::Parser(const QString &dbfile) : KTXmlParserBase(), k(new Private)
{
    if (!QFile::exists(dbfile))
        qWarning() << "Cannot read users database from: " << dbfile;
    
    k->dbfile = dbfile;
}

Parser::~Parser()
{
    delete k;
}

bool Parser::startTag(const QString& tag, const QXmlAttributes& atts)
{
    if (tag == "user") {
        User *u = new User;
        k->users << u;
    } else if (tag == "login" || tag == "password" || tag == "name") {
               setReadText(true);
    } else if (tag == "perm") {
               Right *right = new Right(atts.value("module"), atts.value("read").toInt(), atts.value("write").toInt());
               k->users.last()->addRight(right);
    }
    
    return true;
}

bool Parser::endTag(const QString& /*tag*/)
{
    return true;
}

void Parser::text(const QString & text)
{
    if (currentTag() == "login") {
        k->users.last()->setLogin(text);
    } else if (currentTag() == "password") {
        k->users.last()->setPassword(text);
    } else if (currentTag() == "name") {
        k->users.last()->setName(text);
    }
}

User *Parser::user(const QString &login)
{
    QFile file(k->dbfile);
    
    if (parse(&file)) {
        foreach (User * user, k->users) {
                 if (user->login() == login) {
                     k->users.clear();
                     return user;
                 }
        }
    }

    return 0;
}

QList<User *> Parser::listUsers()
{
    k->users.clear();

    QFile file(k->dbfile);
    if (parse(&file))
        return k->users;

    return QList<User *>();
}

}
