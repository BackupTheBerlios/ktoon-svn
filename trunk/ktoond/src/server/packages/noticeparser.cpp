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

#include "noticeparser.h"

namespace Parsers {

struct NoticeParser::Private
{
    QString message;
};

NoticeParser::NoticeParser() : KTXmlParserBase(), k(new Private())
{
}

NoticeParser::~NoticeParser()
{
}

bool NoticeParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
    if (root() == "notice") {
        if (tag == "message")
            k->message = atts.value("text");
    }
    
    return true;
}

bool NoticeParser::endTag(const QString &)
{
    return true;
}

void NoticeParser::text(const QString &)
{
}

QString NoticeParser::message() const
{
    return k->message;
}

}
