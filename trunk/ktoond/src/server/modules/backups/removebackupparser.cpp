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
