#include <QtTest/QtTest>
#include <removebanparser.h>

class TestRemoveBanParser: public QObject
{
    Q_OBJECT
    private slots:
        void parse();
};

void TestRemoveBanParser::parse()
{
    QString result = "127.0.0.1";
    Parsers::RemoveBanParser parser;
    parser.parse("<removeban> <pattern value=\"127.0.0.1\" /> </removeban>");
    QCOMPARE(parser.pattern(), result);
}

QTEST_MAIN(TestRemoveBanParser)
#include "testremovebanparser.moc"
