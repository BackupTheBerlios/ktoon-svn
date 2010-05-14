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
