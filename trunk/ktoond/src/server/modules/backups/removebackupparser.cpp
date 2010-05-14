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

#include "removebackupparser.h"

namespace Parsers {

struct RemoveBackupParser::Private
{
    QString currentDate;
    QHash<QString, QDateTime> entries;
};

RemoveBackupParser::RemoveBackupParser() : KTXmlParserBase(), k(new Private)
{
}

RemoveBackupParser::~RemoveBackupParser()
{
    delete k;
}

bool RemoveBackupParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
    if (tag == "removebackup") {
        k->entries.clear();
    } else if (tag == "entry") {
               k->currentDate = atts.value("date");
               setReadText(true);
    }
    
    return true;
}

bool RemoveBackupParser::endTag(const QString &)
{
    return true;
}

void RemoveBackupParser::text(const QString &msg)
{
    k->entries[msg] = QDateTime::fromString(k->currentDate, Qt::ISODate);
}

QHash<QString, QDateTime > RemoveBackupParser::entries() const
{
    return k->entries;
}

}
