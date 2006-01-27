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

#include <QDir>

#include "ktprojectparser.h"
#include "ktpathadjuster.h"

KTScene::KTScene(QObject *parent) : KTSerializableObject(parent), m_currentLayer(0), m_layerCount(0), m_fps(24)
{
// 	m_currentLayer = createLayer();
}


KTScene::~KTScene()
{
	KTEND;
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
	ktDebug() << "Loading scene: " << path;
	
	KTProjectParser parser;
	
	connect(&parser, SIGNAL(createLayer()), this, SLOT(loadLayer()));
	connect(&parser, SIGNAL(createFrame()), this, SLOT(loadFrame()));
	connect(&parser, SIGNAL(createComponent( const QStringList&, const QPen &, const QBrush & ) ), this, SLOT( loadComponent(const QStringList &, const QPen &, const QBrush &) ));
	
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
		ktError() << "Error while parse file: " << source.fileName();
	}
}

void KTScene::setSceneName(const QString &name)
{
	ktDebug() << "Setting scene name: " << name;
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
	else
	{
		ktError() << "No layer!";
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

void KTScene::removeLayer( int index)
{
	if(index >= 0 && index < m_layers.count())
	{
		m_layers.removeAt(index);
		setCurrentLayer( index );
		emit layerRemoved(index);
	}
	
}

void KTScene::loadLayer()
{
	KTLayer *layer = createLayer(true);
}

void KTScene::loadFrame()
{
	if ( m_currentLayer )
	{
		KTKeyFrame *frame = m_currentLayer->createFrame(true);
	}
}

void KTScene::loadComponent(const QStringList &polygons, const QPen &pen, const QBrush &brush)
{
	if ( m_currentLayer )
	{
		AGraphicComponent *component = new AGraphicComponent();
		
		component->addGraphic( KTPathAdjuster::buildPath( polygons, ':'), pen, brush);
		m_currentLayer->currentFrame()->addComponent( component );
	}
}
