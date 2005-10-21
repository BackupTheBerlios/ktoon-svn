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

KTScene::KTScene(QObject *parent) : QObject(parent), m_currentLayer(0)
{
	m_currentLayer = createLayer();
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

KTLayer *KTScene::createLayer()
{
	KTLayer *layer = new KTLayer(this);
	
	m_layers << layer;
	
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

