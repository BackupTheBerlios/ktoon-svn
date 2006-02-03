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

#ifndef TLAYERTABLE_H
#define TLAYERTABLE_H

#include <QTableView>
#include <QHash>

class TFramesTable;
class TFramesTableItem;
class TFramesTableItemDelegate;
class TFramesTableModel;

class TFramesTableItem
{
	friend class TFramesTableModel;
	friend class TFramesTable;
	
	public:
		enum Attributes
		{
			IsUsed = 0,
			IsLocked
		};
		
		TFramesTableItem();
		virtual ~TFramesTableItem();

		virtual TFramesTableItem *clone() const;

		inline TFramesTable *tableWidget() const { return m_view; }

		inline Qt::ItemFlags flags() const { return m_itemFlags; }
		inline void setFlags(Qt::ItemFlags flags);

		virtual QVariant data(int role) const;

		virtual void setData(int role, const QVariant &value);

		bool isUsed();
		bool isLocked();
	
	private:
		QHash<Attributes, bool> m_attributes;
		
		TFramesTable *m_view;
		TFramesTableModel *m_model;
		Qt::ItemFlags m_itemFlags;
		
		bool m_isUsed, m_isLocked;
		
};

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class TFramesTable : public QTableView
{
	Q_OBJECT
	public:
		TFramesTable(QWidget *parent = 0);
		~TFramesTable();
		
		struct LayerItem
		{
			LayerItem() : lastItem(-1) {};
			int lastItem;
		};
		
	public slots:
		// Layers
		void addLayer();
		void removeCurrentLayer();
		void removeLayer(int pos);
		
		int lastFrameByLayer(int layerPos);
		
		// Frames
		void addFrame(int layerPos);
		
		void setCurrentFrame(TFramesTableItem *);
		void setCurrentLayer(int layerPos);
		void selectFrame(int index);
		
		void setAttribute(int row, int col, TFramesTableItem::Attributes att, bool value);
		
	private:
		void setup();

	public:
		void setRowCount(int rows);
		int rowCount() const;

		void setColumnCount(int columns);
		int columnCount() const;

		int row(const TFramesTableItem *item) const;
		int column(const TFramesTableItem *item) const;

		TFramesTableItem *item(int row, int column) const;
		
		void setItem(int row, int column, TFramesTableItem *item);
		TFramesTableItem *takeItem(int row, int column);
		
		int currentRow() const;
		int currentColumn() const;
		TFramesTableItem *currentItem() const;
		void setCurrentItem(TFramesTableItem *item);

		bool isItemSelected(const TFramesTableItem *item) const;
		void setItemSelected(const TFramesTableItem *item, bool select);

		QList<TFramesTableItem*> selectedItems();
		QList<TFramesTableItem*> findItems(const QString &text, Qt::MatchFlags flags) const;
		
		TFramesTableItem *itemAt(const QPoint &p) const;
		inline TFramesTableItem *itemAt(int x, int y) const { return itemAt(QPoint(x, y)); };
		QRect visualItemRect(const TFramesTableItem *item) const;

		virtual int verticalOffset () const;
		virtual int horizontalOffset () const;
		
		void setItemSize(int w, int h);
		
	private slots:
		void emitItemPressed(const QModelIndex &index);
		void emitItemClicked(const QModelIndex &index);
		void emitItemDoubleClicked(const QModelIndex &index);
		void emitItemActivated(const QModelIndex &index);
		void emitItemEntered(const QModelIndex &index);
		void emitItemChanged(const QModelIndex &index);
		void emitCurrentItemChanged(const QModelIndex &previous, const QModelIndex &current);

	public slots:
		void scrollToItem(const TFramesTableItem *item, QAbstractItemView::ScrollHint hint = EnsureVisible);
		void insertRow(int row);
		void insertColumn(int column);
		void removeRow(int row);
		void removeColumn(int column);

		void clear();
		void selectCell(int row, int column);
		
	signals:
		void itemPressed(TFramesTableItem *item);
		void itemClicked(TFramesTableItem *item);
		void itemDoubleClicked(TFramesTableItem *item);

		void itemActivated(TFramesTableItem *item);
		void itemEntered(TFramesTableItem *item);
		void itemChanged(TFramesTableItem *item);

		void currentItemChanged(TFramesTableItem *current, TFramesTableItem *previous);
		void itemSelectionChanged();

	protected:
		QModelIndex indexFromItem(TFramesTableItem *item) const;
		TFramesTableItem *itemFromIndex(const QModelIndex &index) const;
		
		virtual QStyleOptionViewItem viewOptions() const;
		
	private:
		TFramesTableModel *m_model;
		int m_rectWidth, m_rectHeight;
		
		QList<LayerItem> m_layers;
};

#endif
