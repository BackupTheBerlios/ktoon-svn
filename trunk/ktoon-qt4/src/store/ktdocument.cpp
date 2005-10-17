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

KTDocument::KTDocument(QObject *parent) : QObject(parent), m_currentScene(0), m_sceneCount(0)
{
	m_currentScene = createScene();
}


KTDocument::~KTDocument()
{
}

Scenes KTDocument::scenes() const
{
	return m_scenes;
}

void KTDocument::setScenes(const Scenes &scenes)
{
	m_scenes = scenes;
}

KTScene *KTDocument::createScene()
{
	KTScene *scene = new KTScene(this);
	
	m_scenes << scene;
	
	emit sceneCreated(tr("Scene %1").arg(m_sceneCount++), m_scenes.count()-1 ); // FIXME: Scene id incorrect
	
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
	}
}
