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


KTScene::KTScene(QObject *parent) : QObject(parent), m_layerCount(0)
{
}


KTScene::~KTScene()
{
	DEND;
	qDeleteAll(m_layers);
}

void KTScene::setSceneName(const QString &name)
{
	dDebug() << "Setting scene name: " << name;
	m_name = name;
}

QString KTScene::sceneName() const
{
	return m_name;
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
		m_layers.removeAt(position);
		delete layer;
		
		return true;
	}
	
	return false;
}

void KTScene::moveCurrentLayer(bool up)
{
	
}


/**
 * Retorna el fotograma de la posicici�n marcada por index
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
				foreach(QGraphicsItem *item,  layer->frames()[index]->items())
				{
					scene->addItem ( item );
				}
			}
		}
	}
	
	return scene;
}

/**
 * Retorna el layer que se encuentra en la posici�n indicada
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


