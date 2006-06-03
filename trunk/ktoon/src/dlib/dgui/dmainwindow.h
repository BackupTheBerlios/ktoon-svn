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

#ifndef DMAINWINDOW_H
#define DMAINWINDOW_H

#include <QSettings>

#include <QMainWindow>
#define MWCLASS QMainWindow

#include "ddockwindow.h"
#include <QList>
#include <QEvent>

class DLSTabWidget;
namespace Ideal {
	class DockSplitter;
}

/**Main window which provides simplified IDEA mode.*/
class DMainWindow: public MWCLASS 
{
	Q_OBJECT
	public:
		DMainWindow(QWidget *parent = 0);
		virtual ~DMainWindow();
    
		/**@return The tool window in given @p position.*/
		DDockWindow *toolWindow(DDockWindow::Position position) const;
		
		/**Adds a tabbed widget into the active (focused) tab widget. 
		If @p widget is null then only tab is created.*/
		virtual void addWidget(QWidget *widget, const QString &title, bool persistant = false);
		virtual void addWidget(DLSTabWidget *tab, QWidget *widget, const QString &title, bool persistant);
		/**Removes widget. Does not delete it.*/
		virtual void removeWidget(QWidget *widget);
		QWidget *findCorrectSeparator();
		void addDockWidget(Qt::DockWidgetArea area, DDockWindow * dockwidget );
    
	public slots:
		DLSTabWidget *splitHorizontal();
		DLSTabWidget *splitVertical();
    
	protected slots:
		/**This does nothing. Reimplement in subclass to close the tab 
		when corner close button is pressed.*/
		virtual void closeTab();
		/**This does nothing. Reimplement in subclass to close the tab
		when hover close button is pressed.*/
		virtual void closeTab(QWidget*);
		/**This does nothing. Reimplement in subclass to show tab context menu.*/
		virtual void tabContext(QWidget*,const QPoint &);

	signals:
		void widgetChanged(QWidget *);
    
	protected:
		bool eventFilter(QObject *obj, QEvent *ev);
		virtual void loadSettings();
        
		virtual void createToolWindows();
		virtual DLSTabWidget *createTab();
    
	protected:
		DDockWindow *m_pLeftDock;
		DDockWindow *m_pRightDock;
		DDockWindow *m_pBottomDock;

		Ideal::DockSplitter *m_pCentral;
		DLSTabWidget *m_pActiveTabWidget;
    
		QList<DLSTabWidget*> m_pTabs;
    
		bool m_pOpenTabAfterCurrent;
		bool m_pShowIconsOnTabs;
		bool m_pFirstRemoved;
    
		QList<QWidget*> m_pWidgets;
		QMap<QWidget*, DLSTabWidget*> m_pWidgetTabs;
		QWidget *m_pCurrentWidget;
		QList<QWidget *> m_separators;

	private slots:
		void invalidateActiveTabWidget();
		
	private:
		QWidgetList m_persistantWidgets;
};

#endif

