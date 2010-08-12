/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
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
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "ktsaveproject.h"

#include <QDir>

#include "ktproject.h"
#include "ktscene.h"
#include "ktlibrary.h"
#include "ktpackagehandler.h"

#include <kcore/kalgorithm.h>

#include <kcore/kdebug.h>

KTSaveProject::KTSaveProject() : QObject()
{
}

KTSaveProject::~KTSaveProject()
{
}

bool KTSaveProject::save(const QString &fileName, const KTProject *project)
{
    QDir projectDir = CACHE_DIR + "/" + project->projectName();

    #ifdef K_DEBUG
           kDebug("project") << "Saving project to: " << projectDir.absolutePath();
    #endif

    if (!projectDir.exists()) {
        if (! projectDir.mkdir(projectDir.path())) {
            #ifdef K_DEBUG
                   kFatal() <<("Can't save project");
            #endif
            return false;
        }
    }

    {
     // Save project
     QFile prj(projectDir.path()+"/project.ktp");

     if (prj.open(QIODevice::WriteOnly | QIODevice::Text)) {
         QTextStream ts(&prj);
         QDomDocument doc;
         doc.appendChild(project->toXml(doc));
         ts << doc.toString();
         prj.close();
     }
    }

    // Save scenes
    {
     int index = 0;
     foreach (KTScene *scene, project->scenes().values()) {
              QDomDocument doc;
              doc.appendChild(scene->toXml(doc));

              QFile scn(projectDir.path()+"/scene"+QString::number(index)+".kts");

              if (scn.open(QIODevice::WriteOnly | QIODevice::Text)) {
                  QTextStream st(&scn);
                  st << doc.toString();
                  index += 1;
                  scn.close();
              }
     }
    }

    {
     // Save library
     QFile lbr(projectDir.path()+"/library.ktl");

     if (lbr.open(QIODevice::WriteOnly | QIODevice::Text)) {
         QTextStream ts(&lbr);

         QDomDocument doc;
         doc.appendChild(project->library()->toXml(doc));

         ts << doc.toString();
         lbr.close();
     }
    }

    KTPackageHandler packageHandler;
    bool ok = packageHandler.makePackage(projectDir.path(), fileName);

    if (ok) {
        #ifdef K_DEBUG
               kWarning() << tr("Project saved in %1!").arg(fileName);
        #endif
    }

    return ok;
}

bool KTSaveProject::load(const QString &fileName, KTProject *project)
{
    #ifdef K_DEBUG
           K_FUNCINFO << fileName;
    #endif

    KTPackageHandler packageHandler;

    if (packageHandler.importPackage(fileName)) {
        QDir projectDir(packageHandler.importedProjectPath());

        QFile pfile(projectDir.path() + "/project.ktp");

        if (pfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            project->fromXml(QString::fromLocal8Bit(pfile.readAll()));
            pfile.close();
        } else {
            #ifdef K_DEBUG
                   kError() << "KTSaveProject::load() - Error: Can't find .ktp file!";
                   kError() << "KTSaveProject::load() - Error while open file: " << pfile.fileName();
                   kError() << "KTSaveProject::load() - Error Description: " << pfile.errorString();
                   kError() << "KTSaveProject::load() - Error type: " << pfile.error();
            #endif
            return false;
        }

        project->loadLibrary(projectDir.path() + "/library.ktl");

        QStringList scenes = projectDir.entryList(QStringList() << "*.kts", QDir::Readable | QDir::Files);

         int index = 0;
         foreach (QString scenePath, scenes) {
                  scenePath = projectDir.path() + "/" + scenePath;
                  KTScene *scene = project->createScene(index, true);

                  QFile f(scenePath);

                  if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
                      QString xml = QString::fromLocal8Bit(f.readAll());
                      scene->fromXml(xml);

                      index += 1;
                      f.close();
                  }
         }

         project->setOpen(true);

         return true;
    }

    return false;
}
