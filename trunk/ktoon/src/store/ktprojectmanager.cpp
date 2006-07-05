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
#include "ktscene.h"
#include "ktlayer.h"
#include "ktframe.h"
#include "ktframeevent.h"


/**
 * Constructor por defecto
 */
KTProject::KTProject(QObject *parent) : QObject(parent), m_isOpen(false), m_currentSceneIndex(-1)
{
	DINIT;
}


/**
 * Destructor por defecto
 */
KTProject::~KTProject()
{
	DEND;
}

/**
 * Cierra el proyecto
 */
void KTProject::close()
{
	qDeleteAll(m_scenes);
	
	m_isOpen = false;
}

/**
 * Pone un nombre al proyecto
 */
void KTProject::setProjectName(const QString &name)
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
QString KTProject::projectName() const
{
	return m_name;
}

/**
 * Retorna verdadero si el proyecto esta abierto
 */
bool KTProject::isOpen()
{
	return m_isOpen;
}

/**
 * Inicializa el proyecto, esta funcion abre un nuevo proyecto
 */
void KTProject::init()
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
KTScene * KTProject::createScene(bool addToEnd)
{
	KTScene *scene = new KTScene(this);
	
	if ( addToEnd )
	{
		m_currentSceneIndex = m_scenes.count();
		m_scenes << scene;
	}
	else if ( m_currentSceneIndex > 0 && m_currentSceneIndex < m_scenes.count() )
	{
		m_currentSceneIndex++;
		m_scenes.insert(m_currentSceneIndex, scene);
	}
	else
	{
		dError() << "Error creating scene manager";
		delete scene;
		return 0;
	}
	
	KTSceneEvent event(KTProjectEvent::Add, scene->sceneName(), m_currentSceneIndex);
	
	emit commandExecuted(&event);
	
	return scene;
}

/**
 * 
 * @return 
 */
KTScene * KTProject::currentScene()
{
	if ( m_currentSceneIndex >= 0 && m_currentSceneIndex < m_scenes.count() )
	{
		return m_scenes[m_currentSceneIndex];
	}
	
	return 0;
}


/**
 * 
 * @param addToEnd 
 * @return 
 */
KTLayer *KTProject::createLayer(bool addToEnd)
{
	KTScene *scene = currentScene();
	
	if ( scene )
	{
		KTLayer *layer = scene->createLayer(addToEnd);
		
		KTLayerEvent event(KTProjectEvent::Add, layer->layerName(),m_currentSceneIndex, scene->currentLayerIndex() );
	
		emit commandExecuted(&event);
		
		return layer;
	}
	else
	{
		D_CHECKPTR(scene);
	}
	
	
	return 0;
}

/**
 * 
 * @return 
 */
KTLayer *KTProject::currentLayer()
{
	if ( currentScene() )
	{
		return currentScene()->currentLayer();
	}
	
	return 0;
}

/**
 * Crea un nuevo frame
 * @param addToEnd 
 * @return 
 */
KTFrame *KTProject::createFrame(bool addToEnd)
{
	KTLayer *layer = currentLayer();
	
	if ( layer )
	{
		KTFrame *frame = layer->createFrame(addToEnd);
		
		KTFrameEvent event(KTProjectEvent::Add, frame->frameName(), m_currentSceneIndex, currentScene()->currentLayerIndex(), layer->currentFrameIndex() );
	
		emit commandExecuted(&event);
		
		return frame;
	}
	else
	{
		D_CHECKPTR(layer);
	}
	
	return 0;
}

/**
 * Retorna el frame actual
 * @return 
 */
KTFrame *KTProject::currentFrame()
{
	if ( currentLayer() )
	{
		return currentLayer()->currentFrame();
	}
	
	return 0;
}
