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
 
#include "projectactionparser.h"

namespace Parsers {

struct ProjectActionParser::Private
{
    QString name;
    QString author;
    QString description;
    QMultiHash<int, QString> users;
    int type;
};

ProjectActionParser::ProjectActionParser() : KTXmlParserBase(), k(new Private())
{
}

ProjectActionParser::~ProjectActionParser()
{
}

bool ProjectActionParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
    if (root() == "addproject" || root() == "removeproject" || 
        root() == "updateproject" || root() == "queryproject") {

        if (tag == "name" || tag == "author" || tag == "description") {
            setReadText(true);
        } else if (tag == "user") {
                   k->type = atts.value("type").toInt();
                   setReadText(true);
        }

    }

    return true;
}

bool ProjectActionParser::endTag(const QString &)
{
    return true;
}

void ProjectActionParser::text(const QString &text)
{
    if (currentTag() == "user") {
        k->users.insert(k->type, text);
    } else if (currentTag() == "name") {
               k->name = text;
    } else if (currentTag() == "author") {
               k->author = text;
    } else if (currentTag() == "description") {
               k->description = text;
    }
}

QString ProjectActionParser::name()
{
    return k->name;
}

QString ProjectActionParser::author()
{
    return k->author;
}

QString ProjectActionParser::description()
{
    return k->description;
}

QMultiHash<int, QString> ProjectActionParser::users()
{
    return k->users;
}

}
