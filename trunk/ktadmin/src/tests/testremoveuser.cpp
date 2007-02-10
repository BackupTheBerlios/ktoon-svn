#include <QtTest/QtTest>
#include <QDomDocument>
#include <../packages/removeuser.h>

class TestRemoveUser: public QObject
{
	Q_OBJECT
	private slots:
		void createRemoveUser();
};

void TestRemoveUser::createRemoveUser()
{
	QDomDocument result;
	result.setContent(QString("<removeuser version=\"0\"><login>dcuadra</login></removeuser>"));
	
	
	Packages::RemoveUser removeuser("dcuadra");
	
	QCOMPARE( result.toString(0), removeuser.toString(0));
}

QTEST_MAIN(TestRemoveUser)
#include "testremoveuser.moc"

