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
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
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

#include "ktcommandexecutor.h"
#include "ktproject.h"
#include "ktprojectrequest.h"

#include "ktprojectresponse.h"
#include <QDebug>

#include <kcore/kdebug.h>

bool KTCommandExecutor::createSymbol(KTLibraryResponse *response)
{
    if (m_project->createSymbol(response->symbolType(), response->arg().toString(), response->data())) {
        emit responsed(response);

        return true;
    } 
    
    return false;
}


bool KTCommandExecutor::removeSymbol(KTLibraryResponse *response)
{
    if ((response->sceneIndex() > -1) && (response->layerIndex() > -1) && (response->frameIndex() > -1)) {
        if (m_project->removeSymbol(response->arg().toString(), response->symbolType(), response->sceneIndex(), response->layerIndex(), response->frameIndex())) {
            emit responsed(response);
            return true;
        } 
    } else {
        if (m_project->removeSymbol(response->arg().toString())) {
            emit responsed(response);
            return true;
        }
    }
    
    return false;
}

bool KTCommandExecutor::addSymbolToProject(KTLibraryResponse *response)
{
    if (m_project->addSymbolToProject(response->arg().toString(), response->sceneIndex(), response->layerIndex(), response->frameIndex())) {
        emit responsed(response);

        return true;
    } 
    
    return false;
}

bool KTCommandExecutor::removeSymbolFromProject(KTLibraryResponse *response)
{
    kFatal() << "KTCommandExecutor::removeSymbolFromProject - Pending for implementation!";

    return false;
}
