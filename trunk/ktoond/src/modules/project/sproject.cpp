/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "sproject.h"
#include <QFileInfo>
#include <ktsaveproject.h>

#include <kcore/kapplicationproperties.h>
#include <kcore/kdebug.h>

struct SProject::Private
{
    QTimer *saver;
    QString filename;
    QMultiHash<UserType, QString> users;
    
    int timerId;
};

SProject::SProject(const QString & filename, QObject *parent) : KTProject(parent), k(new Private() )
{
    KINIT;
    k->filename = filename;
    k->saver = new QTimer(this);
    k->saver->setInterval(30000);
    k->saver->start();
    QObject::connect(k->saver, SIGNAL(timeout ()), this, SLOT(save()));
    k->timerId = startTimer(300000);
}

void SProject::resetTimer()
{
    k->saver->stop();
    k->saver->start();
}

SProject::~SProject()
{
    delete k->saver;
    killTimer(k->timerId);
    delete k;
}

bool SProject::save()
{
    K_FUNCINFOX("server");
    KTSaveProject *saver = new KTSaveProject;
    SHOW_VAR(k->filename);
    bool ok = saver->save(k->filename, this);
    
    if (ok)
        emit requestSendErrorMessage(QObject::tr("project saved"), Packages::Error::Info);
    else
        emit requestSendErrorMessage(QObject::tr("Error saving project"), Packages::Error::Err);

    delete saver;

    return ok;
}

void SProject::timerEvent(QTimerEvent *)
{
    save();
}

QDomElement SProject::infoToXml(QDomDocument &doc) const
{
    QDomElement project = doc.createElement("project");
    
    project.setAttribute("name", projectName());
    project.setAttribute("author", author());
    project.setAttribute("description", description());
    
    QDomElement file = doc.createElement("file");
    
    QString fileName = projectName();

    if (!fileName.endsWith(".ktn"))
        fileName += ".ktn";
    
    QFileInfo fi(k->filename);
    file.setAttribute("name", fi.fileName());
    
    project.appendChild(file);
    QDomElement usersE = doc.createElement("users");
    project.appendChild(usersE);
    
    foreach (UserType key, k->users.uniqueKeys()) {
             foreach (QString login, k->users.values(key)) {
                      QDomElement userE = doc.createElement("user");
                      userE.setAttribute("type", key);
                      userE.appendChild(doc.createTextNode(login));
                      usersE.appendChild(userE);
             }
    }

    return project;
}

bool SProject::addUser(const QString& login, UserType type)
{
    if (!k->users.values(type).contains(login)) {
        k->users.insert(type, login);
        return true;
    }

    return false;
}

void SProject::setUsers(const QMultiHash<SProject::UserType, QString> & users)
{
    k->users = users;
}

QString SProject::fileName()
{
    return k->filename;
}

bool SProject::isOwner(const Users::User* user)
{
    SHOW_VAR(k->users.count());
    SHOW_VAR(k->users.values(Owner));

    return k->users.values(Owner).contains(user->login());
}

