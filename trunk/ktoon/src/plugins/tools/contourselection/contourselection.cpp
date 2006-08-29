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

#include <daction.h>
#include <ddebug.h>
#include <dglobal.h>
#include <dalgorithm.h>

#include "ktgraphicalgorithm.h"
#include "ktscene.h"

// #include "nodemanager.h"
#include <QDebug>
#include <QTimer>

#include "nodegroup.h"

ContourSelection::ContourSelection()
{
	setupActions();
}


ContourSelection::~ContourSelection()
{
	
}

void ContourSelection::init(QGraphicsView *view)
{
// 	m_view = view;
	
	foreach(QGraphicsItem *item, view->scene()->items() )
	{
		item->setFlags (QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable );
	}
}

QStringList ContourSelection::keys() const
{
	return QStringList() << tr("ContourSelection") ;
}

void ContourSelection::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	Q_UNUSED(input);
	Q_UNUSED(brushManager);
// 	Q_UNUSED(scene);
	Q_UNUSED(view);
	view->setDragMode (QGraphicsView::RubberBandDrag);
	
	m_project = scene->project();
}

void ContourSelection::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	Q_UNUSED(input);
	Q_UNUSED(brushManager);
	Q_UNUSED(scene);
	Q_UNUSED(view);
	static int s = 0;
	s++;
	if(scene->selectedItems().count() > 0)
	{
		QTimer::singleShot ( 0, this, SLOT(syncNodes()));;
	}
	
	
}

void ContourSelection::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
// 	D_FUNCINFO;
	Q_UNUSED(input);
	Q_UNUSED(brushManager);
	Q_UNUSED(view);
	
	if(scene->selectedItems().count() > 0)
	{
		QList<QGraphicsItem *> selecteds = scene->selectedItems();
		
#if 1
		QList<NodeGroup *>::iterator it = m_nodes.begin();
		QList<NodeGroup *>::iterator itEnd = m_nodes.end();
		while(it != itEnd)
		{
			int parentIndex = scene->selectedItems().indexOf((*it)->parentItem() );
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
#else
// 		qDeleteAll(m_nodes);
// 		m_nodes.clear();
#endif
		foreach(QGraphicsItem *item, selecteds)
		{
			if(item  )
			{
				if(!qgraphicsitem_cast<ControlNode *>(item))
				{
					NodeGroup *node;
					
					if( !qgraphicsitem_cast<KTPathItem*>(item) )
					{
						QString conv = "<convert type=\"2\" />"; // to path type
						KTItemEvent *event = new KTItemEvent(KTProjectEvent::Convert, scene->index(), scene->currentLayerIndex(), scene->currentFrameIndex(), scene->currentFrame()->graphics().indexOf(item), conv);
						addProjectEvent(event);
					}
					else
					{
						node = new NodeGroup(item, scene);
						m_nodes << node;
					}
				}
			}
		}
	}
	else
	{
		qDeleteAll(m_nodes);
		m_nodes.clear();
	}
}

void ContourSelection::itemEvent(const KTItemEvent *event)
{
	D_FUNCINFO;
	switch(event->action())
	{
		
		case KTProjectEvent::Convert:
		{
			if(m_project)
			{
				KTScene *scene = m_project->scene(event->sceneIndex());
				if ( scene )
				{
					
					KTLayer *layer = scene->layer( event->layerIndex() );
					
					if ( layer )
					{
						
						KTFrame *frame = layer->frame( event->frameIndex() );
						
						if ( frame )
						{
							QGraphicsItem *item = frame->item(event->itemIndex());
							if ( item )
							{
								NodeGroup *node = new NodeGroup(item, scene);
								m_nodes << node;
							}
						}
					}
				}
			}
			else
			{
				dFatal() << "Project not exist";
			}
		}
// 		{
// 			QTimer::singleShot(0, this, SLOT(syncNodes()));
// 		}
		break;
		default: break;
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
// 	if ( ! m_configurator )
// 	{
// 		m_configurator = new ExactnessConfigurator;
// 	}
	return 0;
}
		
bool ContourSelection::isComplete() const
{
	return true;
}

void ContourSelection::aboutToChangeTool()
{
// 	m_view->setDragMode (QGraphicsView::NoDrag);
	qDeleteAll(m_nodes);
	m_nodes.clear();
}

QString ContourSelection::toolToXml() const
{
	QDomDocument doc;
// 	doc.appendChild(m_item->toXml( doc ));
	return doc.toString();
}


void ContourSelection::syncNodes()
{
	
// 	//FIXME: tratar de optimizar esto
	foreach(NodeGroup* node, m_nodes)
	{
		if(node)
		{
			node->syncNodesFromParent();
			
			if(node->parentItem())
			{
// 				node->parentItem()->setEditNodesed(true);
			}
		}
	}
}

void ContourSelection::slotAddProjectEvent(KTProjectEvent *e)
{
	addProjectEvent(e);
}


Q_EXPORT_PLUGIN2(kt_editNodes, ContourSelection )
