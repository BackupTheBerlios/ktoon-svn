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

#include "node.h"

#include <QDebug>

Select::Select()
{
	setupActions();
}


Select::~Select()
{
	
}

void Select::init(QGraphicsView *view)
{
	m_view = view;
	
	foreach(QGraphicsItem *item, view->scene()->items() )
	{
		item->setFlags (QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable );
	}
}

QStringList Select::keys() const
{
	return QStringList() << tr("Select") ;
}

void Select::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	view->setDragMode (QGraphicsView::RubberBandDrag);
}

void Select::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	static int s = 0;
	s++;
}

void Select::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	D_FUNCINFO;
	
	QList<Node *>::iterator it = m_nodes.begin();
	
	while(it != m_nodes.end())
	{
		if(!(*it))
		{
			++it;
			continue;
		}
		
		scene->removeItem((*it));
		m_nodes.erase (it);
		delete (*it);
		++it;
	}
	
	if(scene->selectedItems().count() > 0)
	{
		QList<QGraphicsItem *> selecteds = scene->selectedItems();
		QList<Node *>::iterator it = m_nodes.begin();
		
		foreach(QGraphicsItem *item, selecteds)
		{
			if(item)
			{
				QRectF rect = item->boundingRect();
				Node *topLeft = new Node(Node::TopLeft, rect.topLeft(),item, scene);
				Node *topRight = new Node(Node::TopRight, rect.topRight(),item, scene);
				Node *bottomLeft = new Node(Node::BottomLeft, rect.bottomLeft(),item, scene);
				Node *bottomRight = new Node(Node::BottomRight, rect.bottomRight(),item, scene);
				Node *center = new Node(Node::Center, rect.center() ,item, scene);
				m_nodes << topLeft << topRight<< bottomLeft << bottomRight << center;
			}
		}
	}
}

QPainterPath Select::path() const
{
// 	return m_path;
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
// 	if ( ! m_configurator )
// 	{
// 		m_configurator = new ExactnessConfigurator;;
// 	}
	
	return new QWidget;
}
		
bool Select::isComplete() const
{
	return true;
}

void Select::aboutToChangeTool()
{
	m_view->setDragMode (QGraphicsView::NoDrag);
}

QString Select::toolToXml() const
{
	QDomDocument doc;
// 	doc.appendChild(m_item->toXml( doc ));
	return doc.toString();
}

Q_EXPORT_PLUGIN2(kt_select, Select )
