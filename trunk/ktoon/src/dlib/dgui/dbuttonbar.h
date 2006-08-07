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

#ifndef DBUTTONBAR_H
#define DBUTTONBAR_H

#include <QToolBar>
#include <QButtonGroup>
#include <QMap>

class DViewButton;
class QAction;
class QMenu;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class Q_GUI_EXPORT DButtonBar : public QToolBar
{
	Q_OBJECT;
	public:
		DButtonBar(Qt::ToolBarArea area, QWidget *parent = 0);
		~DButtonBar();
		
		void addButton(DViewButton *viewButton);
		void removeButton(DViewButton *viewButton);
		
		bool isEmpty() const;
		
		void disable(DViewButton *v);
		void enable(DViewButton *v);
		
	public slots:
		void setExclusive(bool excl);
		void setShowOnlyIcons();
		void setShowOnlyTexts();
		
	private:
		void setupMenu();
		
	private slots:
		void hideOthers();
		
	protected:
		virtual void mousePressEvent(QMouseEvent *e);
		
	private:
		QButtonGroup m_buttons;
		QMap<QWidget *, QAction *> m_actionForWidget;
		
		QMenu *m_menu;
};

#endif
