#include <QtTest/QtTest>
#include <QDomDocument>
#include <../packages/addproject.h>

class TestAddProject: public QObject
{
	Q_OBJECT
	private slots:
		void createAddProject();
};

void TestAddProject::createAddProject()
{
	QDomDocument result;
	result.setContent(QString("<addproject version=\"0\"><name>Proyecto 1</name><author>equipo</author><description>nuevo proyecto</description><users /></addproject>"));
	
	
	Packages::AddProject addproject("Proyecto 1", "equipo", "nuevo proyecto");
	
	QCOMPARE( result.toString(0), addproject.toString(0));
}

QTEST_MAIN(TestAddProject)
#include "testaddproject.moc"

