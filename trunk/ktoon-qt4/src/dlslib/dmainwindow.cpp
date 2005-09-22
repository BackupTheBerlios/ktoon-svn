
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

#include "dtabwidget.h"
#include "docksplitter.h"
#include "comdefs.h"

DMainWindow::DMainWindow(QWidget *parent, const char *name)
    :MWCLASS(parent, name), m_pFirstRemoved(false), m_pCurrentWidget(0)
{
    loadSettings();
    createToolWindows();
    m_pCentral = new Ideal::DockSplitter(Qt::Horizontal, this);
    m_pActiveTabWidget = createTab();
    m_pCentral->addDock(0, 0, m_pActiveTabWidget);
    setCentralWidget(m_pCentral);
}

void DMainWindow::loadSettings()
{
    QSettings config;
    config.setPath("NewMDI", qApp->name(), QSettings::User );
    m_pOpenTabAfterCurrent = config.readBoolEntry(SETTINGSPATH+"/UI/OpenNewTabAfterCurrent", true);
    m_pShowIconsOnTabs = config.readBoolEntry(SETTINGSPATH+"/UI/ShowTabIcons", false);
}

DMainWindow::~DMainWindow()
{
/*    for (QValueList<QWidget*>::iterator it = m_pWidgets.begin(); it != m_pWidgets.end(); ++it)
        removeWidget(*it);*/
}

DDockWindow *DMainWindow::toolWindow(DDockWindow::Position position) const
{
    switch (position) {
        case DDockWindow::Bottom: return m_pBottomDock;
        case DDockWindow::Left: return m_pLeftDock;
        case DDockWindow::Right: return m_pRightDock;
    }
    return 0;
}

void DMainWindow::createToolWindows()
{
    m_pBottomDock = new DDockWindow(this, DDockWindow::Bottom);
//     addDockWidget( Qt::BottomDockWidgetArea, m_pBottomDock );
    moveDockWindow(m_pBottomDock, Qt::DockBottom);
    m_pLeftDock = new DDockWindow(this, DDockWindow::Left);
//     addDockWidget( Qt::LeftDockWidgetArea, m_pLeftDock );
    moveDockWindow(m_pLeftDock, Qt::DockLeft);
    m_pRightDock = new DDockWindow(this, DDockWindow::Right);
//     addDockWidget( Qt::RightDockWidgetArea, m_pRightDock );
    moveDockWindow(m_pRightDock, Qt::DockRight);
}

void DMainWindow::addWidget(QWidget *widget, const QString &title)
{
//     invalidateActiveTabWidget();
    if (m_pFirstRemoved && m_pActiveTabWidget == m_pTabs.first())
    {
        m_pCentral->addDock(0, 0, m_pActiveTabWidget);
        m_pFirstRemoved = false;
    }
    
    addWidget(m_pActiveTabWidget, widget, title);
}

void DMainWindow::addWidget(DTabWidget *tab, QWidget *widget, const QString &title)
{
    int idx = -1;
    if (m_pOpenTabAfterCurrent && (tab->count() > 0))
    {
        idx = tab->currentPageIndex() + 1;
    }
    if (m_pShowIconsOnTabs)
    {
        const QPixmap *pixmap = widget->icon();
        const QIcon &icons = (pixmap && (pixmap->size().height() <= 16)) ? *(pixmap) : QIcon((const char **)icon_xpm);
        tab->insertTab(widget, icons, title, idx);
    }
    else
    {
        tab->insertTab(widget, title, idx);
    }
    m_pWidgets.append(widget);
    m_pWidgetTabs[widget] = tab;
    widget->installEventFilter(this);
    tab->showPage(widget);
}

void DMainWindow::removeWidget(QWidget *widget)
{
    if (!m_pWidgets.contains(widget))
    {
        return; //not a widget in main window
    }
    
    if (m_pWidgetTabs.contains(widget))
    {
        DTabWidget *tab = m_pWidgetTabs[widget];
        if (tab->indexOf(widget) >= 0)
        {
            tab->removePage(widget);
            widget->reparent(0,QPoint(0,0),false);
            if (tab->count() == 0)
            {
                if (tab->closeButton())
                {
                    tab->closeButton()->hide();
                }
                //remove and delete tabwidget if it is not the first one
                if (tab != m_pTabs.first())
                {
                    QPair<uint, uint> idx = m_pCentral->indexOf(tab);
                    m_pTabs.remove(tab);
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
                    if (m_pActiveTabWidget->currentPage())
                    {
                        m_pActiveTabWidget->currentPage()->setFocus();
                    }
                }
            }
        }
    }
    
    m_pWidgets.remove(widget);
    m_pWidgetTabs.remove(widget);
}

DTabWidget *DMainWindow::splitHorizontal() 
{
    m_pActiveTabWidget = createTab();
    m_pCentral->addDock(m_pCentral->numRows(), 0, m_pActiveTabWidget);
    return m_pActiveTabWidget;
}

DTabWidget *DMainWindow::splitVertical() 
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
        DTabWidget *tab = m_pWidgetTabs[focused];
        if (tab->indexOf(focused) >= 0)
            m_pActiveTabWidget = tab;
        kdDebug() << "    tab: " << tab << endl;
    }*/
}

DTabWidget *DMainWindow::createTab()
{
    DTabWidget *tab = new DTabWidget(m_pCentral);
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
    else if (ev->type() == QEvent::IconChange)
    {
        if (m_pWidgetTabs.contains(w))
        {
            DTabWidget *tab = m_pWidgetTabs[w];
            tab->setTabIconSet(w, w->icon() ? (*(w->icon())) : QPixmap());
        }
    }
    else if (ev->type() == QEvent::CaptionChange)
    {
        qDebug("caption change");
    }

    return MWCLASS::eventFilter(obj, ev);
}

void DMainWindow::closeTab()
{
    //nothing to do here, should be reimplemented
    
    removeWidget(m_pCurrentWidget);
    
}

void DMainWindow::tabContext(QWidget *, const QPoint &)
{
    //nothing to do here, should be reimplemented
}

void DMainWindow::closeTab(QWidget *)
{
    //nothing to do here, should be reimplemented
}
