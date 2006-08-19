/**************************************************************************
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

#include "geometrictool.h"

#include <QKeySequence>
#include <QDebug>
#include <QImage>
#include <QPaintDevice>


#include "dglobal.h"
#include "dgradientadjuster.h"
#include "ddebug.h"

#include "ktrectitem.h"
#include "ktellipseitem.h"
#include "ktlineitem.h"

#include "ktscene.h"

GeometricTool::GeometricTool()
{
	setupActions();
}

GeometricTool::~GeometricTool()
{
}

QStringList GeometricTool::keys() const
{
	return QStringList() << tr("Rectangle") << tr("Ellipse") << tr("Line");
}

void GeometricTool::setupActions()
{
	DAction *action1 = new DAction( QIcon(THEME_DIR+"/icons/square.png"), tr("Rectangle"), this);
	action1->setShortcut( QKeySequence(tr("Ctrl+R")) );
	action1->setCursor( QCursor(THEME_DIR+"/cursors/square.png") );
	
	m_actions.insert( tr("Rectangle"), action1 );
	
	DAction *action2 = new DAction(QIcon(THEME_DIR+"/icons/ellipse.png"), tr("Ellipse"), this);
	action2->setShortcut( QKeySequence(tr("Ctrl+E")) );
	action2->setCursor( QCursor(THEME_DIR+"/cursors/circle.png") );
	
	m_actions.insert(tr("Ellipse"), action2);
	
	
	DAction *action3 = new DAction( QIcon(THEME_DIR+"/icons/line.png"), tr("Line"), this);
	action3->setShortcut( QKeySequence(tr("Ctrl+L")) );
	m_actions.insert(tr("Line"), action3);
}

void GeometricTool::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	if(input->buttons() == Qt::LeftButton)
	{
		QPoint pos = input->pos();
		
		if ( currentTool() == tr("Rectangle") )
		{
			m_item = new KTRectItem;
			
			static_cast<QAbstractGraphicsShapeItem *>(m_item)->setPen( brushManager->pen() );
			static_cast<QAbstractGraphicsShapeItem *>(m_item)->setBrush( brushManager->brush() );
		}
		else if ( currentTool() == tr("Ellipse") )
		{
			m_item = new KTEllipseItem;
			
			static_cast<QAbstractGraphicsShapeItem *>(m_item)->setPen( brushManager->pen() );
			static_cast<QAbstractGraphicsShapeItem *>(m_item)->setBrush( brushManager->brush() );
		}
		else if ( currentTool() == tr("Line") )
		{
			m_item = new KTLineItem;
			static_cast<QGraphicsLineItem *>(m_item)->setPen( brushManager->pen() );
			static_cast<QGraphicsLineItem *>(m_item)->setLine(QLineF(0,0,0,0));
		}
		
		m_item->setPos(pos);
		
		scene->addItem(m_item);
	}
}

void GeometricTool::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	if (currentTool() == tr("Rectangle") )
	{
		m_rect = static_cast<KTRectItem *>(m_item)->rect();
		
		m_rect.setBottomRight(m_item->mapFromScene(input->pos()) );
		
		static_cast<KTRectItem *>(m_item)->setRect(m_rect);
	}
	else if ( currentTool() == tr("Ellipse"))
	{
		m_rect = static_cast<KTEllipseItem *>(m_item)->rect();
		m_rect.setBottomRight(m_item->mapFromScene(input->pos()) );
		static_cast<KTEllipseItem *>(m_item)->setRect(m_rect);
	}
	else if( currentTool() == tr("Line"))
	{
		QPointF pos = m_item->mapFromScene(input->pos());
		QLineF line(static_cast<KTLineItem *>(m_item)->line().x1(), static_cast<KTLineItem *>(m_item)->line().y1(), pos.x(), pos.y());
		
		static_cast<KTLineItem *>(m_item)->setLine(line);
	}
}

void GeometricTool::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	
}

QMap<QString, DAction *> GeometricTool::actions() const
{
	return m_actions;
}

int GeometricTool::toolType() const
{
	return Brush;
}
		
QWidget  *GeometricTool::configurator()
{
	return  0;
}

void GeometricTool::aboutToChangeTool() 
{

}


Q_EXPORT_PLUGIN2( kt_geometric, GeometricTool )

