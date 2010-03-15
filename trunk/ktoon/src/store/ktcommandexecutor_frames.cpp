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

#include "ktcommandexecutor.h"

#include "ktscene.h"
#include "ktprojectrequest.h"
#include "ktrequestbuilder.h"
#include "ktlayer.h"
#include "ktframe.h"

#include "ktprojectresponse.h"

#include <kcore/kdebug.h>

bool KTCommandExecutor::createFrame(KTFrameResponse *response)
{
    int scenePosition = response->sceneIndex();
    int layerPosition = response->layerIndex();
    int position = response->frameIndex();
    QString name = response->arg().toString();
    
    QString state = response->state();
    
    KTScene *scene = m_project->scene(scenePosition);
    
    if (!scene)
        return false;
    
    KTLayer *layer = scene->layer(layerPosition);
    
    if (layer) {
        KTFrame *frame = layer->createFrame(position);
        
        if (!frame) 
            return false;
        
        if (!name.isEmpty()) {
            #ifdef K_DEBUG
                kDebug("items") << name;
            #endif
            frame->setFrameName(name);
        } else {
            response->setArg(frame->frameName());
        }

        response->setFrameIndex(layer->visualIndexOf(frame));
        emit responsed(response);
        
        if (!state.isEmpty()) {
            frame->fromXml(state);
            response->setArg(frame->frameName());
        }

        /*
        if (layerPosition == 0) {
            frame->setZLevel(0); 
        } else {
            KTLayer *previewlayer = scene->layer(layerPosition-1);
            KTFrame *previewFrame = previewlayer->frame(position);
            if (previewFrame) {
                frame->setZLevel(previewFrame->getTopZLevel());
            }
        }
        */
        
        return true;
    }
    
    return false;
}

bool KTCommandExecutor::removeFrame(KTFrameResponse *response)
{
    int scenePos = response->sceneIndex();
    int layerPos = response->layerIndex();
    int position = response->frameIndex();
    
    KTScene *scene = m_project->scene(scenePos);
    
    if (scene) {
        KTLayer *layer = scene->layer(layerPos);
        if (layer) {
            KTFrame *frame = layer->frame(position);
            if (frame) {
                QDomDocument doc;
                doc.appendChild(frame->toXml(doc));
                response->setArg(frame->frameName());
                
                if (layer->removeFrame(position)) {
                    response->setState(doc.toString(0));
                    emit responsed(response);
                    
                    return true;
                }
            }
        }
    }
    
    return false;
}


bool KTCommandExecutor::moveFrame(KTFrameResponse *response)
{
    int scenePos = response->sceneIndex();
    int layerPos = response->layerIndex();
    int position = response->frameIndex();
    int newPosition = response->arg().toInt();
    KTScene *scene = m_project->scene(scenePos);
    
    if (!scene)
        return false;
    
    KTLayer *layer = scene->layer(layerPos);
    
    if (layer) {
        if (! layer->moveFrame(position, newPosition)) {
            #ifdef K_DEBUG
                kWarning() << "Failed moving frame";
            #endif
            return false;
        } else {
            emit responsed(response);
            return true;
        }
    }
    
    return false;
}


bool KTCommandExecutor::lockFrame(KTFrameResponse *response)
{
    int scenePos = response->sceneIndex();
    int layerPos = response->layerIndex();
    int position = response->frameIndex();
    bool lock = response->arg().toBool();
    
    KTScene *scene = m_project->scene(scenePos);
    
    if (!scene)
        return false;
    
    KTLayer *layer = scene->layer(layerPos);
    
    if (layer) {
        KTFrame *frame = layer->frame(position);
        
        if (! frame) 
            return false;
        
        frame->setLocked(lock);
        emit responsed(response);

        return true;
    }
    
    return false;
}


bool KTCommandExecutor::renameFrame(KTFrameResponse *response)
{
    int scenePos = response->sceneIndex();
    int layerPos = response->layerIndex();
    int position = response->frameIndex();
    QString newName= response->arg().toString();
    
    QString oldName;
    
    KTScene *scene = m_project->scene(scenePos);
    
    if (!scene)
        return false;
    
    KTLayer *layer = scene->layer(layerPos);
    
    if (layer) {
        KTFrame *frame = layer->frame(position);
        
        if (! frame) 
            return false;
        
        oldName = frame->frameName();
        
        frame->setFrameName(newName);
        
        emit responsed(response);

        return true;
    }
    
    return false;
}


bool KTCommandExecutor::selectFrame(KTFrameResponse *response)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    int scenePos = response->sceneIndex();
    int layerPos = response->layerIndex();
    int position = response->frameIndex();
    // bool prioritary = response->arg().toBool();
    
    KTScene *scene = m_project->scene(scenePos);
    
    if (!scene)
        return false;
    
    KTLayer *layer = scene->layer(layerPos);
    
    if (layer) {
        KTFrame *frame = layer->frame(position);
        
        if (! frame) 
            return false;
        
        emit responsed(response);

        return true;
    } 
    
    return false;
}


bool KTCommandExecutor::setFrameVisibility(KTFrameResponse *response)
{
    int scenePos = response->sceneIndex();
    int layerPos = response->layerIndex();
    int position = response->frameIndex();
    bool view = response->arg().toBool();
    
    KTScene *scene = m_project->scene(scenePos);
    
    if (!scene)
        return false;
    
    KTLayer *layer = scene->layer(layerPos);
    
    if (layer) {
        KTFrame *frame = layer->frame(position);
        
        if (! frame) 
            return false;
        
        frame->setVisible(view);
        emit responsed(response);

        return true;
    }
    
    return false;
}

bool KTCommandExecutor::expandFrame(KTFrameResponse *response)
{
    int scenePos = response->sceneIndex();
    int layerPos = response->layerIndex();
    int position = response->frameIndex();
    
    KTScene *scene = m_project->scene(scenePos);
    
    if (!scene)
        return false;
    
    KTLayer *layer = scene->layer(layerPos);
    
    if (layer) {
        if (layer->expandFrame(position, 1)) {
            emit responsed(response);

            return true;
        }
    }

    return false;
}


bool KTCommandExecutor::pasteFrame( KTFrameResponse *response)
{
    int scenePos = response->sceneIndex();
    int layerPos = response->layerIndex();
    int position = response->frameIndex();
    QString copyFrame = response->arg().toString();
    
    KTScene *scene = m_project->scene(scenePos);
    if (scene) {
        KTLayer *layer = scene->layer(layerPos);
        if (layer) {
            KTFrame *frame = layer->frame(position);
            if (frame) {
                QString name = frame->frameName();
                
                QDomDocument doc;
                doc.appendChild(frame->toXml(doc));
                response->setArg(doc.toString(0));
                frame->clean();
                frame->fromXml(copyFrame);
                frame->setFrameName(name);
                emit responsed(response);

                return true;
            }
        }
    }
    return false;
}
