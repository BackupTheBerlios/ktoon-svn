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
#include <newprojectparser.h>
#include <QStringList>

class TestNewProjectParser: public QObject
{
    Q_OBJECT
    private slots:
        void parse_data();
        void parse();
};

void TestNewProjectParser::parse_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("result");
    Parsers::NewProjectParser parser;
    parser.parse("<newproject><author>Animaciones S.A.</author><name>proyecto 1</name><description>proyecto de prueba</description></newproject>");

    QTest::newRow("name") << parser.name() << "proyecto 1";
    QTest::newRow("author") << parser.author() << "Animaciones S.A.";
    QTest::newRow("description") << parser.description() << "proyecto de prueba";
}

void TestNewProjectParser::parse()
{
    QFETCH(QString, string);
    QFETCH(QString, result);
    
    QCOMPARE(string, result);
}

QTEST_MAIN(TestNewProjectParser)
#include "testnewprojectparser.moc"
