/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#include "brush.h"
#include "brush.xpm"
#include "ddebug.h"
#include <QPointF>
#include <QKeySequence>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QMatrix>

#include <daction.h>

#include "dglobal.h"
#include "ktgraphicalgorithm.h"

#include "ktscene.h"


#include <QGraphicsLineItem>
#include <dalgorithm.h>


Brush::Brush() : m_configurator(0), m_item(0)
{
	m_configurator = new ExactnessConfigurator;
	setupActions();
}

Brush::~Brush()
{
	DEND;
	delete m_configurator;
}

void Brush::init(QGraphicsView *view)
{
	view->setDragMode ( QGraphicsView::NoDrag );
	
	foreach(QGraphicsItem *item, view->scene()->items() )
	{
		item->setFlag(QGraphicsItem::ItemIsSelectable, false);
		item->setFlag(QGraphicsItem::ItemIsMovable, false);
	}
}

QStringList Brush::keys() const
{
	return QStringList() << tr("Pencil") ;
}

void Brush::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	m_firstPoint = input->pos();
	m_path = QPainterPath();
	m_path.moveTo(m_firstPoint);
	
	m_oldPos = input->pos();
	
	m_item = new KTPathItem();
	
	m_item->setBrush( brushManager->brush() );
	m_item->setPen( brushManager->pen() );
	scene->addGraphic( m_item );
// 	move(event, brushManager, scene, view);
}

void Brush::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	view->setDragMode (QGraphicsView::NoDrag);
	QPainterPath path;
	path.setFillRule ( Qt::WindingFill );
	
	path.moveTo( m_oldPos);
	path.lineTo( input->pos() );
	
	m_path.closeSubpath();
	m_path.addPath(path);
	
	m_item->setPath(m_path);
	
// 	QPoint first(DAlgorithm::random() % view->width(), DAlgorithm::random() % view->height());
// 	QPoint sec(DAlgorithm::random() % view->width(), DAlgorithm::random() % view->height());
// 	
// 	QGraphicsLineItem *item = new QGraphicsLineItem(QLineF(first, sec) );
// 	scene->addItem(item);
	
	m_oldPos = input->pos();
}

void Brush::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	int smoothness = m_configurator->exactness();
	
	if ( m_firstPoint == input->pos() && m_path.elementCount() == 1)
	{
		smoothness = 0;
		m_path.addEllipse(input->pos().x(), input->pos().y(), brushManager->pen().width(), brushManager->pen().width());
	}
	
	m_firstPoint = QPoint(0,0);
	
	
	QPolygonF pol;
	QList<QPolygonF> polygons = m_path.toSubpathPolygons();
	
	QList<QPolygonF>::iterator it = polygons.begin();
	
	
	QPolygonF::iterator pointIt;
	while(it != polygons.end() )
	{
		pointIt = (*it).begin();
		
		while(pointIt <= (*it).end()-2)
		{
			pol << (*pointIt);
			pointIt += 2;
		}
		++it;
	}
	
	if(smoothness > 0)
	{
		m_path = KTGraphicalAlgorithm::bezierFit(pol, smoothness);
	}
	else
	{
		m_path = QPainterPath();
		m_path.addPolygon(pol);
	}
	
	QPainterPath newPath = m_path;
	QMatrix m;
	QPainterPath::Element e = m_path.elementAt(0);
	
	if(e.type == QPainterPath::MoveToElement)
	{
		QPointF pos = m_path.boundingRect().topLeft();
		m.translate(-pos.x() , -pos.y());
		newPath = m.map(newPath);
		m_item->setPos(pos);
// 		m_item->translate(pos.x(), pos.y());
	}
	
	m_item->setPath(newPath);
}

QPainterPath Brush::path() const
{
	return m_path;
}

void Brush::setupActions()
{
	DAction *pencil = new DAction( QIcon(brush_xpm), tr("Pencil"), this);
	pencil->setShortcut( QKeySequence(tr("Ctrl+B")) );
		
	QPixmap pix(THEME_DIR+"/cursors/pencil.png");
	pencil->setCursor( QCursor(pix, 0, pix.height()) );
		
	m_actions.insert( tr("Pencil"), pencil );
}

QMap<QString, DAction *> Brush::actions() const
{
	return m_actions;
}

int Brush::toolType() const
{
	return KTToolInterface::Brush;
}

QWidget *Brush::configurator() 
{
	if ( ! m_configurator )
	{
		m_configurator = new ExactnessConfigurator;;
	}
	
	return m_configurator;
}
		
bool Brush::isComplete() const
{
	return true;
}

void Brush::aboutToChangeTool() 
{
}

QString Brush::toolToXml() const
{
	QDomDocument doc;
	doc.appendChild(m_item->toXml( doc ));
	return doc.toString();
}

KTProjectEvent::Action Brush::action() const
{
	return KTProjectEvent::Add;
}



Q_EXPORT_PLUGIN2(kt_brush, Brush )
