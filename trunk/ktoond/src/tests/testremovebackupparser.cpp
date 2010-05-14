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
#include <backups/removebackupparser.h>
#include <QHash>
#include <QDateTime>

class TestRemoveBackupParser: public QObject
{
    Q_OBJECT
    private slots:
        void parse();
};

void TestRemoveBackupParser::parse()
{
    QHash<QString, QDateTime > result;
    result.insert("proyecto1", QDateTime::fromString("2006-02-05T12:22:56",Qt::ISODate ));
    Parsers::RemoveBackupParser parser;
    parser.parse("<removebackup> <entry date=\"2006-02-05T12:22:56\" >proyecto1</entry> </removebackup>");
    QCOMPARE(parser.entries(), result);
}

QTEST_MAIN(TestRemoveBackupParser)
#include "testremovebackupparser.moc"
