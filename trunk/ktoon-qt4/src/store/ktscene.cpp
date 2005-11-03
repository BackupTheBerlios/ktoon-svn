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
#include "ktdebug.h"


KTScene::KTScene(QObject *parent) : QObject(parent), m_currentLayer(0), m_layerCount(0), m_fps(24)
{
// 	m_currentLayer = createLayer();
}


KTScene::~KTScene()
{
}

void KTScene::setSceneName(const QString &name)
{
	m_name = name;
}

QString KTScene::sceneName()
{
	return m_name;
}

Layers KTScene::layers() const
{
	return m_layers;
}

void KTScene::setLayers(const Layers &layers)
{
	m_layers = layers;
}

KTLayer *KTScene::createLayer(bool addToEnd )
{
	KT_FUNCINFO;
	KTLayer *layer = new KTLayer(this);
	layer->setLayerName(tr("Layer %1").arg(m_layerCount++));
	
	if ( addToEnd )
	{
		m_layers << layer;
	}
	else
	{
		m_layers.insert( m_layers.indexOf(m_currentLayer)+1, layer);
	}
	
	m_currentLayer = layer;
	
	emit layerCreated( layer->layerName() , addToEnd);
	
	return layer;
}

KTLayer *KTScene::currentLayer()
{
	return m_currentLayer;
}

void KTScene::setCurrentLayer(int index)
{
	KTLayer *layer = m_layers[index];
	if ( layer )
	{
		m_currentLayer = layer;
	}
}

int KTScene::indexCurrentLayer()
{
	return m_layers.indexOf(m_currentLayer);
}

void KTScene::setFPS(int fps)
{
	if (fps > 0 )
	{
		m_fps = fps;
	}
	else
	{
		ktError() << "FPS out of range" << endl;
	}
}

int KTScene::fps()
{
	return m_fps;
}

