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
#include "contourselection.h"

#include <QPointF>
#include <QKeySequence>
#include <QList>

#include <dgui/daction.h>
#include <dcore/ddebug.h>
#include <dcore/dglobal.h>
#include <dcore/dalgorithm.h>

#include "ktgraphicalgorithm.h"
#include "ktscene.h"
#include "ktserializer.h"

#include <QGraphicsView>
#include "ktinputdeviceinformation.h"
#include "ktgraphicsscene.h"
#include "ktprojectrequest.h"
#include "ktprojectresponse.h"

// #include "nodemanager.h"
#include <QDebug>
#include <QTimer>

#include <dgui/dnodegroup.h>

#include "ktrequestbuilder.h"


#include "ktproxyitem.h"


ContourSelection::ContourSelection()
{
	
	setupActions();
	
}


ContourSelection::~ContourSelection()
{
	
}

void ContourSelection::init(KTGraphicsScene *scene)
{
// 	m_view = view;
	foreach(QGraphicsView * view, scene->views())
	{
		view->setDragMode (QGraphicsView::RubberBandDrag);
		
		foreach(QGraphicsItem *item, view->scene()->items() )
		{
			if(!qgraphicsitem_cast<DControlNode *>(item))
			{
				item->setFlags (QGraphicsItem::ItemIsSelectable /*| QGraphicsItem::ItemIsMovable*/ );
			}
		}
	}
}

QStringList ContourSelection::keys() const
{
	return QStringList() << tr("ContourSelection") ;
}

void ContourSelection::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
	Q_UNUSED(input);
	Q_UNUSED(brushManager);
	
	m_scene = scene;
}

void ContourSelection::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
	Q_UNUSED(input);
	Q_UNUSED(brushManager);
	Q_UNUSED(scene);
}

void ContourSelection::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
	Q_UNUSED(input);
	Q_UNUSED(brushManager);
	
	if(scene->selectedItems().count() > 0)
	{
		QList<QGraphicsItem *> selecteds = scene->selectedItems();
		QList<DNodeGroup *>::iterator it = m_nodeGroups.begin();
		QList<DNodeGroup *>::iterator itEnd = m_nodeGroups.end();
		while(it != itEnd)
		{
			int parentIndex = scene->selectedItems().indexOf((*it)->parentItem() );
			if(parentIndex != -1 )
			{
				selecteds.removeAt(parentIndex);
			}
			else
			{
				delete m_nodeGroups.takeAt(m_nodeGroups.indexOf((*it)));
			}
			++it;
		}
		foreach(QGraphicsItem *item, selecteds)
		{
			if(item  )
			{
				if(!qgraphicsitem_cast<DControlNode *>(item))
				{
					if( !qgraphicsitem_cast<KTPathItem*>(item) )
					{
						KTProjectRequest event = KTRequestBuilder::createItemRequest( scene->currentSceneIndex(), scene->currentLayerIndex(), scene->currentFrameIndex(), scene->currentFrame()->indexOf(item), KTProjectRequest::Convert, 2);
						emit requested(&event);
					}
					else
					{
						m_nodeGroups << new DNodeGroup(item, scene);
					}
				}
			}
		}
		
		foreach(DNodeGroup *group, m_nodeGroups)
		{
			if(!group->changedNodes().isEmpty() )
			{
				int position  = scene->currentFrame()->indexOf(group->parentItem());
				if(position != -1 && qgraphicsitem_cast<QGraphicsPathItem *>(group->parentItem()))
				{
					QDomDocument doc;
					doc.appendChild(qgraphicsitem_cast<KTPathItem *>(group->parentItem())->toXml(doc));
					
					KTProjectRequest event = KTRequestBuilder::createItemRequest( scene->currentSceneIndex(), scene->currentLayerIndex(), scene->currentFrameIndex(), position, KTProjectRequest::EditNodes, doc.toString() );
					group->restoreItem();
					emit requested(&event);
					
					
				}
				else
				{
					dDebug("selection") << "position is " << position; 
				}
				group->clearChangesNodes();
			}
		}
	}
	else
	{
		qDeleteAll(m_nodeGroups);
		m_nodeGroups.clear();
	}
}

void ContourSelection::itemResponse(const KTItemResponse *response)
{
	D_FUNCINFO;
	KTProject *project = m_scene->scene()->project();
	QGraphicsItem *item = 0;
	KTScene *scene = 0;
	KTLayer *layer = 0;
	KTFrame *frame = 0;
	if(project)
	{
		scene = project->scene(response->sceneIndex());
		if ( scene )
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
		
		case KTProjectRequest::Convert:
		{
			if ( item && scene)
			{
				DNodeGroup *node = new DNodeGroup(item, m_scene);
				m_nodeGroups << node;
			}
		}
		break;
		case KTProjectRequest::EditNodes:
		{
			
			if ( item )
			{
				foreach(DNodeGroup* group, m_nodeGroups)
				{
					if(qgraphicsitem_cast<QGraphicsPathItem *>(group->parentItem()) == item)
					{
						group->show();
						group->syncNodesFromParent();
						group->saveParentProperties();
						break;
					}
				}
			}
		}
		break;
		default: break;
	}
}

void ContourSelection::keyPressEvent(QKeyEvent *event)
{
	if ( event->key() != Qt::Key_Delete )
	{
		event->ignore();
		return;
	}
	
	bool deleted = false;
	
	foreach(DNodeGroup *nodegroup, m_nodeGroups )
	{
		deleted = deleted || (nodegroup->removeSelectedNodes() > 0);
	}
	
	if ( deleted )
	{
		event->accept();
	}
}

void ContourSelection::setupActions()
{
	DAction *select = new DAction( QIcon(), tr("Contour selection"), this);
// 	pencil->setShortcut( QKeySequence(tr("")) );
	
// 	QPixmap pix(THEME_DIR+"/cursors/pencil.png");
// 	select->setCursor( QCursor(pix, 0, pix.height()) );
		
	m_actions.insert( tr("ContourSelection"), select );
}

QMap<QString, DAction *> ContourSelection::actions() const
{
	return m_actions;
}

int ContourSelection::toolType() const
{
	return KTToolInterface::Selection;
}

QWidget *ContourSelection::configurator() 
{
	return 0;
}


void ContourSelection::aboutToChangeScene(KTGraphicsScene *)
{
	
}

void ContourSelection::aboutToChangeTool()
{
// 	m_view->setDragMode (QGraphicsView::NoDrag);
	qDeleteAll(m_nodeGroups);
	m_nodeGroups.clear();
}


void ContourSelection::syncNodes()
{
	
// 	//FIXME: tratar de optimizar esto
	foreach(DNodeGroup* node, m_nodeGroups)
	{
		if(node)
		{
			if(qgraphicsitem_cast<QGraphicsPathItem *>(node->parentItem()))
			{
				node->show();
				node->syncNodesFromParent();
				node->saveParentProperties();
				node->parentItem()->setSelected(true);
			}
			else
			{
				delete m_nodeGroups.takeAt(m_nodeGroups.indexOf((node)));
			}
		}
	}
}



Q_EXPORT_PLUGIN2(kt_editNodes, ContourSelection )
