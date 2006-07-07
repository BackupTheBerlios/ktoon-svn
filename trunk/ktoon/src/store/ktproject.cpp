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

#include "ktproject.h"

#include <ddebug.h>

#include "ktscene.h"
#include "ktlayer.h"
#include "ktframe.h"


#include "ktframeevent.h" //events

/**
 * Constructor por defecto
 */
KTProject::KTProject(QObject *parent) : QObject(parent)
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
void KTProject::clear()
{
	qDeleteAll(m_scenes);
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


KTScene *KTProject::createScene(int position)
{
	
	
	KTScene *scene = new KTScene(this); // TODO: Ponerle nombre!
	m_scenes.insert(position, scene);
	
	KTSceneEvent event(KTProjectEvent::Add, position);
	event.setPartName(scene->sceneName());
	
	emit commandExecuted(&event);
	
	return scene;
}

KTLayer *KTProject::createLayer(int scenePosition, int position)
{
	KTScene *scene = this->scene(scenePosition);
	
	if ( scene )
	{
		KTLayer *layer = scene->createLayer(position);
		
		KTLayerEvent event(KTProjectEvent::Add, scenePosition, position );
		event.setPartName(layer->layerName());
	
		emit commandExecuted(&event);
		
		return layer;
	}
	else
	{
		D_CHECKPTR(scene);
	}
	
	return 0;
}

KTFrame *KTProject::createFrame(int scenePosition, int layerPosition, int position)
{
	KTScene *scene = this->scene(scenePosition);
	
	if ( !scene)
	{
		return 0;
	}
	
	KTLayer *layer = scene->layer(layerPosition);
	
	if ( layer )
	{
		KTFrame *frame = layer->createFrame(position);
		
		KTFrameEvent event(KTProjectEvent::Add, scenePosition, layerPosition, position );
		event.setPartName(frame->frameName());
		
		
		emit commandExecuted(&event);
		
		return frame;
	}
// 	else
// 	{
// 		D_CHECKPTR(layer);
// 	}
	
	return 0;
}

KTScene *KTProject::scene(int position)
{
	if ( position < 0 || position > m_scenes.count() )
	{
		D_FUNCINFO << " FATAL ERROR: index out of bound";
		return 0;
	}
	
	return m_scenes[position];
}


