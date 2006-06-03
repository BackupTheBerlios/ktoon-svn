/***************************************************************************
 *   Copyright (C) 2004 by Alexander Dymo                                  *
 *   adymo@kdevelop.org                                                    *
 *   David Cuadrado (C) 2005 						   *
 *   krawek@gmail.com							   *
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

#include "dlstabwidget.h"

#include <QToolButton>
#include <QTabBar>

#include <QApplication>
#include <QSettings>
#include <QLayout>

#include "comdefs.h"

DLSTabWidget::DLSTabWidget(QWidget *parent)
	: KTWCLASS(parent), m_closeButton(0)
{
	setFocusPolicy(Qt::NoFocus);

	loadSettings();
    
	if (!m_tabBarShown)
	{
		tabBar()->hide();
	}
	else 
	{
		m_closeButton = new QToolButton(this);

		m_closeButton->setIcon(QIcon( (const char **) close_xpm));

		m_closeButton->adjustSize();
		m_closeButton->hide();
		setCornerWidget(m_closeButton, Qt::TopRightCorner);
        
		if (m_closeOnHover)
		{
//             		setHoverCloseButton(true);
		}
	}
    
	connect(this, SIGNAL(currentChanged(QWidget*)), this, SLOT(setFocus(QWidget*)));
}

void DLSTabWidget::loadSettings()
{
	QSettings config;
	config.beginGroup("DLSLib");
    
	m_tabBarShown = !config.value("TabWidgetVisibility", 0).toBool();
	m_closeOnHover = config.value("CloseOnHover", false).toBool();
	m_closeButtonShown = config.value("ShowCloseTabsButton", true).toBool();

}

void DLSTabWidget::saveSettings()
{
}

QToolButton *DLSTabWidget::closeButton() const
{
	return m_closeButton;
}

void DLSTabWidget::setFocus(QWidget *w)
{
	if (w)
	{
		w->setFocus();
	}
}

void DLSTabWidget::insertTab(QWidget *child, const QString &label, int index)
{
	child->setParent(0);
	if (m_closeButton && m_closeButtonShown)
	{
		m_closeButton->show();
	}
	KTWCLASS::insertTab(index, child, label);
	if (index != -1) tabBar()->repaint();
}

void DLSTabWidget::insertTab(QWidget *child, const QIcon &iconset, 
			   const QString &label, int index)
{
	if (m_closeButton && m_closeButtonShown)
	{
		m_closeButton->show();
	}
	KTWCLASS::insertTab(index, child, iconset, label );
	if (index != -1) tabBar()->repaint();
}


