/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#ifndef DCELLVIEW_H
#define DCELLVIEW_H
/**
 * @file dcellview.h
 * @brief Include this file if you need the class DCellView, DCellViewItem,  DCellViewItemDelegate or DCellViewModel
 */

#include <QTableView>
#include <QStyleOptionViewItem>
#include <QHash>

class DCellView;
class DCellViewItem;
class DCellViewItemDelegate;
class DCellViewModel;

typedef QHash<int, QVariant> ItemData;

/**
 * 
 * @short The DCellViewItem class provides an item for use with the DCellView class.
 * @author David Cuadrado <krawek@toonka.com>
*/

class DCellViewItem
{
	friend class DCellViewModel;
	friend class DCellView;
	
	public:
		/**
		 * construye un DCellViewItem
		 */
		DCellViewItem();
		virtual ~DCellViewItem();

		virtual DCellViewItem *clone() const;

		inline DCellView *tableWidget() const { return m_view; }

		inline Qt::ItemFlags flags() const { return m_itemFlags; }
		inline void setFlags(Qt::ItemFlags flags);

		virtual QVariant data(int role) const;
		
		void setImage(const QImage &);
		QImage image() const;
		
		void setBackground(const QBrush &);
		QBrush background() const;
		
		virtual void setData(int role, const QVariant &value);
	
	private:
		ItemData m_values;
		DCellView *m_view;
		DCellViewModel *m_model;
		Qt::ItemFlags m_itemFlags;
};

/**
 * @author David Cuadrado <krawek@toonka.com>
 */
class DCellView : public QTableView
{
	Q_OBJECT
	public:
		DCellView( QWidget *parent = 0);
		DCellView(int rows, int columns, QWidget *parent = 0);
		~DCellView();

		void setRowCount(int rows);
		int rowCount() const;

		void setColumnCount(int columns);
		int columnCount() const;

		int row(const DCellViewItem *item) const;
		int column(const DCellViewItem *item) const;

		DCellViewItem *item(int row, int column) const;
		
		void setItem(int row, int column, DCellViewItem *item);
		DCellViewItem *takeItem(int row, int column);
		
		int currentRow() const;
		int currentColumn() const;
		DCellViewItem *currentItem() const;
		void setCurrentItem(DCellViewItem *item);

		bool isItemSelected(const DCellViewItem *item) const;
		void setItemSelected(const DCellViewItem *item, bool select);

		QList<DCellViewItem*> selectedItems();
		QList<DCellViewItem*> findItems(const QString &text, Qt::MatchFlags flags) const;
		
		DCellViewItem *itemAt(const QPoint &p) const;
		inline DCellViewItem *itemAt(int x, int y) const { return itemAt(QPoint(x, y)); };
		QRect visualItemRect(const DCellViewItem *item) const;

		virtual int verticalOffset () const;
		virtual int horizontalOffset () const;
		
		void setItemSize(int w, int h);
		
	private:
		void setup();
		
	private slots:
		void emitItemPressed(const QModelIndex &index);
		void emitItemClicked(const QModelIndex &index);
		void emitItemDoubleClicked(const QModelIndex &index);
		void emitItemActivated(const QModelIndex &index);
		void emitItemEntered(const QModelIndex &index);
		void emitItemChanged(const QModelIndex &index);
		void emitCurrentItemChanged(const QModelIndex &previous, const QModelIndex &current);

	public slots:
		void scrollToItem(const DCellViewItem *item, QAbstractItemView::ScrollHint hint = EnsureVisible);
		void insertRow(int row);
		void insertColumn(int column);
		void removeRow(int row);
		void removeColumn(int column);

		void clear();
		void selectCell(int row, int column);
		
	signals:
		void itemPressed(DCellViewItem *item);
		void itemClicked(DCellViewItem *item);
		void itemDoubleClicked(DCellViewItem *item);

		void itemActivated(DCellViewItem *item);
		void itemEntered(DCellViewItem *item);
		void itemChanged(DCellViewItem *item);

		void currentItemChanged(DCellViewItem *current, DCellViewItem *previous);
		void itemSelectionChanged();

	protected:
		QModelIndex indexFromItem(DCellViewItem *item) const;
		DCellViewItem *itemFromIndex(const QModelIndex &index) const;
		
		virtual QStyleOptionViewItem viewOptions() const;
		
	private:
		DCellViewModel *m_model;
		int m_rectWidth, m_rectHeight;
};

#endif
