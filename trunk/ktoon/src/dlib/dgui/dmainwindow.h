/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#ifndef DMAINWINDOW_H
#define DMAINWINDOW_H

#include <dideality.h>

// Project begin: Aug 4 2006

#include <QMainWindow>
#include <QHash>
#include <QMap>

class DButtonBar;
class DToolView;
class DMainWindowAbstractSettings;

/**
 * iDeality Main Window
 * @author David Cuadrado <krawek@gmail.com>
*/

class D_IDEAL_EXPORT DMainWindow : public QMainWindow
{
	Q_OBJECT;
	public:
		enum
		{
			None = 0,
			DefaultWorkspace
		};
		
		DMainWindow(QWidget *parent = 0);
		~DMainWindow();
		
		DToolView *addToolView(QWidget *widget, Qt::DockWidgetArea area, int workspace = DefaultWorkspace);
		void moveToolView(DToolView *view, Qt::DockWidgetArea newPlace);
		
		void addToWorkspace(QWidget *widget, int workspace = DefaultWorkspace);
		void removeFromWorkspace(QWidget *widget);
		
		void addToWorkspace(QAction *action, int workspace);
		void addToWorkspace(const QList<QAction *> &actions, int workspace);
		void removeFromWorkspace(QAction *action);
		
		void setCurrentWorkspace(int wsp);
		int currentWorkspace() const;
		
		void setAutoRestore(bool autoRestore);
		bool autoRestore() const;
		
		virtual QMenu *createPopupMenu();
		
		void setSettingsHandler(DMainWindowAbstractSettings *settings);
		void restoreGUI();
		void saveGUI();
		
		QHash<Qt::ToolBarArea, DButtonBar *> buttonBars() const;
		QHash<DButtonBar *, QList<DToolView*> > toolViews() const;
		
	private:
		Qt::DockWidgetArea toDockWidgetArea(Qt::ToolBarArea area);
		Qt::ToolBarArea toToolBarArea(Qt::DockWidgetArea area);
		
	public slots:
		void setEnableButtonBlending(bool enable);
		
	private slots:
		void relayoutViewButton(bool topLevel);
		void relayoutToolView();
		
	signals:
		void workspaceChanged(int wps);
		
	protected:
		void addButtonBar(Qt::ToolBarArea area);
		
	protected:
		virtual void closeEvent(QCloseEvent *e);
		virtual void showEvent(QShowEvent *e);
		virtual bool event(QEvent *e);
		
	private:
		DToolView *m_forRelayout;
		
	private:
		QHash<Qt::ToolBarArea, DButtonBar *> m_buttonBars;
		QHash<DButtonBar *, QList<DToolView*> > m_toolViews;
		QHash<QWidget *, int> m_managedWidgets;
		QHash<QAction *, int> m_managedActions;
		
		int m_currentWorkspace;
		
		DMainWindowAbstractSettings *m_settings;
		
		bool m_autoRestore;
};

#endif
