#include <QtTest/QtTest>
#include <useractionparser.h>
#include <user.h>

class TestUserActionParser: public QObject
{
	Q_OBJECT
        private slots:
                void parse();
};

void TestUserActionParser::parse()
{
	Users::User result;
	result.setLogin("dcuadra");
	result.setName("David Cuadrado");
	Parsers::UserActionParser parser;
	parser.parse("<adduser> <login>dcuadra</login> <password></password> <name>David Cuadrado</name> <permissions /> </adduser>");
	QVERIFY(parser.user() == result);

}

QTEST_MAIN(TestUserActionParser)
#include "testuseractionparser.moc"
