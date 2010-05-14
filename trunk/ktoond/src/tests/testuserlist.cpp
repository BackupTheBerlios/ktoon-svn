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

#include <QtTest/QtTest>
#include <QDomDocument>
#include <userlist.h>

#include "users/user.h"

class TestUserList: public QObject
{
    Q_OBJECT
    private slots:
        void createUserList();
};

void TestUserList::createUserList()
{
    QDomDocument result;
    result.setContent(QString("<userlist version=\"0\"><user login=\"dcuadra\" name=\"David Cuadrado\" /></userlist>"));
    
    Users::User user;
    user.setName("David Cuadrado");
    user.setLogin("dcuadra");
    
    Packages::UserList userlist;
    userlist.addUser(&user);
    
    QCOMPARE(result.toString(0), userlist.toString(0));
}

QTEST_MAIN(TestUserList)
#include "testuserlist.moc"

