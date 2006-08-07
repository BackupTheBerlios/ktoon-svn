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

#include <QMainWindow>
#include <QHash>
#include <QMap>

class DButtonBar;
class DToolView;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

class Q_GUI_EXPORT DMainWindow : public QMainWindow
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
		
		virtual DToolView *addToolView(QWidget *view, Qt::ToolBarArea defaultPlace, int workspace = DefaultWorkspace);
		
		void setCurrentWorkspace(int wsp);
		int currentWorkspace() const;
		
	private:
		Qt::DockWidgetArea dockWidgetArea(Qt::ToolBarArea area);
		Qt::ToolBarArea toolBarArea(Qt::DockWidgetArea area);
		
	private slots:
		void relayoutViewButton(bool topLevel);
		void relayoutToolView();
		
	signals:
		void workspaceChanged(int wps);
		
	protected:
		void addButtonBar(Qt::ToolBarArea area);
		
	private:
		DToolView *m_forRelayout;
		
	private:
		QHash<Qt::ToolBarArea, DButtonBar *> m_buttonBars;
		QHash<DButtonBar *, QList<DToolView*> > m_toolViews;
		
		int m_currentWorkspace;
};

#endif
