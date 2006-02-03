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

#include "tframestable.h"

#include <QPainter>
#include <QPaintEvent>
#include <QItemSelectionModel>
#include <QPainterPath>
#include <QScrollBar>
#include <QHeaderView>

#include "ktdebug.h"

//////////// TFramesTableModel

class TFramesTableModel : public QAbstractTableModel
{
	public:
		TFramesTableModel(int rows, int columns, TFramesTable *parent);
		~TFramesTableModel();
		
		inline TFramesTableItem *createItem() const { return new TFramesTableItem(); }
		
		
		bool insertRows(int row, int count = 1, const QModelIndex &parent = QModelIndex());
		bool insertColumns(int column, int count = 1, const QModelIndex &parent = QModelIndex());

		bool removeRows(int row, int count = 1, const QModelIndex &parent = QModelIndex());
		bool removeColumns(int column, int count = 1, const QModelIndex &parent = QModelIndex());

		void setItem(int row, int column, TFramesTableItem *item);
		TFramesTableItem *takeItem(int row, int column);
		TFramesTableItem *item(int row, int column) const;
		TFramesTableItem *item(const QModelIndex &index) const;
		void removeItem(TFramesTableItem *item);

		void setHorizontalHeaderItem(int section, TFramesTableItem *item);
		void setVerticalHeaderItem(int section, TFramesTableItem *item);
		TFramesTableItem *horizontalHeaderItem(int section);
		TFramesTableItem *verticalHeaderItem(int section);

		QModelIndex index(const TFramesTableItem *item) const;
		QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

		void setRowCount(int rows);
		void setColumnCount(int columns);

		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;

		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		bool setData(const QModelIndex &index, const QVariant &value, int role);

		Qt::ItemFlags flags(const QModelIndex &index) const;

		bool isValid(const QModelIndex &index) const;
		inline long tableIndex(int row, int column) const
		{ return (row * m_horizontal.count()) + column; }

		void clear();
		void itemChanged(TFramesTableItem *item);
		
		virtual QModelIndex parent ( const QModelIndex & ) const
		{
			return QModelIndex();
		}
		

	private:
		QVector<TFramesTableItem*> m_table;
		QVector<TFramesTableItem*> m_vertical;
		QVector<TFramesTableItem*> m_horizontal;
};

TFramesTableModel::TFramesTableModel(int rows, int columns, TFramesTable *parent)
	: QAbstractTableModel(parent), m_table(rows * columns), m_vertical(rows), m_horizontal(columns)
{
	
}

TFramesTableModel::~TFramesTableModel()
{
	clear();
}

bool TFramesTableModel::insertRows(int row, int count, const QModelIndex &)
{
	if (row < 0)
		row = 0;
	else if (row > m_vertical.count())
		row = m_vertical.count();
	
	beginInsertRows(QModelIndex(), row, row + count - 1);
	
	int rc = m_vertical.count();
	int cc = m_horizontal.count();
	m_vertical.insert(row, count, 0);
	if (rc == 0)
		m_table.resize(cc * count);
	else
		m_table.insert(tableIndex(row, 0), cc * count, 0);
	endInsertRows();
	return true;
}

bool TFramesTableModel::insertColumns(int column, int count, const QModelIndex &)
{
	if (column < 0)
		column = 0;
	else if (column > m_horizontal.count())
		column = m_horizontal.count();
	beginInsertColumns(QModelIndex(), column, column + count - 1);
	int rc = m_vertical.count();
	int cc = m_horizontal.count();
	m_horizontal.insert(column, count, 0);
	if (cc == 0)
		m_table.resize(rc * count);
	else
		for (int row = 0; row < rc; ++row)
			m_table.insert(tableIndex(row, column), count, 0);
	endInsertColumns();
	return true;
}

bool TFramesTableModel::removeRows(int row, int count, const QModelIndex &)
{
	if (row >= 0 && row < m_vertical.count()) 
	{
		beginRemoveRows(QModelIndex(), row, row + count - 1);
		int i = tableIndex(row, 0);
		int n = count * columnCount();
		TFramesTableItem *oldItem = 0;
		for (int j=i; j<n+i; ++j) 
		{
			oldItem = m_table.at(j);
			if (oldItem)
				oldItem->m_model = 0;
			delete oldItem;
		}
		m_table.remove(qMax(i, 0), n);
		for (int v=row; v<row+count; ++v) 
		{
			oldItem = m_vertical.at(v);
			if (oldItem)
				oldItem->m_model = 0;
			delete oldItem;
		}
		m_vertical.remove(row, count);
		endRemoveRows();
		return true;
	}
	return false;
}

bool TFramesTableModel::removeColumns(int column, int count, const QModelIndex &)
{
	if (column >= 0 && column < m_horizontal.count()) 
	{
		beginRemoveColumns(QModelIndex(), column, column + count - 1);
		TFramesTableItem *oldItem = 0;
		for (int row = rowCount() - 1; row >= 0; --row) 
		{
			int i = tableIndex(row, column);
			for (int j=i; j<i+count; ++j) 
			{
				oldItem = m_table.at(j);
				if (oldItem)
					oldItem->m_model = 0;
				delete oldItem;
			}
			m_table.remove(i, count);
		}
		for (int h=column; h<column+count; ++h) 
		{
			oldItem = m_horizontal.at(h);
			if (oldItem)
				oldItem->m_model = 0;
			delete oldItem;
		}
		m_horizontal.remove(column, count);
		endRemoveColumns();
		return true;
	}
	return false;
}

void TFramesTableModel::setItem(int row, int column, TFramesTableItem *item)
{
	int i = tableIndex(row, column);
	if (i < 0 || i >= m_table.count())
		return;
	TFramesTableItem *oldItem = m_table.at(i);
	if (item == oldItem)
		return;

	// remove old
	if (oldItem)
		oldItem->m_model = 0;
	delete m_table.at(i);

	// set new
	if (item)
		item->m_model = this;
	m_table[i] = item;
	QModelIndex idx = index(row, column);
	emit dataChanged(idx, idx);
}

TFramesTableItem *TFramesTableModel::takeItem(int row, int column)
{
	long i = tableIndex(row, column);
	TFramesTableItem *itm = m_table.value(i);
	if (itm) {
		itm->m_model = 0;
		m_table[i] = 0;
	}
	return itm;
}

TFramesTableItem *TFramesTableModel::item(int row, int column) const
{
	return m_table.value(tableIndex(row, column));
}

TFramesTableItem *TFramesTableModel::item(const QModelIndex &index) const
{
	if (!isValid(index))
		return 0;
	return m_table.at(tableIndex(index.row(), index.column()));
}

void TFramesTableModel::removeItem(TFramesTableItem *item)
{
	int i = m_table.indexOf(item);
	if (i != -1) {
		m_table[i] = 0;
		QModelIndex idx = index(item);
		emit dataChanged(idx, idx);
		return;
	}

	i = m_vertical.indexOf(item);

	if (i != -1) {
		m_vertical[i] = 0;
		emit headerDataChanged(Qt::Vertical, i, i);
		return;
	}
	i = m_horizontal.indexOf(item);
	if (i != -1) {
		m_horizontal[i] = 0;
		emit headerDataChanged(Qt::Horizontal, i, i);
		return;
	}
}

void TFramesTableModel::setHorizontalHeaderItem(int section, TFramesTableItem *item)
{
	if (section < 0 || section >= m_horizontal.count())
		return;
	TFramesTableItem *oldItem = m_horizontal.at(section);
	if (item == oldItem)
		return;

	if (oldItem)
		oldItem->m_model = 0;
	delete oldItem;

	if (item)
		item->m_model = this;
	m_horizontal[section] = item;
	emit headerDataChanged(Qt::Horizontal, section, section);
}

void TFramesTableModel::setVerticalHeaderItem(int section, TFramesTableItem *item)
{
	if (section < 0 || section >= m_vertical.count())
		return;
	TFramesTableItem *oldItem = m_vertical.at(section);
	if (item == oldItem)
		return;

	if (oldItem)
		oldItem->m_model = 0;
	delete oldItem;

	if (item)
		item->m_model = this;
	m_vertical[section] = item;
	emit headerDataChanged(Qt::Vertical, section, section);
}

TFramesTableItem *TFramesTableModel::horizontalHeaderItem(int section)
{
	return m_horizontal.value(section);
}

TFramesTableItem *TFramesTableModel::verticalHeaderItem(int section)
{
	return m_vertical.value(section);
}

QModelIndex TFramesTableModel::index(const TFramesTableItem *item) const
{
	int i = m_table.indexOf(const_cast<TFramesTableItem*>(item));
	int row = i / columnCount();
	int col = i % columnCount();
	return index(row, col);
}

QModelIndex TFramesTableModel::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
	{
		TFramesTableItem *item = m_table.at(tableIndex(row, column));
		return createIndex(row, column, item);
	}
	return QModelIndex();
}

void TFramesTableModel::setRowCount(int rows)
{
	int rc = m_vertical.count();
	if (rc == rows)
		return;
	if (rc < rows)
		insertRows(qMax(rc, 0), rows - rc);
	else
		removeRows(qMax(rows, 0), rc - rows);
}

void TFramesTableModel::setColumnCount(int columns)
{
	int cc = m_horizontal.count();
	if (cc == columns)
		return;
	if (cc < columns)
		insertColumns(qMax(cc, 0), columns - cc);
	else
		removeColumns(qMax(columns, 0), cc - columns);
}

int TFramesTableModel::rowCount(const QModelIndex &) const
{
	return m_vertical.count();
}

int TFramesTableModel::columnCount(const QModelIndex &) const
{
	return m_horizontal.count();
}

QVariant TFramesTableModel::data(const QModelIndex &index, int role) const
{
	TFramesTableItem *itm = item(index);
	if (itm)
	{
		return itm->data(role);
	}
	return QVariant();
}

bool TFramesTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	TFramesTableItem *itm = item(index);

	if (itm)
	{
		itm->setData(role, value);
		return true;
	}

	TFramesTable *view = qobject_cast<TFramesTable*>(QObject::parent());
	if (!view)
		return false;

	itm = createItem();
	itm->setData(role, value);
	view->setItem(index.row(), index.column(), itm);
	return true;
}

Qt::ItemFlags TFramesTableModel::flags(const QModelIndex &index) const
{
	TFramesTableItem *itm = item(index);
	if (itm)
		return itm->flags();
	return Qt::ItemIsEditable
			|Qt::ItemIsSelectable
			|Qt::ItemIsUserCheckable
			|Qt::ItemIsEnabled;
}

bool TFramesTableModel::isValid(const QModelIndex &index) const
{
	return index.isValid() && index.row() < m_vertical.count() && index.column() < m_horizontal.count();
}

void TFramesTableModel::clear()
{
	for (int i = 0; i < m_table.count(); ++i) 
	{
		if (m_table.at(i)) 
		{
			m_table.at(i)->m_model = 0;
			delete m_table.at(i);
			m_table[i] = 0;
		}
	}
	for (int j = 0; j < m_vertical.count(); ++j) 
	{
		if (m_vertical.at(j)) 
		{
			m_vertical.at(j)->m_model = 0;
			delete m_vertical.at(j);
			m_vertical[j] = 0;
		}
	}
	for (int k = 0; k < m_horizontal.count(); ++k) 
	{
		if (m_horizontal.at(k)) 
		{
			m_horizontal.at(k)->m_model = 0;
			delete m_horizontal.at(k);
			m_horizontal[k] = 0;
		}
	}
	reset();
}

void TFramesTableModel::itemChanged(TFramesTableItem *item)
{
	QModelIndex idx = index(item);
	emit dataChanged(idx, idx);
}

////////// TFramesTableItemDelegate ///////////

class TFramesTableItemDelegate : public QAbstractItemDelegate
{
	public:
		TFramesTableItemDelegate(QObject * parent = 0 );
		~TFramesTableItemDelegate();
		virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
		virtual QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

TFramesTableItemDelegate::TFramesTableItemDelegate(QObject * parent) :  QAbstractItemDelegate(parent)
{
}

TFramesTableItemDelegate::~TFramesTableItemDelegate()
{
}

void TFramesTableItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	Q_ASSERT(index.isValid());
	const TFramesTableModel *model = reinterpret_cast<const TFramesTableModel*>( index.model() );
	Q_ASSERT(model);
	
	QVariant value;

	QStyleOptionViewItem opt = option;
	
	// draw the background color
	value = model->data(index, Qt::BackgroundColorRole);
	if (value.isValid())
	{
		painter->save();
		painter->fillRect(option.rect, value.value<QColor>() );
		painter->restore();
	}
	else
	{
		painter->save();
		
		if ( index.column() % 5 == 0 )
		{
			painter->fillRect(option.rect, Qt::lightGray );
		}
		else
		{
			painter->fillRect(option.rect, Qt::white );
		}
		painter->restore();
	}
	
		// Selection!
	if (option.showDecorationSelected && (option.state & QStyle::State_Selected))
	{
		QPalette::ColorGroup cg = option.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
		
		painter->save();
		painter->setPen(QPen(option.palette.brush(cg, QPalette::Highlight), 3));
		painter->drawRect(option.rect.adjusted(1,1,-2,-2));
		painter->restore();
	}
	
	// Draw attributes
	
	TFramesTableItem *item = model->item(index);
	
	int offset = option.rect.width() - 2 ;
	

// 	painter->drawEllipse( option.rect.left(), option.rect.bottom() - offset, offset, offset );
	
	if ( item )
	{
		if(item->isUsed() )
		{
			painter->save();
			painter->setBrush(Qt::black);
			painter->drawEllipse( option.rect.left(), option.rect.bottom() - offset, offset, offset);
			painter->restore();
		}
	}
	
// 	painter->drawRect( option.rect.left(), option.rect.bottom() - offset, offset, offset );
	
// 	painter->fillRect( option.rect.left(), option.rect.bottom() - offset, offset, offset, Qt::black );
}

QSize TFramesTableItemDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	Q_ASSERT(index.isValid());
	const QAbstractItemModel *model = index.model();
	Q_ASSERT(model);

	QVariant value = model->data(index, Qt::FontRole);
	QFont fnt = value.isValid() ? qvariant_cast<QFont>(value) : option.font;
	QString text = model->data(index, Qt::DisplayRole).toString();
	QRect pixmapRect;
	if (model->data(index, Qt::DecorationRole).isValid())
		pixmapRect = QRect(0, 0, option.decorationSize.width(),
				   option.decorationSize.height());

	QFontMetrics fontMetrics(fnt);
	
	return (pixmapRect).size();
}


////////// TFramesTableItem ////////
TFramesTableItem::TFramesTableItem()
	:  m_view(0), m_model(0), m_itemFlags(Qt::ItemIsEditable
		|Qt::ItemIsSelectable
		|Qt::ItemIsUserCheckable
		|Qt::ItemIsEnabled
		|Qt::ItemIsDragEnabled
		|Qt::ItemIsDropEnabled), m_isUsed(false), m_isLocked(false)
{
}

TFramesTableItem::~TFramesTableItem()
{
	if (m_model)
	{
		m_model->removeItem(this);
	}
}


TFramesTableItem *TFramesTableItem::clone() const
{
	TFramesTableItem *item = new TFramesTableItem();
	*item = *this;
	return item;
}


void TFramesTableItem::setData(int r, const QVariant &value)
{
	switch(r)
	{
		case IsUsed:
		{
			m_attributes.insert(IsUsed, value.toBool());
		}
		break;
		case IsLocked:
		{
			m_attributes.insert(IsLocked, value.toBool());
		}
		break;
	}
}

QVariant TFramesTableItem::data(int role) const
{
	return QVariant();
}

bool TFramesTableItem::isUsed()
{
	return m_attributes[IsUsed];
}

bool TFramesTableItem::isLocked()
{
	return m_attributes[IsLocked];
}

//// TFramesTable

TFramesTable::TFramesTable(QWidget *parent) : QTableView(parent)
{
	m_model = new TFramesTableModel(0,100,this);
	setModel( m_model );
	
	setup();
}

TFramesTable::~TFramesTable()
{
}

void TFramesTable::setup()
{
	setItemDelegate( new TFramesTableItemDelegate(this));
	setSelectionModel(new QItemSelectionModel(m_model));
	
	connect(this, SIGNAL(pressed(QModelIndex)), this, SLOT(emitItemPressed(QModelIndex)));
	
	connect(this, SIGNAL(clicked(QModelIndex)), this,SLOT(emitItemClicked(QModelIndex)));
	
	connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(emitItemDoubleClicked(QModelIndex)));
	
	connect(this, SIGNAL(activated(QModelIndex)), this,SLOT(emitItemActivated(QModelIndex)));
	
	connect(this, SIGNAL(entered(QModelIndex)), this,SLOT(emitItemEntered(QModelIndex)));
   
	connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),this, SLOT(emitItemChanged(QModelIndex)));
	
	connect(this->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),this, SLOT(emitCurrentItemChanged(QModelIndex,QModelIndex)));
	
	connect(this->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this, SIGNAL(itemSelectionChanged()));
	
	setSelectionBehavior(QAbstractItemView::SelectItems);
	setSelectionMode (QAbstractItemView::SingleSelection);
	
	verticalHeader()->hide();
	horizontalHeader()->hide();
	
	setItemSize( 10, 25 );
	
	horizontalHeader()->setResizeMode(QHeaderView::Custom);
	verticalHeader()->setResizeMode(QHeaderView::Custom);
	
#if QT_VERSION >= 0x040100
	setAutoFillBackground(true);
#endif
}

void TFramesTable::setItemSize(int w, int h)
{
	m_rectHeight = h;
	m_rectWidth = w;
	
	for(int column = 0; column < columnCount(); column++)
	{
		horizontalHeader()->resizeSection(column, m_rectWidth);
	}
	for( int row = 0; row < rowCount(); row++)
	{
		verticalHeader()->resizeSection(row, m_rectHeight);
	}
}

void TFramesTable::emitItemPressed(const QModelIndex &index)
{
	emit itemPressed( m_model->item(index) );
}

void TFramesTable::emitItemClicked(const QModelIndex &index)
{
	emit itemClicked(m_model->item(index));
}

void TFramesTable::emitItemDoubleClicked(const QModelIndex &index)
{
	emit itemDoubleClicked(m_model->item(index));
}

void TFramesTable::emitItemActivated(const QModelIndex &index)
{
	emit itemActivated(m_model->item(index));
}

void TFramesTable::emitItemEntered(const QModelIndex &index)
{
	emit itemEntered(m_model->item(index));
}

void TFramesTable::emitItemChanged(const QModelIndex &index)
{
	emit itemChanged(m_model->item(index));
}

void TFramesTable::emitCurrentItemChanged(const QModelIndex &previous, const QModelIndex &current)
{
	emit currentItemChanged(m_model->item(current), m_model->item(previous));
}

// void TFramesTable::emitItemPressed()
// {
// }

void TFramesTable::setRowCount(int rows)
{
	m_model->setRowCount(rows);
}


int TFramesTable::rowCount() const
{
	return m_model->rowCount();
}


void TFramesTable::setColumnCount(int columns)
{
	m_model->setColumnCount(columns);
}


int TFramesTable::columnCount() const
{
	return m_model->columnCount();
}

int TFramesTable::row(const TFramesTableItem *item) const
{
	Q_ASSERT(item);
	return m_model->index(item).row();
}

int TFramesTable::column(const TFramesTableItem *item) const
{
	Q_ASSERT(item);
	return m_model->index(item).column();
}

TFramesTableItem *TFramesTable::item(int row, int column) const
{
	return m_model->item(row, column);
}

void TFramesTable::setItem(int row, int column, TFramesTableItem *item)
{
	Q_ASSERT(item);
	item->m_view = this;
	m_model->setItem(row, column, item);
}

TFramesTableItem *TFramesTable::takeItem(int row, int column)
{
	TFramesTableItem *item = m_model->takeItem(row, column);
	item->m_view = 0;
	return item;
}

int TFramesTable::currentRow() const
{
	return currentIndex().row();
}

int TFramesTable::currentColumn() const
{
	return currentIndex().column();
}


TFramesTableItem *TFramesTable::currentItem() const
{
	return m_model->item(currentIndex());
}

void TFramesTable::setCurrentItem(TFramesTableItem *item)
{
	setCurrentIndex(m_model->index(item));
}

bool TFramesTable::isItemSelected(const TFramesTableItem *item) const
{
	QModelIndex index = m_model->index(item);
	return selectionModel()->isSelected(index) && !isIndexHidden(index);
}


void TFramesTable::setItemSelected(const TFramesTableItem *item, bool select)
{
	QModelIndex index = m_model->index(item);
	selectionModel()->select(index, select ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);
}

QList<TFramesTableItem*> TFramesTable::selectedItems()
{
	QModelIndexList indexes = selectedIndexes();
	QList<TFramesTableItem*> items;
	for (int i = 0; i < indexes.count(); ++i) {
		QModelIndex index = indexes.at(i);
		TFramesTableItem *item = m_model->item(index);
		if (item)
			items.append(item);
	}
	return items;
}


QList<TFramesTableItem*> TFramesTable::findItems(const QString &text, Qt::MatchFlags flags) const
{
	QModelIndexList indexes;
	for (int column = 0; column < columnCount(); ++column)
	{
		indexes += m_model->match(model()->index(0, column, QModelIndex()),
					  Qt::DisplayRole, text, -1, flags);
	}
	QList<TFramesTableItem*> items;
	for (int i = 0; i < indexes.size(); ++i)
		items.append(m_model->item(indexes.at(i)));
	return items;
}

TFramesTableItem *TFramesTable::itemAt(const QPoint &p) const
{
	return m_model->item(indexAt(p));
}


QRect TFramesTable::visualItemRect(const TFramesTableItem *item) const
{
// 	KT_FUNCINFO;
	Q_ASSERT(item);
	QModelIndex index = m_model->index(const_cast<TFramesTableItem*>(item));
	Q_ASSERT(index.isValid());
	return visualRect(index);
}

int TFramesTable::verticalOffset () const
{
	return m_rectHeight;
}

int TFramesTable::horizontalOffset () const
{
	return m_rectWidth;
}

void TFramesTable::scrollToItem(const TFramesTableItem *item, ScrollHint hint)
{
	Q_ASSERT(item);
	QModelIndex index = m_model->index(const_cast<TFramesTableItem*>(item));
	Q_ASSERT(index.isValid());
	scrollTo(index, hint);
}

void TFramesTable::insertRow(int row)
{
	m_model->insertRows(row);
	
	verticalHeader()->resizeSection(row-1, m_rectHeight);
}

void TFramesTable::insertColumn(int column)
{
	m_model->insertColumns(column);
	horizontalHeader()->resizeSection(column-1, m_rectWidth);
}


void TFramesTable::removeRow(int row)
{
	m_model->removeRows(row);
}


void TFramesTable::removeColumn(int column)
{
	m_model->removeColumns(column);
}

void TFramesTable::clear()
{
	selectionModel()->clear();
	m_model->clear();
}

QModelIndex TFramesTable::indexFromItem(TFramesTableItem *item) const
{
	Q_ASSERT(item);
	return m_model->index(item);
}

void TFramesTable::selectCell(int row, int column)
{
	KT_FUNCINFO;
	if (row >= 0 && row < model()->rowCount(rootIndex()) && column >= 0 && column < model()->columnCount(rootIndex()))
	{
		QItemSelectionModel::SelectionFlags command = selectionCommand(QModelIndex());
		QModelIndex index = model()->index(row, column, rootIndex());
		
		selectionModel()->setCurrentIndex(index, command);
	}
}

QStyleOptionViewItem TFramesTable::viewOptions() const
{
	QStyleOptionViewItem option = QAbstractItemView::viewOptions();
	option.showDecorationSelected = true;
	option.decorationSize = QSize(22,22);
	option.decorationPosition = QStyleOptionViewItem::Right;
	
	return option;
}


void TFramesTable::addLayer()
{
	int pos = rowCount() ;
	insertRow( pos + 1);
	
	LayerItem layer;
	
	m_layers << layer;
	
	selectCell( pos, 0);
}

void TFramesTable::removeCurrentLayer()
{
	int pos = currentRow();
	removeRow( pos );
	m_layers.removeAt(pos);
}

void TFramesTable::removeLayer(int pos)
{
	removeRow(pos);
	m_layers.removeAt(pos);
}

int TFramesTable::lastFrameByLayer(int layerPos)
{
	return m_layers[layerPos].lastItem;
}

// FRAMES

void TFramesTable::addFrame(int layerPos)
{
	m_layers[layerPos].lastItem++;
	
	setAttribute( layerPos, m_layers[layerPos].lastItem, TFramesTableItem::IsUsed, true);
}

void TFramesTable::setCurrentFrame(TFramesTableItem *item)
{
	setCurrentItem(item);
}

void TFramesTable::setCurrentLayer(int layerPos)
{
	setCurrentItem(item(layerPos, 0));
}

void TFramesTable::selectFrame(int index)
{
	setCurrentItem( item( currentRow(), index ) );
}

void TFramesTable::setAttribute(int row, int col, TFramesTableItem::Attributes att, bool value)
{
	m_model->setData( m_model->index(row, col), value, att);
}

