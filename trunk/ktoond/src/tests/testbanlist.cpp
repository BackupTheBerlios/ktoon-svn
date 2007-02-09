#include <QtTest/QtTest>
#include <QDomDocument>
#include <banlist.h>

class TestBanList: public QObject
{
	Q_OBJECT
	private slots:
		void createBanList();
};

void TestBanList::createBanList()
{
	QDomDocument result;
	result.setContent(QString("<banlist><entry value=\"127.0.0.1\" /></banlist>"));
	
	
	Packages::BanList banlist;
	banlist.setBans(QStringList() << "127.0.0.1");
	
	QCOMPARE( result.toString(0), banlist.toString(0));
}

QTEST_MAIN(TestBanList)
#include "testbanlist.moc"

