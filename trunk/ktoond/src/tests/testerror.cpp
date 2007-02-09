#include <QtTest/QtTest>
#include <QDomDocument>

#include <error.h>

class TestError: public QObject
{
	Q_OBJECT
	private slots:
		void createError();
};

void TestError::createError()
{
	QDomDocument result;
	result.setContent(QString("<error version=\"0\" > <message level=\"0\">Solicitud invalida</message> </error>"));
	
	
	Packages::Error error("Solicitud invalida", Packages::Error::Level(0));
	
	QCOMPARE( result.toString(0), error.toString(0));
}

QTEST_MAIN(TestError)
#include "testerror.moc"

