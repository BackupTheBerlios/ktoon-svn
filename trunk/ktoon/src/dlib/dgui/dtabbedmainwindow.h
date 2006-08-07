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
#ifndef DTABBEDMAINWINDOW_H
#define DTABBEDMAINWINDOW_H

#include <dmainwindow.h>

class QTabWidget;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class Q_GUI_EXPORT DTabbedMainWindow : public DMainWindow
{
	Q_OBJECT;
	public:
		DTabbedMainWindow(QWidget *parent = 0);
		~DTabbedMainWindow();
		
		void addWidget(QWidget *widget, bool persistant = false);
		void removeWidget(QWidget *widget);
		void setTabWidget(QTabWidget *w);
		
	protected:
		virtual void setupTabWidget(QTabWidget *w);
		
	protected slots:
		void closeCurrentTab();
		
	signals:
		void widgetChanged(QWidget *widget);
		
	private slots:
		void emitWidgetChanged(int index);
		
	private:
		QTabWidget *m_tabWidget;
		QWidgetList m_persistantWidgets;
};

#endif
