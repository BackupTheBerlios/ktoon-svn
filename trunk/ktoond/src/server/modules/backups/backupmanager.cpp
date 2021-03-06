
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

#include "backupmanager.h"

#include <QDir>
#include <QFileInfo>
#include <QDateTime>

#include <kcore/kdebug.h>

#include "backuplist.h"
#include "removebackupparser.h"

#include "users/user.h"
#include "base/settings.h"
#include "base/package.h"
#include "core/connection.h"
#include "core/server.h"

namespace Backups {

struct Manager::Private
{
    QString dbfile;
    Backups::Database *database;
    
    QString fileName(const QString &origFile, const QDateTime &date) {
        QFileInfo fi(origFile);
        
        QString destfile = fi.baseName()+"-"+date.toString(Qt::ISODate);
        
        if (! fi.completeSuffix().isEmpty())
            destfile += "."+fi.completeSuffix();
        
        return destfile;
    }
};

Manager::Manager() : k(new Private)
{
    k->dbfile = Base::Settings::self()->databaseDirPath() + "/backups.xml";
    
    QDir dir(Base::Settings::self()->backupDirPath());

    if (! dir.exists())
        dir.mkpath(Base::Settings::self()->backupDirPath());
    
    k->database = new Backups::Database(k->dbfile);
}

Manager::~Manager()
{
    delete k;
}

bool Manager::makeBackup(const QString &filepath, const QDateTime &date, const QString &name)
{
    kDebug() << "Making backup: " << filepath;
    QFile file(filepath);
    
    if (file.exists()) {
        QString destfile = k->fileName(filepath, date);
        
        if (file.copy(Base::Settings::self()->backupDirPath() + "/" + destfile)) {
            k->database->addEntry(filepath, destfile, name, date);
            return true;
        }
    }
    
    return false;
}

bool Manager::removeBackup(const QString &name, const QDateTime &date)
{
    QHash<QString, QList<Backups::Database::Entry> > entries = k->database->entries();
    
    if (entries.contains(name)) {
        QList<Backups::Database::Entry> backups = entries[name];
        
        foreach (Backups::Database::Entry backup, backups) {
                 if (backup.date == date) {
                     if (k->database->removeEntry(name, date))
                         return QFile::remove(Base::Settings::self()->backupDirPath() + "/" + backup.file);
                 }
        }
    }
    
    return false;
}

bool Manager::restoreBackup(const QString &name, const QDateTime &date)
{
    QHash<QString, QList<Backups::Database::Entry> > entries = k->database->entries();
    
    if (entries.contains(name)) {
        QList<Backups::Database::Entry> backups = entries[name];
        
        foreach (Backups::Database::Entry backup, backups) {
                 if (backup.date == date) {
                     QFile::remove(backup.origin);
                     QFile::copy(Base::Settings::self()->backupDirPath()+"/"+backup.file, backup.origin);
                 }
        }
    }
    
    return false;
}

QHash<QString, QList<Backups::Database::Entry> > Manager::entries()
{
    return k->database->entries();
}

void Manager::handlePackage(Base::Package *const pkg)
{
    pkg->accept();
    
    QString root = pkg->root();
    Server::Connection *cnn = pkg->source();
    Server::TcpServer *server = cnn->server();
    
    if (root == "listbackups") {
        if (cnn->user()->canReadOn("admin")) {
            Backups::Manager *bm = server->backupManager();
            
            QHash<QString, QList<Backups::Database::Entry> > entries = bm->entries();
            QHashIterator<QString, QList<Backups::Database::Entry> > it(entries);
            
            Packages::BackupList pkg;
            
            while (it.hasNext()) {
                   it.next();
                   QList<Backups::Database::Entry> pentries = it.value();
                
                   QStringList backups;
                   foreach (Backups::Database::Entry e, pentries)
                            backups << e.date.toString(Qt::ISODate);
                
                   pkg.addEntry(it.key(), backups);
            }
            
            cnn->sendToClient(pkg);
        } else {
            cnn->sendErrorPackageToClient(QObject::tr("Permission denied."), Packages::Error::Err);
        }
    } else if (root == "removebackup") {
               if (cnn->user()->canWriteOn("admin")) {
                   Backups::Manager *bm = server->backupManager();
                   Parsers::RemoveBackupParser parser;
            
                   if (parser.parse(pkg->xml())) {
                       QHashIterator<QString, QDateTime > it(parser.entries());
                
                       while (it.hasNext()) {
                              it.next();
                              bm->removeBackup(it.key(), it.value());
                       }
                
                       server->sendToAdmins(pkg->xml());
                   }
                } else {
                   cnn->sendErrorPackageToClient(QObject::tr("Permission denied."), Packages::Error::Err);
                }
    }
    else if (root == "restorebackup") {
             if (cnn->user()->canWriteOn("admin")) {
                 Backups::Manager *bm = server->backupManager();
                 Parsers::RemoveBackupParser parser;
            
                 if (parser.parse(pkg->xml())) {
                     QHashIterator<QString, QDateTime > it(parser.entries());
                
                     while (it.hasNext()) {
                            it.next();
                            bm->restoreBackup(it.key(), it.value());
                     }
                
                     server->sendToAdmins(pkg->xml());
                 }
             } else {
                 cnn->sendErrorPackageToClient(QObject::tr("Permission denied."), Packages::Error::Err);
             }
    } else { 
      pkg->ignore();
    }
}

}
