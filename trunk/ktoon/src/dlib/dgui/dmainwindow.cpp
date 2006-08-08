/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                *
 *   krawek@gmail.com                                                      *
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

#include "dmainwindow.h"
#include "dbuttonbar.h"
#include "dtoolview.h"
#include "dviewbutton.h"

#include <QTimer>
#include <QMenu>
#include <QApplication>
#include <QDesktopWidget>

#include <QtDebug>


DMainWindow::DMainWindow(QWidget *parent)
	: QMainWindow(parent), m_forRelayout(0)
{
	addButtonBar(Qt::LeftToolBarArea);
	addButtonBar(Qt::RightToolBarArea);
	addButtonBar(Qt::TopToolBarArea);
	addButtonBar(Qt::BottomToolBarArea);

	setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
	setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
	
	setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
	setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
	
#if QT_VERSION >= 0x040200
	setDockNestingEnabled (false);
#endif
}


DMainWindow::~DMainWindow()
{
}


void DMainWindow::addButtonBar(Qt::ToolBarArea area)
{
	DButtonBar *bar = new DButtonBar(area, this);
	
	addToolBar(area, bar);
	m_buttonBars.insert(area, bar);
	
	bar->hide();
}


DToolView *DMainWindow::addToolView(QWidget *view, Qt::ToolBarArea defaultPlace, int workspace)
{
	DToolView *toolView = new DToolView(view->windowTitle(), view->windowIcon());
	toolView->setWidget(view);
	toolView->setWorkspace( workspace );
	
	toolView->button()->setArea(defaultPlace);
	m_buttonBars[defaultPlace]->addButton(toolView->button());
	
	addDockWidget(dockWidgetArea(defaultPlace), toolView);
	view->show();
	
	m_toolViews[m_buttonBars[defaultPlace]] << toolView;
	
	connect(toolView, SIGNAL(topLevelChanged(bool)), this, SLOT(relayoutViewButton(bool)));
	
	return toolView;
}

Qt::DockWidgetArea DMainWindow::dockWidgetArea(Qt::ToolBarArea area)
{
	switch(area)
	{
		case Qt::LeftToolBarArea:
		{
			return Qt::LeftDockWidgetArea;
		}
		break;
		case Qt::RightToolBarArea:
		{
			return Qt::RightDockWidgetArea;
		}
		break;
		case Qt::TopToolBarArea:
		{
			return Qt::TopDockWidgetArea;
		}
		break;
		case Qt::BottomToolBarArea:
		{
			return Qt::BottomDockWidgetArea;
		}
		break;
		default: qWarning("Floating..."); break;
	}
	
	return Qt::LeftDockWidgetArea;
}

Qt::ToolBarArea DMainWindow::toolBarArea(Qt::DockWidgetArea area)
{
	switch(area)
	{
		case Qt::LeftDockWidgetArea:
		{
			return Qt::LeftToolBarArea;
		}
		break;
		case Qt::RightDockWidgetArea:
		{
			return Qt::RightToolBarArea;
		}
		break;
		case Qt::TopDockWidgetArea:
		{
			return Qt::TopToolBarArea;
		}
		break;
		case Qt::BottomDockWidgetArea:
		{
			return Qt::BottomToolBarArea;
		}
		break;
		default: qWarning("Floating..."); break;
	}
	
	return Qt::LeftToolBarArea;
}


void DMainWindow::relayoutViewButton(bool topLevel)
{
	if ( !topLevel )
	{
		if ( DToolView *toolView = dynamic_cast<DToolView *>(sender()) )
		{
			m_forRelayout = toolView;
			QTimer::singleShot( 100, this, SLOT(relayoutToolView()));
			
			// if a tool view is floating the button bar isn't exclusive
			DButtonBar *bar = m_buttonBars[m_forRelayout->button()->area()];
			bool exclusive = true;
			
			foreach(DToolView *v, m_toolViews[bar] )
			{
				exclusive = exclusive && !v->isFloating();
			}
	
			bar->setExclusive( exclusive );
		}
	}
	else
	{
		// Floating tool views aren't exclusive
		if ( DToolView *toolView = dynamic_cast<DToolView *>(sender()) )
		{
			m_buttonBars[toolView->button()->area()]->setExclusive(false);
		}
	}
	
}

void DMainWindow::relayoutToolView()
{
	if ( !m_forRelayout ) return;
	
	DViewButton *button = m_forRelayout->button();
	
	Qt::ToolBarArea area = toolBarArea( QMainWindow::dockWidgetArea(m_forRelayout) );
	if ( area != button->area() && !m_forRelayout->isFloating() )
	{
		setUpdatesEnabled( false );
		
		m_buttonBars[button->area()]->removeButton(button);
		
		m_toolViews[m_buttonBars[button->area()]].removeAll(m_forRelayout);
		m_toolViews[m_buttonBars[area]] << m_forRelayout;
		
		button->setArea(area);
		m_buttonBars[area]->addButton(button);
		
		setUpdatesEnabled( true );
	}
	
	m_forRelayout = 0;
}

void DMainWindow::setCurrentWorkspace(int wsp)
{
	if ( m_currentWorkspace == wsp ) return;
	
	typedef QList<DToolView *> Views;
	
	QList<Views > viewsList = m_toolViews.values();
	
	foreach(Views views, viewsList)
	{
		setUpdatesEnabled( false );
		foreach(DToolView *v, views )
		{
			if ( v->workspace() & wsp )
			{
				m_buttonBars[ v->button()->area() ]->enable( v->button() );
				
				if ( v->button()->isChecked() )
				{
					v->show();
				}
			}
			else
			{
				m_buttonBars[ v->button()->area() ]->disable( v->button() );
				if ( v->button()->isChecked() )
				{
					v->hide();
				}
			}
		}
		setUpdatesEnabled( true );
	}
	
	m_currentWorkspace = wsp;
	
	emit workspaceChanged( m_currentWorkspace );
}

int DMainWindow::currentWorkspace() const
{
	return m_currentWorkspace;
}

QMenu *DMainWindow::createPopupMenu()
{
	QMenu *menu = QMainWindow::createPopupMenu();
	menu->addSeparator();
	
	return menu;
}


