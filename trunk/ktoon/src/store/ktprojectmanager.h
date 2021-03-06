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

#ifndef KTPROJECTMANAGER_H
#define KTPROJECTMANAGER_H

#include <QObject>
#include "ktglobal_store.h"
// #include "ktcommandexecutor.h"

class KTProject;
class KTProjectRequest;
class KTProjectCommand;
class KTProjectManagerParams;
class KTAbstractProjectHandler;
class QUndoStack;
class KTCommandExecutor;
class KTProjectResponse;

/**
 * Events handler class for the project
 * @author David Cuadrado \<krawek@gmail.com\>
*/
class STORE_EXPORT KTProjectManager : public QObject
{
    Q_OBJECT;
    public:
        KTProjectManager(QObject *parent = 0);
        virtual ~KTProjectManager();

        void setParams(KTProjectManagerParams *params);
        KTProjectManagerParams *params() const;

        virtual void setupNewProject();
        virtual void closeProject();

        bool isOpen() const;
        bool isModified() const;
        KTProject *project() const;
        void setHandler(KTAbstractProjectHandler *handler);
        KTAbstractProjectHandler *handler() const;

        QUndoStack *undoHistory() const;

        virtual bool saveProject(const QString &fileName);
        virtual bool loadProject(const QString &fileName);

        bool isValid() const;
        void undoModified();
    
    private:
        void setupProjectDir();

    protected slots:
        virtual void handleProjectRequest(const KTProjectRequest *request);
        virtual void handleLocalRequest(const KTProjectRequest *request);
        virtual void createCommand(const KTProjectRequest *request, bool addToStack);

    private slots:
        void emitResponse(KTProjectResponse *response);

    signals:
        void responsed(KTProjectResponse *reponse);
        void requestOpenProject(const QString& filename);
        //void updateAnimationModule(KTProject *project, int sceneIndex, int layerIndex, int frameIndex);

    private:
        class Private;
        Private *const k;
};

#endif
