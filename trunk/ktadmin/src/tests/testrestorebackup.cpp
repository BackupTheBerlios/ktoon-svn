#include <QtTest/QtTest>
#include <QDomDocument>
#include <../packages/restorebackup.h>
#include <QDateTime>

class TestRestoreBackup: public QObject
{
	Q_OBJECT
	private slots:
		void createRestoreBackup();
};

void TestRestoreBackup::createRestoreBackup()
{
	QDomDocument result;
	result.setContent(QString("<restorebackup version=\"0\"><entry date=\"2007-02-05T12:22:56\" >Proyecto 1</entry></restorebackup>"));
	
	
	Packages::RestoreBackup restorebackup;
	restorebackup.addEntry("Proyecto 1", QDateTime::fromString("2007-02-05T12:22:56", Qt::ISODate));
	QCOMPARE( result.toString(0), restorebackup.toString(0));
}

QTEST_MAIN(TestRestoreBackup)
#include "testrestorebackup.moc"

