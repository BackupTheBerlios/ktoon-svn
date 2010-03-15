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

#include "ktrequestbuilder.h"

#include <QDomDocument>

#include <kcore/kdebug.h>

#include "ktprojectrequest.h"
#include "ktprojectresponse.h"

KTRequestBuilder::KTRequestBuilder()
{
}

KTRequestBuilder::~KTRequestBuilder()
{
}

KTProjectRequest KTRequestBuilder::createItemRequest(int sceneIndex, int layerIndex, int frameIndex, int itemIndex, int actionId, const QVariant &arg, const QByteArray &data)
{
    QDomDocument doc;

    QDomElement root = doc.createElement("request");

    QDomElement scene = doc.createElement("scene");
    scene.setAttribute("index", sceneIndex);

    QDomElement layer = doc.createElement("layer");
    layer.setAttribute("index", layerIndex);

    QDomElement frame = doc.createElement("frame");
    frame.setAttribute("index", frameIndex);

    QDomElement item = doc.createElement("item");
    item.setAttribute("index", itemIndex);

    QDomElement action = doc.createElement("action");
    action.setAttribute("id", actionId);
    action.setAttribute("arg", arg.toString());
    action.setAttribute("part", KTProjectRequest::Item);

    KTRequestBuilder::appendData(doc, action, data);
    root.appendChild(action);
    frame.appendChild(item);
    layer.appendChild(frame);
    scene.appendChild(layer);
    root.appendChild(scene);

    doc.appendChild(root);
    KTProjectRequest request(doc.toString(0));

    return request;
}

KTProjectRequest KTRequestBuilder::createFrameRequest(int sceneIndex, int layerIndex, int frameIndex, int actionId, const QVariant &arg, const QByteArray &data)
{
    QDomDocument doc;

    QDomElement root = doc.createElement("request");

    QDomElement scene = doc.createElement("scene");
    scene.setAttribute("index", sceneIndex);

    QDomElement layer = doc.createElement("layer");
    layer.setAttribute("index", layerIndex);

    QDomElement frame = doc.createElement("frame");
    frame.setAttribute("index", frameIndex);

    QDomElement action = doc.createElement("action");
    action.setAttribute("id", actionId);
    action.setAttribute("arg", arg.toString());
    action.setAttribute("part", KTProjectRequest::Frame);

    KTRequestBuilder::appendData(doc, action, data);

    root.appendChild(action);
    layer.appendChild(frame);
    scene.appendChild(layer);
    root.appendChild(scene);
    doc.appendChild(root);

    return KTProjectRequest(doc.toString(0));
}


KTProjectRequest KTRequestBuilder::createLayerRequest(int sceneIndex, int layerIndex, int actionId, const QVariant &arg, const QByteArray &data)
{
    QDomDocument doc;

    QDomElement root = doc.createElement("request");

    QDomElement scene = doc.createElement("scene");
    scene.setAttribute("index", sceneIndex);

    QDomElement layer = doc.createElement("layer");
    layer.setAttribute("index", layerIndex);

    QDomElement action = doc.createElement("action");
    action.setAttribute("id", actionId);
    action.setAttribute("arg", arg.toString());
    action.setAttribute("part", KTProjectRequest::Layer);

    KTRequestBuilder::appendData(doc, action, data);

    root.appendChild(action);
    scene.appendChild(layer);
    root.appendChild(scene);

    doc.appendChild(root);

    return KTProjectRequest(doc.toString(0));
}

KTProjectRequest KTRequestBuilder::createSceneRequest(int sceneIndex, int actionId, const QVariant &arg, const QByteArray &data)
{
    QDomDocument doc;
    QDomElement root = doc.createElement("request");

    QDomElement scene = doc.createElement("scene");
    scene.setAttribute("index", sceneIndex);

    QDomElement action = doc.createElement("action");
    action.setAttribute("id", actionId);
    action.setAttribute("arg", arg.toString());
    action.setAttribute("part", KTProjectRequest::Scene);

    KTRequestBuilder::appendData(doc, action, data);

    root.appendChild(action);
    root.appendChild(scene);

    doc.appendChild(root);

    return KTProjectRequest(doc.toString(0));
}

KTProjectRequest KTRequestBuilder::createLibraryRequest(int actionId, const QVariant &arg, KTLibraryObject::Type type, const QByteArray &data, int sceneIndex, int layerIndex, int frameIndex)
{
    QDomDocument doc;
    QDomElement root = doc.createElement("request");

    QDomElement scene = doc.createElement("scene");
    scene.setAttribute("index", sceneIndex);

    QDomElement layer = doc.createElement("layer");
    layer.setAttribute("index", layerIndex);

    QDomElement frame = doc.createElement("frame");
    frame.setAttribute("index", frameIndex);

    QDomElement library = doc.createElement("library");

    QDomElement symbol = doc.createElement("symbol");
    symbol.setAttribute("type", type);

    QDomElement action = doc.createElement("action");
    action.setAttribute("id", actionId);
    action.setAttribute("arg", arg.toString());
    action.setAttribute("part", KTProjectRequest::Library);

    KTRequestBuilder::appendData(doc, action, data);

    root.appendChild(action);

    library.appendChild(symbol);

    root.appendChild(library);

    root.appendChild(scene);
    scene.appendChild(layer);
    layer.appendChild(frame);

    doc.appendChild(root);

    return KTProjectRequest(doc.toString(0));
}

void KTRequestBuilder::appendData(QDomDocument &doc, QDomElement &element, const QByteArray &data)
{
    if (!data.isNull() && !data.isEmpty()) {
        QDomElement edata = doc.createElement("data");

        QDomCDATASection cdata = doc.createCDATASection(QString(data.toBase64()));

        edata.appendChild(cdata);
        element.appendChild(edata);
    }
}

KTProjectRequest KTRequestBuilder::fromResponse(KTProjectResponse *response)
{
    KTProjectRequest request;

    switch (response->part()) {
            case KTProjectRequest::Item:
                 {
                    request = KTRequestBuilder::createItemRequest(static_cast<KTItemResponse*> (response)->sceneIndex(), static_cast<KTItemResponse*> (response)->layerIndex(), static_cast<KTItemResponse*> (response)->frameIndex(), static_cast<KTItemResponse*> (response)->itemIndex(), response->action(), response->arg().toString(), response->data());
                 }
            break;
            case KTProjectRequest::Frame:
                 {
                    request = KTRequestBuilder::createFrameRequest(static_cast<KTFrameResponse*> (response)->sceneIndex(), static_cast<KTFrameResponse*> (response)->layerIndex(), static_cast<KTFrameResponse*> (response)->frameIndex(), response->action(), response->arg().toString(), response->data());
                 }
            break;
            case KTProjectRequest::Layer:
                 {
                    request = KTRequestBuilder::createLayerRequest(static_cast<KTLayerResponse*> (response)->sceneIndex(), static_cast<KTLayerResponse*> (response)->layerIndex(), response->action(), response->arg().toString(), response->data());
                 }
            break;
            case KTProjectRequest::Scene:
                 {
                    request = KTRequestBuilder::createSceneRequest(static_cast<KTSceneResponse*> (response)->sceneIndex(), response->action(), response->arg().toString(), response->data());
                 }
            break;
            case KTProjectRequest::Library:
                 {
                    request = KTRequestBuilder::createLibraryRequest(response->action(), response->arg().toString(), KTLibraryObject::Type(static_cast<KTLibraryResponse*>(response)->symbolType()), response->data(), static_cast<KTLibraryResponse*>(response)->sceneIndex(), static_cast<KTLibraryResponse*>(response)->layerIndex(), static_cast<KTLibraryResponse*>(response)->frameIndex());
                 }
            break;
            default:
                 {
                    #ifdef K_DEBUG
                           kWarning() << "wOw! Unknown response! O_o";
                    #endif
                 }
    }

    return request;
}
