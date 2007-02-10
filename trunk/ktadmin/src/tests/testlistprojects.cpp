#include <QtTest/QtTest>
#include <QDomDocument>
#include <packages/listprojects.h>

class TestListProjects: public QObject
{
	Q_OBJECT
	private slots:
		void createListProjects();
};

void TestListProjects::createListProjects()
{
	QDomDocument result;
	result.setContent(QString("<listprojects/>"));
	
	
	Packages::ListProjects listprojects;
	
	QCOMPARE( result.toString(0), listprojects.toString(0));
}

QTEST_MAIN(TestListProjects)
#include "testlistprojects.moc"

