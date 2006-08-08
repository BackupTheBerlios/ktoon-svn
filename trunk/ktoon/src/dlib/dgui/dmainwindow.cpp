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
#include "dmainwindowabstractsettings.h"

#include <QTimer>
#include <QMenu>
#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>
#include <QApplication>
#include <QtDebug>

#include <QCloseEvent>

class DefaultSettings : public DMainWindowAbstractSettings
{
	public:
		DefaultSettings(QObject *parent);
		~DefaultSettings();
		
		void save(DMainWindow *w);
		void restore(DMainWindow *w);
};

DefaultSettings::DefaultSettings(QObject *parent) : DMainWindowAbstractSettings(parent)
{
}

DefaultSettings::~DefaultSettings()
{
}

void DefaultSettings::save(DMainWindow *w)
{
	QSettings settings(qApp->applicationName(), "ideality", this);
// 	settings.clear();
	
	QHash<Qt::ToolBarArea, DButtonBar *> buttonBars = w->buttonBars();
	QHash<DButtonBar *, QList<DToolView*> > toolViews = w->toolViews();
	
	foreach( DButtonBar *bar, buttonBars.values())
	{
		settings.beginGroup(bar->windowTitle());
		settings.setValue("exclusive", bar->isExclusive() );
		settings.endGroup();
		
		foreach(DToolView *view, toolViews[bar] )
		{
			settings.beginGroup(view->objectName());
			
// 			qDebug() << view->windowTitle() << view->button()->area();
			
			settings.setValue("area", int( view->button()->area() ));
	
			settings.setValue("size", view->fixedSize());
			settings.setValue("style", view->button()->toolButtonStyle() );
			settings.setValue("sensibility", view->button()->isSensible());
	
			settings.setValue("visible", view->isVisible() );
			settings.setValue("floating", view->isFloating());
			settings.setValue("position", view->pos());
			
			settings.endGroup();
		}
	}
	
	settings.beginGroup( "MainWindow" );
	settings.setValue("size", w->size());
	settings.setValue("maximized", w->isMaximized() );
	settings.setValue("position", w->pos());
	
	settings.endGroup();
}

void DefaultSettings::restore(DMainWindow *w)
{
	QSettings settings(qApp->applicationName(), "ideality", this);
	
	QHash<Qt::ToolBarArea, DButtonBar *> buttonBars = w->buttonBars();
	QHash<DButtonBar *, QList<DToolView*> > toolViews = w->toolViews();
	
	QList<DToolView *> toHide;
	
	foreach( DButtonBar *bar, buttonBars.values())
	{
		bar->setExclusive(false);
		foreach(DToolView *view, toolViews[bar] )
		{
			settings.beginGroup(view->objectName());
			
			// Restore position
			
			Qt::DockWidgetArea area = Qt::DockWidgetArea(settings.value("area", 0).toInt());
			
			w->moveToolView(view, area);
			
			view->setFixedSize(settings.value("size").toInt());
			
			view->button()->setToolButtonStyle( Qt::ToolButtonStyle(settings.value("style", int(view->button()->toolButtonStyle()) ).toInt()) );
			view->button()->setSensible( settings.value("sensibility", view->button()->isSensible()).toBool() );
			
			bool visible = settings.value("visible", false ).toBool();
			
			if ( visible )
			{
				view->button()->setChecked(true);
				view->show();
			}
			else
			{
				toHide << view;
			}
			
			view->setFloating(settings.value("floating", false).toBool() );
			if ( view->isFloating() )
			{
				view->move(settings.value("position").toPoint());
			}
			
			settings.endGroup();
		}
		
		settings.beginGroup(bar->windowTitle());
		bar->setExclusive(settings.value("exclusive", true ).toBool());
		settings.endGroup();
	}
	
	foreach(DToolView *v, toHide )
	{
		v->button()->setChecked(false);
		v->setVisible(false);
		v->close();
	}
	
	settings.beginGroup( "MainWindow" );
	w->resize(settings.value("size").toSize());
	bool maximized = settings.value("maximized", false ).toBool();
	if ( maximized )
	{
		w->showMaximized();
	}
	
	w->move(settings.value("position").toPoint());
	
	settings.endGroup();
}

// DMainWindow


DMainWindow::DMainWindow(QWidget *parent)
	: QMainWindow(parent), m_forRelayout(0), m_autoRestore(false)
{
	setObjectName("DMainWindow");
	
	m_settings = new DefaultSettings(this);
	
	addButtonBar(Qt::LeftToolBarArea);
	addButtonBar(Qt::RightToolBarArea);
	addButtonBar(Qt::TopToolBarArea);
	addButtonBar(Qt::BottomToolBarArea);

#if 0
	setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
	setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
	
	setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
	setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
#endif

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


DToolView *DMainWindow::addToolView(QWidget *widget, Qt::DockWidgetArea area, int workspace)
{
	DToolView *toolView = new DToolView(widget->windowTitle(), widget->windowIcon());
	toolView->setWidget(widget);
	toolView->setWorkspace( workspace );
	
	toolView->button()->setArea( toToolBarArea( area ) );
	m_buttonBars[toToolBarArea( area) ]->addButton(toolView->button());
	
	addDockWidget(area, toolView);
	widget->show();
	
	m_toolViews[m_buttonBars[toToolBarArea( area ) ]] << toolView;
	
	
	// Show separators
	if ( area == Qt::TopDockWidgetArea || area == Qt::BottomDockWidgetArea )
	{
		m_buttonBars[toToolBarArea( area ) ]->showSeparator(! m_buttonBars[Qt::LeftToolBarArea]->isEmpty());
	}
	else if ( area == Qt::LeftDockWidgetArea )
	{
		m_buttonBars[Qt::TopToolBarArea]->showSeparator( m_buttonBars[Qt::TopToolBarArea]->isEmpty() );
	}
	////
	
	connect(toolView, SIGNAL(topLevelChanged(bool)), this, SLOT(relayoutViewButton(bool)));
	
	return toolView;
}

void DMainWindow::moveToolView(DToolView *view, Qt::DockWidgetArea newPlace)
{
	if ( toDockWidgetArea(view->button()->area()) == newPlace || newPlace == Qt::AllDockWidgetAreas || newPlace == 0 )
	{
		return;
	}
	
	addDockWidget(newPlace, view);
	
	m_forRelayout = view;
	
	relayoutToolView();
}

Qt::DockWidgetArea DMainWindow::toDockWidgetArea(Qt::ToolBarArea area)
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
		default: qWarning("toDockWidgetArea: Floating... %d", area); break;
	}
	
	return Qt::LeftDockWidgetArea;
}

Qt::ToolBarArea DMainWindow::toToolBarArea(Qt::DockWidgetArea area)
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
		default: qWarning("toToolBarArea: Floating... %d", area); break;
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
			
			QTimer::singleShot( 0, this, SLOT(relayoutToolView()));
			
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
	
	bool isVisible = m_forRelayout->isVisible();
	
	if ( !isVisible ) m_forRelayout->show();
	
	DViewButton *button = m_forRelayout->button();
	
	Qt::ToolBarArea area = toToolBarArea( QMainWindow::dockWidgetArea(m_forRelayout) );
	
	if ( !isVisible ) m_forRelayout->close();
	
// 	qDebug() << "Relayout: " << m_forRelayout->windowTitle() << " from " << button->area() << " to " << area;
	
	if ( area != button->area() && !m_forRelayout->isFloating() )
	{
		setUpdatesEnabled( false );
		
		m_buttonBars[button->area()]->removeButton(button);
		
		// Show separators
		if ( button->area() == Qt::LeftToolBarArea )
		{
			m_buttonBars[Qt::BottomToolBarArea]->showSeparator(! m_buttonBars[Qt::LeftToolBarArea]->isEmpty());
			m_buttonBars[Qt::TopToolBarArea]->showSeparator(! m_buttonBars[Qt::LeftToolBarArea]->isEmpty());
		}
		else if ( area == Qt::LeftToolBarArea )
		{
			m_buttonBars[Qt::BottomToolBarArea]->showSeparator(m_buttonBars[Qt::LeftToolBarArea]->isEmpty());
			
			m_buttonBars[Qt::TopToolBarArea]->showSeparator(m_buttonBars[Qt::LeftToolBarArea]->isEmpty());
		}
		//////////
		
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

void DMainWindow::setAutoRestore(bool autoRestore)
{
	m_autoRestore = autoRestore;
}

bool DMainWindow::autoRestore() const
{
	return m_autoRestore;
}

QMenu *DMainWindow::createPopupMenu()
{
	QMenu *menu = QMainWindow::createPopupMenu();
	menu->addSeparator();
	
	return menu;
}

void DMainWindow::setSettingsFactory(DMainWindowAbstractSettings *settings)
{
	delete m_settings;
	
	m_settings = settings;
	m_settings->setParent(this);
}

void DMainWindow::closeEvent(QCloseEvent *e)
{
	saveGUI();
	
	QMainWindow::closeEvent(e);
}

void DMainWindow::showEvent(QShowEvent *e)
{
	QMainWindow::showEvent(e);
	
	if ( !m_autoRestore )
	{
		m_autoRestore = true;
		restoreGUI();
	}
}

void DMainWindow::saveGUI()
{
	m_settings->save( this );
}

void DMainWindow::restoreGUI()
{
	setUpdatesEnabled(false);
	m_settings->restore(this);
	setUpdatesEnabled(true);
}

QHash<Qt::ToolBarArea, DButtonBar *> DMainWindow::buttonBars() const
{
	return m_buttonBars;
}

QHash<DButtonBar *, QList<DToolView*> > DMainWindow::toolViews() const
{
	return m_toolViews;
}

