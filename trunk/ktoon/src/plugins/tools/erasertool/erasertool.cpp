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

#include "erasertool.h"

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
#include "ktpathitem.h"

#include "ktitemconverter.h"

#include "ktscene.h"

EraserTool::EraserTool()
{
	setupActions();
}

EraserTool::~EraserTool()
{
}

QStringList EraserTool::keys() const
{
	return QStringList() << tr("Eraser");
}

void EraserTool::setupActions()
{
	DAction *action1 = new DAction( QIcon(THEME_DIR+"/icons/eraser.png"), tr("Eraser"), this);
// 	action1->setShortcut( QKeySequence(tr("Ctrl+F")) );
	action1->setCursor( QCursor(THEME_DIR+"/cursors/eraser.png") );
	
	m_actions.insert( tr("Eraser"), action1 );
	
// 	DAction *action2 = new DAction(QIcon(THEME_DIR+"/icons/ellipse.png"), tr("Ellipse"), this);
// 	action2->setShortcut( QKeySequence(tr("Ctrl+E")) );
// 	action2->setCursor( QCursor(THEME_DIR+"/cursors/circle.png") );
// 	
// 	m_actions.insert(tr("Ellipse"), action2);
// 	
// 	
// 	DAction *action3 = new DAction( QIcon(THEME_DIR+"/icons/line.png"), tr("Line"), this);
// 	action3->setShortcut( QKeySequence(tr("Ctrl+L")) );
// 	m_actions.insert(tr("Line"), action3);
}

void EraserTool::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	if(input->buttons() == Qt::LeftButton)
	{
		QPoint pos = input->pos();
		
		if ( currentTool() == tr("Eraser") )
		{
			QList<QGraphicsItem *> items = scene->items(input->pos());
	
			if ( items.count() > 0 )
			{
				QGraphicsItem *itemPress = items[0];
				itemPressed(itemPress, brushManager, pos);
			}
		}
	}
}

void EraserTool::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
}

void EraserTool::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
}

QMap<QString, DAction *> EraserTool::actions() const
{
	return m_actions;
}

int EraserTool::toolType() const
{
	return Brush;
}
		
QWidget  *EraserTool::configurator()
{
	return  0;
}

void EraserTool::aboutToChangeTool() 
{
	
}

void EraserTool::itemPressed(QGraphicsItem *item, const KTBrushManager *brush, const QPoint &pos)
{
	QList<QGraphicsItem *> collides = item->collidingItems();
	
	if ( collides.count() == 0)
	{
		QRect intersectRect(pos.x()-(brush->pen().width()/2), pos.y()-(brush->pen().width())/2, brush->pen().width(), brush->pen().width());
		
		KTPathItem *path = qgraphicsitem_cast<KTPathItem*>(item);
		
		if ( !path )
		{
// 			QString conv = "<convert type=\"2\" />"; // to path type
// 			KTItemRequest *event = new KTItemRequest(KTProjectRequest::Convert, scene->index(), scene->currentLayerIndex(), scene->currentFrameIndex(), scene->currentFrame()->graphics().indexOf(item), conv);
// 			addProjectEvent(event);
		}
		else
		{
			QPainterPath ppath = path->path();
			
			
			
		}
	}
}

Q_EXPORT_PLUGIN2( kt_eraser, EraserTool )

