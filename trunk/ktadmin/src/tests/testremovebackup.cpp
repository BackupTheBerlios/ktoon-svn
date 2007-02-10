#include <QtTest/QtTest>
#include <QDomDocument>
#include <../packages/removebackup.h>

class TestRemoveBackup: public QObject
{
	Q_OBJECT
	private slots:
		void createRemoveBackup();
};

void TestRemoveBackup::createRemoveBackup()
{
	QDomDocument result;
	result.setContent(QString("<removebackup version=\"0\"><entry date=2007-02-05T12:22:56 >Proyecto 1</entry></removebackup>"));
	
	
	Packages::RemoveBackup removebackup;
	
	QCOMPARE( result.toString(0), removebackup.toString(0));
}

QTEST_MAIN(TestRemoveBackup)
#include "testremovebackup.moc"

