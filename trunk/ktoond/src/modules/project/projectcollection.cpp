/***************************************************************************
 *   Project KTOOND: KToon Collaboration Daemon                            *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 *
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *                                                                         *
 *  License:                                                               *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *    This program is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#include "projectcollection.h"

#include "ktproject.h"
#include "ktrequestparser.h"
#include "ktsaveproject.h"
#include "ktcommandexecutor.h"
#include "ktprojectcommand.h"
#include "core/connection.h"
#include "core/server.h"

#include "global.h"
#include "project.h"
#include "users/user.h"
#include "sproject.h"
#include "database.h"
#include "error.h"

#include "projectactionparser.h"
#include "newprojectparser.h"
#include "openprojectparser.h"
#include "importprojectparser.h"
#include "listparser.h"
#include "error.h"
#include "listprojectsparser.h"

#include "base/settings.h"
#include "base/package.h"

#include "packages/addbackupparser.h"

#include "backups/addbackup.h"
#include "backups/backupmanager.h"

#include <kcore/kapplicationproperties.h>
#include <kcore/kdebug.h>

#include <QDir>
#include <QHash>

namespace Projects {

struct ProjectCollection::Private
{
    QHash<QString, SProject * > projects;
    Database *db;
    QHash<QString, QList<Server::Connection *> > connections;
};

ProjectCollection::ProjectCollection() : k(new Private())
{
    k->db = new Database(Base::Settings::self()->databaseDirPath() + "/projects.xml");
}


ProjectCollection::~ProjectCollection()
{
    delete k->db;
    delete k;
}

void ProjectCollection::createProject(Server::Connection *cnn, const QString &author) 
{
    QString projectName = cnn->data(Info::ProjectName).toString();
    
    QMultiHash<int, QString> user;
    user.insert(int(SProject::Owner), cnn->user()->login());
    
    if (!addProject(projectName, author, "new project", user))
        cnn->sendErrorPackageToClient(QObject::tr("Cannot create project %1").arg(projectName), Packages::Error::Warning);

    openProject(cnn);
}

bool ProjectCollection::addProject(const QString& name, const QString& author, const QString& description, const QMultiHash<int, QString> & users)
{
    if (!k->db->exists(name)) {
        if (!k->projects.contains(name)) {
            
            QMultiHash<SProject::UserType, QString> newusers; 
            foreach (int type, users.uniqueKeys()) {
                     foreach (QString login, users.values(type))
                              newusers.insert( SProject::UserType(type), login);
            }

            if (newusers.values(SProject::Owner).empty())
                return false;
            
            SProject *project = new SProject( kAppProp->cacheDir() +"/"+ k->db->nextFileName());
            project->setProjectName(name);
            project->setAuthor(author);
            project->setDescription(description);
            bool okAddProject = false;
            
            project->setUsers(newusers);
            
            bool okSaveProject = project->save();
            if (okSaveProject)
                okAddProject = k->db->addProject(project);

            delete project;

            return okAddProject;
        }
    }

    return false;
}

bool ProjectCollection::removeProject(Server::Connection *cnn, const QString& name)
{
    if (!k->projects[name]) {
        SProject *project = k->db->loadProject(name);

        if (project) {
            bool ok = k->db->removeProject(project);
            delete project;
            return ok;
        } else {
            cnn->sendErrorPackageToClient(QObject::tr("project not exist"), Packages::Error::Err);
            return false;
        }
    }
    
    cnn->sendErrorPackageToClient(QObject::tr("project is busy"), Packages::Error::Err);

    return false;
}

bool ProjectCollection::updateProject(Server::Connection *cnn, const QString& name, const QString& author, const QString& description, const QMultiHash<int, QString> &users)
{
    SProject *p = k->db->loadProject(name);

    if (p) {
        if (!author.isNull())
            p->setAuthor(author);

        if (!description.isNull())
            p->setDescription(description);
        
        QMultiHash<SProject::UserType, QString> newusers; 

        foreach (int type, users.uniqueKeys()) {
                 foreach (QString login, users.values(type))
                          newusers.insert(SProject::UserType(type), login);
        }
        
        p->setUsers(newusers);
        
        bool ok = k->db->updateProject(p);
        delete p;

        if (!ok) {
            cnn->sendErrorPackageToClient(QObject::tr("cannot update project"), Packages::Error::Err);
            return false;
        }

        return true;
    }
    
    cnn->sendErrorPackageToClient(QObject::tr("Project %1 not exist").arg(name), Packages::Error::Err);

    return false;
}

SProject *ProjectCollection::project(const QString &name)
{
    return k->db->loadProject(name);
}

bool ProjectCollection::openProject(Server::Connection *cnn)
{
    QString projectName = cnn->data(Info::ProjectName).toString();
    SProject *project;
    
    if (!k->projects.contains(projectName)) {

        project = k->db->loadProject(projectName);

        if (project) {

            KTSaveProject *loader = new KTSaveProject;
            QObject::connect(project, SIGNAL(requestSendErrorMessage(const QString&, Packages::Error::Level)), cnn, SLOT(sendErrorPackageToClient(const QString&, Packages::Error::Level)));
            bool ok  = loader->load(project->fileName(), project);
            delete loader;

            if (!ok) {
                cnn->sendErrorPackageToClient(QObject::tr("Project %1 not exists").arg(projectName), Packages::Error::Err);
                return false;
            }
            
            k->projects.insert(projectName, project);

        } else {

            cnn->sendErrorPackageToClient(QObject::tr("Project %1 not exists").arg(projectName), Packages::Error::Err);
            cnn->close();

            return false;
        }

    } else {

        project = k->projects[projectName];
        k->projects[projectName]->save();
        QObject::connect(k->projects[projectName], SIGNAL(requestSendErrorMessage(const QString&, int)), cnn, SLOT(sendErrorPackageToClient(const QString&, int)));

    }
    
    Packages::Project projectPackage(project->fileName());

    if (!projectPackage.isValid())
        return false;

    cnn->sendToClient(projectPackage.toString());
    
    k->connections[projectName].append(cnn);

    return true;
}

void ProjectCollection::importProject(Server::Connection *cnn, const QByteArray& data)
{
    QString filename = k->db->nextFileName();
    QFile file(filename);

    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
        SProject *project = new SProject(filename);
        QObject::connect(project, SIGNAL(requestSendErrorMessage(const QString&, Packages::Error::Level)), cnn, SLOT(sendErrorPackageToClient(const QString&, Packages::Error::Level)));
        KTSaveProject *loader = new KTSaveProject;
        bool ok  = loader->load(filename, project);

        if (ok) {
            cnn->setData( Info::ProjectName , project->projectName());
            k->projects.insert(project->projectName(), project);
            k->db->addProject(project);
            k->connections.insert(project->projectName(), QList<Server::Connection *>() << cnn);
            
            Packages::Project projectPackage(project->fileName());
            cnn->sendToClient(projectPackage.toString());
        } else {
            //TODO: notify error / send message
        }
    }
}

void ProjectCollection::handlePackage(Base::Package *const pkg)
{
    QString root = pkg->root();
    QString package = pkg->xml();
    Server::Connection *cnx = pkg->source();
    Server::TcpServer *server = cnx->server();
    
    pkg->accept();
    
    if (root == "request") {

        if (cnx->user()->canWriteOn("project")) {

            if (!cnx->data(Info::ProjectName).toString().isNull()) {
                if (handleProjectRequest(cnx , package)) {
                    QDomDocument request;
                    request.setContent(package);
                    sendToProjectMembers(cnx, request);
                } else {
                    cnx->sendErrorPackageToClient(QObject::tr("Cannot handle project request"), Packages::Error::Warning);
                }
            } else {
                kWarning() << "NO PROJECT NAME!";
            }

        } else {
            cnx->sendErrorPackageToClient(QObject::tr("You doen't have rights on project."), Packages::Error::Warning );
        }

    } else if (root == "openproject") {
               if (cnx->user()->canReadOn("project")) {
                   Parsers::OpenProjectParser parser;
                   if (parser.parse(package)) {
                       cnx->setData(Info::ProjectName, parser.name());
                       openProject(cnx);
                   }
               } else {
                   cnx->sendErrorPackageToClient(QObject::tr("You doen't have rights on project."), Packages::Error::Warning );
               }
    } else if (root == "newproject") {
               if (cnx->user()->canWriteOn("project")) {
                   Parsers::NewProjectParser parser;
                   if (parser.parse(package)) {
                       cnx->setData(Info::ProjectName, parser.name());
                       createProject(cnx, parser.author());
                   } else {
                       cnx->sendErrorPackageToClient(QObject::tr("You doen't have rights on project."), Packages::Error::Warning);
                   }
               }
    } else if (root == "importproject") {
               if (cnx->user()->canWriteOn("project")) {
                   Parsers::ImportProjectParser parser;
                   if (parser.parse(package))
                      importProject(cnx, parser.data());
               }
    } else if (root == "listprojects") {
               if (cnx->user()->canReadOn("project")) {
                   Parsers::ListProjectsParser parser;
            
                   if (parser.parse(package)) {
                       if (parser.readAll())
                           listAllProjects(cnx);
                       else
                           listUserProjects(cnx);
                   }
               } else {
                   cnx->sendErrorPackageToClient(QObject::tr("You don't have rights on this project."), Packages::Error::Warning);
               }
    } else if (root == "addproject") {
               if (cnx->user()->canWriteOn("project") && cnx->user()->canReadOn("admin")) {
                   Parsers::ProjectActionParser parser;
                   if (parser.parse(package)) {
                       cnx->setData(Info::ProjectName, parser.name());
                
                       if (!addProject( parser.name(), parser.author(), parser.description(), parser.users()))
                           cnx->sendErrorPackageToClient(QObject::tr("Cannot create project %1").arg(parser.name()), Packages::Error::Warning);
                       else
                           cnx->server()->sendToAdmins(package);
                   }
               } else {
                   cnx->sendErrorPackageToClient(QObject::tr("You doen't have rights on project."), Packages::Error::Warning);
               }
    } else if (root == "removeproject") {
               if (cnx->user()->canWriteOn("project") && cnx->user()->canReadOn("admin")) {
                   Parsers::ProjectActionParser parser;
                   if (parser.parse(package)) {
                       if (!removeProject(cnx ,parser.name())) {
                           cnx->sendErrorPackageToClient(QObject::tr("Cannot remove project %1").arg(parser.name()), Packages::Error::Warning);
                       }
                   }
                   cnx->server()->sendToAdmins(package); //TODO: Send a different package
               } else {
                   cnx->sendErrorPackageToClient(QObject::tr("You doen't have rights on project."), Packages::Error::Warning);
               }
    } else if (root == "queryproject") {
               if (cnx->user()->canWriteOn("project") && cnx->user()->canReadOn("admin")) {
                   Parsers::ProjectActionParser parser;
                   if (parser.parse(package)) {
                       SProject *project = this->project(parser.name());
                       if (project) {
                           QDomDocument doc;
                           QDomElement projectquery = doc.createElement("projectquery");
                           projectquery.appendChild(project->infoToXml(doc));
                           doc.appendChild(projectquery);
                    
                           cnx->sendToClient(doc.toString());
                           delete project;
                       }
                   }
               } else {
                   cnx->sendErrorPackageToClient(QObject::tr("You doen't have rights on project."), Packages::Error::Warning);
               }
    } else if (root == "updateproject") {
               if (cnx->user()->canWriteOn("project") && cnx->user()->canReadOn("admin")) {
                   Parsers::ProjectActionParser parser;
                   if (parser.parse(package))
                       updateProject(cnx, parser.name(), parser.author(), parser.description(), parser.users());
               } else {
                   cnx->sendErrorPackageToClient(QObject::tr("You doen't have rights on project."), Packages::Error::Warning );
               }

    } else if (root == "addbackup") {

        if (cnx->user()->canWriteOn("admin")) {

            Parsers::AddBackupParser parser;
            if (parser.parse(package)) {
                Packages::AddBackup pkg;
                
                Backups::Manager *bm = server->backupManager();
                
                foreach (QString project, parser.backups()) {
                         QDateTime date = QDateTime::currentDateTime();
                    
                         Projects::Database::ProjectInfo info = projectInfo(project);
                         bm->makeBackup(info.file, date, info.name);
                    
                         pkg.addEntry(info.name, date);
                }
                
                server->sendToAdmins(pkg.toString());
            }
        } else {
            cnx->sendErrorPackageToClient(QObject::tr("Permission denied."), Packages::Error::Err);
        }

    } else {
        pkg->ignore();
    }
}

QStringList ProjectCollection::projects() const
{
    QDir dir(kAppProp->cacheDir());

    return dir.entryList();
}

void ProjectCollection::closeProject(const QString & name)
{
    //FIXME: verificar que no hay otros con el proyecto abierto, antes de ejecutar esta funcion
    saveProject(name);
    delete k->projects.take(name);
    
    k->connections.remove(name);
}

bool ProjectCollection::saveProject(const QString & name)
{
    if (k->projects[name])
        return k->projects[name]->save();

    return false;
}

void ProjectCollection::connectionClosed(Server::Connection *cnn)
{
    K_FUNCINFO;
    QString projectName = cnn->data(Info::ProjectName).toString();
    
    k->connections[projectName].removeAll(cnn);
    
    if (k->connections[projectName].isEmpty())
        closeProject(projectName);
}

bool ProjectCollection::handleProjectRequest(Server::Connection *cnn, const QString strRequest)
{
    QString projectName = cnn->data(Info::ProjectName).toString();
    SHOW_VAR(projectName);
    KTRequestParser parser;

    if (parser.parse(strRequest)) {
        KTProject *project = k->projects[projectName];
        if (project) {
            KTCommandExecutor * commandExecutor = new KTCommandExecutor(project);
            KTProjectCommand command(commandExecutor, parser.response());
            command.redo();
            delete commandExecutor;
            k->projects[projectName]->resetTimer();
            //debug
            //saveProject(projectName);
            
            return true;
        } else {
            kWarning() << "NO PROJECT FOR: " << projectName;
        }
    }
    
    return false;
}

void ProjectCollection::listAllProjects(Server::Connection *cnn)
{
    Packages::ProjectList list;
    
    foreach (Database::ProjectInfo info, k->db->allProjects())
             list.addProject(info.name, info.author, info.description);
    
    cnn->sendToAll(list);
}

void ProjectCollection::listUserProjects(Server::Connection *cnn)
{
    Packages::ProjectList list;
    foreach (Database::ProjectInfo info, k->db->userProjects(cnn->user()->login()))
             list.addProject(info.name, info.author, info.description);

    cnn->sendToClient(list.toString());
}

void ProjectCollection::sendToProjectMembers(Server::Connection *cnn, QDomDocument &doc)
{
    QString projectName = cnn->data(Info::ProjectName).toString();
    cnn->signPackage(doc);
    
    foreach (Server::Connection *cnn, k->connections[projectName]) {
             if (cnn->user()->canReadOn("project"))
                 cnn->sendToClient(doc.toString(0));
    }
}

void ProjectCollection::addUser(Server::Connection *cnn, const QString & login, SProject::UserType type  )
{
    QString projectName = cnn->data(Info::ProjectName).toString();
    SProject *project = 0;

    if (k->projects.contains(projectName))
        project = k->projects[projectName];

    if (project) {
        if (project->isOwner(cnn->user())) {
            project->addUser(login, type);
            k->db->updateProject(project);
        } else {
            kDebug() << cnn->user()->login()  + " isn't the owner";
        }
    } else {
        kDebug() << "Project wasn't loaded";
    }
}

Database::ProjectInfo ProjectCollection::projectInfo(const QString &projectName)
{
    return k->db->projectInfo(projectName);
}

}


