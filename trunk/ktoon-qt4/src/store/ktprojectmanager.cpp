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

#include "ktprojectmanager.h"
#include "ktdebug.h"

KTProjectManager::KTProjectManager(QObject *parent) : QObject(parent), m_currentDocument(0), m_copyFrame(0)
{
	KTINIT;

}


KTProjectManager::~KTProjectManager()
{
	KTEND;
}

Documents KTProjectManager::documents() const
{
	return m_documents;
}

void KTProjectManager::setDocuments(const Documents &docs)
{
	m_documents = docs;
}

KTDocument *KTProjectManager::createDocument(const QString &name)
{
	KTDocument *doc = new KTDocument(this);
	
	m_currentDocument = doc;
	
	m_documents << doc;
	
	connect(doc, SIGNAL(sceneCreated(const QString &, bool)), this, SIGNAL(sceneCreated(const QString &, bool)));
	
	
	createScene();
	createLayer();
	createFrame();
	
	return doc;
}

KTDocument *KTProjectManager::currentDocument()
{
	return m_currentDocument;
}

KTScene *KTProjectManager::currentScene()
{
	if ( ! m_currentDocument )
	{
		ktFatal() << "No current document" << endl;
		return 0;
	}
	
	return m_currentDocument->currentScene();
}

KTLayer *KTProjectManager::currentLayer()
{
	if ( ! currentScene() )
	{
		ktFatal() << "No current scene" << endl;
		return 0;
	}
	
	return currentScene()->currentLayer();
}

KTKeyFrame *KTProjectManager::currentKeyFrame()
{
	if ( ! currentLayer() )
	{
		ktFatal() << "No current layer" << endl;
		return 0;
	}
	
	return currentLayer()->currentFrame();
}

void KTProjectManager::setCurrentDocument(int index)
{
	KTDocument *doc = m_documents[index];
	if ( doc )
	{
		m_currentDocument = doc;
	}
}

void KTProjectManager::setCurrentScene(int index)
{
	if ( m_currentDocument )
	{
		m_currentDocument->setCurrentScene( index );
	}

}

void KTProjectManager::setCurrentLayer(int index)
{
	KTScene *scene = currentScene();
	if( scene )
	{
		scene->setCurrentLayer( index );
	}
}

void KTProjectManager::setLayerVisibility(int idLayer, bool value)
{
	KTScene *scene = currentScene();
	if( scene && scene->layers().count() > idLayer)
	{
		scene->layers()[idLayer]->setVisible(value);
	}
	else
	{
		ktFatal() << "ERROR" << endl;
	}
}


void KTProjectManager::setCurrentFrame(int index)
{
	KTLayer *layer = currentLayer();
	if ( layer )
	{
		layer->setCurrentFrame( index );
	}
}

void KTProjectManager::createScene(bool addToEnd)
{
	if ( m_currentDocument )
	{
		KTScene *scene = m_currentDocument->createScene(addToEnd);
		connect(scene, SIGNAL(layerCreated( const QString&, bool)), this, SIGNAL(layerCreated( const QString &, bool)));
		connect(scene, SIGNAL(layerRemoved( int)), this, SIGNAL(layerRemoved(int))) ;
	}
	else
	{
		ktFatal() << "--> No current document" << endl;
	}
}


void KTProjectManager::removeScene()
{
}

void KTProjectManager::renameScene(const QString &name, int index)
{
// 	emit sceneRenamed(name, index);
}

// Layers
void KTProjectManager::createLayer(bool addToEnd)
{
	KT_FUNCINFO;
	KTScene *scene = currentScene();
	if ( scene )
	{
		KTLayer *layer = scene->createLayer(addToEnd);
		connect(layer, SIGNAL(frameCreated( const QString &, bool)), this, SIGNAL(frameCreated(  const QString& , bool)));
		
		connect(layer, SIGNAL(frameMoved(bool)), this, SIGNAL(frameMoved(bool))); 
		
		connect(layer, SIGNAL(frameRemoved()), this, SIGNAL(frameRemoved()));
		connect(layer, SIGNAL(frameLocked()), this, SIGNAL(frameLocked()));
		connect(layer, SIGNAL(visibilityChanged(bool)), this, SLOT(emitLayerVisibility(bool)));
		
		
	}
	else
	{
		ktFatal() << "--> No current scene" << endl;
	}
}

void KTProjectManager::emitLayerVisibility(bool isVisible )
{
	KTScene *scene = currentScene();
	KTLayer *layer = dynamic_cast<KTLayer *>(sender());
	
	if ( scene && layer)
	{
		emit layerVisibilityChanged( scene->layers().indexOf(layer), isVisible );
	}
}

// Frames

void KTProjectManager::createFrame(bool addToEnd)
{
	ktDebug() << "#KTProjectManager:: Inserting frame " << addToEnd <<  endl;
	
	KTLayer *layer = currentLayer();
	if ( layer )
	{
		KTKeyFrame *keyFrame = layer->createFrame(addToEnd);
	}
	else
	{
		ktFatal() << "--> No current layer" << endl;
	}
}

void KTProjectManager::copyFrame(int index)
{
	ktDebug() << "KTProjectManager::copyFrame()";
	KTLayer *layer = currentLayer();
	if ( layer && layer->frames().count() > index )
	{
		KTKeyFrame *frame = layer->frames()[index];
		if ( frame )
		{
			m_copyFrame = frame;
			if ( m_copyFrame->components()[0] == frame->components()[0] )
			{
				ktError() << "HEREEEEEEEEEEEEEE";
			}
		}
	}
	else
	{
		ktFatal() << "--> No current layer" << endl;
	}
}

void KTProjectManager::pasteFrame(int index )
{
	ktDebug() << "KTProjectManager::pasteFrame()" << endl;
	KTLayer *layer = currentLayer();
	if ( layer && m_copyFrame)
	{
		layer->pasteFrame(index, new KTKeyFrame(*m_copyFrame) );
	}
	else
	{
		ktFatal() << "--> No current layer" << endl;
	}
}

void KTProjectManager::moveFrame(bool up)
{
	KTLayer *layer = currentLayer();
	if ( layer )
	{
		layer->moveCurrentFrame(up);
	}
}

void KTProjectManager::removeFrame()
{
// 	ktDebug() << "emit KTProjectManager::removeFrame();";
	KTLayer *layer = currentLayer();
	if ( layer )
	{
		layer->removeCurrentFrame();
	}
}

void KTProjectManager::lockCurrentFrame()
{
	KTLayer *layer = currentLayer();
	if ( layer )
	{
		layer->lockCurrentFrame();
	}
}

void KTProjectManager::removeLayer()
{
	KTScene *scene = currentScene();
	if( scene )
	{
		scene->removeLayer(scene->indexCurrentLayer());
	}
}




