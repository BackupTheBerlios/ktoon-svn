#include <QtTest/QtTest>
#include <projectactionparser.h>
#include <QMultiHash>

class TestProjectActionParser: public QObject
{
	Q_OBJECT
        private slots:
                void parse();
};

void TestProjectActionParser::parse()
{
	Parsers::ProjectActionParser parser;
	parser.parse("<addproject version=\"0\" > <name>proyecto1</name> <author>Animaciones</author> <description>proyecto de prueba</description> <users> <user type=\"0\" >kuadrosx</user> </users> </addproject>");
	QMultiHash<int, QString> result;
	result.insert( 0, "kuadrosx");
	QCOMPARE(parser.name(), QString("proyecto1"));
	QCOMPARE(parser.author(), QString("Animaciones"));
	QCOMPARE(parser.description(), QString("proyecto de prueba"));
	QCOMPARE(parser.users(), result);

}

QTEST_MAIN(TestProjectActionParser)
#include "testprojectactionparser.moc"
