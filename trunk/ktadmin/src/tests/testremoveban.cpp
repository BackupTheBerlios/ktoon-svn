#include <QtTest/QtTest>
#include <QDomDocument>
#include <../packages/removeban.h>

class TestRemoveBan: public QObject
{
	Q_OBJECT
	private slots:
		void createRemoveBan();
};

void TestRemoveBan::createRemoveBan()
{
	QDomDocument result;
	result.setContent(QString("<removeban version=\"0\"><pattern value=\"127.0.0.1\" /></removeban>"));
	
	
	Packages::RemoveBan removeban("127.0.0.1");
	
	QCOMPARE( result.toString(0), removeban.toString(0));
}

QTEST_MAIN(TestRemoveBan)
#include "testremoveban.moc"

