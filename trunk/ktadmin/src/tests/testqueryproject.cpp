#include <QtTest/QtTest>
#include <QDomDocument>
#include <../packages/queryproject.h>

class TestQueryProject: public QObject
{
	Q_OBJECT
	private slots:
		void createQueryProject();
};

void TestQueryProject::createQueryProject()
{
	QDomDocument result;
	result.setContent(QString("<queryproject version=\"0\"><name>Proyecto 1</name></queryproject>"));
	
	
	Packages::QueryProject queryproject("Proyecto 1");
	
	QCOMPARE( result.toString(0), queryproject.toString(0));
}

QTEST_MAIN(TestQueryProject)
#include "testqueryproject.moc"

