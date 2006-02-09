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

#include "ktapplication.h"

#include "ktprojectparser.h"

KTProjectManager::KTProjectManager(QObject *parent) : KTSerializableObject(parent), m_currentDocument(0), m_copyFrame(0), m_isOpen(false)
{
	KTINIT;
}


KTProjectManager::~KTProjectManager()
{
	KTEND;
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
	
	QDomElement renderType = doc.createElement("renderType");
	renderType.setAttribute("value", m_typeRender);
	meta.appendChild(renderType);
	
	project.appendChild(meta);
	
	Documents::ConstIterator documentIt = m_documents.begin();
	
	int documentCounter = 0;
	while( documentIt != m_documents.end() )
	{
		QString location = QString("Document%1").arg(documentCounter++);
		QString docPath = ktapp->repository() + +"/"+m_name+"/"+location;
		
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
	
	QDir repository(KTOON_REPOSITORY);

	if ( ! repository.exists() ) 
	{ 
		return;
	}
	else
	{
		if ( !repository.exists(m_name) )
		{
			repository.mkdir(m_name);
		}
	}
	QDomDocument doc;
	QDomElement root = doc.createElement("KToon");
	doc.appendChild(root);
	root.appendChild(createXML( doc ));
	
	QFile save(ktapp->repository() + +"/"+m_name+"/"+m_name+".ktn");
	
	if ( save.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&save);
		out << doc.toString();
		
		save.close();
	}
}

void KTProjectManager::load(const QString &path)
{
	ktDebug() << "Loading: " << path;
	KTProjectParser parser;
	QXmlSimpleReader reader;
	reader.setContentHandler(&parser);
	reader.setErrorHandler(&parser);
		
	QFile source(path);
	QXmlInputSource xmlsource(&source);
		
	if ( reader.parse(&xmlsource) )
	{
		setProjectName( parser.partName() );
		setDocumentSize( parser.documentSize());
		
		QFileInfo info(source);
		foreach(QString location, parser.locations())
		{
			QString docPath = info.absolutePath ()+"/"+location+"/document.ktd";
			
			KTDocument *doc = createDocument();
			doc->load(docPath);
			
			Scenes scenes = doc->scenes();
			foreach(KTScene *scene, scenes )
			{
				emit sceneCreated( scene->sceneName(), true);
				connect(scene, SIGNAL(layerCreated( const QString&, bool)), this, SIGNAL(layerCreated( const QString &, bool)));
				connect(scene, SIGNAL(layerRemoved( int)), this, SIGNAL(layerRemoved(int))) ;
				
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
		ktError() << "Error while parse file: " << source.fileName();
	}
	
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
		scene->setFPS( m_fps );
		connect(scene, SIGNAL(layerCreated( const QString&, bool)), this, SIGNAL(layerCreated( const QString &, bool)));
		connect(scene, SIGNAL(layerRemoved( int)), this, SIGNAL(layerRemoved(int))) ;
		connect(scene, SIGNAL(layerSelected( int)), this, SIGNAL(layerSelected(int))) ;
		connect(scene, SIGNAL(layerMoved(bool)), this, SIGNAL(layerMoved(bool)));
	}
	else
	{
		ktFatal() << "--> No current document" << endl;
	}
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
		KTKeyFrame *frame = layer->createFrame(addToEnd);
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

void  KTProjectManager::setDocumentSize(const QSize& size )
{
	m_size = size;
}

void KTProjectManager::setProjectRender( const QString &typeRender)
{
	m_typeRender = typeRender;
}
void KTProjectManager::setProjectFPS( int fps )
{
	m_fps = fps;
}

QString KTProjectManager::projectRender( ) const
{
	return m_typeRender;
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



