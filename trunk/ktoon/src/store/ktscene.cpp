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
#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>

#include "ktgraphicobject.h"

#include "ktitemgroup.h"

KTScene::KTScene(KTProject *parent) : QGraphicsScene(parent), m_isLocked(false),  m_layerCount(0), m_isVisible(true)
{
	setItemIndexMethod(QGraphicsScene::NoIndex);
	setCurrentFrame( -1, -1);
	
	m_onionSkin.next = 0;
	m_onionSkin.previous = 0;
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
		dDebug() << "Error in createLayer";
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
				foreach(KTGraphicObject *object,  layer->frames()[index]->graphics())
				{
					scene->addItem ( object->item() );
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
	if ( layer < 0 || frame < 0 ) return;
	
	m_framePosition.layer = layer;
	m_framePosition.frame = frame;
	
	if (! currentFrame() )
	{
		foreach(QGraphicsView *view, views() )
		{
			view->setDragMode(QGraphicsView::NoDrag);
		}
	}
	else
	{
		foreach(QGraphicsView *view, views() )
		{
			view->setDragMode(QGraphicsView::NoDrag);
		}
	}
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

void KTScene::drawItems(QPainter *painter, int numItems, QGraphicsItem *items[], const QStyleOptionGraphicsItem options[], QWidget *widget)
{
	for (int i = 0; i < numItems; ++i)
	{
		QGraphicsItem *item = items[i];
		painter->save();
		painter->setMatrix(item->sceneMatrix(), true);
		
		if ( m_onionSkin.opacityMap.contains(item) )
		{
			painter->setOpacity( m_onionSkin.opacityMap[item] );
		}
		
		item->paint(painter, &options[i], widget);
		
		painter->restore();
	}
}

void KTScene::drawPhotogram(int photogram)
{
	if ( photogram < 0 ) return;
	
	clean();
	
	foreach(KTLayer *layer, layers())
	{
		if ( layer->isVisible() )
		{
			double opacityFactor = 0.5 / (double)qMin(layer->frames().count(),m_onionSkin.previous);
			
			double opacity = 0.6;
			
			for(int frameIndex = photogram-1; frameIndex > photogram-m_onionSkin.previous-1; frameIndex-- )
			{
				addFrame( layer->frame(frameIndex), opacity );
				
				opacity -= opacityFactor;
			}
			
			opacityFactor = 0.5 / (double)qMin(layer->frames().count(), m_onionSkin.next);
			opacity = 0.6;
			
			for(int frameIndex = photogram+1; frameIndex < photogram+m_onionSkin.next+1; frameIndex++ )
			{
				addFrame( layer->frame(frameIndex), opacity );
				
				opacity -= opacityFactor;
			}
			
			addFrame(layer->frame( photogram ));
		}
	}
}

void KTScene::addFrame(KTFrame *frame, double opacity)
{
	if ( frame )
	{
		foreach(KTGraphicObject *object, frame->graphics() )
		{
			QGraphicsItem *item = object->item();
			
			m_onionSkin.opacityMap.insert(item, opacity);
			
			addItem(item);
			
			if ( KTItemGroup *group = qgraphicsitem_cast<KTItemGroup *>(item) )
			{
				group->recoverChilds();
			}
		}
	}
}

void KTScene::clean()
{
	m_onionSkin.opacityMap.clear();
	
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

KTProject *KTScene::project() const
{
	return static_cast<KTProject *>(parent());
}

void KTScene::setNextOnionSkinCount(int n)
{
	m_onionSkin.next = n;
	drawCurrentPhotogram();
}

void KTScene::setPreviousOnionSkinCount(int n)
{
	m_onionSkin.previous = n;
	
	drawCurrentPhotogram();
}

