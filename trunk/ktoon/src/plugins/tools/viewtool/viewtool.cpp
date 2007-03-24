/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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
#include "viewtool.h"

#include <QKeySequence>
#include <QDebug>
#include <QImage>
#include <QPaintDevice>


#include <dcore/dglobal.h>
#include <dcore/dgradientadjuster.h>
#include <dcore/ddebug.h>

#include "ktrectitem.h"
#include "ktellipseitem.h"
#include "ktlineitem.h"

#include "ktscene.h"

#include <QGraphicsView>
#include "ktinputdeviceinformation.h"
#include "ktgraphicsscene.h"

ViewTool::ViewTool() : m_rect(0), m_view(0)
{
	setupActions();
}

ViewTool::~ViewTool()
{
}

QStringList ViewTool::keys() const
{
	return QStringList() << tr("Zoom") << tr("Hand");
}

void ViewTool::setupActions()
{
	DAction *action1 = new DAction( QIcon(THEME_DIR+"/icons/magnifying.png"), tr("Zoom"), this);
	action1->setShortcut( QKeySequence(tr("")) );
// 	action1->setCursor( QCursor(THEME_DIR+"/cursors/square.png") );
	
	m_actions.insert( tr("Zoom"), action1 );
	
	DAction *action2 = new DAction( QIcon(THEME_DIR+"/icons/hand.png"), tr("Hand"), this);
	m_actions.insert( tr("Hand"), action2 );
	
}

void ViewTool::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene, QGraphicsView *view)
{
// 	if(input->buttons() == Qt::LeftButton)
	{
		m_rect = new QGraphicsRectItem(QRectF(input->pos(), QSize(0,0)));
		scene->addItem(m_rect);
	}
}

void ViewTool::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene, QGraphicsView *view)
{
	m_view = view;
	if ( currentTool() == tr("Zoom") )
	{
		view->setDragMode(QGraphicsView::NoDrag);
		QRectF rect = m_rect->rect();
		rect.setBottomLeft(input->pos());
		m_rect->setRect(rect);
	}
	else if( currentTool() == tr("Hand"))
	{
		view->setDragMode(QGraphicsView::ScrollHandDrag);
	}
}

void ViewTool::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene, QGraphicsView *view)
{
	if( currentTool() == tr("Zoom"))
	{
		QRectF rect = m_rect->rect();
		if ( input->button() == Qt::LeftButton )
		{
			view->fitInView( rect, Qt::KeepAspectRatio);
		}
		else 
		{
			QRect visibleRect = view->visibleRegion().boundingRect();
			
			view->fitInView( visibleRect.adjusted((rect.width()+50), 0, 0, (rect.height()+50)), Qt::KeepAspectRatio ); // FIXME
		}
	}
	delete m_rect;
}

QMap<QString, DAction *> ViewTool::actions() const
{
	return m_actions;
}

int ViewTool::toolType() const
{
	return View;
}

QWidget *ViewTool::configurator()
{
	return  0;
}

void ViewTool::aboutToChangeTool()
{
	if( currentTool() == tr("Hand"))
	{
		if(m_view)
		{
			m_view->setDragMode(QGraphicsView::NoDrag);
		}
	}
}

Q_EXPORT_PLUGIN2( kt_view, ViewTool )
