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

#ifndef KTIMAGESTABLE_H
#define KTIMAGESTABLE_H

#include <QTableView>
#include <QStyleOptionViewItem>

class KTImagesTable;
class KTImagesTableItem;
class KTImagesTableModel;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTImagesTableItem
{
	friend class KTImagesTableModel;
	friend class KTImagesTable;
	
	public:
		KTImagesTableItem();
		virtual ~KTImagesTableItem();

		virtual KTImagesTableItem *clone() const;

		inline KTImagesTable *tableWidget() const { return m_view; }

		inline Qt::ItemFlags flags() const { return m_itemFlags; }
		inline void setFlags(Qt::ItemFlags flags);

		virtual QVariant data(int role) const;
		
		void setImage(const QImage &);
		QImage image() const;
		
		virtual void setData(int role, const QVariant &value);
	
	private:
		QImage m_value;
		KTImagesTable *m_view;
		KTImagesTableModel *m_model;
		Qt::ItemFlags m_itemFlags;
};

class KTImagesTable : public QAbstractItemView
{
	Q_OBJECT
	public:
		KTImagesTable( QWidget *parent = 0);
		KTImagesTable(int rows, int columns, QWidget *parent = 0);
		~KTImagesTable();

		void setRowCount(int rows);
		int rowCount() const;

		void setColumnCount(int columns);
		int columnCount() const;

		int row(const KTImagesTableItem *item) const;
		int column(const KTImagesTableItem *item) const;

		KTImagesTableItem *item(int row, int column) const;
		void setItem(int row, int column, KTImagesTableItem *item);
		KTImagesTableItem *takeItem(int row, int column);

		KTImagesTableItem *verticalHeaderItem(int row) const;
		void setVerticalHeaderItem(int row, KTImagesTableItem *item);

		KTImagesTableItem *horizontalHeaderItem(int column) const;
		void setHorizontalHeaderItem(int column, KTImagesTableItem *item);
		void setVerticalHeaderLabels(const QStringList &labels);
		void setHorizontalHeaderLabels(const QStringList &labels);

		int currentRow() const;
		int currentColumn() const;
		KTImagesTableItem *currentItem() const;
		void setCurrentItem(KTImagesTableItem *item);

		void sortItems(int column, Qt::SortOrder order = Qt::AscendingOrder);
		void setSortingEnabled(bool enable);
		bool isSortingEnabled() const;

		bool isItemSelected(const KTImagesTableItem *item) const;
		void setItemSelected(const KTImagesTableItem *item, bool select);

		QList<KTImagesTableItem*> selectedItems();
		QList<KTImagesTableItem*> findItems(const QString &text, Qt::MatchFlags flags) const;
		
		KTImagesTableItem *itemAt(const QPoint &p) const;
		inline KTImagesTableItem *itemAt(int x, int y) const { return itemAt(QPoint(x, y)); };
		QRect visualItemRect(const KTImagesTableItem *item) const;

		const KTImagesTableItem *itemPrototype() const;
		
		virtual void scrollTo ( const QModelIndex & index, ScrollHint hint = EnsureVisible );
		virtual QRect visualRect ( const QModelIndex & index ) const;
		
		virtual bool isIndexHidden ( const QModelIndex & index ) const;
		virtual int verticalOffset () const;
		virtual int horizontalOffset () const;
		virtual QModelIndex indexAt ( const QPoint & p ) const;
		
		virtual QModelIndex moveCursor ( CursorAction cursorAction, Qt::KeyboardModifiers modifiers );
		virtual QRegion visualRegionForSelection ( const QItemSelection & selection ) const;
		virtual void setSelection ( const QRect & rect, QItemSelectionModel::SelectionFlags flags );
		
		
		
	private:
		void setup();

	public slots:
		void scrollToItem(const KTImagesTableItem *item, QAbstractItemView::ScrollHint hint = EnsureVisible);
		void insertRow(int row);
		void insertColumn(int column);
		void removeRow(int row);
		void removeColumn(int column);

		void clear();
		void selectCell(int row, int column);

	signals:
		void itemPressed(KTImagesTableItem *item);
		void itemClicked(KTImagesTableItem *item);
		void itemDoubleClicked(KTImagesTableItem *item);

		void itemActivated(KTImagesTableItem *item);
		void itemEntered(KTImagesTableItem *item);
		void itemChanged(KTImagesTableItem *item);

		void currentItemChanged(KTImagesTableItem *current, KTImagesTableItem *previous);
		void itemSelectionChanged();

	protected:
		QModelIndex indexFromItem(KTImagesTableItem *item) const;
		KTImagesTableItem *itemFromIndex(const QModelIndex &index) const;
		
		virtual void paintEvent(QPaintEvent *e);
		
		QStyleOptionViewItem viewOptions() const;
		
	private:
		void setModel(QAbstractItemModel *model);
		
		KTImagesTableModel *m_model;
};

#endif
