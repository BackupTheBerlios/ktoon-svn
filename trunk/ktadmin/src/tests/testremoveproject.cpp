#include <QtTest/QtTest>
#include <QDomDocument>
#include <../packages/removeproject.h>

class TestRemoveProject: public QObject
{
	Q_OBJECT
	private slots:
		void createRemoveProject();
};

void TestRemoveProject::createRemoveProject()
{
	QDomDocument result;
	result.setContent(QString("<removeproject version=\"0\"><name>Proyecto 1</name></removeproject>"));
	
	
	Packages::RemoveProject removeproject("Proyecto 1");
	
	QCOMPARE( result.toString(0), removeproject.toString(0));
}

QTEST_MAIN(TestRemoveProject)
#include "testremoveproject.moc"

