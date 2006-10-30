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

#include <daction.h>
#include <ddebug.h>
#include <dglobal.h>
#include <dalgorithm.h>

#include "ktgraphicalgorithm.h"
#include "ktscene.h"

#include "nodemanager.h"
#include "ktserializer.h"

#include <QDebug> // ddebug! lazy bitch
#include <QTimer>

Select::Select()
{
	setupActions();
}


Select::~Select()
{
	
}

void Select::init(QGraphicsView *view)
{
// 	m_view = view;
	
	foreach(QGraphicsItem *item, view->scene()->items() )
	{
		if(!qgraphicsitem_cast<Node *>(item))
		{
			item->setFlags (QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable );
		}
	}
}

QStringList Select::keys() const
{
	return QStringList() << tr("Select") ;
}

void Select::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	D_FUNCINFO;
	Q_UNUSED(input);
	Q_UNUSED(brushManager);
	Q_UNUSED(scene);
	Q_UNUSED(view);
	
	view->setDragMode (QGraphicsView::RubberBandDrag);
	
	
	
	if ( input->keyModifiers() != Qt::ControlModifier )
	{
		foreach(NodeManager *nodeManager, m_nodes)
		{
			if(!nodeManager->isPress())
			{
				nodeManager->parentItem()->setSelected(false);
				m_nodes.removeAll(nodeManager);
				delete nodeManager;
			}
		}
	}
	
	m_project = scene->project();
}

void Select::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	Q_UNUSED(input);
	Q_UNUSED(brushManager);
	Q_UNUSED(scene);
	Q_UNUSED(view);
	
	static int s = 0;
	s++;
	if(input->buttons() == Qt::LeftButton && scene->selectedItems().count() > 0)
	{
		QTimer::singleShot ( 0, this, SLOT(syncNodes()));;
	}
}

void Select::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	Q_UNUSED(input);
	Q_UNUSED(brushManager);
	Q_UNUSED(view);
	
	if(scene->selectedItems().count() > 0)
	{
		QList<QGraphicsItem *> selecteds = scene->selectedItems();
		QList<NodeManager *>::iterator it = m_nodes.begin();
		QList<NodeManager *>::iterator itEnd = m_nodes.end();
		while(it != itEnd)
		{
			int parentIndex = scene->selectedItems().indexOf((*it)->parentItem() );
			(*it)->beginToEdit();
			if(parentIndex != -1 )
			{
				selecteds.removeAt(parentIndex);
			}
			else
			{
				delete m_nodes.takeAt(m_nodes.indexOf((*it)));
			}
			++it;
			
		}
		foreach(QGraphicsItem *item, selecteds)
		{
			if(item && dynamic_cast<KTAbstractSerializable* > (item) )
			{
				NodeManager *manager = new NodeManager(item, scene);
				m_nodes << manager;
			}
		}
		
		foreach(NodeManager *manager, m_nodes)
		{
			if(manager->isModified())
			{
				
				QDomDocument doc;
				doc.appendChild(KTSerializer::properties( manager->parentItem(), doc ));
				
				int position  = scene->currentFrame()->indexOf(manager->parentItem());
				if(position != -1)
				{
					KTItemRequest *event = new KTItemRequest(KTProjectRequest::Transform, scene->index(), scene->currentLayerIndex(), scene->currentFrameIndex(), position, doc.toString() );
					addProjectEvent(event);
					
					// Restore matrix
					manager->restoreItem();
				}
				else
				{
					dDebug("selection") << "position is " << position; 
				}
				manager->setModify(false);
			}
		}
	}
	else
	{
		qDeleteAll(m_nodes);
		m_nodes.clear();
	}
}

void Select::setupActions()
{
	DAction *select = new DAction( QIcon(), tr("Select"), this);
// 	pencil->setShortcut( QKeySequence(tr("")) );
	
// 	QPixmap pix(THEME_DIR+"/cursors/pencil.png");
// 	select->setCursor( QCursor(pix, 0, pix.height()) );
	
	m_actions.insert( tr("Select"), select );
}

QMap<QString, DAction *> Select::actions() const
{
	return m_actions;
}

int Select::toolType() const
{
	return KTToolInterface::Selection;
}

QWidget *Select::configurator() 
{
	return 0;
}
		
bool Select::isComplete() const
{
	return true;
}

void Select::aboutToChangeTool()
{
// 	m_view->setDragMode (QGraphicsView::NoDrag);
	qDeleteAll(m_nodes);
	m_nodes.clear();
}

void Select::itemRequest(const KTItemRequest *event)
{
	D_FUNCINFO;
	
	QGraphicsItem *item = 0;
	KTScene *scene = 0;
	KTLayer *layer = 0;
	KTFrame *frame = 0;
	if(m_project)
	{
		scene = m_project->scene(event->sceneIndex());
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
				foreach(NodeManager* node, m_nodes)
				{
					node->show();
					node->syncNodesFromParent();
					node->beginToEdit();
					node->parentItem()->setSelected(true);
					break;
				}
			}
// 			QTimer::singleShot(0, this, SLOT(syncNodes()));
		}
		break;
		default: break;
	}
}

void Select::syncNodes()
{
	foreach(NodeManager* node, m_nodes)
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

