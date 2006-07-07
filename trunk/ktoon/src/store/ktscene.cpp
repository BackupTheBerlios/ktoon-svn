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
	KTLayer *layer = new KTLayer(this);
	
	m_layerCount++;
	
	layer->setLayerName(tr("Layer %1").arg(m_layerCount));
	
	m_layers.insert( position, layer);
	
	return layer;
}


void KTScene::removeLayer( int position)
{
}

void KTScene::moveCurrentLayer(bool up)
{

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
 * Retorna el layer que se encuentra en la posición indicada
 * @param position 
 * @return 
 */
KTLayer *KTScene::layer(int position)
{
	if ( position < 0 || position > m_layers.count() )
	{
		D_FUNCINFO << " FATAL ERROR: index out of bound";
		return 0;
	}
	
	return m_layers[position];
}

