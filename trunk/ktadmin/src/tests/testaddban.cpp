#include <QtTest/QtTest>
#include <QDomDocument>
#include <../packages/addban.h>

class TestAddBan: public QObject
{
	Q_OBJECT
	private slots:
		void createAddBan();
};

void TestAddBan::createAddBan()
{
	QDomDocument result;
	result.setContent(QString("<addban version=\"0\"><pattern value=\"127.0.0.1\" /></addban>"));
	
	
	Packages::AddBan addban("127.0.0.1");
	
	QCOMPARE( result.toString(0), addban.toString(0));
}

QTEST_MAIN(TestAddBan)
#include "testaddban.moc"

