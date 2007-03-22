/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#ifndef DTREELISTWIDGET_H
#define DTREELISTWIDGET_H

#include <QTreeWidget>
#include <dcore/dglobal.h>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class D_GUI_EXPORT DTreeListWidget : public QTreeWidget
{
	Q_OBJECT
	public:
		DTreeListWidget(QWidget *parent = 0);
		~DTreeListWidget();
		void addItems(const QStringList &items);
		QList<QTreeWidgetItem *> topLevelItems();
		
		void setEditable(bool isEditable);
		bool isEditable() const;
		
	public slots:
		void removeAll();
		
	private slots:
		void editDoubleClickedItem(QTreeWidgetItem *item, int col);
		
	protected slots:
		virtual void closeEditor ( QWidget * editor, QAbstractItemDelegate::EndEditHint hint );
		
	signals:
		void itemRenamed(QTreeWidgetItem *item);
		
	private:
		bool m_isEditable;
};

#endif
