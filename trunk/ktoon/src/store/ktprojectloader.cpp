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

#include "ktprojectloader.h"
#include "ktproject.h"

#include "ktprojectresponse.h"
#include "ktlibraryobject.h"

KTProjectLoader::KTProjectLoader()
{
}

KTProjectLoader::~KTProjectLoader()
{
}

void KTProjectLoader::createItem(int scenePosition, int layerPosition, int framePosition, int itemPosition, QPointF point, KTLibraryObject::Type type, const QString &xml, KTProject *project)
{
    KTItemResponse response(KTProjectRequest::Item, KTProjectRequest::Add);

    response.setSceneIndex(scenePosition);
    response.setLayerIndex(layerPosition);
    response.setFrameIndex(framePosition);
    response.setItemIndex(itemPosition);
    response.setItemType(type);
    response.setPosX(point.x());
    response.setPosY(point.y());
    
    response.setArg(xml);
    
    project->emitResponse(&response);
}

void KTProjectLoader::createFrame(int scenePosition, int layerPosition, int framePosition, const QString &name, KTProject *project)
{
    KTFrameResponse response(KTProjectRequest::Frame, KTProjectRequest::Add);
    
    response.setSceneIndex(scenePosition);
    response.setLayerIndex(layerPosition);
    response.setFrameIndex(framePosition);
    response.setArg(name);
    
    project->emitResponse(&response);
}


void KTProjectLoader::createLayer(int scenePosition, int layerPosition, const QString &name, KTProject *project)
{
    KTLayerResponse response(KTProjectRequest::Layer, KTProjectRequest::Add);
    
    response.setSceneIndex(scenePosition);
    response.setLayerIndex(layerPosition);
    response.setArg(name);
    
    project->emitResponse(&response);
}

void KTProjectLoader::createSoundLayer(int scenePosition, int layerPosition, const QString &name, KTProject *project)
{
    KTLibraryResponse response(KTProjectRequest::Library, KTProjectRequest::AddSymbolToProject);
    
    response.setSceneIndex(scenePosition);
    response.setLayerIndex(layerPosition);
    response.setArg(name);
    response.setSymbolType(KTLibraryObject::Sound);
    
    project->emitResponse(&response);
}

void KTProjectLoader::createScene(int scenePosition, const QString &name, KTProject *project)
{
    KTSceneResponse response(KTProjectRequest::Scene, KTProjectRequest::Add);

    response.setSceneIndex(scenePosition);
    response.setArg(name);
    
    project->emitResponse(&response);
}

void KTProjectLoader::createSymbol(KTLibraryObject::Type type, const QString &name, const QByteArray &data, KTProject *project)
{
    KTLibraryResponse response(KTProjectRequest::Library, KTProjectRequest::Add);
    
    response.setArg(name);
    response.setData(data);
    response.setSymbolType(type);
    
    project->emitResponse(&response);
}
