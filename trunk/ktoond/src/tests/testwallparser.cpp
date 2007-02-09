#include <QtTest/QtTest>
#include <wallparser.h>

class TestWallParser: public QObject
{
	Q_OBJECT
        private slots:
                void parse();
};

void TestWallParser::parse()
{
	QString result = "hola";
	Parsers::WallParser parser;
	parser.parse("<wall version=\"0\"> <message text=\"hola\" /> </wall>");
	QCOMPARE( parser.message() ,result );
}

QTEST_MAIN(TestWallParser)
#include "testwallparser.moc"
