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
static char * closeTab_xpm[] = {
"16 16 2 1",
" 	c None",
".	c #B33838",
"                ",
"   ...     ...  ",
"    ..     ..   ",
"     ..   ..    ",
"     ... ...    ",
"      .. ..     ",
"       ...      ",
"       ...      ",
"       ...      ",
"      .....     ",
"     ... ..     ",
"     ..  ...    ",
"    ..    ..    ",
"   ...     ..   ",
"  ...      ...  ",
"                "};
#endif

#include "dtabwidget.h"

#include <qtoolbutton.h>
#include <qtabbar.h>

#ifdef USE_KDE
#include <kconfig.h>
#include <kiconloader.h>
#include <kapplication.h>
#else
#include <qapplication.h>
#include <qsettings.h>
#endif

#include "comdefs.h"

DTabWidget::DTabWidget(QWidget *parent, const char *name)
    :KTWCLASS(parent, name), m_closeButton(0)
{
    setFocusPolicy(NoFocus);
    setMargin(0);

    loadSettings();
    
    if (!m_tabBarShown)
    {
        tabBar()->hide();
    }
    else {
        m_closeButton = new QToolButton(this);
#ifdef USE_KDE
        m_closeButton->setIconSet(SmallIcon("tab_remove"));
#else
        m_closeButton->setIconSet(QIconSet( (const char **) closeTab_xpm));
#endif
        m_closeButton->adjustSize();
        m_closeButton->hide();
        setCornerWidget(m_closeButton, TopRight);
        
        if (m_closeOnHover)
        {
#ifdef USE_KDE
            setHoverCloseButton(true);
#endif
        }
#ifdef USE_KDE
        setTabReorderingEnabled(true);
#endif
    }
    
    connect(this, SIGNAL(currentChanged(QWidget*)), this, SLOT(setFocus(QWidget*)));
//    connect(this, SIGNAL(currentChanged(QWidget*)), this, SLOT(updateHistory(QWidget*)));
}

void DTabWidget::loadSettings()
{
#ifdef USE_KDE
    KConfig *config = kapp->config();
    config->setGroup("UI");
//    m_tabBarShown = config->readBoolEntry("TabBarShown", true);
    m_tabBarShown = ! config->readNumEntry("TabWidgetVisibility", 0);
    m_closeOnHover = config->readBoolEntry("CloseOnHover", false);
    m_closeButtonShown = config->readBoolEntry("ShowCloseTabsButton", true);

    //we do not delay hover close buttons - that looks and feels ugly
    setHoverCloseButtonDelayed(false);
#else
    QSettings config;
    config.setPath("NewMDI", qApp->name(), QSettings::User );
    
    m_tabBarShown = !config.readNumEntry(SETTINGSPATH+"/UI/TabWidgetVisibility", 0);
    m_closeOnHover = config.readBoolEntry(SETTINGSPATH+"/UI/CloseOnHover", false);
    m_closeButtonShown = config.readBoolEntry(SETTINGSPATH+"/UI/ShowCloseTabsButton", true);
    
#endif
    
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
    if (m_closeButton && m_closeButtonShown)
    {
        m_closeButton->show();
    }
    KTWCLASS::insertTab(child, label, index);
    if (index != -1) tabBar()->repaint();
}

void DTabWidget::insertTab(QWidget *child, const QIconSet &iconset, 
    const QString &label, int index)
{
    if (m_closeButton && m_closeButtonShown)
    {
        m_closeButton->show();
    }
    KTWCLASS::insertTab(child, iconset, label, index);
    if (index != -1) tabBar()->repaint();
}

/*void DTabWidget::updateHistory(QWidget *w)
{
    if (m_history.top() != w)
        m_history.push(w);
}*/

#ifdef USE_KDE
#include "dtabwidget.moc"
#endif
