#include <QtTest/QtTest>
#include <QDomDocument>
#include <../packages/connect.h>

class TestConnect: public QObject
{
	Q_OBJECT
	private slots:
		void createConnect();
};

void TestConnect::createConnect()
{
	QDomDocument result;
	result.setContent(QString("<connect version=\"0\" ><client type=\"1\" /><login>dcuadra</login><password>d41d8cd98f00b204e9800998ecf8427e</password></connect>"));
	
	Packages::Connect connect("dcuadra", "");
	
	QCOMPARE( result.toString(0), connect.toString(0));
}

QTEST_MAIN(TestConnect)
#include "testconnect.moc"

