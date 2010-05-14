#include <QtTest/QtTest>
#include <listprojectsparser.h>
#include <QStringList>

class TestListProjectsParser: public QObject
{
    Q_OBJECT
    private slots:
        void parse();
};

void TestListProjectsParser::parse()
{
    bool result = true;
    Parsers::ListProjectsParser parser;
    parser.parse("<listprojects><option>all</option></listprojects>");
    QCOMPARE(parser.readAll(), result);
}

QTEST_MAIN(TestListProjectsParser)
#include "testlistprojectsparser.moc"

