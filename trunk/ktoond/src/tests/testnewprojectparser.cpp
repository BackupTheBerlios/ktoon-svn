#include <QtTest/QtTest>
#include <newprojectparser.h>
#include <QStringList>

class TestNewProjectParser: public QObject
{
    Q_OBJECT
    private slots:
        void parse_data();
        void parse();
};

void TestNewProjectParser::parse_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("result");
    Parsers::NewProjectParser parser;
    parser.parse("<newproject><author>Animaciones S.A.</author><name>proyecto 1</name><description>proyecto de prueba</description></newproject>");

    QTest::newRow("name") << parser.name() << "proyecto 1";
    QTest::newRow("author") << parser.author() << "Animaciones S.A.";
    QTest::newRow("description") << parser.description() << "proyecto de prueba";
}

void TestNewProjectParser::parse()
{
    QFETCH(QString, string);
    QFETCH(QString, result);
    
    QCOMPARE(string, result);
}

QTEST_MAIN(TestNewProjectParser)
#include "testnewprojectparser.moc"
