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

#include "newprojectparser.h"

namespace Parsers {

struct NewProjectParser::Private
{
    QString author;
    QString name;
    QString description;
};

NewProjectParser::NewProjectParser() : KTXmlParserBase(), k(new Private())
{
}

NewProjectParser::~NewProjectParser()
{
    delete k;
}

bool NewProjectParser::startTag(const QString &tag, const QXmlAttributes &)
{
    if (root() == "newproject") {
        if (tag == "author") {
            setReadText(true);
        } else if (tag == "name") {
            setReadText(true);
        } else if (tag == "description") {
            setReadText(true);
        }
    }
    
    return true;
}

bool NewProjectParser::endTag(const QString &)
{
    return true;
}

void NewProjectParser::text(const QString &text)
{
    if (currentTag() == "name") {
        k->name = text;
    } else if (currentTag() == "author") {
        k->author = text;
    } else if (currentTag() == "description") {
        k->description = text;
    }
}

QString NewProjectParser::author() const
{
    return k->author;
}

QString NewProjectParser::name() const
{
    return k->name;
}

QString NewProjectParser::description() const
{
    return k->description;
}

}
