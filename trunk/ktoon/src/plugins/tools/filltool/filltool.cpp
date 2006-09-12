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

#include "filltool.h"

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

FillTool::FillTool()
{
	setupActions();
}

FillTool::~FillTool()
{
}

QStringList FillTool::keys() const
{
	return QStringList() << tr("Fill");
}

void FillTool::setupActions()
{
	DAction *action1 = new DAction( QIcon(THEME_DIR+"/icons/fill.png"), tr("Fill"), this);
	action1->setShortcut( QKeySequence(tr("Ctrl+F")) );
	action1->setCursor( QCursor(THEME_DIR+"/cursors/paint.png") );
	
	m_actions.insert( tr("Fill"), action1 );
	
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

void FillTool::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
	if(input->buttons() == Qt::LeftButton)
	{
		QPoint pos = input->pos();
		
		if ( currentTool() == tr("Fill") )
		{
			QList<QGraphicsItem *> items = scene->items(input->pos());
	
			if ( items.count() > 0 )
			{
				QGraphicsItem *itemPress = items[0];
				if  ( KTPathItem *fillItem = itemPressed(itemPress, brushManager) )
				{
					QDomDocument doc;
					doc.appendChild(fillItem->toXml( doc ));
		
					KTItemEvent *event = new KTItemEvent(KTProjectEvent::Add, scene->index(), scene->currentLayerIndex(), scene->currentFrameIndex(), -1, doc.toString()); // Adds to end
	
					addProjectEvent(event);
					
					return;
				}
			}
		}
	}
}

void FillTool::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
}

void FillTool::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTScene *scene, QGraphicsView *view)
{
}

QMap<QString, DAction *> FillTool::actions() const
{
	return m_actions;
}

int FillTool::toolType() const
{
	return Fill;
}
		
QWidget  *FillTool::configurator()
{
	return  0;
}

void FillTool::aboutToChangeTool() 
{
	
}

KTPathItem *FillTool::itemPressed(QGraphicsItem *item, const KTBrushManager *brush)
{
	dDebug() << "Pressing item " << item->type();
	
	KTPathItem *fillItem = 0;
	
	QList<QGraphicsItem *> collides = item->collidingItems();
	
	if ( collides.count() == 0)
	{
		fillItem = KTItemConverter::convertToPath( item );
		
		fillItem->setBrush(brush->pen().brush());
		fillItem->setPen(Qt::NoPen);
		
		fillItem->setZValue(item->zValue()+1);
		
		item->scene()->addItem(fillItem );
	}
	
	
	return fillItem;
}

Q_EXPORT_PLUGIN2( kt_fill, FillTool )

