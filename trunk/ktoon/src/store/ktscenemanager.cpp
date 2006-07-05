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

#include "ktscenemanager.h"
#include "ddebug.h"

#include <QDir>


KTScene::KTScene(QObject *parent) : QObject(parent), m_currentLayerIndex(-1), m_layerCount(0), m_fps(24)
{
}


KTScene::~KTScene()
{
	DEND;
	qDeleteAll(m_layers);
}

// QDomElement KTScene::createXML( QDomDocument &doc )
// {
// 	QDomElement scene = doc.createElement("Scene");
// 	return scene;
// }
// 
// void KTScene::save(const QString &scenePath)
// {
// 	QDir sceneDir(scenePath);
// 		
// 	if ( ! sceneDir.exists() )
// 	{
// 		sceneDir.mkdir(sceneDir.path());
// 	}
// 	
// 	QDomDocument document;
// 	
// 	QDomElement root = createXML(document);
// 	root.setAttribute("name", m_name );
// 	document.appendChild(root);
// 	
// 	Layers::ConstIterator iterator = m_layers.begin();
// 	
// 	
// 	while ( iterator != m_layers.end() )
// 	{
// 		root.appendChild((*iterator)->createXML(document));
// 		++iterator;
// 	}
// 	
// 	QFile save(scenePath+"/"+"scene"+".kts");
// 	
// 	if ( save.open(QIODevice::WriteOnly | QIODevice::Text))
// 	{
// 		QTextStream out(&save);
// 		out << document.toString();
// 		
// 		save.close();
// 	}
// }

// void KTScene::load(const QString &path)
// {
// 	dDebug() << "Loading scene: " << path;
// 	
// 	KTProjectParser parser;
// 	
// 	connect(&parser, SIGNAL(createLayer(const QString &)), this, SLOT(loadLayer(const QString &)));
// 	connect(&parser, SIGNAL(createFrame(const QString &, int)), this, SLOT(loadFrame(const QString &, int)));
// 	connect(&parser, SIGNAL(createComponent( KTGraphicComponent * ) ), this, SLOT( loadComponent(KTGraphicComponent *) ));
// 	
// 	if ( parser.parse(path) )
// 	{
// 		setSceneName( parser.partName() );
// 	}
// 	else
// 	{
// 		dError() << "Error while parse file: " << path;
// 	}
// }

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

KTLayer *KTScene::createLayer(bool addToEnd )
{
	KTLayer *layer = new KTLayer(this);
	
	m_layerCount++;
	
	layer->setLayerName(tr("Layer %1").arg(m_layerCount));
	
	if ( addToEnd )
	{
		m_currentLayerIndex = m_layers.count();
		m_layers << layer;
	}
	else
	{
		m_currentLayerIndex++;
		m_layers.insert( m_currentLayerIndex, layer);
	}
	
	emit layerCreated( layer->layerName() , addToEnd );
	
	return layer;
}

/**
 * Retorna el layer actual
 */
KTLayer *KTScene::currentLayer()
{
	if (  m_currentLayerIndex >= 0  && m_currentLayerIndex < m_layers.count() )
	{
		return m_layers[m_currentLayerIndex];
	}
	
	return 0;
}

int KTScene::currentLayerIndex() const
{
	return m_currentLayerIndex;
}

void KTScene::setCurrentLayer(int index)
{
	if (  index > 0  && index < m_layers.count() )
	{
		m_currentLayerIndex = index;
	}
}


void KTScene::setFPS(int fps)
{
	if (fps > 0 )
	{
		m_fps = fps;
	}
	else
	{
		dError() << "FPS out of range";
	}
}

int KTScene::fps() const
{
	return m_fps;
}

void KTScene::removeLayer( int index)
{
	if(index >= 0 && index < m_layers.count())
	{
		m_layers.removeAt(index);
		setCurrentLayer( index );
		emit layerRemoved(index);
	}
	
}

void KTScene::moveCurrentLayer(bool up)
{
	D_FUNCINFO;
	
	if( m_currentLayerIndex == -1 )
	{
		return;
	}
	
	if(up )
	{
		if(m_currentLayerIndex > 0)
		{
			m_layers.swap ( m_currentLayerIndex, m_currentLayerIndex-1);
			emit layerMoved(up);
		}
	}
	else if(m_currentLayerIndex < m_layers.count()-1 )
	{
		m_layers.swap ( m_currentLayerIndex, m_currentLayerIndex+1);
		emit layerMoved(up);
	}
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

