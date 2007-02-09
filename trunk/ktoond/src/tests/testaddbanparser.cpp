
#include <QtTest/QtTest>
#include <addbanparser.h>
#include <QStringList>

class TestAddBanParser: public QObject
{
	Q_OBJECT
        private slots:
                void parse();
};


void TestAddBanParser::parse()
{
	QString result = "127.0.0.1";
	Parsers::AddBanParser parser;
	parser.parse("<addban><pattern value= \"127.0.0.1\" /></addban>");
	
	QCOMPARE(parser.pattern(), result);
}

QTEST_MAIN(TestAddBanParser)
#include "testaddbanparser.moc"
