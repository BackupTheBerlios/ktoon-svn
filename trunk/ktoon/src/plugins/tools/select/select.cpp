/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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

#include "select.h"

#include <QPointF>
#include <QKeySequence>
#include <QList>

#include <dgui/daction.h>
#include <dcore/ddebug.h>
#include <dcore/dglobal.h>
#include <dcore/dalgorithm.h>

#include "ktgraphicalgorithm.h"
#include "ktscene.h"
#include "ktlayer.h"

#include <QGraphicsView>
#include "ktinputdeviceinformation.h"
#include "ktgraphicsscene.h"

#include "nodemanager.h"
#include "ktserializer.h"

#include "ktrequestbuilder.h"
#include "ktprojectresponse.h"

#include <QTimer>

struct Select::Private
{
	QMap<QString, DAction *> actions;
	QList<NodeManager*> nodeManagers;
	QGraphicsView *view;
	KTProject *project;
	NodeManager* changedManager;
};

Select::Select(): d(new Private)
{
	d->changedManager = 0;
	setupActions();
}


Select::~Select()
{
	delete d;
}

void Select::init(KTGraphicsScene *scene)
{
	foreach(QGraphicsView * view, scene->views())
	{
		view->setDragMode (QGraphicsView::RubberBandDrag);
		foreach(QGraphicsItem *item, view->scene()->items() )
		{
			if(!qgraphicsitem_cast<Node *>(item))
			{
				item->setFlags (QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable );
			}
		}
	}
}

QStringList Select::keys() const
{
	return QStringList() << tr("Select") ;
}

void Select::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
	D_FUNCINFOX("tools");
	Q_UNUSED(brushManager);
	Q_UNUSED(scene);
	
	
	if(d->changedManager)
	{
		d->changedManager = 0;
	}
	
	foreach(QGraphicsView * view, scene->views())
	{
		view->setDragMode (QGraphicsView::RubberBandDrag);
	}
	
	if ( input->keyModifiers() != Qt::ControlModifier )
	{
		foreach(NodeManager *nodeManager, d->nodeManagers)
		{
			if(scene->mouseGrabberItem() == nodeManager->parentItem())
			{
				nodeManager->toggleAction();
				d->changedManager = nodeManager;
				break;
			}
			else if(!nodeManager->isPress())
			{
				nodeManager->parentItem()->setSelected(false);
				d->nodeManagers.removeAll(nodeManager);
				delete nodeManager;
			}
		}
	}
	
	QList<QGraphicsItem *> selecteds = scene->selectedItems();
	selecteds << scene->mouseGrabberItem();
	
	foreach(QGraphicsItem *item, selecteds)
	{
		if(item && dynamic_cast<KTAbstractSerializable* > (item) )
		{
			bool finded = false;
			foreach(NodeManager *nodeManager, d->nodeManagers)
			{
				if(nodeManager->parentItem() == nodeManager->parentItem())
				{
					finded = true;
					break;
				}
			}
			
			if(!finded )
			{
				NodeManager *manager = new NodeManager(item, scene);
				d->nodeManagers << manager;
			}
		}
	}
	
	d->project = scene->scene()->project();
}

void Select::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
// 	D_FUNCINFOX("tools");
	Q_UNUSED(input);
	Q_UNUSED(brushManager);
	Q_UNUSED(scene);
	if(d->changedManager)
	{
		d->changedManager->toggleAction();
		d->changedManager = 0;
	}
	
	if(input->buttons() == Qt::LeftButton && scene->selectedItems().count() > 0)
	{
		QTimer::singleShot ( 0, this, SLOT(syncNodes()));;
	}
}

void Select::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
	D_FUNCINFOX("tools") << scene->selectedItems().count();
	
	Q_UNUSED(input);
	Q_UNUSED(brushManager);
	
	if(scene->selectedItems().count() > 0)
	{
		QList<QGraphicsItem *> selecteds = scene->selectedItems();
		QList<NodeManager *>::iterator it = d->nodeManagers.begin();
		QList<NodeManager *>::iterator itEnd = d->nodeManagers.end();
		while(it != itEnd)
		{
			int parentIndex = scene->selectedItems().indexOf((*it)->parentItem() );
			
			if(parentIndex != -1 )
			{
				selecteds.removeAt(parentIndex);
			}
			else
			{
				delete d->nodeManagers.takeAt(d->nodeManagers.indexOf((*it)));
			}
			++it;
		}
		
		foreach(QGraphicsItem *item, selecteds)
		{
			if(item && dynamic_cast<KTAbstractSerializable* > (item) )
			{
				NodeManager *manager = new NodeManager(item, scene);
				d->nodeManagers << manager;
			}
		}
		
		foreach(NodeManager *manager, d->nodeManagers)
		{
			if(manager->isModified())
			{
				QDomDocument doc;
				doc.appendChild(KTSerializer::properties( manager->parentItem(), doc ));
				
				int position  = scene->currentFrame()->indexOf(manager->parentItem());
				dDebug("selection") << "position = "<<  position;
				if(position != -1)
				{
					// Restore matrix
					manager->restoreItem();
					
					KTProjectRequest event = KTRequestBuilder::createItemRequest( scene->currentSceneIndex(), scene->currentLayerIndex(), scene->currentFrameIndex(), position, KTProjectRequest::Transform, doc.toString() );
					emit requested(&event);
				}
				else
				{
					dDebug("selection") << "position is " << position; 
				}
			}
		}
	}
	else
	{
		qDeleteAll(d->nodeManagers);
		d->nodeManagers.clear();
	}
}

void Select::setupActions()
{
	DAction *select = new DAction( QIcon(), tr("Select"), this);
// 	pencil->setShortcut( QKeySequence(tr("")) );
	
// 	QPixmap pix(THEME_DIR+"/cursors/pencil.png");
// 	select->setCursor( QCursor(pix, 0, pix.height()) );
	
	d->actions.insert( tr("Select"), select );
}

QMap<QString, DAction *> Select::actions() const
{
	return d->actions;
}

int Select::toolType() const
{
	return KTToolInterface::Selection;
}

QWidget *Select::configurator() 
{
	return 0;
}

void Select::aboutToChangeScene(KTGraphicsScene *scene)
{
	d->changedManager = 0;
	init(scene);
}

void Select::aboutToChangeTool()
{
	qDeleteAll(d->nodeManagers);
	d->nodeManagers.clear();
}


void Select::itemResponse(const KTItemResponse *event)
{
	D_FUNCINFO;
	
	QGraphicsItem *item = 0;
	KTScene *scene = 0;
	KTLayer *layer = 0;
	KTFrame *frame = 0;
	if(d->project)
	{
		scene = d->project->scene(event->sceneIndex());
		if ( scene )
		{
			layer = scene->layer( event->layerIndex() );
			if ( layer )
			{
				
				frame = layer->frame( event->frameIndex() );
				
				if ( frame )
				{
					item = frame->item(event->itemIndex());
				}
			}
		}
	}
	else
	{
		dFatal() << "Project not exist";
	}
	
	
	switch(event->action())
	{
		case KTProjectRequest::Transform:
		{
			if ( item )
			{
				foreach(NodeManager* node, d->nodeManagers)
				{
					node->show();
					node->syncNodesFromParent();
					node->beginToEdit();
					node->parentItem()->setSelected(true);
					break;
				}
			}
		}
		break;
		default: break;
	}
}

void Select::syncNodes()
{
	foreach(NodeManager* node, d->nodeManagers)
	{
		if(node)
		{
			node->show();
			if(node->parentItem())
			{
				node->parentItem()->setSelected(true);
				node->syncNodesFromParent();
			}
		}
	}
}

Q_EXPORT_PLUGIN2(kt_select, Select );

