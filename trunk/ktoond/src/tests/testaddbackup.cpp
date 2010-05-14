#include <QtTest/QtTest>
#include <QDomDocument>

#include <backups/addbackup.h>

class TestAddBackup: public QObject
{
    Q_OBJECT
    private slots:
        void createAddBackup();
};

void TestAddBackup::createAddBackup()
{
    QDomDocument result;
    result.setContent(QString("<addbackup> <entry date=\"2006-02-05T12:22:56\">proyecto 1</entry></addbackup>"));
    
    Packages::AddBackup addbackup;
    addbackup.addEntry("proyecto 1", QDateTime::fromString("2006-02-05T12:22:56", Qt::ISODate));
    
    QCOMPARE( result.toString(0), addbackup.toString(0));
}

QTEST_MAIN(TestAddBackup)
#include "testaddbackup.moc"


