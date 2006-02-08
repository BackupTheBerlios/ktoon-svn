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

#ifndef KTCELLVIEW_H
#define KTCELLVIEW_H
/**
 * @file ktcellview.h
 * @brief Include this file if you need the class KTCellView, KTCellViewItem,  KTCellViewItemDelegate or KTCellViewModel
 */

#include <QTableView>
#include <QStyleOptionViewItem>
#include <QHash>

class KTCellView;
class KTCellViewItem;
class KTCellViewItemDelegate;
class KTCellViewModel;

typedef QHash<int, QVariant> ItemData;

/**
 * 
 * @short The KTCellViewItem class provides an item for use with the KTCellView class.
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTCellViewItem
{
	friend class KTCellViewModel;
	friend class KTCellView;
	
	public:
		/**
		 * construye un KTCellViewItem
		 */
		KTCellViewItem();
		virtual ~KTCellViewItem();

		virtual KTCellViewItem *clone() const;

		inline KTCellView *tableWidget() const { return m_view; }

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
		KTCellView *m_view;
		KTCellViewModel *m_model;
		Qt::ItemFlags m_itemFlags;
};

/**
 * @author David Cuadrado <krawek@toonka.com>
 */
class KTCellView : public QTableView
{
	Q_OBJECT
	public:
		KTCellView( QWidget *parent = 0);
		KTCellView(int rows, int columns, QWidget *parent = 0);
		~KTCellView();

		void setRowCount(int rows);
		int rowCount() const;

		void setColumnCount(int columns);
		int columnCount() const;

		int row(const KTCellViewItem *item) const;
		int column(const KTCellViewItem *item) const;

		KTCellViewItem *item(int row, int column) const;
		
		void setItem(int row, int column, KTCellViewItem *item);
		KTCellViewItem *takeItem(int row, int column);
		
		int currentRow() const;
		int currentColumn() const;
		KTCellViewItem *currentItem() const;
		void setCurrentItem(KTCellViewItem *item);

		bool isItemSelected(const KTCellViewItem *item) const;
		void setItemSelected(const KTCellViewItem *item, bool select);

		QList<KTCellViewItem*> selectedItems();
		QList<KTCellViewItem*> findItems(const QString &text, Qt::MatchFlags flags) const;
		
		KTCellViewItem *itemAt(const QPoint &p) const;
		inline KTCellViewItem *itemAt(int x, int y) const { return itemAt(QPoint(x, y)); };
		QRect visualItemRect(const KTCellViewItem *item) const;

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
		void scrollToItem(const KTCellViewItem *item, QAbstractItemView::ScrollHint hint = EnsureVisible);
		void insertRow(int row);
		void insertColumn(int column);
		void removeRow(int row);
		void removeColumn(int column);

		void clear();
		void selectCell(int row, int column);
		
	signals:
		void itemPressed(KTCellViewItem *item);
		void itemClicked(KTCellViewItem *item);
		void itemDoubleClicked(KTCellViewItem *item);

		void itemActivated(KTCellViewItem *item);
		void itemEntered(KTCellViewItem *item);
		void itemChanged(KTCellViewItem *item);

		void currentItemChanged(KTCellViewItem *current, KTCellViewItem *previous);
		void itemSelectionChanged();

	protected:
		QModelIndex indexFromItem(KTCellViewItem *item) const;
		KTCellViewItem *itemFromIndex(const QModelIndex &index) const;
		
		virtual QStyleOptionViewItem viewOptions() const;
		
	private:
		KTCellViewModel *m_model;
		int m_rectWidth, m_rectHeight;
};

#endif
