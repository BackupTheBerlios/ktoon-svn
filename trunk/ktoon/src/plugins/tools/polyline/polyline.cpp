/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
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
 
#include "polyline.h"

#include <QGraphicsPathItem>
#include <QGraphicsLineItem>

#include <QPainterPath>
#include <QMatrix>

#include "ktscene.h"
#include "ktrequestbuilder.h"

#include <dcore/ddebug.h>
#include <dcore/dglobal.h>
#include <dgui/daction.h>

#include <QGraphicsView>
#include "ktinputdeviceinformation.h"
#include "ktgraphicsscene.h"
#include "ktprojectrequest.h"
#include "ktbrushmanager.h"

#include "ktprojectresponse.h"

struct PolyLine::Private
{
	bool begin, ok;
	QPointF center;
	QPointF rigth;
	QPointF mirror;
	
	DNodeGroup *nodegroup;
	QPainterPath path;
	
	QMap<QString, DAction *> actions;
	
	KTPathItem *item;
	KTGraphicsScene *scene;
	
	QGraphicsLineItem *line1, *line2;
	
};


PolyLine::PolyLine(): d(new Private)
{
	d->begin = false;
	d->ok = false;
	
	d->nodegroup = 0;
	d->item = 0;
	
	d->line1 = new QGraphicsLineItem(0,0,0,0);
	d->line1->setPen ( QPen(Qt::red) );
	d->line2 = new QGraphicsLineItem(0,0,0,0);
	d->line2->setPen ( QPen(Qt::green) );
	
	setupActions();
}

PolyLine::~PolyLine()
{
}

void PolyLine::init(KTGraphicsScene *scene)
{
	foreach(QGraphicsView *view,  scene->views() )
	{
		view->setDragMode ( QGraphicsView::NoDrag );
		
		Q_CHECK_PTR(view->scene());
		if ( QGraphicsScene *sscene = qobject_cast<QGraphicsScene *>(view->scene()) )
		{
			foreach(QGraphicsItem *item, scene->items() )
			{
				item->setFlag(QGraphicsItem::ItemIsSelectable, false);
				item->setFlag(QGraphicsItem::ItemIsMovable, false);
			}
			sscene->addItem( d->line1 );
			sscene->addItem( d->line2 );
		}
	}
}

QStringList PolyLine::keys() const
{
	return QStringList() << tr("PolyLine") ;
}

void PolyLine::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
	D_FUNCINFOX("tools");
	scene->clearSelection();
	
	
	if(!d->item)
	{
		d->path = QPainterPath();
		d->path.moveTo(input->pos());
		d->item = new KTPathItem();
		
		scene->addItem( d->item );
		d->begin = true;
	}
	else
	{
		if(!scene->items().contains(d->item))
		{
			scene->addItem( d->item );
		}
		
		d->begin = false;
		d->path = d->item->path();
		d->path.cubicTo(d->rigth, d->mirror, input->pos());
	}
	
	d->center = input->pos();
	d->item->setPen( brushManager->pen() );
	
	if(!scene->items().contains(d->line1))
	{
		scene->addItem( d->line1 );
	}
	if(!scene->items().contains(d->line2))
	{
		scene->addItem( d->line2 );
	}
	
}

void PolyLine::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
// 	D_FUNCINFOX("tools");
	Q_UNUSED(brushManager);
	Q_UNUSED(scene);
	
	foreach(QGraphicsView *view,  scene->views() )
	{
		view->setDragMode (QGraphicsView::NoDrag);
	}
	
	d->mirror = d->center - ( input->pos() - d->center);
	if(d->begin)
	{
		d->rigth = input->pos();
	}
	else
	{
		for(int i = d->path.elementCount()-1; i >= 0; i--)
		{
			if(d->path.elementAt(i).type == QPainterPath::CurveToElement)
			{
				d->rigth = input->pos();
				if(d->path.elementAt(i+1).type == QPainterPath::CurveToDataElement)
				{
					d->path.setElementPositionAt(i+1, d->mirror.x(), d->mirror.y() );
				}
				break;
			}
		}
	}
	
	Q_CHECK_PTR(d->item);
	if(d->item)
	{
		d->item->setPath(d->path);
	}
	
	d->line1->setLine(QLineF(d->mirror, d->center));
	d->line2->setLine(QLineF(d->rigth, d->center));
}

void PolyLine::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
	Q_UNUSED(input);
	Q_UNUSED(brushManager);
	
	d->scene = scene;
	
	if(!d->nodegroup)
	{
		d->nodegroup = new DNodeGroup(d->item, scene);
		connect(d->nodegroup, SIGNAL(nodeClicked()), this, SLOT(nodeChanged()));
	}
	else
	{
		d->nodegroup->createNodes(d->item);
	}
	
	QDomDocument doc;
	SHOW_VAR(d->nodegroup->isSelected());
	if(d->begin)
	{
		doc.appendChild(d->item->toXml( doc ));
		KTProjectRequest request = KTRequestBuilder::createItemRequest( d->scene->currentSceneIndex(), d->scene->currentLayerIndex(), d->scene->currentFrameIndex(), d->scene->currentFrame()->graphics().count(), KTProjectRequest::Add, doc.toString() );
		
		emit requested(&request);
	}
	else if(!d->nodegroup->isSelected())
	{
		int position  = scene->currentFrame()->indexOf(d->nodegroup->parentItem());
		if(position != -1 && qgraphicsitem_cast<QGraphicsPathItem *>(d->nodegroup->parentItem()))
		{
			doc.appendChild(qgraphicsitem_cast<KTPathItem *>(d->nodegroup->parentItem())->toXml(doc));
			
			KTProjectRequest event = KTRequestBuilder::createItemRequest( scene->currentSceneIndex(), scene->currentLayerIndex(), scene->currentFrameIndex(), position, KTProjectRequest::EditNodes, doc.toString() );
			d->nodegroup->restoreItem();
			emit requested(&event);
		}
		else
		{
			dDebug("selection") << "position is " << position; 
		}
		d->nodegroup->clearChangesNodes();
	}
}


void PolyLine::itemResponse(const KTItemResponse *response)
{
	D_FUNCINFOX("tools");
	KTProject *project = d->scene->scene()->project();
	QGraphicsItem *item = 0;
	KTScene *scene = 0;
	KTLayer *layer = 0;
	KTFrame *frame = 0;
	if(project)
	{
		scene = project->scene(response->sceneIndex());
		if( scene )
		{
			layer = scene->layer( response->layerIndex() );
			
			if ( layer )
			{
				
				frame = layer->frame( response->frameIndex() );
				
				if ( frame )
				{
					item = frame->item(response->itemIndex());
				}
			}
		}
	}
	else
	{
		dFatal() << "Project not exist";
	}
	
	switch(response->action())
	{
		case KTProjectRequest::Add:
		{
			if(!d->begin)
			{
				if(KTPathItem * path = qgraphicsitem_cast<KTPathItem *>(item))
				{
					d->item = path;
				}
			}
		}
		break;
		case KTProjectRequest::Remove:
		{
			d->path = QPainterPath();
			delete d->item;
			d->item = 0;
			delete d->nodegroup;
			d->nodegroup = 0;
		}
		break;
		case KTProjectRequest::EditNodes:
		{
			if(d->nodegroup && item)
			{
				if(qgraphicsitem_cast<QGraphicsPathItem *>(d->nodegroup->parentItem()) != item)
				{
					delete d->item;
					d->item = qgraphicsitem_cast<KTPathItem *>(item);
					d->nodegroup->setParentItem(item);
				}
				
				d->nodegroup->createNodes(d->item);
				d->nodegroup->saveParentProperties();
				d->nodegroup->expandAllNodes();
			}
		}
		break;
		default: break;
	}
}


void PolyLine::keyPressEvent(QKeyEvent *event)
{
	if(event->key() != Qt::Key_Escape )
	{
		event->ignore();
		return;
	}
	else
	{
		endItem();
		event->accept();
	}
}

void PolyLine::endItem()
{
	if(d->item)
	{
		d->path = QPainterPath();
// 		delete d->item;
		d->item = 0;
		delete d->nodegroup;
		d->nodegroup = 0;
	}
}

void PolyLine::nodeChanged()
{
	D_FUNCINFO;
	if(d->nodegroup)
	{
		SHOW_VAR(!d->nodegroup->changedNodes().isEmpty());
		if(!d->nodegroup->changedNodes().isEmpty())
		{
			d->ok = true;
			QDomDocument doc;
			int position  = d->scene->currentFrame()->indexOf(d->nodegroup->parentItem());
			if(position != -1 && qgraphicsitem_cast<QGraphicsPathItem *>(d->nodegroup->parentItem()))
			{
				doc.appendChild(qgraphicsitem_cast<KTPathItem *>(d->nodegroup->parentItem())->toXml(doc));
				
				KTProjectRequest event = KTRequestBuilder::createItemRequest( d->scene->currentSceneIndex(), d->scene->currentLayerIndex(), d->scene->currentFrameIndex(), position, KTProjectRequest::EditNodes, doc.toString() );
				d->nodegroup->restoreItem();
				emit requested(&event);
			}
		}
	}
}

void PolyLine::setupActions()
{
	DAction *pencil = new DAction( QIcon(), tr("Poly line"), this);
	pencil->setShortcut( QKeySequence(tr("")) );
	
	d->actions.insert( tr("PolyLine"), pencil );
}

QMap<QString, DAction *> PolyLine::actions() const
{
	return d->actions;
}

int PolyLine::toolType() const
{
	return KTToolInterface::Brush;
}

QWidget *PolyLine::configurator() 
{
	return 0;
}


void PolyLine::aboutToChangeScene(KTGraphicsScene *)
{
	d->path = QPainterPath();
	d->item = 0;
	delete d->nodegroup;
	d->nodegroup = 0;
}

void PolyLine::aboutToChangeTool()
{
	endItem();
}


Q_EXPORT_PLUGIN2(kt_polyline, PolyLine );




