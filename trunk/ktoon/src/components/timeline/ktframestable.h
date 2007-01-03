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

#ifndef KTLAYERTABLE_H
#define KTLAYERTABLE_H

#include <QTableView>
#include <QHash>

class KTFramesTable;
class KTFramesTableItem;
class KTFramesTableItemDelegate;
class KTFramesTableModel;

class KTFramesTableItem
{
	friend class KTFramesTableModel;
	friend class KTFramesTable;
	
	public:
		enum Attributes
		{
			IsUsed = 0,
			IsLocked
		};
		
		KTFramesTableItem();
		virtual ~KTFramesTableItem();

		virtual KTFramesTableItem *clone() const;

		inline KTFramesTable *tableWidget() const { return m_view; }

		inline Qt::ItemFlags flags() const { return m_itemFlags; }
		inline void setFlags(Qt::ItemFlags flags);

		virtual QVariant data(int role) const;

		virtual void setData(int role, const QVariant &value);

		bool isUsed();
		bool isLocked();
	
	private:
		QHash<Attributes, bool> m_attributes;
		
		KTFramesTable *m_view;
		KTFramesTableModel *m_model;
		Qt::ItemFlags m_itemFlags;
		
		bool m_isUsed, m_isLocked;
		
};

class KTTLRuler;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTFramesTable : public QTableView
{
	Q_OBJECT
	public:
		KTFramesTable(QWidget *parent = 0);
		~KTFramesTable();
		
		struct LayerItem
		{
			LayerItem() : lastItem(-1) {};
			int lastItem;
		};
		
	public slots:
		// Layers
		void insertLayer(int layerPos, const QString &name);
		void removeCurrentLayer();
		void removeLayer(int pos);
		void moveLayer(int pos, int newPos);
		
		int lastFrameByLayer(int layerPos);
		
		// Frames
		void insertFrame(int layerPos, const QString &name);
		
		void setCurrentFrame(KTFramesTableItem *);
		void setCurrentLayer(int layerPos);
		void selectFrame(int index);
		
		void setAttribute(int row, int col, KTFramesTableItem::Attributes att, bool value);
		
		void removeFrame(int layerPos, int position);
		
		void lockFrame(int layerPosition, int position, bool lock);
		
	private:
		void setup();

	public:
		void setRowCount(int rows);
		int rowCount() const;

		void setColumnCount(int columns);
		int columnCount() const;

		int row(const KTFramesTableItem *item) const;
		int column(const KTFramesTableItem *item) const;

		KTFramesTableItem *item(int row, int column) const;
		
		void setItem(int row, int column, KTFramesTableItem *item);
		KTFramesTableItem *takeItem(int row, int column);
		
		int currentRow() const;
		int currentColumn() const;
		KTFramesTableItem *currentItem() const;
		void setCurrentItem(KTFramesTableItem *item);

		bool isItemSelected(const KTFramesTableItem *item) const;
		void setItemSelected(const KTFramesTableItem *item, bool select);

		QList<KTFramesTableItem*> selectedItems();
		QList<KTFramesTableItem*> findItems(const QString &text, Qt::MatchFlags flags) const;
		
		KTFramesTableItem *itemAt(const QPoint &p) const;
		inline KTFramesTableItem *itemAt(int x, int y) const { return itemAt(QPoint(x, y)); };
		QRect visualItemRect(const KTFramesTableItem *item) const;

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
		
// 		void fixSectionMoved(int logical, int visual, int newVisual);

	public slots:
		void scrollToItem(const KTFramesTableItem *item, QAbstractItemView::ScrollHint hint = EnsureVisible);
		void insertRow(int row);
		void insertColumn(int column);
		void removeRow(int row);
		void removeColumn(int column);

		void clear();
		void selectCell(int row, int column);
		void selectColumn(int logicalIndex);
		
	signals:
		void itemPressed(KTFramesTableItem *item);
		void itemClicked(KTFramesTableItem *item);
		void itemDoubleClicked(KTFramesTableItem *item);

		void itemActivated(KTFramesTableItem *item);
		void itemEntered(KTFramesTableItem *item);
		void itemChanged(KTFramesTableItem *item);

		void currentItemChanged(KTFramesTableItem *current, KTFramesTableItem *previous);
		void itemSelectionChanged();
		
		void frameRequest(int action, int frame, int layer, int scene, const QVariant &argument = QVariant());
		

	protected:
		QModelIndex indexFromItem(KTFramesTableItem *item) const;
		KTFramesTableItem *itemFromIndex(const QModelIndex &index) const;
		
		virtual QStyleOptionViewItem viewOptions() const;
		
		void keyPressEvent ( QKeyEvent * event );
		
		void fixSize();
		
	private:
		KTFramesTableModel *m_model;
		int m_rectWidth, m_rectHeight;
		
		QList<LayerItem> m_layers;
		
		KTTLRuler *m_ruler;
};

#endif
