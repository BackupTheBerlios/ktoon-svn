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

#include "ktlocalprojectmanagerhandler.h"
#include "ktprojectrequest.h"
#include "ktprojectcommand.h"
#include "ktsaveproject.h"

#include <kcore/kdebug.h>

/**
 * This class handles all the procedures related to the projects local format for KToon.
 *
 * @author David Cuadrado <krawek@toonka.com>
*/

KTLocalProjectManagerHandler::KTLocalProjectManagerHandler(QObject *parent) : KTAbstractProjectHandler(parent)
{
}

KTLocalProjectManagerHandler::~KTLocalProjectManagerHandler()
{
}

void KTLocalProjectManagerHandler::handleProjectRequest(const KTProjectRequest *request)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    if (request->isValid()) {
        emit sendCommand( request, true );
    } else {
        #ifdef K_DEBUG
               kfDebug << "INVALID REQUEST! ID: " << request->id();
        #endif
    }
}

bool KTLocalProjectManagerHandler::saveProject(const QString &fileName, const KTProject *project)
{
    bool result = false;
    QString fn = fileName;

    if (!fileName.endsWith(".ktn"))
        fn+=".ktn";

    KTSaveProject *saver = new KTSaveProject;
    result = saver->save(fn, project);

    delete saver;

    return result;
}

bool KTLocalProjectManagerHandler::loadProject(const QString &fileName, KTProject *project)
{
    bool result = false;
    KTSaveProject *loader = 0;

    if (fileName.endsWith(".ktn")) {
        loader = new KTSaveProject;
        result = loader->load(fileName, project);
        delete loader;
    } else {
        result = false;
    }

    return result;
}

void KTLocalProjectManagerHandler::setProject(KTProject *)
{

}
