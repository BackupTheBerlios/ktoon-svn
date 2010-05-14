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
#include <backups/backuplist.h>

class TestBackupList: public QObject
{
    Q_OBJECT
    private slots:
        void createBackupList();
};

void TestBackupList::createBackupList()
{
    QDomDocument result;
    result.setContent(QString("<backuplist><entry name=\"proyecto 1\" ><backup date=\"2006-02-05T12:22:56\" /></entry></backuplist>"));
    
    Packages::BackupList backuplist;
    backuplist.addEntry("proyecto 1", QStringList() << "2006-02-05T12:22:56");
    
    QCOMPARE(result.toString(0), backuplist.toString(0));
}

QTEST_MAIN(TestBackupList)
#include "testbackuplist.moc"

