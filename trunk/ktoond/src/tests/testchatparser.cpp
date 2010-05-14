#include <QtTest/QtTest>
#include <chatparser.h>
#include <QStringList>

class TestChatParser: public QObject
{
    Q_OBJECT
    private slots:
        void parse();
};

void TestChatParser::parse()
{
    QString result = "hola";
    Parsers::ChatParser parser;
    parser.parse("<chat version=\"0\"><message text=\"hola\" /></chat>");
    
    QCOMPARE(parser.message(), result);
}

QTEST_MAIN(TestChatParser)
#include "testchatparser.moc"
