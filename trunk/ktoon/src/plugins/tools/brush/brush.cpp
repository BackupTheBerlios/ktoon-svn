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

#include <QPointF>
#include <QKeySequence>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QMatrix>
#include <QGraphicsLineItem>


#include "ktgraphicalgorithm.h"
#include "ktscene.h"
#include "ktrequestbuilder.h"

#include <dgui/daction.h>
#include <dcore/dalgorithm.h>
#include <dcore/dglobal.h>
#include <dcore/ddebug.h>

Brush::Brush() : m_configurator(0), m_item(0)
{
	m_configurator = new ExactnessConfigurator;
	
	setupActions();
}

Brush::~Brush()
{
	delete m_configurator;
	m_configurator = 0;
}

void Brush::init(QGraphicsView *view)
{
	view->setDragMode ( QGraphicsView::NoDrag );
	
	Q_CHECK_PTR(view->scene());
	if ( QGraphicsScene *scene = qobject_cast<QGraphicsScene *>(view->scene()) )
	{
		foreach(QGraphicsItem *item, scene->items() )
		{
			item->setFlag(QGraphicsItem::ItemIsSelectable, false);
			item->setFlag(QGraphicsItem::ItemIsMovable, false);
		}
	}
}

QStringList Brush::keys() const
{
	return QStringList() << tr("Pencil") ;
}

void Brush::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	// TODO: seria interesante que si el item presionado es un pixmap tratar de seguir un patron!
	
	Q_UNUSED(view);
	
	m_firstPoint = input->pos();
	m_path = QPainterPath();
	m_path.moveTo(m_firstPoint);
	
	m_oldPos = input->pos();
	
	m_item = new KTPathItem();
	
	m_item->setPen( brushManager->pen() );
	scene->addItem( m_item );
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
	Q_UNUSED(scene);
	Q_UNUSED(view);
	
	double smoothness = m_configurator->exactness();
	
	if ( m_firstPoint == input->pos() && m_path.elementCount() == 1)
	{
		smoothness = 0;
		m_path.addEllipse(input->pos().x(), input->pos().y(), brushManager->pen().width(), brushManager->pen().width());
	}
	
	m_firstPoint = QPoint(0,0);
	
	smoothPath( m_path, smoothness );
	
	m_item->setBrush( brushManager->brush() );
	m_item->setPath(m_path);
	
	
	// Add KTProjectRequest
	
	QDomDocument doc;
	doc.appendChild(m_item->toXml( doc ));
	
	KTProjectRequest request = KTRequestBuilder::createItemRequest( scene->index(), scene->currentLayerIndex(), scene->currentFrameIndex(), scene->currentFrame()->graphics().count(), KTProjectRequest::Add, doc.toString() );
	
	emit requested(&request);
}

void Brush::smoothPath(QPainterPath &path, double smoothness, int from, int to)
{
	QPolygonF pol;
	QList<QPolygonF> polygons = path.toSubpathPolygons();
	
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
		path = KTGraphicalAlgorithm::bezierFit(pol, smoothness, from, to);
	}
	else
	{
		path = QPainterPath();
		path.addPolygon(pol);
	}
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


Q_EXPORT_PLUGIN2(kt_brush, Brush );

