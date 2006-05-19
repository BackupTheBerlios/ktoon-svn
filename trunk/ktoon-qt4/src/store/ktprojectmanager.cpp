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
#include "ddebug.h"

#include "ktapplication.h"

#include "ktprojectparser.h"

KTProjectManager::KTProjectManager(QObject *parent) : KTSerializableObject(parent), m_currentDocument(0), m_copyFrame(0), m_isOpen(false), m_fps(24)
{
	DINIT;
}


KTProjectManager::~KTProjectManager()
{
	DEND;
}



QDomElement KTProjectManager::createXML( QDomDocument &doc )
{
	QDomElement project = doc.createElement("Project");
	//TODO: a�dir nombre al proyecto
	project.setAttribute("name", m_name);
	project.setAttribute("width", m_size.width());
	project.setAttribute("height", m_size.height());
	
	QDomElement meta = doc.createElement("meta");
	
	QDomElement author = doc.createElement("author");
	author.setAttribute("value", "Anonymous");
	meta.appendChild(author);
	
// 	QDomElement renderType = doc.createElement("renderType");
// 	renderType.setAttribute("value", m_typeRender);
// 	meta.appendChild(renderType);
	
	project.appendChild(meta);
	
	Documents::ConstIterator documentIt = m_documents.begin();
	
	int documentCounter = 0;
	while( documentIt != m_documents.end() )
	{
		QString location = QString("Document%1").arg(documentCounter++);
		QString docPath = REPOSITORY + +"/"+m_name+"/"+location;
		
		QDomElement docElement = (*documentIt)->createXML( doc );
		docElement.setAttribute("location",location);
		project.appendChild(docElement);
		
		(*documentIt)->save(docPath);
		
		++documentIt;
	}
	return  project;
}

void KTProjectManager::save()
{
	if ( m_name.isNull() ) return;
	
	QDir repository(REPOSITORY);

	if ( ! repository.exists() ) 
	{ 
		return;
	}
	else
	{
		if ( !repository.exists(m_name) )
		{
			repository.mkpath(m_name);
		}
	}
	
	QDir resources(repository.path()+"/"+m_name+"/resources");
	
	if ( ! repository.exists(resources.path()) )
	{
		repository.mkdir(resources.path());
	}
	
	QDomDocument doc;
	QDomElement root = doc.createElement("KToon");
	doc.appendChild(root);
	root.appendChild(createXML( doc ));
	
	QFile save(REPOSITORY + +"/"+m_name+"/"+m_name+".ktp");
	
	if ( ! save.open(QIODevice::WriteOnly | QIODevice::Text)) return;
	
	
	QTextStream out(&save);
	out << doc.toString();
	
	save.close();
	
	foreach(KTDocument *document, documents())
	{
		document->saveResources(resources.path());
		foreach(KTScene *scene, document->scenes() )
		{
			scene->saveResources(resources.path());
			foreach(KTLayer *layer, scene->layers() )
			{
				layer->saveResources(resources.path());
				foreach(KTKeyFrame *frame, layer->frames())
				{
					frame->saveResources(resources.path());
					foreach(AGraphicComponent *component, frame->components())
					{
						component->saveResources(resources.path());
					}
				}
			}
		}
	}
}

bool KTProjectManager::load(const QString &path)
{
	dDebug() << "Loading: " << path;
	
	KTProjectParser parser;
	
	if ( parser.parse(path) )
	{
		setProjectName( parser.partName() );
		setDocumentSize( parser.documentSize());
		
		QFileInfo info(path);
		foreach(QString location, parser.locations())
		{
			QString docPath = info.absolutePath ()+"/"+location+"/document.ktd";
			
			KTDocument *doc = createDocument();
			doc->load(docPath);
			
			Scenes scenes = doc->scenes();
			foreach(KTScene *scene, scenes )
			{
				emit sceneCreated( scene->sceneName(), true);
				
				connectScene( scene );
				
				Layers layers = scene->layers();
				foreach(KTLayer *layer, layers)
				{
					emit layerCreated(layer->layerName(), true);
					
					connect(layer, SIGNAL(frameCreated( const QString &, bool)), this, SIGNAL(frameCreated(  const QString& , bool)));
		
					connect(layer, SIGNAL(frameMoved(bool)), this, SIGNAL(frameMoved(bool))); 
		
					connect(layer, SIGNAL(frameRemoved()), this, SIGNAL(frameRemoved()));
					connect(layer, SIGNAL(frameLocked()), this, SIGNAL(frameLocked()));
					connect(layer, SIGNAL(visibilityChanged(bool)), this, SLOT(emitLayerVisibility(bool)));
					
					Frames frames = layer->frames();
					
					foreach(KTKeyFrame *frame, frames)
					{
						emit frameCreated( frame->frameName(), true);
					}
				}
			}
		}
		m_isOpen = true;
	}
	else
	{
		dError() << "Error while parse file: " << path;
		
		return false;
	}
	
	return m_isOpen;
	
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
	
	
// 	createScene();
// 	createLayer();
// 	createFrame();
	
	return doc;
}

KTDocument *KTProjectManager::currentDocument() const
{
	return m_currentDocument;
}

KTScene *KTProjectManager::currentScene()
{
	if ( ! m_currentDocument )
	{
		dFatal() << "No current document" << endl;
		return 0;
	}
	
	return m_currentDocument->currentScene();
}

KTLayer *KTProjectManager::currentLayer()
{
	if ( ! currentScene() )
	{
		dFatal() << "No current scene" << endl;
		return 0;
	}
	
	return currentScene()->currentLayer();
}

KTKeyFrame *KTProjectManager::currentKeyFrame()
{
	if ( ! currentLayer() )
	{
		dFatal() << "No current layer" << endl;
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
		dFatal() << "ERROR" << endl;
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
		scene->setFPS( m_fps );
		connectScene( scene );
	}
	else
	{
		dFatal() << "--> No current document" << endl;
	}
}


void KTProjectManager::connectScene(const KTScene *scene)
{
	connect(scene, SIGNAL(layerCreated( const QString&, bool)), this, SIGNAL(layerCreated( const QString &, bool)));
	connect(scene, SIGNAL(layerRemoved( int)), this, SIGNAL(layerRemoved(int))) ;
	connect(scene, SIGNAL(layerSelected( int)), this, SIGNAL(layerSelected(int))) ;
	connect(scene, SIGNAL(layerMoved(bool)), this, SIGNAL(layerMoved(bool)));
}


void KTProjectManager::removeScene()
{
	FUNC_NOT_IMPLEMENTED;
}

void KTProjectManager::renameScene(const QString &name, int index)
{
	FUNC_NOT_IMPLEMENTED;
}

// Layers
void KTProjectManager::createLayer(bool addToEnd)
{
	D_FUNCINFO;
	KTScene *scene = currentScene();
	if ( scene )
	{
		KTLayer *layer = scene->createLayer(QString::null, addToEnd);
		connect(layer, SIGNAL(frameCreated( const QString &, bool)), this, SIGNAL(frameCreated(  const QString& , bool)));
		
		connect(layer, SIGNAL(frameMoved(bool)), this, SIGNAL(frameMoved(bool))); 
		
		connect(layer, SIGNAL(frameRemoved()), this, SIGNAL(frameRemoved()));
		connect(layer, SIGNAL(frameLocked()), this, SIGNAL(frameLocked()));
		connect(layer, SIGNAL(visibilityChanged(bool)), this, SLOT(emitLayerVisibility(bool)));
		
		
	}
	else
	{
		dFatal() << "--> No current scene" << endl;
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
	dDebug() << "#KTProjectManager:: Inserting frame " << addToEnd <<  endl;
	
	KTLayer *layer = currentLayer();
	if ( layer )
	{
		KTKeyFrame *frame = layer->createFrame(QString::null , addToEnd);
	}
	else
	{
		dFatal() << "--> No current layer" << endl;
	}
}

void KTProjectManager::copyFrame(int index)
{
	dDebug() << "KTProjectManager::copyFrame()";
	KTLayer *layer = currentLayer();
	if ( layer && layer->frames().count() > index )
	{
		KTKeyFrame *frame = layer->frames()[index];
		if ( frame )
		{
			m_copyFrame = frame;
			if ( m_copyFrame->components()[0] == frame->components()[0] )
			{
			}
		}
	}
	else
	{
		dFatal() << "--> No current layer" << endl;
	}
}

void KTProjectManager::pasteFrame(int index )
{
	dDebug() << "KTProjectManager::pasteFrame()" << endl;
	KTLayer *layer = currentLayer();
	if ( layer && m_copyFrame)
	{
		layer->pasteFrame(index, m_copyFrame );
	}
	else
	{
		dFatal() << "--> No current layer" << endl;
	}
}

void  KTProjectManager::cloneFrame(int index, int nClones)
{
	KTLayer *layer = currentLayer();
	if ( layer )
	{
		layer->cloneFrame(index, nClones  );
	}
	else
	{
		dFatal() << "--> No current layer" << endl;
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
// 	dDebug() << "emit KTProjectManager::removeFrame();";
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

void  KTProjectManager::renameLayer(int indexLayer, const QString & name)
{
	dDebug() << "KTProjectManager::renameLayer(int indexLayer, const QString & name)";
	if(currentScene())
	{
		currentScene()->layers()[indexLayer]->setLayerName(name);
		emit layerRenamed( indexLayer, name);
	}
}

void KTProjectManager::renameFrame(int indexLayer, int indexFrame, const QString & name)
{
	dDebug() << "KTProjectManager::renameFrame(int indexLayer, int indexFrame, const QString & name)";
	if(currentScene())
	{
		currentScene()->layers()[indexLayer]->frames()[indexFrame]->setFrameName(name);
		emit frameRenamed(indexLayer, indexFrame,  name);
	}
	
}



void KTProjectManager::removeLayer(int index)
{
	KTScene *scene = currentScene();
	if( scene )
	{
		scene->removeLayer(index/*scene->indexCurrentLayer()*/);
	}
}

void KTProjectManager::moveLayer(bool up)
{
	D_FUNCINFO;
	KTScene *scene = currentScene();
	if ( scene )
	{
		scene->moveCurrentLayer(up);
	}
}

void KTProjectManager::close()
{
	for(int i = 0; i < m_documents.count(); i++ )
	{
		delete m_documents.takeAt(i);
	}
	
	m_currentDocument = 0;
	m_isOpen = false;
}

void KTProjectManager::setProjectName(const QString &name)
{
	m_name = name;
}

QString KTProjectManager::projectName() const
{
	return m_name;
}

void  KTProjectManager::setDocumentSize(const QSize& size )
{
	m_size = size;
}

void KTProjectManager::setProjectFPS( int fps )
{
	m_fps = fps;
}

int KTProjectManager::fps()
{
	return m_fps;
}


QSize KTProjectManager::documentSize() const
{
	return m_size;
}

bool KTProjectManager::isOpen()
{
	return m_isOpen;
}

void KTProjectManager::init()
{
	if ( m_isOpen )
	{
		close();
	}
	
	m_isOpen = true;
}



