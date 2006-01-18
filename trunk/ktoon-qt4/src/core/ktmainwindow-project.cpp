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

#include "ktmainwindow.h"

#include "ktdebug.h"
#include "ktapplication.h"

// Animation

void KTMainWindow::insertScene( const QString &name, bool addedToEnd)
{
	m_scenes->insertScene( name, addedToEnd);
	m_exposureSheet->addScene( name);
	m_timeLine->addScene(name);
	
	
	
}

void KTMainWindow::changeScene(int index)
{
	m_exposureSheet->setScene( index);
	m_timeLine->setScene( index);
	
	m_scenes->setScene(index);
	m_projectManager->setCurrentScene( index);
	
	KTViewCamera *camera = qobject_cast<KTViewCamera *>(m_animationSpace->activeWindow());
	if(camera)
	{
		camera->animationArea()->setScene(m_projectManager->currentScene());
	}
	
}

void KTMainWindow::insertLayer(const QString &name, bool addedToEnd)
{
// 	ktDebug() << "KTMainWindow::insertLayer(" << name << "," << addedToEnd << ")";
// 	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
// 	if ( doc )
// 	{
// 		doc->drawArea()->setScene( m_projectManager->currentScene() );
// 	}
	
	m_exposureSheet->insertLayer(name);
	m_timeLine->createLayer( name );
}

void KTMainWindow::removeLayer(int index)
{
	m_exposureSheet->removeCurrentLayer();
	m_timeLine->removeCurrentLayer();
	
}

void KTMainWindow::setLayerVisibilityChanged(int idLayer, bool isVisible)
{
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		doc->drawArea()->redrawAll();
	}
}


void KTMainWindow::insertFrame(const QString &name, bool addedToEnd)
{
	KT_FUNCINFO;
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if(!doc)
	{
		newViewDocument( name);
	}
	
	m_exposureSheet->addFrame(m_projectManager->currentScene()->indexCurrentLayer(), name, addedToEnd);
	m_timeLine->addFrame(m_projectManager->currentScene()->indexCurrentLayer(), name, addedToEnd);
}

void KTMainWindow::moveFrame(bool up)
{
	m_exposureSheet->moveFrame(up);
}
void KTMainWindow::removeFrame()
{
	m_exposureSheet->removeCurrentFrame();
}

void KTMainWindow::lockFrame()
{
	m_exposureSheet->lockCurrentFrame();
}


void KTMainWindow::selectFrame(int layer, int frame)
{
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		m_projectManager->currentScene()->setCurrentLayer(layer);
		m_projectManager->setCurrentFrame( frame );
		doc->drawArea()->setLayer( layer );
		doc->drawArea()->setKeyFrame( frame );
		
		m_exposureSheet->setCurrentCell(layer, frame);
		
	}
}

// Graphic Components
void KTMainWindow::rotateCurrentElement(int a)
{
	KT_FUNCINFO;
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		AGraphicComponent *selected = doc->drawArea()->selectedGraphic();
		
		if ( selected )
		{
			selected->rotate(a);
			doc->drawArea()->redrawAll();
		}
	}
}

void KTMainWindow::scaleCurrentElement(double dx,double dy)
{
	KT_FUNCINFO;
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		AGraphicComponent *selected = doc->drawArea()->selectedGraphic();
		if ( selected )
		{
			selected->scale(dx, dy);
			doc->drawArea()->redrawAll();
		}
	}
}

void KTMainWindow::translateCurrentElement(double dx ,double dy)
{
	KT_FUNCINFO;
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		AGraphicComponent *selected = doc->drawArea()->selectedGraphic();
		if ( selected )
		{
			selected->translate(dx, dy);
			doc->drawArea()->redrawAll();
		}
	}
}

void KTMainWindow::shearCurrentElement(double dx,double dy)
{
	KT_FUNCINFO;
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		AGraphicComponent *selected = doc->drawArea()->selectedGraphic();
		if ( selected )
		{
			selected->shear(dx, dy);
			doc->drawArea()->redrawAll();
		}
	}
}

void KTMainWindow::addCurrentGraphicToLibrary()
{
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		AGraphicComponent *selected = doc->drawArea()->selectedGraphic();
		if ( selected )
		{
			m_libraryWidget->addGraphic(selected);
		}
	}
}

void KTMainWindow::addGraphicComponent(const AGraphicComponent *graphic)
{
	AGraphicComponent *copy = new AGraphicComponent(*graphic);
	
	KTKeyFrame *frame = m_projectManager->currentKeyFrame();
	
	if ( ! frame )
	{
		return;
	}
	
	frame->addComponent( copy );
	
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		doc->drawArea()->redrawAll();
	}
}
