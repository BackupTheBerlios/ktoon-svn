/***************************************************************************
 *   Project KOM: KToon Open Media library                                 *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *                                                                         *
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef KTABBEDMAINWINDOW_H
#define KTABBEDMAINWINDOW_H

#include <kgui/kmainwindow.h>

class QTabWidget;

/**
 * A tabbed main window.
 * @author David Cuadrado <krawek@gmail.com>
*/
class K_IDEAL_EXPORT KTabbedMainWindow : public KMainWindow
{
    Q_OBJECT;
    public:

        enum Perspective {
             Drawing = 0x01,
             Animation = 0x02,
             Help = 0x04,
             News = 0x08,
             All = Drawing | Animation | Help | News
        };

        KTabbedMainWindow(QWidget *parent = 0);
        ~KTabbedMainWindow();

        void addWidget(QWidget *widget, bool persistant = true, int perspective = All);
        void removeWidget(QWidget *widget, bool force = false);
        void setTabWidget(QTabWidget *w);
        QTabWidget *tabWidget() const;
        void setCurrentTab(int index);

    protected:
        virtual void setupTabWidget(QTabWidget *w);

    protected slots:
        void closeCurrentTab();
        //virtual void setupPerspective(int wps);

    signals:
        void widgetChanged(QWidget *widget);
        void tabHasChanged(int);
		
    private slots:
        void emitWidgetChanged(int index);

    private:
        QTabWidget *m_tabWidget;
        QWidgetList m_persistantWidgets;
        QHash<QWidget *, int> m_tabs;
        QWidgetList m_pages;
};

#endif
