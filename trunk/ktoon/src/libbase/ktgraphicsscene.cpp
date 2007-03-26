/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#include "ktgraphicsscene.h"

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

#include <ktscene.h>

#include "ktgraphicobject.h"
#include "ktitemgroup.h"
#include "ktprojectloader.h"
#include "ktitemfactory.h"
#include "kttoolplugin.h"
#include "ktinputdeviceinformation.h"
#include "ktbrushmanager.h"
#include "ktframe.h"

#include "ktprojectresponse.h"

#include <dcore/ddebug.h>

#include "ktlineguide.h"

struct KTGraphicsScene::Private
{
	KTToolPlugin *tool;
	
	KTScene *scene;
	struct OnionSkin
	{
		int previous;
		int next;
		QMap<QGraphicsItem *, double> opacityMap;
	} onionSkin;
	
	struct FramePosition
	{
		int layer;
		int frame;
	} framePosition;
	
	bool isDrawing;
	KTBrushManager *brushManager;
	KTInputDeviceInformation *inputInformation;
};

KTGraphicsScene::KTGraphicsScene() : QGraphicsScene(), d(new Private)
{
	setItemIndexMethod(QGraphicsScene::NoIndex);
	setCurrentFrame( -1, -1);
	
	d->onionSkin.next = 0;
	d->onionSkin.previous = 0;
	d->tool = 0;
	d->isDrawing = false;
	
	
	d->inputInformation = new KTInputDeviceInformation(this);
	d->brushManager = new KTBrushManager(this);
	
}


KTGraphicsScene::~KTGraphicsScene()
{
	DEND;
	
	clearFocus();
	clearSelection();
	
	foreach ( QGraphicsView *view, this->views() )
	{
		view->setScene(0);
	}
	
	foreach(QGraphicsItem *item, items())
	{
		removeItem(item);
	}
	
	delete d;
}

void KTGraphicsScene::setCurrentFrame(int layer, int frame)
{
	d->framePosition.layer = layer;
	d->framePosition.frame = frame;
	
	if (! this->currentFrame() )
	{
		foreach(QGraphicsView *view, views() )
		{
			view->setDragMode(QGraphicsView::NoDrag);
		}
	}
	else
	{
		foreach(QGraphicsView *view, views() )
		{
			view->setDragMode(QGraphicsView::NoDrag);
		}
	}
}

void KTGraphicsScene::drawCurrentPhotogram()
{
	drawPhotogram( d->framePosition.frame );
}



void KTGraphicsScene::drawItems(QPainter *painter, int numItems, QGraphicsItem *items[], const QStyleOptionGraphicsItem options[], QWidget *widget)
{
	for (int i = 0; i < numItems; ++i)
	{
		QGraphicsItem *item = items[i];
		painter->save();
		painter->setMatrix(item->sceneMatrix(), true);
		
		if ( d->onionSkin.opacityMap.contains(item) )
		{
			painter->setOpacity( d->onionSkin.opacityMap[item] );
		}
		
		item->paint(painter, &options[i], widget);
		
		painter->restore();
	}
}


void KTGraphicsScene::drawPhotogram(int photogram)
{
	if ( photogram < 0 ) return;
	
	clean();
	
	foreach(KTLayer *layer, d->scene->layers())
	{
		if ( layer->isVisible() )
		{
			double opacityFactor = 0.5 / (double)qMin(layer->frames().count(),d->onionSkin.previous);
			
			double opacity = 0.6;
			
			for(int frameIndex = photogram-1; frameIndex > photogram-d->onionSkin.previous-1; frameIndex-- )
			{
				addFrame( layer->frame(frameIndex), opacity );
				
				opacity -= opacityFactor;
			}
			
			opacityFactor = 0.5 / (double)qMin(layer->frames().count(), d->onionSkin.next);
			opacity = 0.6;
			
			for(int frameIndex = photogram+1; frameIndex < photogram+d->onionSkin.next+1; frameIndex++ )
			{
				addFrame( layer->frame(frameIndex), opacity );
				
				opacity -= opacityFactor;
			}
			
			addFrame(layer->frame( photogram ));
		}
	}
}

void KTGraphicsScene::addFrame(KTFrame *frame, double opacity )
{
	D_FUNCINFO;
	if ( frame )
	{
		foreach(KTGraphicObject *object, frame->graphics() )
		{
			QGraphicsItem *item = object->item();
			d->onionSkin.opacityMap.insert(item, opacity);
			
// 			KTItemFactory factory;
// 			QDomDocument doc;
			
			
			if( ! qgraphicsitem_cast<KTItemGroup *>(item->parentItem()))
			{
// 				doc.appendChild(object->toXml(doc));
// 				addItem(factory.create( doc.toString() ));
				addItem(item);
			}
			
			
			if ( KTItemGroup *group = qgraphicsitem_cast<KTItemGroup *>(item) )
			{
				group->recoverChilds();
			}
		}
	}
}

void KTGraphicsScene::clean()
{
	d->onionSkin.opacityMap.clear();
	
	foreach(QGraphicsItem *item, items() )
	{
		if ( item->scene() == this )
		{
			removeItem(item);
		}
	}
}

int KTGraphicsScene::currentFrameIndex() const
{
	return d->framePosition.frame;
}

int KTGraphicsScene::currentLayerIndex() const
{
	return d->framePosition.layer;
}

int KTGraphicsScene::currentSceneIndex() const
{
	return d->scene->index();
}

void KTGraphicsScene::setNextOnionSkinCount(int n)
{
	d->onionSkin.next = n;
	drawCurrentPhotogram();
}

void KTGraphicsScene::setPreviousOnionSkinCount(int n)
{
	d->onionSkin.previous = n;
	
	drawCurrentPhotogram();
}

KTFrame *KTGraphicsScene::currentFrame()
{
	KTLayer *layer = d->scene->layer( d->framePosition.layer );
	if ( layer )
	{
		return layer->frame(d->framePosition.frame );
	}
	
	return 0;
}


void KTGraphicsScene::setCurrentScene(KTScene *scene)
{
	clean();
	setItemIndexMethod(QGraphicsScene::NoIndex);
	
	setBackgroundBrush(Qt::gray);
	
// 	if( d->scene )
// 	{
// 		setCurrentFrame( currentFrameIndex(), currentLayerIndex() );
// 	}
// 	else
// 	{
// 		if ( scene->layers().count() > 0 )
// 		{
// 			setCurrentFrame(0, 0);
// 		}
// 	}
	
	d->scene = scene;
	
	drawCurrentPhotogram();
}

void KTGraphicsScene::setLayerVisible(int layerIndex, bool visible)
{
	if( KTLayer *layer = d->scene->layer(layerIndex) )
	{
		layer->setVisible(visible);
	}
}

KTScene *KTGraphicsScene::scene() const
{
	return d->scene;
}

void KTGraphicsScene::setTool(KTToolPlugin *tool)
{
	if(d->tool)
	{
		d->tool->aboutToChangeTool();
	}
	
	d->tool = tool;
	d->tool->init(this);
	
}

KTToolPlugin *KTGraphicsScene::currentTool() const
{
	return d->tool;
}


void KTGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);
	
	d->inputInformation->updateFromMouseEvent(event);
	
	if ( event->buttons() == Qt::LeftButton &&  (event->modifiers () == (Qt::ShiftModifier | Qt::ControlModifier)))
	{
		d->isDrawing = false;
	}
	else if (d->tool )
	{
		if ( event->buttons() == Qt::LeftButton && !currentFrame()->isLocked() )
		{
			d->tool->begin();
			d->isDrawing = true;
			d->tool->press(d->inputInformation, d->brushManager, this );
		}
	}
}

void KTGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseMoveEvent(event);
	
	d->inputInformation->updateFromMouseEvent( event );
	
	if (d->tool && d->isDrawing )
	{
		d->tool->move(d->inputInformation, d->brushManager,  this );
	}
}

void KTGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseReleaseEvent(event);
	
	d->inputInformation->updateFromMouseEvent( event );
	
	if ( d->tool && d->isDrawing )
	{
		d->tool->release(d->inputInformation, d->brushManager, this );
		
		d->tool->end();
	}
	
	d->isDrawing = false;
}

void KTGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseDoubleClickEvent(event);
	
	d->inputInformation->updateFromMouseEvent( event );
	
	if (d->tool)
	{
		d->tool->doubleClick( d->inputInformation, this );
	}
}

void KTGraphicsScene::keyPressEvent(QKeyEvent *event)
{
	if ( d->tool )
	{
		d->tool->keyPressEvent(event);
		
		if ( event->isAccepted() )
		{
			return;
		}
	}
	
	QGraphicsScene::keyPressEvent(event);
}


void KTGraphicsScene::dragEnterEvent ( QGraphicsSceneDragDropEvent * event )
{
	//crear linea
	if (event->mimeData()->hasFormat("text/plain"))
		event->acceptProposedAction();
	
}


void  KTGraphicsScene::dragLeaveEvent ( QGraphicsSceneDragDropEvent * event )
{
	//TODO: eliminar linea
	
}

void KTGraphicsScene::dragMoveEvent ( QGraphicsSceneDragDropEvent * event )
{
	
	//TODO: mover linea
}

void KTGraphicsScene::dropEvent ( QGraphicsSceneDragDropEvent * event )
{
	KTGuideLine *line;
	if(event->mimeData()->text() == "lineVertical")
	{
		line  = new KTGuideLine(Qt::Vertical, this);
		line->setPos(event->scenePos());
	}
	else
	{
		line = new KTGuideLine(Qt::Horizontal, this);
		line->setPos(event->scenePos());
	}
}



void KTGraphicsScene::itemResponse(KTItemResponse *event)
{
	if ( d->tool )
	{
		d->tool->init(this); //FIXME:d->tool->init(this); in itemResponse ???
		d->tool->itemResponse(event);
	}
}

bool KTGraphicsScene::isDrawing() const
{
	return d->isDrawing;
}

KTBrushManager *KTGraphicsScene::brushManager() const
{
	return d->brushManager;
}

