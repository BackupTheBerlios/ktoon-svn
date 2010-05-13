/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosxx@gmail.com                                                   *
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

#include "database.h"
#include "databaseparser.h"

#include <QDomDocument>
#include <QFile>
#include <QTextStream>

#include <kcore/kdebug.h>

namespace Projects 
{

Database::Database(const QString &dbfile): m_dbfile(dbfile)
{
    loadDataBase();
}

Database::~Database()
{
}

void Database::setDBFile(const QString& dbfile)
{
    m_dbfile = dbfile;
    loadDataBase();
}

QDomDocument Database::loadDataBase()
{
    kDebug() << "loading database: " << m_dbfile;
    QFile file(m_dbfile);
    QDomDocument document;

    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream ts(&file);
            document.appendChild(document.createElement("projects"));
            ts << document.toString();
            file.close();
        }
    }
    
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (!document.setContent(file.readAll()))
            kWarning() << file.fileName() << " is corrupted!";

        file.close();
    }

    return document;
}

QString Database::nextFileName()
{
    if (m_lastFileName.isEmpty()) {
        Parsers::DatabaseParser parser;
        QFile dbf(m_dbfile);
        parser.parse(&dbf);
        parser.gc();
        
        m_lastFileName = parser.lastFileName();
    }

    if (m_lastFileName.isEmpty()) {
        m_lastFileName = "project0.ktn";
    } else {
        QRegExp reg("project(\\d+).ktn");
        if (reg.indexIn(m_lastFileName) != -1) {
            QStringList list = reg.capturedTexts();
            m_lastFileName = QString("project%1.ktn").arg(list[1].toInt()+1);
        }
    }

    return m_lastFileName;
}

bool Database::addProject(const SProject * project)
{
    QDomDocument db = loadDataBase();
    QDomNode  root = db.firstChild();
    
    root.appendChild(project->infoToXml(db));
    
    QFile file(m_dbfile);
    
    if (file.open(QIODevice::WriteOnly| QIODevice::Text)) {
        QTextStream ts(&file);
        ts << db.toString();
        file.close();
    } else {
        return false;
    }

    return true;
}

bool Database::updateProject(const SProject * project)
{
    QDomDocument doc = loadDataBase();
    
    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChild();

    while (!n.isNull()) {
           QDomElement e = n.toElement(); 
           if (!e.isNull()) {
               if (e.attribute("name") == project->projectName()) {
                   docElem.removeChild(n);
                   docElem.appendChild(project->infoToXml(doc));
               }
           }
           n = n.nextSibling();
    }
    
    QFile file(m_dbfile);
    
    if (file.open(QIODevice::WriteOnly| QIODevice::Text)) {
        QTextStream ts(&file);
        ts << doc.toString();
        file.close();
    } else {
        return false;
    }

    return true;
}

bool Database::removeProject(const SProject * project)
{
    QDomDocument doc = loadDataBase();
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    while (!n.isNull()) {
        QDomElement e = n.toElement(); 
        if (!e.isNull()) {
            if (e.attribute("name") == project->projectName())
                docElem.removeChild(n);
        }
        n = n.nextSibling();
    }

    QFile file(m_dbfile);
    
    if (file.open(QIODevice::WriteOnly| QIODevice::Text)) {
        QTextStream ts(&file);
        ts << doc.toString();
        file.close();
    } else {
        return false;
    }

    return true;
}

QList<Database::ProjectInfo> Database::allProjects()
{
    Parsers::DatabaseParser parser;
    return parser.allProjects(loadDataBase().toString());
}

QList< Database::ProjectInfo> Database::userProjects(const QString& user)
{
    Parsers::DatabaseParser parser;
    return parser.userProjects(user, loadDataBase().toString());
}

Database::ProjectInfo Database::projectInfo(const QString &project)
{
    Parsers::DatabaseParser parser;
    return parser.projectInfo(project, loadDataBase().toString());
}

bool Database::exists(const QString &projectName)
{
    Parsers::DatabaseParser parser;
    return parser.exists(projectName, loadDataBase().toString());
}

SProject *Database::loadProject(const QString &projectName)
{
    Parsers::DatabaseParser parser;
    return parser.loadProject( projectName, loadDataBase().toString());
}

}
