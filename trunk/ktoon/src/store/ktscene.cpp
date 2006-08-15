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
#include "ddebug.h"

#include <QDir>
#include <QGraphicsItem>

KTScene::KTScene(KTProject *parent) : QGraphicsScene(parent), m_isLocked(false),  m_layerCount(0), m_isVisible(true)
{
	setItemIndexMethod(QGraphicsScene::NoIndex);
	
	m_framePosition.frame = -1;
	m_framePosition.layer = -1;
}


KTScene::~KTScene()
{
	DEND;
	qDeleteAll(m_layers);
}

void KTScene::setSceneName(const QString &name)
{
	m_name = name;
}

void KTScene::setLocked(bool isLocked)
{
	m_isLocked = isLocked;
}

QString KTScene::sceneName() const
{
	return m_name;
}

bool KTScene::isLocked() const
{
	return m_isLocked;
}

void KTScene::setVisible(bool isVisible)
{
	m_isVisible = isVisible;
}

bool KTScene::isVisible() const
{
	return m_isVisible;
}

Layers KTScene::layers() const
{
	return m_layers;
}

/**
 * Pone la lista de layers, esta funcion sobreescribe los layers anteriores
 */
void KTScene::setLayers(const Layers &layers)
{
	m_layers = layers;
}

KTLayer *KTScene::createLayer(int position)
{
	D_FUNCINFO << position;
	if ( position < 0 || position > m_layers.count() )
	{
		return 0;
	}
	
	KTLayer *layer = new KTLayer(this);
	
	m_layerCount++;
	
	layer->setLayerName(tr("Layer %1").arg(m_layerCount));
	
	m_layers.insert( position, layer);
	
	return layer;
}


bool KTScene::removeLayer( int position)
{
	KTLayer *layer = this->layer(position);
	if ( layer )
	{
		m_layers.remove(position);
		delete layer;
		
		return true;
	}
	
	return false;
}

/**
 * Retorna el fotograma de la posicición marcada por index
 * @param index 
 * @return 
 */
QGraphicsScene *KTScene::photogram(int index)
{
	QGraphicsScene *scene = 0;
	foreach(KTLayer *layer, m_layers)
	{
		if( 0 < index &&  index < layer->frames().count())
		{
			if(layer->frames()[index])
			{
				foreach(QGraphicsItem *item,  layer->frames()[index]->graphics())
				{
					scene->addItem ( item );
				}
			}
		}
	}
	
	return scene;
}

/**
 * Retorna el layer que se encuentra en la posición indicada
 * @param position 
 * @return 
 */
KTLayer *KTScene::layer(int position)
{
	if ( position < 0 || position >= m_layers.count() )
	{
		D_FUNCINFO << " FATAL ERROR: index out of bound " << position;
		return 0;
	}
	
	return m_layers[position];
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
				KTLayer *layer = createLayer( m_layers.count() );
				
				if ( layer )
				{
					QDomDocument newDoc;
					newDoc.appendChild(newDoc.importNode(n, true ));
					
					layer->fromXml( newDoc.toString(0) );
				}
			}
		}
		
		n = n.nextSibling();
	}
}

QDomElement KTScene::toXml(QDomDocument &doc)
{
	QDomElement root = doc.createElement("scene");
	root.setAttribute("name", m_name );
	doc.appendChild(root);
	
	Layers::ConstIterator iterator = m_layers.begin();
	
	
	while ( iterator != m_layers.end() )
	{
		root.appendChild( (*iterator)->toXml(doc) );
		++iterator;
	}
	
	return root;
}


bool KTScene::moveLayer(int from, int to)
{
	if ( from < 0 || from >= m_layers.count() || to < 0 || to >= m_layers.count() )
	{
		return false;
	}
	
	KTLayer *layer = m_layers[from];
	
	m_layers.insert(to, layer);
	
	m_layers.remove(from);
	
	return true;
}

void KTScene::setCurrentFrame(int layer, int frame)
{
	m_framePosition.layer = layer;
	m_framePosition.frame = frame;
}

void KTScene::addGraphic(QGraphicsItem *item)
{
	KTFrame *frame = currentFrame();
	if ( frame )
	{
// 		frame->addGraphic(item); // FIXME: remove this function
	}
	
	addItem(item);
}

KTFrame *KTScene::currentFrame()
{
	KTLayer *layer = this->layer( m_framePosition.layer );
	if ( layer )
	{
		return layer->frame(m_framePosition.frame );
	}
	
	return 0;
}

void KTScene::drawCurrentPhotogram()
{
	drawPhotogram( m_framePosition.frame );
}

void KTScene::drawPhotogram(int photogram)
{
	if ( photogram < 0 ) return;
	
	
	clean();
	
	foreach(KTLayer *layer, layers())
	{
		if ( layer->isVisible() )
		{
			KTFrame *frame = layer->frame( photogram );
			
			if ( frame )
			{
				frame->recoverItems();
				foreach(QGraphicsItem *item, frame->graphics() )
				{
					addItem(item);
				}
			}
		}
	}
}

void KTScene::clean()
{
	foreach(QGraphicsItem *item, items() )
	{
		if ( item->scene() == this )
		{
			removeItem(item);
		}
	}
}

int KTScene::currentFrameIndex() const
{
	return m_framePosition.frame;
}

int KTScene::currentLayerIndex() const
{
	return m_framePosition.layer;
}


int KTScene::index() const
{
	if ( KTProject *project = dynamic_cast<KTProject *>(parent()) )
	{
		return project->indexOf(const_cast<KTScene *>(this) );
	}
	
	return -1;
}


