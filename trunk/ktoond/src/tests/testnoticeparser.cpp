#include <QtTest/QtTest>
#include <noticeparser.h>
#include <QStringList>

class TestNoticeParser: public QObject
{
    Q_OBJECT
    private slots:
        void parse();
};

void TestNoticeParser::parse()
{
    QString result = "hola";
    Parsers::NoticeParser parser;
    parser.parse("<notice version=\"0\"><message text=\"hola\" /></notice>");
    QCOMPARE(parser.message(), result);
}

QTEST_MAIN(TestNoticeParser)
#include "testnoticeparser.moc"

