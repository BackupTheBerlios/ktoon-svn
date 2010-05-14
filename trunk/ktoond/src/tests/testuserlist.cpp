#include <QtTest/QtTest>
#include <QDomDocument>
#include <userlist.h>

#include "users/user.h"

class TestUserList: public QObject
{
    Q_OBJECT
    private slots:
        void createUserList();
};

void TestUserList::createUserList()
{
    QDomDocument result;
    result.setContent(QString("<userlist version=\"0\"><user login=\"dcuadra\" name=\"David Cuadrado\" /></userlist>"));
    
    Users::User user;
    user.setName("David Cuadrado");
    user.setLogin("dcuadra");
    
    Packages::UserList userlist;
    userlist.addUser(&user);
    
    QCOMPARE(result.toString(0), userlist.toString(0));
}

QTEST_MAIN(TestUserList)
#include "testuserlist.moc"

