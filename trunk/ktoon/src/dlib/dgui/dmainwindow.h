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

#include "dgui/didockwidget.h"
#include <QList>
#include <QEvent>

class DiTabWidget;
namespace Ideal {
	class DiSplitter;
}

/**Main window which provides simplified IDEA mode.*/
class DMainWindow: public MWCLASS 
{
	Q_OBJECT
	public:
		DMainWindow(QWidget *parent = 0);
		virtual ~DMainWindow();
    
		/**@return The tool window in given @p position.*/
		DiDockWidget *toolWindow(DiDockWidget::Position position) const;
		
		/**Adds a tabbed widget into the active (focused) tab widget. 
		If @p widget is null then only tab is created.*/
		virtual void addWidget(QWidget *widget, const QString &title, bool persistant = false);
		virtual void addWidget(DiTabWidget *tab, QWidget *widget, const QString &title, bool persistant);
		/**Removes widget. Does not delete it.*/
		virtual void removeWidget(QWidget *widget);
		QWidget *findCorrectSeparator();
		void addDockWidget(Qt::DockWidgetArea area, DiDockWidget * dockwidget );
    
	public slots:
		DiTabWidget *splitHorizontal();
		DiTabWidget *splitVertical();
    
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
		virtual DiTabWidget *createTab();
    
	protected:
		DiDockWidget *m_pLeftDock;
		DiDockWidget *m_pRightDock;
		DiDockWidget *m_pBottomDock;

		Ideal::DiSplitter *m_pCentral;
		DiTabWidget *m_pActiveTabWidget;
    
		QList<DiTabWidget*> m_pTabs;
    
		bool m_pOpenTabAfterCurrent;
		bool m_pShowIconsOnTabs;
		bool m_pFirstRemoved;
    
		QList<QWidget*> m_pWidgets;
		QMap<QWidget*, DiTabWidget*> m_pWidgetTabs;
		QWidget *m_pCurrentWidget;
		QList<QWidget *> m_separators;

	private slots:
		void invalidateActiveTabWidget();
		
	private:
		QWidgetList m_persistantWidgets;
};

#endif

