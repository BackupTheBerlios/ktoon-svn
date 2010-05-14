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
#include <projectlist.h>

class TestProjectList: public QObject
{
    Q_OBJECT
    private slots:
        void createProjectList();
};

void TestProjectList::createProjectList()
{
    QDomDocument result;
    result.setContent(QString("<projectlist version=\"0\"><project description=\"animaciones de prueba\" author=\"Animaciones S.A.\" name=\"proyecto 1\"/></projectlist>"));
    
    Packages::ProjectList projectlist;
    projectlist.addProject("proyecto 1", "Animaciones S.A.", "animacion de prueba");
    
    QCOMPARE( result.toString(0), projectlist.toString(0));
}

QTEST_MAIN(TestProjectList)
#include "testprojectlist.moc"
