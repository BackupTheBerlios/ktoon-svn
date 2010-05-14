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

#ifndef __PARSER_H_
#define __PARSER_H_

#include <ktxmlparserbase.h>

namespace Users {

class User;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class Parser : public KTXmlParserBase
{
    public:
        Parser(const QString &dbfile);
        ~Parser();
        
        bool startTag(const QString& tag, const QXmlAttributes& atts);
        bool endTag(const QString& tag);
        
        void text(const QString & ch);
        
    public:
        User *user(const QString &login);
        QList<User *> listUsers();
        
    private:
        struct Private;
        Private *const k;
};

}

#endif
