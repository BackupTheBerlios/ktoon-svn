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

#include "ktprojectparser.h"
#include "dpathadjuster.h"

KTScene::KTScene(QObject *parent) : KTSerializableObject(parent), m_currentLayer(0), m_layerCount(0), m_fps(24)
{
// 	m_currentLayer = createLayer();
}


KTScene::~KTScene()
{
	DEND;
	for(int i = 0; i < m_layers.count(); i++ )
	{
		delete m_layers.takeAt(i);
	}
}

QDomElement KTScene::createXML( QDomDocument &doc )
{
	QDomElement scene = doc.createElement("Scene");
	return scene;
}

void KTScene::save(const QString &scenePath)
{
	QDir sceneDir(scenePath);
		
	if ( ! sceneDir.exists() )
	{
		sceneDir.mkdir(sceneDir.path());
	}
	
	QDomDocument document;
	
	QDomElement root = createXML(document);
	root.setAttribute("name", m_name );
	document.appendChild(root);
	
	Layers::ConstIterator iterator = m_layers.begin();
	
	
	while ( iterator != m_layers.end() )
	{
		root.appendChild((*iterator)->createXML(document));
		++iterator;
	}
	
	QFile save(scenePath+"/"+/*m_name*/"scene"+".kts");
	
	if ( save.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&save);
		out << document.toString();
		
		save.close();
	}
	
}

void KTScene::load(const QString &path)
{
	dDebug() << "Loading scene: " << path;
	
	KTProjectParser parser;
	
	connect(&parser, SIGNAL(createLayer(const QString &)), this, SLOT(loadLayer(const QString &)));
	connect(&parser, SIGNAL(createFrame(const QString &, int)), this, SLOT(loadFrame(const QString &, int)));
	connect(&parser, SIGNAL(createComponent( AGraphicComponent * ) ), this, SLOT( loadComponent(AGraphicComponent *) ));
	
	QXmlSimpleReader reader;
	reader.setContentHandler(&parser);
	reader.setErrorHandler(&parser);
		
	QFile source(path);
	QXmlInputSource xmlsource(&source);
		
	if ( reader.parse(&xmlsource) )
	{
		setSceneName( parser.partName() );
		
	}
	else
	{
		dError() << "Error while parse file: " << source.fileName();
	}
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

void KTScene::setLayers(const Layers &layers)
{
	m_layers = layers;
}

KTLayer *KTScene::createLayer(const QString& name, bool addToEnd )
{
	D_FUNCINFO;
	KTLayer *layer = new KTLayer(this);
	
	m_layerCount++;
	
	if(name.isNull())
	{
		layer->setLayerName(tr("Layer %1").arg(m_layerCount));
	}
	else
	{
		layer->setLayerName(name);
	}
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
		emit layerSelected(index);
	}
	else
	{
		dError() << "No layer!";
	}
}

int KTScene::indexCurrentLayer() const
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
		dError() << "FPS out of range" << endl;
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

void KTScene::loadLayer(const QString & name)
{
	KTLayer *layer = createLayer(name);
}

void KTScene::loadFrame(const QString & name, int clones)
{
	if ( m_currentLayer )
	{
		KTKeyFrame *frame = m_currentLayer->createFrame(name);
		if(clones>0)
		{
			m_currentLayer->cloneFrame( m_currentLayer->frames().count()-1, clones);
		}
	}
}

void KTScene::loadComponent(AGraphicComponent *component)
{
	if ( component )
	{
		m_currentLayer->currentFrame()->addComponent( component );
	}
}

void KTScene::moveCurrentLayer(bool up)
{
	D_FUNCINFO;
	if(m_currentLayer)
	{
		int index = m_layers.indexOf(m_currentLayer);
		
		if( index == -1 )
		{
			return;
		}
		
		if(up )
		{
			if(index > 0)
			{
				m_layers.swap ( index, index-1);
				emit layerMoved(up);
			}
		}
		else if(m_currentLayer != m_layers.last() )
		{
			m_layers.swap ( index, index+1);
			emit layerMoved(up);
		}
	}
}

