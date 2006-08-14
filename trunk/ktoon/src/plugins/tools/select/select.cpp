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
#include <QDebug>
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
	m_view = view;
	
	foreach(QGraphicsItem *item, view->scene()->items() )
	{
		if(dynamic_cast<KTAbstractSerializable* >(item))
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
	Q_UNUSED(input);
	Q_UNUSED(brushManager);
	Q_UNUSED(scene);
	Q_UNUSED(view);
	view->setDragMode (QGraphicsView::RubberBandDrag);
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
	/*QTimer::singleShot ( 0, this, SLOT(*//*) );*/ ;
}

void Select::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
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
				NodeManager *manager = new NodeManager(item, scene);
				m_nodes << manager;
			}
		}
	}
	else
	{
		qDeleteAll(m_nodes);
		m_nodes.clear();
	}
}

QPainterPath Select::path() const
{
	return QPainterPath();
	
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
// 		m_configurator = new ExactnessConfigurator;
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


void Select::syncNodes()
{
	//FIXME: tratar de optimizar esto
	foreach(NodeManager* node, m_nodes)
	{
		if(node)
		{
			node->syncNodesFromParent();
			if(node->parentItem())
			{
				node->parentItem()->setSelected(true);
			}
		}
	}
}

Q_EXPORT_PLUGIN2(kt_select, Select )
