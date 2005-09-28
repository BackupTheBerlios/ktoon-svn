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
 
#ifndef USE_KDE
#include "close.xpm"
#endif

#include "dtabwidget.h"

#include <QToolButton>
#include <QTabBar>

#include <QApplication>
#include <QSettings>

#include "comdefs.h"

DTabWidget::DTabWidget(QWidget *parent, const char *name)
    :KTWCLASS(parent), m_closeButton(0)
{
	setObjectName(name);
	setFocusPolicy(Qt::NoFocus);
    setMargin(0);

    loadSettings();
    
    if (!m_tabBarShown)
    {
        tabBar()->hide();
    }
    else {
        m_closeButton = new QToolButton(this);

        m_closeButton->setIconSet(QIcon( (const char **) close_xpm));

        m_closeButton->adjustSize();
        m_closeButton->hide();
	setCornerWidget(m_closeButton, Qt::TopRight);
        
        if (m_closeOnHover)
        {
//             setHoverCloseButton(true);
        }
    }
    
    connect(this, SIGNAL(currentChanged(QWidget*)), this, SLOT(setFocus(QWidget*)));
//    connect(this, SIGNAL(currentChanged(QWidget*)), this, SLOT(updateHistory(QWidget*)));
}

void DTabWidget::loadSettings()
{
    QSettings config;
    config.beginGroup("DLSLib");
    
    m_tabBarShown = !config.value("TabWidgetVisibility", 0).toBool();
    m_closeOnHover = config.value("CloseOnHover", false).toBool();
    m_closeButtonShown = config.value("ShowCloseTabsButton", true).toBool();

}

void DTabWidget::saveSettings()
{
}

QToolButton *DTabWidget::closeButton() const
{
    return m_closeButton;
}

void DTabWidget::setFocus(QWidget *w)
{
    if (w)
    {
        w->setFocus();
    }
}

void DTabWidget::insertTab(QWidget *child, const QString &label, int index)
{
	child->setParent(0);
    if (m_closeButton && m_closeButtonShown)
    {
        m_closeButton->show();
    }
    KTWCLASS::insertTab(index, child, label);
    if (index != -1) tabBar()->repaint();
}

void DTabWidget::insertTab(QWidget *child, const QIcon &iconset, 
    const QString &label, int index)
{
    if (m_closeButton && m_closeButtonShown)
    {
        m_closeButton->show();
    }
    KTWCLASS::insertTab(index, child, iconset, label );
    if (index != -1) tabBar()->repaint();
}


