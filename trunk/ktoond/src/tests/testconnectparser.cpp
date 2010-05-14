#include <QtTest/QtTest>
#include <connectparser.h>
#include <QStringList>

class TestConnectParser: public QObject
{
    Q_OBJECT
    private slots:
        void parse();
};

void TestConnectParser::parse()
{
    QString result = "dcuadra";
    Parsers::ConnectParser parser;
    parser.parse("<connect version=\"0\" ><client type=\"0\" />    <login>dcuadra</login><password></password></connect>");
    QCOMPARE(parser.login(), result);
}

QTEST_MAIN(TestConnectParser)
#include "testconnectparser.moc"
