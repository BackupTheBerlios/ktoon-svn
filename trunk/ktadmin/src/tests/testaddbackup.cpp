#include <QtTest/QtTest>
#include <QDomDocument>
#include <packages/addbackup.h>

class TestAddBackup: public QObject
{
	Q_OBJECT
	private slots:
		void createAddBackup();
};

void TestAddBackup::createAddBackup()
{
	QDomDocument result;
	result.setContent(QString("<addbackup version=\"0\"><entry>proyecto1</entry></addbackup>"));
	
	
	Packages::AddBackup addbackup;
	addbackup.addEntry("proyecto1");
	QCOMPARE( result.toString(0), addbackup.toString(0));
}

QTEST_MAIN(TestAddBackup)
#include "testaddbackup.moc"

