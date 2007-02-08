
#include <QtTest/QtTest>
#include <QDomDocument>
#include <ack.h>

class TestAck: public QObject
{
	Q_OBJECT
	private slots:
		void createAck();
};

void TestAck::createAck()
{
	QDomDocument result;
	result.setContent(QString("<ack version=\"0\"><motd>No soy un hombre, soy un campo de batalla</motd><sign>aabbcc</sign><permissions /></ack>"));
	
	
	Packages::Ack ack("No soy un hombre, soy un campo de batalla", "aabbcc");

	QCOMPARE( result.toString(0), ack.toString(0));
}

QTEST_MAIN(TestAck)
#include "testack.moc"

