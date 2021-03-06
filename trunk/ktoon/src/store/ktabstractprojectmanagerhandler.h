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

#ifndef KTABSTRACTPROJECTHANDLER_H
#define KTABSTRACTPROJECTHANDLER_H

#include <qobject.h>
#include "ktglobal_store.h"

class KTProjectResponse;
class KTProjectRequest;
class KTProjectManagerParams;
class KTProjectCommand;
class KTProject;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class STORE_EXPORT KTAbstractProjectHandler : public QObject
{
    Q_OBJECT;

    public:
        KTAbstractProjectHandler(QObject *parent = 0);
        virtual ~KTAbstractProjectHandler();

        virtual bool initialize(KTProjectManagerParams *params);
        virtual bool setupNewProject(KTProjectManagerParams *params);
        virtual bool closeProject();
        virtual void handleProjectRequest(const KTProjectRequest *request) = 0;
        virtual bool commandExecuted(KTProjectResponse *response);
        virtual bool saveProject(const QString &fileName, const KTProject *project) = 0;
        virtual bool loadProject(const QString &fileName, KTProject *project) = 0;
        virtual void setProject(KTProject *project) = 0;
        virtual bool isValid() const;

    signals:
        void sendCommand(const KTProjectRequest *request, bool addToStack);
        void sendLocalCommand(const KTProjectRequest *request);
        void openNewArea(const QString &name);
};

#endif
