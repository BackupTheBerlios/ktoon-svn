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

#include "ktdocument.h"

#include "ddebug.h"

#include <QDir>
#include <QDomDocument>

#include "ktprojectparser.h"

KTDocument::KTDocument(QObject *parent) : KTSerializableObject(parent), m_currentScene(0), m_sceneCount(0)
{
	DINIT;
// 	m_currentScene = createScene();
}


KTDocument::~KTDocument()
{
	DEND;
	for(int i = 0; i < m_scenes.count(); i++ )
	{
		delete m_scenes.takeAt(i);
	}
}

QDomElement KTDocument::createXML(  QDomDocument &doc )
{
	QDomElement document = doc.createElement("Document");

	return document;
}

void KTDocument::save(const QString &docPath)
{
	QDir docDir(docPath);
	
	if ( ! docDir.exists() )
	{
		docDir.mkdir(docDir.path());
	}
	
	
	//TODO: añadir nombre al documento
	
	QDomDocument document;
	
	QDomElement root = createXML(document);
	root.setAttribute("name", m_name);
// 	Scenes::ConstIterator sceneIt = m_scenes.begin();
	
	document.appendChild(root);
	for( int i = 0; i < m_scenes.count(); i++)
	{
		QString scenePath = docPath+"/"+QString("Scene%1").arg(i);
		QDomElement scn = m_scenes[i]->createXML(document);
		scn.setAttribute("location", QString("Scene%1").arg(i));
			
		root.appendChild(scn);
			
		m_scenes[i]->save( scenePath );
	}
	
	QFile save(docPath+"/"+/*m_name*/"document"+".ktd");
	
	if ( save.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&save);
		out << document.toString();
		
		save.close();
	}
	
}

void KTDocument::load(const QString &path)
{
	dDebug() << "Loading doc: " << path;
	
	KTProjectParser parser;
	QXmlSimpleReader reader;
	reader.setContentHandler(&parser);
	reader.setErrorHandler(&parser);
		
	QFile source(path);
	QXmlInputSource xmlsource(&source);
		
	if ( reader.parse(&xmlsource) )
	{
		setDocumentName( parser.partName() );
		
		QFileInfo info(source);
		foreach(QString location, parser.locations())
		{
			QString scenePath = info.absolutePath ()+"/"+location+"/scene.kts";
			
			KTScene *scene = new KTScene(this);
			m_currentScene = scene;
			
			m_scenes << scene;
			scene->load(scenePath);
		}
	}
	else
	{
		dError() << "Error while parse file: " << source.fileName();
	}
}

Scenes KTDocument::scenes() const
{
	return m_scenes;
}

void KTDocument::setScenes(const Scenes &scenes)
{
	m_scenes = scenes;
}

KTScene *KTDocument::createScene(bool addToEnd)
{
	dDebug() << "Creating Scene" << m_sceneCount+1 << endl;
	
	KTScene *scene = new KTScene(this);
	scene->setSceneName( tr("Scene %1").arg(m_sceneCount++));

	if ( addToEnd )
	{
		m_scenes << scene;
	}
	else
	{
		m_scenes.insert(m_scenes.indexOf(m_currentScene)+1, scene);
	}
	m_currentScene = scene;
	
	emit sceneCreated(scene->sceneName(), addToEnd);
	
	return scene;
}


KTScene *KTDocument::currentScene()
{
	return m_currentScene;
}

void KTDocument::setCurrentScene(int index)
{
	KTScene *scene = m_scenes[index];
	if ( scene )
	{
		m_currentScene = scene;
		emit sceneChanged(m_currentScene);
	}
}

void KTDocument::setDocumentName(const QString &name)
{
	m_name = name;
}
