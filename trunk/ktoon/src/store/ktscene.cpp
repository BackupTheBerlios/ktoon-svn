/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@toonka.com                                          	   *
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
#include <dcore/ddebug.h>

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

KTScene::KTScene(KTProject *parent) : QObject(parent), d(new Private)
{
	d->isLocked = false;
	d->layerCount = 0;
	d->isVisible = true;
}


KTScene::~KTScene()
{
	DEND;
	
	d->layers.clear(true);
	
	delete d;
}

void KTScene::setSceneName(const QString &name)
{
	d->name = name;
}

void KTScene::setLocked(bool isLocked)
{
	d->isLocked = isLocked;
}

QString KTScene::sceneName() const
{
	return d->name;
}

bool KTScene::isLocked() const
{
	return d->isLocked;
}

void KTScene::setVisible(bool isVisible)
{
	d->isVisible = isVisible;
}

bool KTScene::isVisible() const
{
	return d->isVisible;
}

Layers KTScene::layers() const
{
	return d->layers;
}

SoundLayers KTScene::soundLayers() const
{
	return d->soundLayers;
}

/**
 * Pone la lista de layers, esta funcion sobreescribe los layers anteriores
 */
void KTScene::setLayers(const Layers &layers)
{
	d->layers = layers;
}

KTLayer *KTScene::createLayer(int position, bool loaded)
{
	D_FUNCINFO << position;
	
	if ( position < 0 || position > d->layers.count() )
	{
		dDebug() << "Error in createLayer";
		return 0;
	}
	
	KTLayer *layer = new KTLayer(this);
	
	d->layerCount++;
	
	layer->setLayerName(tr("Layer %1").arg(d->layerCount));
	
	d->layers.insert( position, layer);
	
	if ( loaded )
	{
		KTProjectLoader::createLayer( project()->visualIndexOf(this), position, layer->layerName(), project() );
	}
	
	return layer;
}

KTSoundLayer *KTScene::createSoundLayer(int position, bool loaded)
{
	D_FUNCINFO << position;
	
	if ( position < 0 || position > d->soundLayers.count() )
	{
		dDebug() << "Error in createLayer";
		return 0;
	}
	
	KTSoundLayer *layer = new KTSoundLayer(this);
	
	d->layerCount++;
	
	layer->setLayerName(tr("Sound layer %1").arg(d->layerCount));
	
	d->soundLayers.insert( position, layer);
	
	if ( loaded )
	{
		KTProjectLoader::createSoundLayer( visualIndex(), position, layer->layerName(), project() );
	}
	
	return layer;
}

bool KTScene::removeLayer( int position)
{
	KTLayer *layer = this->layer(position);
	if ( layer )
	{
		d->layers.remove(position);
		delete layer;
		
		return true;
	}
	
	return false;
}


/**
 * Retorna el layer que se encuentra en la posición indicada
 * @param position 
 * @return 
 */
KTLayer *KTScene::layer(int position)
{
	if ( position < 0 || position >= d->layers.count() )
	{
		D_FUNCINFO << " FATAL ERROR: index out of bound " << position;
		return 0;
	}
	
	return d->layers.visualValue(position);
}

KTSoundLayer *KTScene::soundLayer(int position)
{
	if ( position < 0 || position >= d->soundLayers.count() )
	{
		D_FUNCINFO << " FATAL ERROR: index out of bound " << position;
		return 0;
	}
	
	return d->soundLayers.visualValue(position);
}


void KTScene::fromXml(const QString &xml )
{
	QDomDocument document;
	
	if (! document.setContent(xml) )
	{
		return;
	}
	
	QDomElement root = document.documentElement();
	
	setSceneName( root.attribute( "name", sceneName()) );
	
	QDomNode n = root.firstChild();
	
	while( !n.isNull() )
	{
		QDomElement e = n.toElement();
		
		if(!e.isNull())
		{
			if ( e.tagName() == "layer" )
			{
				int pos = d->layers.count();
				KTLayer *layer = createLayer( pos, true );
				
				if ( layer )
				{
					QString newDoc;
					
					{
						QTextStream ts(&newDoc);
						ts << n;
					}
					
					layer->fromXml( newDoc );
				}
			}
			else if( e.tagName() == "soundlayer" )
			{
				int pos = d->soundLayers.count();
				KTSoundLayer *layer = createSoundLayer( pos, true );
				
				if ( layer )
				{
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
	}
}

QDomElement KTScene::toXml(QDomDocument &doc) const
{
	QDomElement root = doc.createElement("scene");
	root.setAttribute("name", d->name );
	
	foreach(KTLayer *layer, d->layers.visualValues())
	{
		root.appendChild( layer->toXml(doc) );
	}
	
	foreach(KTSoundLayer *sound, d->soundLayers.visualValues())
	{
		root.appendChild(sound->toXml(doc));
	}
	
	return root;
}


bool KTScene::moveLayer(int from, int to)
{
	if ( from < 0 || from >= d->layers.count() || to < 0 || to >= d->layers.count() )
	{
		return false;
	}
	
	KTLayer *layer = d->layers[from];
	
	d->layers.insert(to, layer);
	
	d->layers.remove(from);
	
	return true;
}

int KTScene::logicalIndex() const
{
	if ( KTProject *project = dynamic_cast<KTProject *>(parent()) )
	{
		return project->logicalIndexOf(const_cast<KTScene *>(this) );
	}
	
	return -1;
}

int KTScene::visualIndex() const
{
	if ( KTProject *project = dynamic_cast<KTProject *>(parent()) )
	{
		return project->visualIndexOf(const_cast<KTScene *>(this) );
	}
	
	return -1;
}

int KTScene::visualIndexOf(KTLayer *layer) const
{
	return d->layers.visualIndex(layer);
}

int KTScene::logicalIndexOf(KTLayer *layer) const
{
	return d->layers.logicalIndex(layer);
}

KTProject *KTScene::project() const
{
	return static_cast<KTProject *>(parent());
}

void KTScene::addTweeningObject(KTGraphicObject *object)
{
	d->tweeningObjects << object;
}

void KTScene::removeTweeningObject(KTGraphicObject *object)
{
	d->tweeningObjects.removeAll(object);
}

QList<KTGraphicObject *> KTScene::tweeningObjects() const
{
	return d->tweeningObjects;
}

