#include <QtTest/QtTest>
#include <QDomDocument>
#include <../packages/queryuser.h>

class TestQueryUser: public QObject
{
	Q_OBJECT
	private slots:
		void createQueryUser();
};

void TestQueryUser::createQueryUser()
{
	QDomDocument result;
	result.setContent(QString("<queryuser version=\"0\"><login>dcuadra</login></queryuser>"));
	
	
	Packages::QueryUser queryuser("dcuadra");
	
	QCOMPARE( result.toString(0), queryuser.toString(0));
}

QTEST_MAIN(TestQueryUser)
#include "testqueryuser.moc"

