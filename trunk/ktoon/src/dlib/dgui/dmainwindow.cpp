/***************************************************************************
 *   Copyright (C) 2005 by Alexander Dymo                                  *
 *   adymo@kdevelop.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

static const char* const icon_xpm[]={
"5 5 2 1",
"# c black",
". c None",
"####.",
"##...",
"#.#..",
"#..#.",
"....#"};

#include "dmainwindow.h"

#include <QPixmap>
#include <QEvent>
#include <QApplication>
#include <QToolButton>
#include <QDebug>

#include "ditabwidget.h"
#include "displitter.h"
#include "comdefs.h"

#include <ddebug.h>

DMainWindow::DMainWindow(QWidget *parent)
	:MWCLASS(parent), m_pFirstRemoved(false), m_pCurrentWidget(0)
	{
		loadSettings();
		createToolWindows();
		m_pCentral = new Ideal::DiSplitter(Qt::Horizontal, 0);
		setCentralWidget(m_pCentral);
	
		m_pActiveTabWidget = createTab();
		m_pCentral->addDock(0, 0, m_pActiveTabWidget);
	
		m_pCentral->setMaximumSize(maximumSize());
	}

	void DMainWindow::loadSettings()
	{
		QSettings config;
		config.beginGroup("DLSLib");
//     config.setPath("NewMDI", qApp->name(), QSettings::User );
		m_pOpenTabAfterCurrent = config.value("OpenNewTabAfterCurrent", true).toBool();
		m_pShowIconsOnTabs = config.value("ShowTabIcons", true).toBool();
	}

	DMainWindow::~DMainWindow()
	{
/*    for (QValueList<QWidget*>::iterator it = m_pWidgets.begin(); it != m_pWidgets.end(); ++it)
		removeWidget(*it);*/
	}

	DiDockWidget *DMainWindow::toolWindow(DiDockWidget::Position position) const
	{
		switch (position) 
		{
			case DiDockWidget::Bottom: return m_pBottomDock;
			case DiDockWidget::Left: return m_pLeftDock;
			case DiDockWidget::Right: return m_pRightDock;
		}
		return 0;
	}

	QWidget *DMainWindow::findCorrectSeparator()
	{
		// FIXME FIXME FIXME
		QList<QWidget*> widgets = findChildren<QWidget*>();
		QList<QWidget*>::ConstIterator it = widgets.begin();
	
		while (it != widgets.end())
		{
			if ((*it)->metaObject()->className() == QString("QDockSeparator")) // HACK
			{
// 				m_separators << (*it);
			}
			++it;
		}
	
		return m_separators[m_separators.count()-1];
	}

	void DMainWindow::createToolWindows()
	{
		m_pBottomDock = new DiDockWidget(this, DiDockWidget::Bottom);
		addDockWidget( Qt::BottomDockWidgetArea, m_pBottomDock );
    
		m_pLeftDock = new DiDockWidget(this, DiDockWidget::Left);
		addDockWidget( Qt::LeftDockWidgetArea, m_pLeftDock );
    
		m_pRightDock = new DiDockWidget(this, DiDockWidget::Right);
		addDockWidget( Qt::RightDockWidgetArea, m_pRightDock );
	}

	void DMainWindow::addDockWidget(Qt::DockWidgetArea area, DiDockWidget * dockwidget )
	{
		QMainWindow::addDockWidget(area, dockwidget );
		QWidget *separator = findCorrectSeparator();
		if (separator)
		{
			dockwidget->centralWidget()->setSeparator( separator );
		}
	}

	void DMainWindow::addWidget(QWidget *widget, const QString &title, bool persistant)
	{
		if (m_pFirstRemoved && m_pActiveTabWidget == m_pTabs.first() )
		{
			m_pCentral->addDock(0, 0, m_pActiveTabWidget);
			m_pFirstRemoved = false;
			m_pActiveTabWidget->setVisible(true);
		}

		addWidget(m_pActiveTabWidget, widget, title, persistant);
	}

	void DMainWindow::addWidget(DiTabWidget *tab, QWidget *widget, const QString &title, bool persistant)
	{
		int idx = -1;
		if (m_pOpenTabAfterCurrent && (tab->count() > 0))
		{
			idx = tab->currentIndex() + 1;
		}

		if (m_pShowIconsOnTabs)
		{
			QPixmap pixmap = widget->windowIcon().pixmap(16,16);
			const QIcon &icons = (!pixmap.isNull() && (pixmap.size().height() <= 16)) ? pixmap : QIcon((const char **)icon_xpm);
			tab->insertTab(widget, icons, title, idx);
		}
		else
		{
			tab->insertTab(widget, title, idx);
		}
		m_pWidgets.append(widget);
		m_pWidgetTabs[widget] = tab;
		widget->installEventFilter(this);
		tab->setCurrentWidget(widget);
    
		if ( persistant )
		{
			m_persistantWidgets << widget;
		}
    
		widget->setFocus();
	}

	void DMainWindow::removeWidget(QWidget *widget)
	{
		if (!m_pWidgets.contains(widget))
		{
			return; //not a widget in main window
		}
    
		if (m_pWidgetTabs.contains(widget))
		{
			DiTabWidget *tab = m_pWidgetTabs[widget];
			if (tab->indexOf(widget) >= 0 && m_pActiveTabWidget->count() > 0)
			{
				tab->removeTab(tab->indexOf(widget));
				widget->setParent(0);
				if (tab->count() == 0)
				{
					if (tab->closeButton())
					{
						tab->closeButton()->hide();
					}
                //remove and delete tabwidget if it is not the first one
					if (tab != m_pTabs.first())
					{
						QPair<int, int> idx = m_pCentral->indexOf(tab);
						m_pTabs.removeAll(tab);
						m_pActiveTabWidget = m_pTabs.first();
						m_pCentral->removeDock(idx.first, idx.second, true);
					}
                //only temporarily remove the first tabwidget
					else
					{
						m_pCentral->removeDock(0, 0, false);
						m_pFirstRemoved = true;
					}
                //focus smth in m_pActiveTabWidget
					if (m_pActiveTabWidget)
					{
						if (m_pActiveTabWidget->currentWidget())
						{
							m_pActiveTabWidget->currentWidget()->setFocus();
						}
					}
				}
			}
		}
    
		m_pWidgets.removeAll(widget);
		m_pWidgetTabs.remove(widget);
	}

	DiTabWidget *DMainWindow::splitHorizontal() 
	{
		m_pActiveTabWidget = createTab();
		m_pCentral->addDock(m_pCentral->numRows(), 0, m_pActiveTabWidget);
		return m_pActiveTabWidget;
	}

	DiTabWidget *DMainWindow::splitVertical() 
	{
//     invalidateActiveTabWidget();
		int row = m_pCentral->indexOf(m_pActiveTabWidget).first;
		m_pActiveTabWidget = createTab();
		m_pCentral->addDock(row, m_pCentral->numCols(row), m_pActiveTabWidget);
		return m_pActiveTabWidget;
	}

	void DMainWindow::invalidateActiveTabWidget()
	{
/*    QWidget *focused = m_pCentral->focusWidget();
		kdDebug() << "invalidate: " << focused << endl;
		if (focused == 0)
		return;
		if (!m_pWidgets.contains(focused))
		{
		kdDebug() << "    focused is not in m_pWidgets" << endl;
		return;
	}
		if (m_pWidgetTabs.contains(focused))
		{
		kdDebug() << "    focused is in m_pWidgets and m_pWidgetTabs" << endl;
		DiTabWidget *tab = m_pWidgetTabs[focused];
		if (tab->indexOf(focused) >= 0)
		m_pActiveTabWidget = tab;
		kdDebug() << "    tab: " << tab << endl;
	}*/
	}

	DiTabWidget *DMainWindow::createTab()
	{
		DiTabWidget *tab = new DiTabWidget(m_pCentral);
		m_pTabs.append(tab);
		if (tab->closeButton())
		{
			connect(tab->closeButton(), SIGNAL(clicked()), this, SLOT(closeTab()));
		}

		return tab;
	}

	bool DMainWindow::eventFilter(QObject *obj, QEvent *ev)
	{
		QWidget *w = (QWidget*)obj;
    
		if (!m_pWidgets.contains(w))
		{
			return MWCLASS::eventFilter(obj, ev);
		}
    
		if ((m_pCurrentWidget != w) && (ev->type() == QEvent::FocusIn))
		{
			m_pCurrentWidget = w;
			emit widgetChanged(w);
		}
    
//     else if (ev->type() == QEvent::IconChange)
//     {
//         if (m_pWidgetTabs.contains(w))
//         {
//             DiTabWidget *tab = m_pWidgetTabs[w];
//             tab->setTabIconSet(w, w->icon() ? (*(w->icon())) : QPixmap());
//         }
//     }
//     else if (ev->type() == QEvent::CaptionChange)
//     {
//         qDebug("caption change");
//     }

		return MWCLASS::eventFilter(obj, ev);
	}

	void DMainWindow::closeTab()
	{
		if ( ! m_persistantWidgets.contains(m_pCurrentWidget) )
		{
			QWidget *w = m_pCurrentWidget;
			removeWidget(w);
			delete w;
		}
	}

	void DMainWindow::tabContext(QWidget *, const QPoint &)
	{
    //nothing to do here, should be reimplemented
	}

	void DMainWindow::closeTab(QWidget *w)
	{
		removeWidget( w );
	}
