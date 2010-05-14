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

