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

#ifndef REGISTERSDATABASE_H
#define REGISTERSDATABASE_H

#include <QString>
#include <QHash>
#include <ktxmlparserbase.h>

namespace Registers {

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class Database : public KTXmlParserBase
{
    public:
        Database(const QString &dbfile);
        ~Database();
        
        QString fileName() const;
        void removeRegister(const QString &email);
        
        bool startTag(const QString &tag, const QXmlAttributes &atts);
        bool endTag(const QString &tag);
        void text(const QString &msg);
        
        QHash<QString, QString> findRegisterByEmail(const QString &email);
        
    private:
        struct Private;
        Private *const k;
};

}

#endif
