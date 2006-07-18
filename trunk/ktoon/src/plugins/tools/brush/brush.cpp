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

#include <daction.h>

#include "dglobal.h"
#include "ktgraphicalgorithm.h"

#include "ktscene.h"

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

QStringList Brush::keys() const
{
	return QStringList() << tr("Pencil") ;
}

void Brush::press(const QMouseEvent *event, KTBrushManager *brushManager, KTScene *scene)
{
	D_FUNCINFO;
	m_firstPoint = event->pos();
	m_path = QPainterPath();
	m_path.moveTo(m_firstPoint);
	
	m_oldPos = event->pos();
	
	m_item = new QGraphicsPathItem();
	
	m_item->setBrush( brushManager->brush() );
	m_item->setPen( brushManager->pen() );
	scene->addGraphic( m_item );
	move(event, brushManager, scene);
}

void Brush::move( const QMouseEvent *event, KTBrushManager *brushManager, KTScene *scene)
{
	int rad = brushManager->pen().width();
	QRect boundingRect = QRect(m_oldPos, event->pos()).normalized().adjusted(-rad, -rad, +rad, +rad);
	QColor color = brushManager->pen().color();
	int thickness = brushManager->pen().width();
	QColor transparentColor(color.red(), color.green(), color.blue(), 0);
	
	QPainterPath path;
	path.setFillRule ( Qt::WindingFill );
	
	path.moveTo( m_oldPos);
	path.lineTo( event->pos() );
	
	m_path.closeSubpath();
	m_path.addPath(path);
	
	m_item->setPath(m_path);
	
	
	m_oldPos = event->pos();
}

void Brush::release(const QMouseEvent *event, KTBrushManager *brushManager, KTScene *scene)
{
	D_FUNCINFO;
	
	int smoothness = m_configurator->exactness();
	
	if ( m_firstPoint == event->pos() && m_path.elementCount() == 1)
	{
		smoothness = 0;
		m_path.addEllipse(event->pos().x(), event->pos().y(), brushManager->pen().width(), brushManager->pen().width());
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
	
	
	
	m_item->setPath(m_path);
	
// 	scene->addGraphic(m_item);
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


Q_EXPORT_PLUGIN( Brush )

