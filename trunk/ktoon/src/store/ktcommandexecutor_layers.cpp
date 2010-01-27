/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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

#include "ktcommandexecutor.h"

#include "ktscene.h"
#include "ktlayer.h"

#include "ktprojectrequest.h"
#include "ktrequestbuilder.h"
#include "ktprojectresponse.h"

#include <kcore/kdebug.h>

bool KTCommandExecutor::createLayer(KTLayerResponse *response)
{
    int scenePosition = response->sceneIndex();
    int position = response->layerIndex();

    QString name = response->arg().toString();
    QString state = response->state();

    KTScene *scene = m_project->scene(scenePosition);

    if (scene) {
        KTLayer *layer = scene->createLayer(position);

        if (! layer) 
            return false;

        if (!name.isEmpty())
            layer->setLayerName(name);
        else
            response->setArg(layer->layerName());

        emit responsed(response);

        layer->fromXml(state);
        response->setArg(layer->layerName());

        return true;
    }

    return false;
}


bool KTCommandExecutor::removeLayer(KTLayerResponse *response)
{
    int scenePos = response->sceneIndex();
    int position = response->layerIndex();

    KTScene *scene = m_project->scene(scenePos);

    if (scene) {
        KTLayer *layer = scene->layer(position);
        if (layer) {
            QDomDocument document;
            document.appendChild(layer->toXml(document));
            response->setState(document.toString());
            response->setArg(layer->layerName());

            if (scene->removeLayer(position)) {
                emit responsed(response);

                return true;
            }
        }
    }

    return false;
}


bool KTCommandExecutor::moveLayer(KTLayerResponse *response)
{
    int scenePos = response->sceneIndex();
    int position = response->layerIndex();
    int newPosition = response->arg().toInt();

    KTScene *scene = m_project->scene(scenePos);

    if (!scene)
        return false;

    if (! scene->moveLayer(position, newPosition)) {
        #ifdef K_DEBUG
               kWarning() << "Failed moving layer";
        #endif
        return false;
    } else {
        emit responsed(response);
        return true;
    }

    return false;
}

bool KTCommandExecutor::lockLayer(KTLayerResponse *response)
{
    int scenePos = response->sceneIndex();
    int position = response->layerIndex();
    bool lock = response->arg().toBool();

    KTScene *scene = m_project->scene(scenePos);

    if (!scene)
        return false;

    KTLayer *layer = scene->layer(position);

    if (layer) {
        layer->setLocked(lock);
        emit responsed(response);
        return true;
    }

    return false;
}

bool KTCommandExecutor::renameLayer(KTLayerResponse *response)
{
    int scenePos = response->sceneIndex();
    int position = response->layerIndex();
    QString newName = response->arg().toString();

    #ifdef K_DEBUG
           kWarning() << "Renamed layer: " << newName;
    #endif

    QString oldName;

    KTScene *scene = m_project->scene(scenePos);

    if (!scene)
        return false;

    KTLayer *layer = scene->layer(position);

    if (layer) {
        QString current = layer->layerName();
        layer->setLayerName(newName);

        emit responsed(response);
        response->setArg(current);

        return true;
    }

    return false;
}

bool KTCommandExecutor::selectLayer(KTLayerResponse *response)
{
    emit responsed(response);

    return true;
}


bool KTCommandExecutor::setLayerVisibility(KTLayerResponse *response)
{
    int scenePos = response->sceneIndex();
    int position = response->layerIndex();
    bool view = response->arg().toBool();

    KTScene *scene = m_project->scene(scenePos);

    if (!scene)
        return false;

    KTLayer *layer = scene->layer(position);

    if (layer) {
        layer->setVisible(view);
        responsed(response);

        return true;
    }

    return false;
}
