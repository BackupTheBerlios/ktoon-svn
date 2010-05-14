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

#include "useractionparser.h"
#include "users/user.h"
#include "users/right.h"

namespace Parsers {

struct UserActionParser::Private
{
    Users::User user;
};

UserActionParser::UserActionParser() : KTXmlParserBase(), k(new Private)
{
}

UserActionParser::~UserActionParser()
{
}

bool UserActionParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
    if (root() == "adduser" || root() == "removeuser" || root() == "updateuser" || root() == "queryuser") {
        if (tag == "login") {
            setReadText(true);
        } else if(tag == "password") {
            setReadText(true);
        } else if(tag == "name") {
            setReadText(true);
        } else if(tag == "perm") {
            Users::Right *right = new Users::Right(atts.value("module"),
            bool(atts.value("read").toInt()),
            bool(atts.value("write").toInt()));
            
            k->user.addRight(right);
        }
    }

    return true;
}

bool UserActionParser::endTag(const QString &)
{
    return true;
}

void UserActionParser::text(const QString &text)
{
    if (currentTag() == "login") {
        k->user.setLogin(text);
    } else if(currentTag() == "password") {
        k->user.setPassword(text);
    } else if(currentTag() == "name") {
        k->user.setName(text);
    }
}

Users::User UserActionParser::user() const
{
    return k->user;
}

}
