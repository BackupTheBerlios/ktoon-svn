#include <QtTest/QtTest>
#include <QDomDocument>
#include <../packages/updateuser.h>

class TestUpdateUser: public QObject
{
	Q_OBJECT
	private slots:
		void createUpdateUser();
};

void TestUpdateUser::createUpdateUser()
{
	QDomDocument result;
	result.setContent(QString("<updateuser version=\"0\"><login>dcuadra</login><name>David Cuadrado</name></updateuser>"));
	
	
	Packages::UpdateUser updateuser("dcuadra");
	updateuser.setName("David Cuadrado");
	
	QCOMPARE( result.toString(0), updateuser.toString(0));
}

QTEST_MAIN(TestUpdateUser)
#include "testupdateuser.moc"

