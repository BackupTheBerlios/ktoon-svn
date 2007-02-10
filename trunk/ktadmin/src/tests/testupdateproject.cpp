#include <QtTest/QtTest>
#include <QDomDocument>
#include <../packages/updateproject.h>

class TestUpdateProject: public QObject
{
	Q_OBJECT
	private slots:
		void createUpdateProject();
};

void TestUpdateProject::createUpdateProject()
{
	QDomDocument result;
	result.setContent(QString("<updateproject version=\"0\"><name>Proyecto 1</name><author>Un autor</author></updateproject>"));
	
	
	Packages::UpdateProject updateproject("Proyecto 1");
	updateproject.setAuthor("Un autor");
	
	QCOMPARE( result.toString(0), updateproject.toString(0));
}

QTEST_MAIN(TestUpdateProject)
#include "testupdateproject.moc"

