#include <QtTest/QtTest>
#include <openprojectparser.h>
#include <QStringList>

class TestOpenProjectParser: public QObject
{
    Q_OBJECT
    private slots:
        void parse();
};

void TestOpenProjectParser::parse()
{
    QString result = "proyecto 1";
    Parsers::OpenProjectParser parser;
    parser.parse("<openproject version=\"0\"> <project name=\"proyecto 1\" /> </openproject>");
    QCOMPARE(parser.name(), result);
}

QTEST_MAIN(TestOpenProjectParser)
#include "testopenprojectparser.moc"
