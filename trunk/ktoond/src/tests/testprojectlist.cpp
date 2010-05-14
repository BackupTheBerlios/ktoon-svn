#include <QtTest/QtTest>
#include <QDomDocument>
#include <projectlist.h>

class TestProjectList: public QObject
{
    Q_OBJECT
    private slots:
        void createProjectList();
};

void TestProjectList::createProjectList()
{
    QDomDocument result;
    result.setContent(QString("<projectlist version=\"0\"><project description=\"animaciones de prueba\" author=\"Animaciones S.A.\" name=\"proyecto 1\"/></projectlist>"));
    
    Packages::ProjectList projectlist;
    projectlist.addProject("proyecto 1", "Animaciones S.A.", "animacion de prueba");
    
    QCOMPARE( result.toString(0), projectlist.toString(0));
}

QTEST_MAIN(TestProjectList)
#include "testprojectlist.moc"
