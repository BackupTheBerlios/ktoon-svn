
#include <QtTest/QtTest>
#include <QDomDocument>
#include <addbackupparser.h>
#include <QStringList>

class TestAddBackupParser: public QObject
{
	Q_OBJECT
        private slots:
                void parse();
};


void TestAddBackupParser::parse()
{
	QStringList result = QStringList() << "proyecto1";
	Parsers::AddBackupParser parser;
	parser.parse( "<addbackup><entry>proyecto1</entry></addbackup>");
	
	QCOMPARE(parser.backups(), result);
}

QTEST_MAIN(TestAddBackupParser)
#include "testaddbackupparser.moc"
