#include <QtTest/QtTest>
#include <QDomDocument>
#include <project.h>

class TestProject: public QObject
{
    Q_OBJECT
    private slots:
        void createProject();
};

void TestProject::createProject()
{
    bool result = false;
    
    Packages::Project project("project1.ktn");
    
    QCOMPARE(result, project.isValid());
}

QTEST_MAIN(TestProject)
#include "testproject.moc"
