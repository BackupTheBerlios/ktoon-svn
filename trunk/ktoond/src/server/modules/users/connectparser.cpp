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

#include "connectparser.h"

namespace Parsers {

class ConnectParser::Private
{
    public:
        QString login;
        QString password;
        int client;
};

ConnectParser::ConnectParser() : KTXmlParserBase(), k(new Private)
{
    k->client = 0;
}

ConnectParser::~ConnectParser()
{
    delete k;
}

bool ConnectParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
    if (root() == "connect") {
        if (tag == "login") {
            setReadText(true);
        } else if (tag == "password") {
                   setReadText(true);
        } else if (tag == "client") {
                   k->client = atts.value("type").toInt();
        }
    }
    
    return true;
}

bool ConnectParser::endTag(const QString &)
{
    return true;
}

void ConnectParser::text(const QString &text)
{
    if (currentTag() == "login") {
        k->login = text;
    } else if (currentTag() == "password") {
               k->password = text;
    }
}

QString ConnectParser::login() const
{
    return k->login;
}

QString ConnectParser::password() const
{
    return k->password;
}

int ConnectParser::client() const
{
    return k->client;
}

}
