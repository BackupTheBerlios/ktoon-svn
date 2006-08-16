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
	Q_UNUSED(scene);
	Q_UNUSED(view);
	view->setDragMode (QGraphicsView::RubberBandDrag);
}

void ContourSelection::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
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
	/*QTimer::singleShot ( 0, this, SLOT(*//*) );*/ ;
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
		qDeleteAll(m_nodes);
		m_nodes.clear();
		foreach(QGraphicsItem *item, selecteds)
		{
			if(item)
			{
// 				NodeManager *manager = new NodeManager(item, scene);
// 				m_nodes << manager;
				QGraphicsPathItem *tmp = dynamic_cast<QGraphicsPathItem *>(item);
				
				
				if(!tmp)
				{
					continue;
				}
				QPainterPath path = item->matrix().map( tmp->path());
				QMatrix m;
// 				QPointF t = item->sceneBoundingRect().topLeft();
				QPointF t = item->pos();
				m.translate( t.x(), t.y() );
// 				QMatrix m;
// 				m.translate(item->scenePos().x(), item->scenePos().x());
				path = m.map( path );
				
				int index = 0;
				while(index < path.elementCount())
				{
					QPainterPath::Element e = path.elementAt(index);
					if(e.type == QPainterPath::CurveToDataElement)
					{
						if(index - 2 < 0) continue;
						
						if( path.elementAt(index-2).type == QPainterPath::CurveToElement )
						{
							ControlNode *node = new ControlNode(index, path.elementAt(index), item, scene);
							QPainterPath::Element e1 = path.elementAt(index-1);
							QPainterPath::Element e2 = path.elementAt(index+1);
							node->setLeft(new ControlNode(index-1, e1, item, scene));
							if(index+1 < path.elementCount() && e2.type == QPainterPath::CurveToElement)
							{
								node->setRight(new ControlNode(index+1, e2, item, scene));
								m_nodes << node->right();
								index++;
							}
								
							m_nodes << node;
							m_nodes << node->left();
						}
					}
					else if( (e.type == QPainterPath::LineToElement || e.type == QPainterPath::MoveToElement ) &&  path.elementAt(index+1).type == QPainterPath::CurveToElement )
					{
						ControlNode *node = new ControlNode(index,path.elementAt(index), item, scene);
							
						node->setRight(new ControlNode(index+1, path.elementAt(index+1), item, scene));
							
						index++;
							
						m_nodes << node;
						m_nodes << node->right();
					}
					else
					{
						ControlNode *node = new ControlNode(index, path.elementAt(index), item, scene);
						m_nodes << node;
					}
					index++;
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


void ContourSelection::setupActions()
{
	DAction *select = new DAction( QIcon(), tr("Contour Selection"), this);
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
	return new QWidget;
}
		
bool ContourSelection::isComplete() const
{
	return true;
}

void ContourSelection::aboutToChangeTool()
{
// 	m_view->setDragMode (QGraphicsView::NoDrag);
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
// 	foreach(NodeManager* node, m_nodes)
// 	{
// 		if(node)
// 		{
// 			node->syncNodesFromParent();
// 			if(node->parentItem())
// 			{
// 				node->parentItem()->setEditNodesed(true);
// 			}
// 		}
// 	}
}

Q_EXPORT_PLUGIN2(kt_editNodes, ContourSelection )
