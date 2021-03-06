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

#include "ktprojectmanager.h"

#include <QUndoStack>
#include <QDir>

#include "ktproject.h"

#include "ktscene.h"
#include "ktlayer.h"
#include "ktframe.h"

#include "ktprojectrequest.h"
#include "ktprojectcommand.h"
#include "ktcommandexecutor.h"

#include "ktprojectmanagerparams.h"
#include "ktabstractprojectmanagerhandler.h"

#include "ktprojectresponse.h"
#include "ktrequestbuilder.h"
#include "ktrequestparser.h"

#include <kcore/kalgorithm.h>
#include <kcore/kdebug.h>

// This class handles the current animation project 

class KTProjectManager::Private
{
    public:
        Private() : handler(0), params(0)
        {
        }
        ~Private()
        {
           delete handler;
           delete undoStack;
           delete commandExecutor;
           delete params;
        }

    public:
        KTProject *project;
        bool isModified;
        int sceneIndex;
        int layerIndex;
        int frameIndex;
        KTAbstractProjectHandler *handler;
        QUndoStack *undoStack;
        KTCommandExecutor *commandExecutor;
        KTProjectManagerParams *params;
        QString copyFrame;
};

KTProjectManager::KTProjectManager(QObject *parent) : QObject(parent), k(new Private())
{
    #ifdef K_DEBUG
           KINIT;
    #endif

    k->isModified = false;
    k->handler = 0;

    k->project = new KTProject(this);
    k->undoStack = new QUndoStack(this);
    k->commandExecutor = new KTCommandExecutor(k->project);

    connect(k->commandExecutor, SIGNAL(responsed(KTProjectResponse*)), this, SLOT(emitResponse(KTProjectResponse *)));
    connect(k->project, SIGNAL(responsed(KTProjectResponse*)), this, SIGNAL(responsed(KTProjectResponse *)));
}

KTProjectManager::~KTProjectManager()
{
    #ifdef K_DEBUG
           KEND;
    #endif

    delete k;
}

void KTProjectManager::setParams(KTProjectManagerParams *params)
{
    if (k->params) 
        delete k->params;

    k->params = params;
    k->handler->initialize(k->params);
}

KTProjectManagerParams *KTProjectManager::params() const
{
    return k->params;
}

void KTProjectManager::setHandler(KTAbstractProjectHandler *handler)
{
    if (k->handler) {
        disconnect(k->handler, SIGNAL(sendCommand(const KTProjectRequest *, bool)), this, SLOT(createCommand(const KTProjectRequest *, bool)));
        disconnect(k->handler, SIGNAL(sendLocalCommand(const KTProjectRequest *)), this, SLOT(handleLocalRequest(const KTProjectRequest *)));
        delete k->handler;
        k->handler = 0;
    }

    k->handler = handler;
    k->handler->setParent(this);
    k->handler->setProject(k->project);

    connect(k->handler, SIGNAL(sendCommand(const KTProjectRequest *, bool)), this, SLOT(createCommand(const KTProjectRequest *, bool)));
    connect(k->handler, SIGNAL(sendLocalCommand(const KTProjectRequest *)), this, SLOT(handleLocalRequest(const KTProjectRequest *)));
}

KTAbstractProjectHandler *KTProjectManager::handler() const
{
    return k->handler;
}

void KTProjectManager::setupNewProject()
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    if (!k->handler || !k->params) {
        qDebug("ERROR: HANDLER!");
        return;
    }

    closeProject();

    k->project->setProjectName(k->params->projectName());
    k->project->setAuthor(k->params->author());
    k->project->setDimension(k->params->dimension());
    k->project->setFPS(k->params->fps());

    if (! k->handler->setupNewProject(k->params)) {
        qDebug("ERROR WHILE SETUP PROJECT");
        return;
    }

    k->project->setOpen(true);
    setupProjectDir();

    KTProjectRequest request = KTRequestBuilder::createSceneRequest(0, KTProjectRequest::Add, QString());
    handleProjectRequest(&request);

    request = KTRequestBuilder::createLayerRequest(0, 0, KTProjectRequest::Add, QString());
    handleProjectRequest(&request);

    request = KTRequestBuilder::createFrameRequest(0, 0, 0, KTProjectRequest::Add, QString());
    handleProjectRequest(&request);
}


void KTProjectManager::closeProject()
{
    if (!k->handler) 
        return;

    if (k->project->isOpen()) {
        if (! k->handler->closeProject()) {
            qDebug("ERROR: WHILE CLOSING THE PROJECT");
            return;
        }

        k->project->clear();
    }

    k->project->setOpen(false);
    k->isModified = false;
    k->undoStack->clear();
}

bool KTProjectManager::saveProject(const QString &fileName)
{
    bool result = k->handler->saveProject(fileName, k->project);
    k->isModified = !result;

    return result;
}

bool KTProjectManager::loadProject(const QString &fileName)
{
    if (! k->handler) {
        #ifdef K_DEBUG
               kFatal() << "NO HANDLER!";
        #endif
        return false;
    }

    bool ok = k->handler->loadProject(fileName, k->project);

    if (ok) {
        k->project->setOpen(true);
        k->isModified = false;
    }

    return ok;
}

/**
 * Returns true if project is open
 */
bool KTProjectManager::isOpen() const
{
    return k->project->isOpen();
}

bool KTProjectManager::isModified() const
{
    return k->isModified;
}

void KTProjectManager::undoModified()
{
    k->isModified = false;
}

bool KTProjectManager::isValid() const
{
    if (!k->handler) 
        return false;

    return k->handler->isValid();
}

void KTProjectManager::setupProjectDir()
{
    QString dataDir = CACHE_DIR + "/" + (k->project->projectName().isEmpty() ? KAlgorithm::randomString(6) : k->project->projectName());
    QDir project = dataDir;

    if (!project.exists()) {
        if (project.mkpath(project.absolutePath())) {
            QStringList dirs;
            dirs << "audio" << "images" << "video";

            foreach (QString dir, dirs)
                     project.mkdir(dir);
        }
    }
}

/**
 * This function is called when some event is triggered by the project
 * It must be re-implemented if you want to deal with the event in another way, i.ex: send it through the net.
 * By default, it sends the event through the signal commandExecuted
 * @param event 
 */
void KTProjectManager::handleProjectRequest(const KTProjectRequest *request)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
           kWarning() << "Package: ";
           kWarning() << request->xml();
    #endif

    // TODO: the handler must advise when to build the command
    
    if (k->handler)
        k->handler->handleProjectRequest(request);
    else
        qDebug("ERROR: NO HANDLER");
}

void KTProjectManager::handleLocalRequest(const KTProjectRequest *request)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
           kWarning() << "Package: ";
           kWarning() << request->xml();
    #endif

    KTRequestParser parser;

    if (parser.parse(request->xml())) {
        if (KTFrameResponse *response = static_cast<KTFrameResponse *>(parser.response())) {
            k->sceneIndex = response->sceneIndex();
            k->layerIndex = response->layerIndex();
            k->frameIndex = response->frameIndex();

            if (response->action() == KTProjectRequest::Copy) {
                KTScene *scene = k->project->scene(k->sceneIndex);
                if (scene) {
                    KTLayer *layer = scene->layer(k->layerIndex);
                    if (layer) {
                        KTFrame *frame = layer->frame(k->frameIndex);
                        if (frame) {
                            QDomDocument doc;
                            doc.appendChild(frame->toXml(doc));
                            k->copyFrame = doc.toString(0);
                            response->setArg(k->copyFrame);
                        }
                    }
                }
            } else if (response->action() == KTProjectRequest::Paste) {
                       response->setArg(k->copyFrame);
                       handleProjectRequest(& KTRequestBuilder::fromResponse(response));
                       return;
            }
        }
        parser.response()->setExternal(request->isExternal());

        emit responsed(parser.response());
    }
}

/**
 * This function creates a command to execute an action, i.e. add a frame. 
 * The command has the information necessary to undo its effect.
 * Usually this command must be added in the commands stack.
 * The command created is not deleted by this class, this task depends on the user.
 * @param event 
 * @return 
 */
void KTProjectManager::createCommand(const KTProjectRequest *request, bool addToStack)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    if (request->isValid()) {
        KTProjectCommand *command = new KTProjectCommand(k->commandExecutor, request);

        if (addToStack)
            k->undoStack->push(command);
         else 
            command->redo();
    } else {
        #ifdef K_DEBUG
               kWarning() << "invalid request";
        #endif
    }
}

KTProject *KTProjectManager::project() const
{
    return k->project;
}

QUndoStack *KTProjectManager::undoHistory() const
{
    return k->undoStack;
}

void KTProjectManager::emitResponse(KTProjectResponse *response)
{
    #ifdef K_DEBUG
           K_FUNCINFO << response->action();
    #endif

    if (response->action() != KTProjectRequest::Select)
        k->isModified = true;

    if (!k->handler) {
        emit responsed(response);
    } else if (k->handler->commandExecuted(response)) {
        emit responsed(response);
    }
}
