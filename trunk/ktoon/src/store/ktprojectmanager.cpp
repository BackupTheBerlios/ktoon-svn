/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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


// #include "ktprojectparser.h"
#include "ktscenemanager.h"
#include "ktlayer.h"
#include "ktframe.h"
#include "ktframecommand.h"


/**
 * Constructor por defecto
 */
KTProjectManager::KTProjectManager(QObject *parent) : QObject(parent), m_isOpen(false), m_currentSceneManagerIndex(-1)
{
	DINIT;
}


/**
 * Destructor por defecto
 */
KTProjectManager::~KTProjectManager()
{
	DEND;
}

/**
 * Cierra el proyecto
 */
void KTProjectManager::close()
{
	qDeleteAll(m_sceneManagerList);
	
	m_isOpen = false;
}

/**
 * Pone un nombre al proyecto
 */
void KTProjectManager::setProjectName(const QString &name)
{
	m_name = name;
}

/**
 * @if spanish
 * Retorna el nombre del proyecto
 * @endif
 * 
 * @if english
 * Returns project name
 * @endif
 */
QString KTProjectManager::projectName() const
{
	return m_name;
}

/**
 * Retorna verdadero si el proyecto esta abierto
 */
bool KTProjectManager::isOpen()
{
	return m_isOpen;
}

/**
 * Inicializa el proyecto, esta funcion abre un nuevo proyecto
 */
void KTProjectManager::init()
{
	if ( m_isOpen )
	{
		close();
	}
	
	m_isOpen = true;
}


/**
 * 
 * @param addToEnd 
 * @return 
 */
KTSceneManager * KTProjectManager::createSceneManager(bool addToEnd)
{
	KTSceneManager *sceneManager = new KTSceneManager(this);
	
	if ( addToEnd )
	{
		m_sceneManagerList << sceneManager;
	}
	else if ( m_currentSceneManagerIndex > 0 && m_currentSceneManagerIndex < m_sceneManagerList.count() )
	{
		m_sceneManagerList.insert(m_currentSceneManagerIndex+1, sceneManager);
	}
	else
	{
		dError() << "Error creating scene manager";
		delete sceneManager;
		return 0;
	}
	
// 	connect(sceneManager, SIGNAL(layerCreated(const QString &, bool)), this, SIGNAL(layerCreated(const QString &, bool)));
	
	return sceneManager;
}

/**
 * 
 * @return 
 */
KTSceneManager * KTProjectManager::currentSceneManager()
{
	if ( m_currentSceneManagerIndex >= 0 && m_currentSceneManagerIndex < m_sceneManagerList.count() )
	{
		return m_sceneManagerList[m_currentSceneManagerIndex];
	}
	
	return 0;
}


/**
 * 
 * @param addToEnd 
 * @return 
 */
KTLayer *KTProjectManager::createLayer(bool addToEnd)
{
	KTSceneManager *scene = currentSceneManager();
	
	if ( scene )
	{
		KTLayer *layer = scene->createLayer(addToEnd);
		
// 		connect(layer, SIGNAL(frameCreated( const QString &, bool)), this, SIGNAL(frameCreated(const QString& , bool)));
		
		KTProjectCommand *command = new KTProjectCommand(KTFrameCommand::Add, layer->layerName(), scene->currentLayerIndex());
		
		emit commandExecuted(command);
		
		return layer;
	}
	else
	{
		dFatal() << "--> No current scene" << endl;
	}
	
	return 0;
}

/**
 * 
 * @return 
 */
KTLayer *KTProjectManager::currentLayer()
{
	if ( currentSceneManager() )
	{
		return currentSceneManager()->currentLayer();
	}
	
	return 0;
}

/**
 * Crea un nuevo frame
 * @param addToEnd 
 * @return 
 */
KTFrame *KTProjectManager::createFrame(bool addToEnd)
{
	KTLayer *layer = currentLayer();
	
	if ( layer )
	{
		KTFrame *frame = layer->createFrame(addToEnd);
		
		
		return frame;
	}
	else
	{
		dFatal() << "--> No current layer" << endl;
	}
	
	return 0;
}

/**
 * Retorna el frame actual
 * @return 
 */
KTFrame *KTProjectManager::currentFrame()
{
	if ( currentLayer() )
	{
		return currentLayer()->currentFrame();
	}
	
	return 0;
}
