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

KTProjectManager::KTProjectManager(QObject *parent) : QObject(parent), m_currentDocument(0)
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
	m_documents << doc;
	
	connect(doc, SIGNAL(sceneCreated(const QString &)), this, SIGNAL(sceneInserted(const QString &)));
	
	doc->createScene();
	
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
		return 0;
	}
	
	return m_currentDocument->currentScene();
}

KTLayer *KTProjectManager::currentLayer()
{
	if ( ! currentScene() )
	{
		return 0;
	}
	return currentScene()->currentLayer();
}

KTKeyFrame *KTProjectManager::currentKeyFrame()
{
	if ( ! currentLayer() )
	{
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

void KTProjectManager::insertScene()
{
	KTScene *scene = m_currentDocument->createScene();
}

void KTProjectManager::removeScene()
{
}

void KTProjectManager::renameScene(const QString &name, int index)
{
// 	emit sceneRenamed(name, index);
}

// Frames

void KTProjectManager::insertFrame()
{
	ktDebug() << "Inserting frame" << endl;
	KTKeyFrame *keyFrame = currentLayer()->createFrame();
	
	emit frameInserted();
}
