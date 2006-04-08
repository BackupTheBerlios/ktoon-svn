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

#include "ddebug.h"
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

void KTMainWindow::selectLayer(int index)
{
	m_exposureSheet->setLayer(index);
	m_timeLine->setLayer( index );
}


void KTMainWindow::setLayerName(int indexLayer, const QString& name)
{
	m_exposureSheet->setLayerName(indexLayer, name);
}

void KTMainWindow::insertLayer(const QString &name, bool addedToEnd)
{
// 	dDebug() << "KTMainWindow::insertLayer(" << name << "," << addedToEnd << ")";
// 	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
// 	if ( doc )
// 	{
// 		doc->drawArea()->setScene( m_projectManager->currentScene() );
// 	}
	
	m_exposureSheet->insertLayer(name);
	m_timeLine->insertLayer( name, addedToEnd );
}

void KTMainWindow::moveLayer(bool up)
{
	D_FUNCINFO;
 	m_exposureSheet->moveLayer(up);
	m_timeLine->moveLayer(up);
	
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		doc->drawArea()->redrawAll();
	}
}

void KTMainWindow::removeLayer(int index)
{
	m_exposureSheet->removeLayer(index);
	m_timeLine->removeLayer(index);
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
	D_FUNCINFO;
	
	m_exposureSheet->setUpdatesEnabled(false);
	m_timeLine->setUpdatesEnabled(false);
	setUpdatesEnabled(false);
	
	m_exposureSheet->addFrame(m_projectManager->currentScene()->indexCurrentLayer(), name, addedToEnd); // FIXME: insert!
	m_timeLine->insertFrame(m_projectManager->currentScene()->indexCurrentLayer(), name, addedToEnd);
	
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if(!doc && m_projectManager->isOpen())
	{
		newViewDocument( name);
	}
	
	m_exposureSheet->setUpdatesEnabled(true);
	m_timeLine->setUpdatesEnabled(true);
	setUpdatesEnabled(true);
}

void KTMainWindow::moveFrame(bool up)
{
	m_exposureSheet->moveFrame(up);
	m_timeLine->moveFrame(up);
}
void KTMainWindow::removeFrame()
{
	m_exposureSheet->removeCurrentFrame();
	m_timeLine->removeCurrentFrame();
}

void KTMainWindow::lockFrame()
{
	m_exposureSheet->lockCurrentFrame();
	m_timeLine->lockCurrentFrame();
}

void KTMainWindow::setFrameName(int indexLayer, int indexFrame, const QString& name )
{
	m_exposureSheet->setFrameName( indexLayer,  indexFrame,  name );
}

void KTMainWindow::selectFrame(int layer, int frame)
{
	D_FUNCINFO;
	SHOW_VAR(layer);
	SHOW_VAR(frame);
	
	if ( layer < 0 || frame < 0 ) return;
	
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		KTScene *currentScene = m_projectManager->currentScene();
		if ( currentScene )
		{
			if ( layer >= 0 && layer < currentScene->layers().count() )
			{
				currentScene->setCurrentLayer(layer);
			}
			
			KTLayer *currentLayer = m_projectManager->currentLayer();
			
			if ( currentLayer )
			{
// 				if ( frame >= 0 && frame < currentLayer->frames().count() )
				{
					m_projectManager->setCurrentFrame( frame );
					
					doc->drawArea()->setLayer( layer );
					doc->drawArea()->setKeyFrame( frame );
					
					m_exposureSheet->setCurrentCell( layer, frame);
					m_timeLine->setCurrentCell( layer, frame);
				}
			}
		}
	}
	else
	{
		if(m_drawingSpace->isVisible() && m_projectManager->isOpen() )
		{
			newViewDocument( m_projectManager->currentDocument()->currentScene()->layers()[layer]->frames()[frame]->frameName() );
// 			selectFrame(layer, frame);
		}
	}
	
#if 0
	KTKeyFrame *currentFrame = m_projectManager->currentKeyFrame();
	foreach(AGraphicComponent *component, currentFrame->components())
	{
		KTGCEditor::SGCItem item;
		item.name = component->componentName();
		
		
		m_gcEditor->addItem( item);
	}
#endif
}

// Graphic Components
void KTMainWindow::rotateCurrentElement(int a)
{
	D_FUNCINFO;
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		//FIXME: tomar todos los graficos seleccionados
		
		KTKeyFrame *cFrame = m_projectManager->currentKeyFrame();
		
		if ( cFrame )
		{
			if ( cFrame->hasSelections() )
			{
				AGraphicComponent *selected = cFrame->selectedComponents()[0];
		
				if ( selected )
				{
					selected->rotate(a);
					doc->drawArea()->redrawAll();
				}
			}
		}
	}
}

void KTMainWindow::scaleCurrentElement(double dx,double dy)
{
	D_FUNCINFO;
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		KTKeyFrame *cFrame = m_projectManager->currentKeyFrame();
		
		if ( cFrame )
		{
			QList<AGraphicComponent *>selecteds = cFrame->selectedComponents();
			if ( selecteds.count() > 0 )
			{
				foreach( AGraphicComponent * selected,  selecteds )
				{
					selected->scale(dx, dy);
				}
				doc->drawArea()->redrawAll();
			}
		}
	}
}

void KTMainWindow::translateCurrentElement(double dx ,double dy)
{
	D_FUNCINFO;
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		KTKeyFrame *cFrame = m_projectManager->currentKeyFrame();
		
		if ( cFrame )
		{
			QList<AGraphicComponent *>selecteds = cFrame->selectedComponents();
		
			if ( selecteds.count() > 0 )
			{
				foreach( AGraphicComponent * selected,  selecteds )
				{
					selected->translate(dx, dy);
				}
				doc->drawArea()->redrawAll();
			}
		}
	}
}

void KTMainWindow::shearCurrentElement(double dx,double dy)
{
	D_FUNCINFO;
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		KTKeyFrame *cFrame = m_projectManager->currentKeyFrame();
		
		if ( cFrame )
		{
			QList<AGraphicComponent *>selecteds = cFrame->selectedComponents();
		
			if ( selecteds.count() > 0 )
			{
				foreach( AGraphicComponent * selected,  selecteds )
				{
					selected->shear(dx, dy);
				}
				doc->drawArea()->redrawAll();
			}
		}
	}
}

void KTMainWindow::addCurrentGraphicToLibrary()
{
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		KTKeyFrame *cFrame = m_projectManager->currentKeyFrame();
		
		if ( cFrame )
		{
			QList<AGraphicComponent *>selecteds = cFrame->selectedComponents();
		
			if ( selecteds.count() > 0 )
			{
				AGraphicComponent *form = new AGraphicComponent();
				foreach( AGraphicComponent * selected,  selecteds )
				{
					form->addChild( new AGraphicComponent(*selected));
				}
				
				m_libraryWidget->addGraphic(form);
			}
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
