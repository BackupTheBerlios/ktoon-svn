#include <QtTest/QtTest>
#include <backups/removebackupparser.h>
#include <QHash>
#include <QDateTime>

class TestRemoveBackupParser: public QObject
{
    Q_OBJECT
    private slots:
        void parse();
};

void TestRemoveBackupParser::parse()
{
    QHash<QString, QDateTime > result;
    result.insert("proyecto1", QDateTime::fromString("2006-02-05T12:22:56",Qt::ISODate ));
    Parsers::RemoveBackupParser parser;
    parser.parse("<removebackup> <entry date=\"2006-02-05T12:22:56\" >proyecto1</entry> </removebackup>");
    QCOMPARE(parser.entries(), result);
}

QTEST_MAIN(TestRemoveBackupParser)
#include "testremovebackupparser.moc"
