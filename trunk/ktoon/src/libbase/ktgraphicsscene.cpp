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
#include <ktlayer.h>

#include "ktgraphicobject.h"
#include "ktitemgroup.h"
#include "ktprojectloader.h"
#include "ktitemfactory.h"
#include "kttoolplugin.h"
#include "ktinputdeviceinformation.h"
#include "ktbrushmanager.h"
#include "ktframe.h"
#include "ktitemtweener.h"

#include "ktprojectresponse.h"

#include <dcore/ddebug.h>

#include "ktguideline.h"

struct KTGraphicsScene::Private
{
	KTToolPlugin *tool;
	
	KTScene *scene;
	struct OnionSkin
	{
		int previous;
		int next;
		QHash<QGraphicsItem *, double> opacityMap;
	} onionSkin;
	
	struct FramePosition
	{
		int layer;
		int frame;
	} framePosition;
	
	bool isDrawing;
	KTBrushManager *brushManager;
	KTInputDeviceInformation *inputInformation;
	
	QList<KTLineGuide *> lines;
};

KTGraphicsScene::KTGraphicsScene() : QGraphicsScene(), d(new Private)
{
	setItemIndexMethod(QGraphicsScene::NoIndex);
	setCurrentFrame( -1, -1);
	
	d->onionSkin.next = 0;
	d->onionSkin.previous = 0;
	d->tool = 0;
	d->isDrawing = false;
	
	setBackgroundBrush(Qt::gray);
	
	
	d->inputInformation = new KTInputDeviceInformation(this);
	d->brushManager = new KTBrushManager(this);
}


KTGraphicsScene::~KTGraphicsScene()
{
	DEND;
	
	clearFocus();
	clearSelection();
	
	foreach( QGraphicsView *view, this->views() )
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
	
	foreach(QGraphicsView *view, views() )
	{
		view->setDragMode(QGraphicsView::NoDrag);
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
	if ( photogram < 0 || !d->scene ) return;
	
	clean();
	
	bool valid = false;
	
	foreach(KTLayer *layer, d->scene->layers().values())
	{
		if ( layer->isVisible() )
		{
			if( d->onionSkin.previous > 0 )
			{
				double opacityFactor = 0.5 / (double)qMin(layer->frames().count(),d->onionSkin.previous);
				
				double opacity = 0.6;
				
				for(int frameIndex = photogram-1; frameIndex > photogram-d->onionSkin.previous-1; frameIndex-- )
				{
					KTFrame * frame = layer->frame(frameIndex);
					if(frame)
					{
						addFrame( frame, opacity );
					}
					opacity -= opacityFactor;
				}
			}
			
			if ( d->onionSkin.next > 0 )
			{
				double opacityFactor = 0.5 / (double)qMin(layer->frames().count(), d->onionSkin.next);
				double opacity = 0.6;
				
				for(int frameIndex = photogram+1; frameIndex < photogram+d->onionSkin.next+1; frameIndex++ )
				{
					
					KTFrame * frame = layer->frame(frameIndex);
					if(frame)
					{
						addFrame( frame, opacity );
					}
					opacity -= opacityFactor;
				}
			}
			
			KTFrame *frame = layer->frame( photogram );
			
			if( frame )
			{
				valid = true;
				addFrame(frame);
			}
		}
	}
	
	if( valid )
	{
		foreach(KTGraphicObject *object, d->scene->tweeningObjects())
		{
			if(object->frame()->layer()->isVisible() )
			{
				int origin = object->frame()->visualIndex();
				
				if( KTItemTweener *tweener = object->tweener() )
				{
					tweener->setStep(0);
					
					if( origin < photogram && photogram < origin+tweener->frames() )
					{
						int step = photogram - origin;
						
						tweener->setStep(step);
						
						addGraphicObject(object);
					}
				}
			}
		}
		
		update();
	}
	
	if( d->tool )
	{
		d->tool->updateScene(this);
	}
}

void KTGraphicsScene::addFrame(KTFrame *frame, double opacity )
{
	if ( frame )
	{
		foreach(KTGraphicObject *object, frame->graphics().values() )
		{
			addGraphicObject(object, opacity);
		}
	}
}

void KTGraphicsScene::addGraphicObject(KTGraphicObject *object, double opacity)
{
	QGraphicsItem *item = object->item();
	d->onionSkin.opacityMap.insert(item, opacity);
	
	if ( KTItemGroup *group = qgraphicsitem_cast<KTItemGroup *>(item) )
	{
		group->recoverChilds();
	}
	
	if( ! qgraphicsitem_cast<KTItemGroup *>(item->parentItem()))
	{
		item->setSelected(false);
		addItem(item);
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
	
	foreach(KTLineGuide *line, d->lines )
	{
		addItem(line);
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
	if( !d->scene ) return -1;
	
	return d->scene->visualIndex();
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
	if(d->scene)
	{
		KTLayer *layer = d->scene->layer( d->framePosition.layer );
		if ( layer )
		{
			if(!layer->frames().isEmpty())
			{
				return layer->frame(d->framePosition.frame );
			}
		}
	}
	return 0;
}


void KTGraphicsScene::setCurrentScene(KTScene *scene)
{
	Q_CHECK_PTR(scene);
	
	if(d->tool)
	{
		d->tool->aboutToChangeScene(this);
	}

	qDeleteAll(d->lines);
	d->lines.clear();
	

	clean();
	d->scene = scene;
	
	drawCurrentPhotogram();
}

void KTGraphicsScene::setLayerVisible(int layerIndex, bool visible)
{
	if( !d->scene ) return;
	
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
	drawCurrentPhotogram();
	if(d->tool)
	{
		if(d->tool->toolType() == KTToolPlugin::Selection )
		{
			foreach(KTLineGuide *line, d->lines)
			{
				line->setFlag( QGraphicsItem::ItemIsMovable, false );
				line->setEnabledSyncCursor(true);
			}
		}
		d->tool->aboutToChangeTool();
	}
	
	d->tool = tool;
	if(tool->toolType() == KTToolPlugin::Selection )
	{
		foreach(KTLineGuide *line, d->lines)
		{
			line->setFlag( QGraphicsItem::ItemIsMovable, true );
			line->setEnabledSyncCursor(false);
		}
	}
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
	
	d->isDrawing = false;
	
	if( event->buttons() == Qt::LeftButton &&  (event->modifiers () == (Qt::ShiftModifier | Qt::ControlModifier)))
	{
	}
	else if (d->tool )
	{
		if( d->tool->toolType() == KTToolPlugin::Brush && event->isAccepted() ) return;
		if(currentFrame())
		{
			if ( event->buttons() == Qt::LeftButton && !currentFrame()->isLocked() )
			{
				d->tool->begin();
				d->isDrawing = true;
				d->tool->press(d->inputInformation, d->brushManager, this );
			}
		}
	}
}

void KTGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseMoveEvent(event);
	mouseMoved(event);
}

void KTGraphicsScene::mouseMoved(QGraphicsSceneMouseEvent *event)
{
	d->inputInformation->updateFromMouseEvent( event );
	
	if (d->tool && d->isDrawing )
	{
		d->tool->move(d->inputInformation, d->brushManager,  this );
	}
}

void KTGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseReleaseEvent(event);
	mouseReleased(event);
}

void KTGraphicsScene::mouseReleased(QGraphicsSceneMouseEvent *event)
{
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
	if (event->mimeData()->hasFormat("ktoon-ruler"))
		event->acceptProposedAction();
	
	KTLineGuide *line = 0;
	if(event->mimeData()->data("ktoon-ruler") == "verticalLine")
	{
		line  = new KTLineGuide(Qt::Vertical, this);
		line->setPos(event->scenePos());
	}
	else
	{
		line = new KTLineGuide(Qt::Horizontal, this);
		line->setPos(event->scenePos());
	}
	if(line)
	{
		d->lines << line;
	}
}


void  KTGraphicsScene::dragLeaveEvent ( QGraphicsSceneDragDropEvent * event )
{
	removeItem(d->lines.last());
	delete d->lines.takeLast();
}

void KTGraphicsScene::dragMoveEvent ( QGraphicsSceneDragDropEvent * event )
{
	if(!d->lines.isEmpty())
	{
		d->lines.last()->setPos(event->scenePos());
	}
}

void KTGraphicsScene::dropEvent ( QGraphicsSceneDragDropEvent * event )
{
	Q_UNUSED(event);
	if(d->tool)
	{
		if(d->tool->toolType() == KTToolPlugin::Selection )
		{
			d->lines.last()->setEnabledSyncCursor(false);
			d->lines.last()->setFlag( QGraphicsItem::ItemIsMovable, true );
		}
	}
}


bool KTGraphicsScene::event(QEvent *e)
{
	return QGraphicsScene::event(e);
}


void KTGraphicsScene::itemResponse(KTItemResponse *event)
{
	D_FUNCINFOX("scene");
	if ( d->tool )
	{
		//d->tool->init(this); //FIXME:d->tool->init(this); in itemResponse ???
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

void KTGraphicsScene::aboutToMousePress()
{
	QHash<QGraphicsItem *, double>::iterator it = d->onionSkin.opacityMap.begin();

	while(it != d->onionSkin.opacityMap.end() )
	{
		if( it.value() != 1.0 )
		{
			it.key()->setAcceptedMouseButtons(Qt::NoButton);
			it.key()->setFlag(QGraphicsItem::ItemIsSelectable, false);
		}
		else
		{
			it.key()->setAcceptedMouseButtons( Qt::LeftButton | Qt::RightButton | Qt::MidButton | Qt::XButton1 | Qt::XButton2 );
		}

		++it;
	}
}


