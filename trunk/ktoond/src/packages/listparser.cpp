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

#include "listparser.h"

namespace Parsers {

struct ListParser::Private
{
    QString pattern;
    int type;
    bool caseSensitive;
    bool regexp;
};


ListParser::ListParser(): KTXmlParserBase(), k(new Private())
{
}

ListParser::~ListParser()
{
}

bool ListParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
    if (root() == "list") {
        if (tag == "options") {
            k->pattern = atts.value("pattern");
            k->type = atts.value("type").toInt();
        } else if (tag == "caseSensitive") {
            k->caseSensitive = bool(atts.value("enabled").toInt());
        } else if (tag == "regexp") {
            k->regexp = bool(atts.value("enabled").toInt());
        }
    }

    return true;
}

bool ListParser::endTag(const QString &tag)
{
    Q_UNUSED(tag);

    return true;
}

void ListParser::text(const QString &text)
{
    Q_UNUSED(text);
}

bool ListParser::isRegexp()
{
    return k->regexp;
}

bool ListParser::isCaseSensitive()
{
    return k->caseSensitive;
}

QString ListParser::pattern()
{
    return k->pattern;
}

int ListParser::type()
{
    return k->type;
}

}
