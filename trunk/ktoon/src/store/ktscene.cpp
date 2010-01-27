/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#include "ktscene.h"
#include <kcore/kdebug.h>

#include <QDir>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>

#include "ktgraphicobject.h"
#include "ktsoundlayer.h"

#include "ktitemgroup.h"
#include "ktprojectloader.h"
#include "ktitemfactory.h"

struct KTScene::Private
{
    Layers layers;
    SoundLayers soundLayers;
    QString name;
    bool isLocked;
    int layerCount;
    bool isVisible;

    QList<KTGraphicObject *> tweeningObjects;
};

KTScene::KTScene(KTProject *parent) : QObject(parent), k(new Private)
{
    k->isLocked = false;
    k->layerCount = 0;
    k->isVisible = true;
}

KTScene::~KTScene()
{
    #ifdef K_DEBUG
           KEND;
    #endif

    k->layers.clear(true);
    delete k;
}

void KTScene::setSceneName(const QString &name)
{
    k->name = name;
}

void KTScene::setLocked(bool isLocked)
{
    k->isLocked = isLocked;
}

QString KTScene::sceneName() const
{
    return k->name;
}

bool KTScene::isLocked() const
{
    return k->isLocked;
}

void KTScene::setVisible(bool isVisible)
{
    k->isVisible = isVisible;
}

bool KTScene::isVisible() const
{
    return k->isVisible;
}

Layers KTScene::layers() const
{
    return k->layers;
}

SoundLayers KTScene::soundLayers() const
{
    return k->soundLayers;
}

/**
 * Set the layers list, this function overwrites the old layers
 */
void KTScene::setLayers(const Layers &layers)
{
    k->layers = layers;
}

KTLayer *KTScene::createLayer(int position, bool loaded)
{
    #ifdef K_DEBUG
           K_FUNCINFO << position;
    #endif

    if (position < 0 || position > k->layers.count()) {
        #ifdef K_DEBUG
               kDebug() << "Error in createLayer";
        #endif
        return 0;
    }

    KTLayer *layer = new KTLayer(this);

    k->layerCount++;
    layer->setLayerName(tr("Layer %1").arg(k->layerCount));

    k->layers.insert(position, layer);

    kFatal() << "*** Layer logical position from KTScene: " << position;

    if (loaded)
        KTProjectLoader::createLayer(project()->visualIndexOf(this), position, layer->layerName(), project());

    return layer;
}

KTSoundLayer *KTScene::createSoundLayer(int position, bool loaded)
{
    #ifdef K_DEBUG
           K_FUNCINFO << position;
    #endif

    if (position < 0 || position > k->soundLayers.count()) {
        kDebug() << "Error in createLayer";
        return 0;
    }

    KTSoundLayer *layer = new KTSoundLayer(this);
    k->layerCount++;

    layer->setLayerName(tr("Sound layer %1").arg(k->layerCount));

    k->soundLayers.insert( position, layer);

    if (loaded)
        KTProjectLoader::createSoundLayer(visualIndex(), position, layer->layerName(), project());

    return layer;
}

bool KTScene::removeLayer( int position)
{
    KTLayer *layer = this->layer(position);

    if (layer) {
        k->layers.remove(position);
        delete layer;

        return true;
    }

    return false;
}

/**
 * Retorna el layer que se encuentra en la posicion indicada
 * @param position 
 * @return 
 */
KTLayer *KTScene::layer(int position) const
{
    if (position < 0 || position >= k->layers.count()) {
        #ifdef K_DEBUG
               K_FUNCINFO << " FATAL ERROR: index out of bound " << position;
        #endif
        return 0;
    }

    return k->layers.visualValue(position);
}

KTSoundLayer *KTScene::soundLayer(int position) const
{
    if (position < 0 || position >= k->soundLayers.count()) {
        #ifdef K_DEBUG
               K_FUNCINFO << " FATAL ERROR: index out of bound " << position;
        #endif
        return 0;
    }

    return k->soundLayers.visualValue(position);
}

void KTScene::fromXml(const QString &xml)
{
    QDomDocument document;

    if (! document.setContent(xml))
        return;

    QDomElement root = document.documentElement();

    setSceneName(root.attribute("name", sceneName()));

    QDomNode n = root.firstChild();

    while (!n.isNull()) {
           QDomElement e = n.toElement();

           if (!e.isNull()) {
               if (e.tagName() == "layer") {
                   int pos = k->layers.count();
                   KTLayer *layer = createLayer(pos, true);

                   if (layer) {
                       QString newDoc;
                       {
                         QTextStream ts(&newDoc);
                         ts << n;
                       }

                       layer->fromXml(newDoc);
                   }
               } else if (e.tagName() == "soundlayer") {
                          int pos = k->soundLayers.count();
                          KTSoundLayer *layer = createSoundLayer(pos, true);

                          if (layer) {
                              QString newDoc;
                               {
                                 QTextStream ts(&newDoc);
                                 ts << n;
                               }

                              layer->fromXml( newDoc );
                          }
               }
           }
           n = n.nextSibling();
    } // end while
}

QDomElement KTScene::toXml(QDomDocument &doc) const
{
    QDomElement root = doc.createElement("scene");
    root.setAttribute("name", k->name );

    foreach (KTLayer *layer, k->layers.visualValues())
             root.appendChild( layer->toXml(doc));

    foreach (KTSoundLayer *sound, k->soundLayers.visualValues())
             root.appendChild(sound->toXml(doc));

    return root;
}

bool KTScene::moveLayer(int from, int to)
{
    if (from < 0 || from >= k->layers.count() || to < 0 || to >= k->layers.count())
        return false;

    KTLayer *layer = k->layers[from];

    k->layers.insert(to, layer);
    k->layers.remove(from);

    return true;
}

int KTScene::logicalIndex() const
{
    if (KTProject *project = dynamic_cast<KTProject *>(parent()))
        return project->logicalIndexOf(const_cast<KTScene *>(this));
	
    return -1;
}

int KTScene::visualIndex() const
{
    if (KTProject *project = dynamic_cast<KTProject *>(parent()))
        return project->visualIndexOf(const_cast<KTScene *>(this));

    return -1;
}

int KTScene::visualIndexOf(KTLayer *layer) const
{
    return k->layers.visualIndex(layer);
}

int KTScene::logicalIndexOf(KTLayer *layer) const
{
    return k->layers.logicalIndex(layer);
}

KTProject *KTScene::project() const
{
    return static_cast<KTProject *>(parent());
}

void KTScene::addTweeningObject(KTGraphicObject *object)
{
    k->tweeningObjects << object;
}

void KTScene::removeTweeningObject(KTGraphicObject *object)
{
    k->tweeningObjects.removeAll(object);
}

QList<KTGraphicObject *> KTScene::tweeningObjects() const
{
    return k->tweeningObjects;
}

int KTScene::framesTotal()
{
    int total = 0;
    foreach (KTLayer *layer, k->layers.values()) {
             int frames =layer->framesNumber();
             if (frames > total)
                 total = frames;
    }

    return total;
}
