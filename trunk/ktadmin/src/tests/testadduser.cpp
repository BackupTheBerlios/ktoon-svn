#include <QtTest/QtTest>
#include <QDomDocument>
#include <../packages/adduser.h>

class TestAddUser: public QObject
{
	Q_OBJECT
	private slots:
		void createAddUser();
};

void TestAddUser::createAddUser()
{
	QDomDocument result;
	result.setContent(QString("<adduser version=\"0\"><login>dcuadra</login><password>d41d8cd98f00b204e9800998ecf8427e</password><name>David Cuadrado</name><permissions /></adduser>"));
	
	
	Packages::AddUser adduser("dcuadra", "", "David Cuadrado");
	
	QCOMPARE( result.toString(0), adduser.toString(0));
}

QTEST_MAIN(TestAddUser)
#include "testadduser.moc"

