/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado                                  *
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

#ifndef BASEMODULELISTWIDGET_H
#define BASEMODULELISTWIDGET_H

#include <modulewidgetbase.h>
#include "modulebuttonbar.h"

class QTreeWidget;
class QTreeWidgetItem;
class QMenu;


namespace Base {

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class ModuleListWidget : public ModuleWidget
{
	Q_OBJECT
	public:
		ModuleListWidget(ModuleButtonBar::Buttons buttons, QWidget *parent = 0);
		~ModuleListWidget();
		
		void setHeaders(const QStringList &headers);
		
		QTreeWidget *tree() const;
		
		void setFilled(bool f);
		bool filled() const;
		
		virtual QMenu *createMenu();
		
	private slots:
		void actionSelected(int action);
		void showMenu(const QPoint &pos);
		void selectFromAction();
		
	protected slots:
		virtual void addActionSelected(QTreeWidgetItem *current);
		virtual void delActionSelected(QTreeWidgetItem *current);
		virtual void queryActionSelected(QTreeWidgetItem *current);
		virtual void modifyActionSelected(QTreeWidgetItem *current);
		
	protected:
		virtual void updateList() = 0;
		
	protected:
		virtual void showEvent(QShowEvent *ev);
		
	private:
		struct Private;
		Private *const d;
};

}

#endif
