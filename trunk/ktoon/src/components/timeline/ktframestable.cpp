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

#include "ktframestable.h"

#include <QPainter>
#include <QPaintEvent>
#include <QItemSelectionModel>
#include <QPainterPath>
#include <QScrollBar>
#include <QHeaderView>

#include "ddebug.h"
#include "kttlruler.h"

//////////// KTFramesTableModel

class KTFramesTableModel : public QAbstractTableModel
{
	public:
		KTFramesTableModel(int rows, int columns, KTFramesTable *parent);
		~KTFramesTableModel();
		
		inline KTFramesTableItem *createItem() const { return new KTFramesTableItem(); }
		
		
		bool insertRows(int row, int count = 1, const QModelIndex &parent = QModelIndex());
		bool insertColumns(int column, int count = 1, const QModelIndex &parent = QModelIndex());

		bool removeRows(int row, int count = 1, const QModelIndex &parent = QModelIndex());
		bool removeColumns(int column, int count = 1, const QModelIndex &parent = QModelIndex());

		void setItem(int row, int column, KTFramesTableItem *item);
		KTFramesTableItem *takeItem(int row, int column);
		KTFramesTableItem *item(int row, int column) const;
		KTFramesTableItem *item(const QModelIndex &index) const;
		void removeItem(KTFramesTableItem *item);

		void setHorizontalHeaderItem(int section, KTFramesTableItem *item);
		void setVerticalHeaderItem(int section, KTFramesTableItem *item);
		KTFramesTableItem *horizontalHeaderItem(int section);
		KTFramesTableItem *verticalHeaderItem(int section);

		QModelIndex index(const KTFramesTableItem *item) const;
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
		void itemChanged(KTFramesTableItem *item);
		
		virtual QModelIndex parent ( const QModelIndex & ) const
		{
			return QModelIndex();
		}
		

	private:
		QVector<KTFramesTableItem*> m_table;
		QVector<KTFramesTableItem*> m_vertical;
		QVector<KTFramesTableItem*> m_horizontal;
};

KTFramesTableModel::KTFramesTableModel(int rows, int columns, KTFramesTable *parent)
	: QAbstractTableModel(parent), m_table(rows * columns), m_vertical(rows), m_horizontal(columns)
{
	
}

KTFramesTableModel::~KTFramesTableModel()
{
	clear();
}

bool KTFramesTableModel::insertRows(int row, int count, const QModelIndex &)
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

bool KTFramesTableModel::insertColumns(int column, int count, const QModelIndex &)
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

bool KTFramesTableModel::removeRows(int row, int count, const QModelIndex &)
{
	if (row >= 0 && row < m_vertical.count()) 
	{
		beginRemoveRows(QModelIndex(), row, row + count - 1);
		int i = tableIndex(row, 0);
		int n = count * columnCount();
		KTFramesTableItem *oldItem = 0;
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

bool KTFramesTableModel::removeColumns(int column, int count, const QModelIndex &)
{
	if (column >= 0 && column < m_horizontal.count()) 
	{
		beginRemoveColumns(QModelIndex(), column, column + count - 1);
		KTFramesTableItem *oldItem = 0;
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

void KTFramesTableModel::setItem(int row, int column, KTFramesTableItem *item)
{
	int i = tableIndex(row, column);
	if (i < 0 || i >= m_table.count())
		return;
	KTFramesTableItem *oldItem = m_table.at(i);
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

KTFramesTableItem *KTFramesTableModel::takeItem(int row, int column)
{
	long i = tableIndex(row, column);
	KTFramesTableItem *itm = m_table.value(i);
	if (itm) {
		itm->m_model = 0;
		m_table[i] = 0;
	}
	return itm;
}

KTFramesTableItem *KTFramesTableModel::item(int row, int column) const
{
	return m_table.value(tableIndex(row, column));
}

KTFramesTableItem *KTFramesTableModel::item(const QModelIndex &index) const
{
	if (!isValid(index))
		return 0;
	return m_table.at(tableIndex(index.row(), index.column()));
}

void KTFramesTableModel::removeItem(KTFramesTableItem *item)
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

void KTFramesTableModel::setHorizontalHeaderItem(int section, KTFramesTableItem *item)
{
	if (section < 0 || section >= m_horizontal.count())
		return;
	KTFramesTableItem *oldItem = m_horizontal.at(section);
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

void KTFramesTableModel::setVerticalHeaderItem(int section, KTFramesTableItem *item)
{
	if (section < 0 || section >= m_vertical.count())
		return;
	KTFramesTableItem *oldItem = m_vertical.at(section);
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

KTFramesTableItem *KTFramesTableModel::horizontalHeaderItem(int section)
{
	return m_horizontal.value(section);
}

KTFramesTableItem *KTFramesTableModel::verticalHeaderItem(int section)
{
	return m_vertical.value(section);
}

QModelIndex KTFramesTableModel::index(const KTFramesTableItem *item) const
{
	int i = m_table.indexOf(const_cast<KTFramesTableItem*>(item));
	int row = i / columnCount();
	int col = i % columnCount();
	return index(row, col);
}

QModelIndex KTFramesTableModel::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
	{
		KTFramesTableItem *item = m_table.at(tableIndex(row, column));
		return createIndex(row, column, item);
	}
	return QModelIndex();
}

void KTFramesTableModel::setRowCount(int rows)
{
	int rc = m_vertical.count();
	if (rc == rows)
		return;
	if (rc < rows)
		insertRows(qMax(rc, 0), rows - rc);
	else
		removeRows(qMax(rows, 0), rc - rows);
}

void KTFramesTableModel::setColumnCount(int columns)
{
	int cc = m_horizontal.count();
	if (cc == columns)
		return;
	if (cc < columns)
		insertColumns(qMax(cc, 0), columns - cc);
	else
		removeColumns(qMax(columns, 0), cc - columns);
}

int KTFramesTableModel::rowCount(const QModelIndex &) const
{
	return m_vertical.count();
}

int KTFramesTableModel::columnCount(const QModelIndex &) const
{
	return m_horizontal.count();
}

QVariant KTFramesTableModel::data(const QModelIndex &index, int role) const
{
	KTFramesTableItem *itm = item(index);
	if (itm)
	{
		return itm->data(role);
	}
	return QVariant();
}

bool KTFramesTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	KTFramesTableItem *itm = item(index);

	if (itm)
	{
		itm->setData(role, value);
		return true;
	}

	KTFramesTable *view = qobject_cast<KTFramesTable*>(QObject::parent());
	if (!view)
		return false;

	itm = createItem();
	itm->setData(role, value);
	view->setItem(index.row(), index.column(), itm);
	return true;
}

Qt::ItemFlags KTFramesTableModel::flags(const QModelIndex &index) const
{
	KTFramesTableItem *itm = item(index);
	if (itm)
		return itm->flags();
	return Qt::ItemIsEditable
			|Qt::ItemIsSelectable
			|Qt::ItemIsUserCheckable
			|Qt::ItemIsEnabled;
}

bool KTFramesTableModel::isValid(const QModelIndex &index) const
{
	return index.isValid() && index.row() < m_vertical.count() && index.column() < m_horizontal.count();
}

void KTFramesTableModel::clear()
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

void KTFramesTableModel::itemChanged(KTFramesTableItem *item)
{
	QModelIndex idx = index(item);
	emit dataChanged(idx, idx);
}

////////// KTFramesTableItemDelegate ///////////

class KTFramesTableItemDelegate : public QAbstractItemDelegate
{
	public:
		KTFramesTableItemDelegate(QObject * parent = 0 );
		~KTFramesTableItemDelegate();
		virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
		virtual QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

KTFramesTableItemDelegate::KTFramesTableItemDelegate(QObject * parent) :  QAbstractItemDelegate(parent)
{
}

KTFramesTableItemDelegate::~KTFramesTableItemDelegate()
{
}

void KTFramesTableItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	Q_ASSERT(index.isValid());
	
	const KTFramesTableModel *model = reinterpret_cast<const KTFramesTableModel*>( index.model() );
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
	
	KTFramesTableItem *item = model->item(index);
	
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
		if ( item->isLocked() )
		{
			painter->save();
			painter->setBrush(Qt::red);
			
			painter->drawEllipse( option.rect.left(), option.rect.bottom() - offset, offset, offset);
			
			painter->restore();
		}
	}
	
// 	painter->drawRect( option.rect.left(), option.rect.bottom() - offset, offset, offset );
	
// 	painter->fillRect( option.rect.left(), option.rect.bottom() - offset, offset, offset, Qt::black );
}

QSize KTFramesTableItemDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
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


////////// KTFramesTableItem ////////
KTFramesTableItem::KTFramesTableItem()
	:  m_view(0), m_model(0), m_itemFlags(Qt::ItemIsEditable
		|Qt::ItemIsSelectable
		|Qt::ItemIsUserCheckable
		|Qt::ItemIsEnabled
		|Qt::ItemIsDragEnabled
		|Qt::ItemIsDropEnabled), m_isUsed(false), m_isLocked(false)
{
}

KTFramesTableItem::~KTFramesTableItem()
{
	if (m_model)
	{
		m_model->removeItem(this);
	}
}


KTFramesTableItem *KTFramesTableItem::clone() const
{
	KTFramesTableItem *item = new KTFramesTableItem();
	*item = *this;
	return item;
}


void KTFramesTableItem::setData(int r, const QVariant &value)
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

QVariant KTFramesTableItem::data(int role) const
{
	return QVariant();
}

bool KTFramesTableItem::isUsed()
{
	return m_attributes[IsUsed];
}

bool KTFramesTableItem::isLocked()
{
	return m_attributes[IsLocked];
}

//// KTFramesTable

KTFramesTable::KTFramesTable(QWidget *parent) : QTableView(parent)
{
	m_model = new KTFramesTableModel(0,100,this);
	m_ruler = new KTTLRuler;
	
	setModel( m_model );
	setup();
}

KTFramesTable::~KTFramesTable()
{
}

void KTFramesTable::setup()
{
	setItemDelegate( new KTFramesTableItemDelegate(this));
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
	
	setHorizontalHeader(m_ruler);
	connect(m_ruler, SIGNAL(logicalSectionSelected( int )), this, SLOT(selectColumn( int )));
	
	verticalHeader()->hide();
	
	setItemSize( 10, 25 );
	
	horizontalHeader()->setResizeMode(QHeaderView::Custom);
	verticalHeader()->setResizeMode(QHeaderView::Custom);
	
// 	connect(verticalHeader(), SIGNAL(sectionMoved(int, int, int)), this, SLOT(fixSectionMoved(int, int, int)));
	
// 	setAutoFillBackground(true);
}

void KTFramesTable::setItemSize(int w, int h)
{
	m_rectHeight = h;
	m_rectWidth = w;
	
	fixSize();
}

void KTFramesTable::emitItemPressed(const QModelIndex &index)
{
	emit itemPressed( m_model->item(index) );
}

void KTFramesTable::emitItemClicked(const QModelIndex &index)
{
	emit itemClicked(m_model->item(index));
}

void KTFramesTable::emitItemDoubleClicked(const QModelIndex &index)
{
	emit itemDoubleClicked(m_model->item(index));
}

void KTFramesTable::emitItemActivated(const QModelIndex &index)
{
	emit itemActivated(m_model->item(index));
}

void KTFramesTable::emitItemEntered(const QModelIndex &index)
{
	emit itemEntered(m_model->item(index));
}

void KTFramesTable::emitItemChanged(const QModelIndex &index)
{
	emit itemChanged(m_model->item(index));
}

void KTFramesTable::emitCurrentItemChanged(const QModelIndex &previous, const QModelIndex &current)
{
	emit currentItemChanged(m_model->item(current), m_model->item(previous));
}

void KTFramesTable::keyPressEvent ( QKeyEvent * event )
{
	QTableView::keyPressEvent(event);
	
	if( event->key() == Qt::Key_Up ||  event->key() == Qt::Key_Down ||  event->key() == Qt::Key_Left ||  event->key() == Qt::Key_Right )
	{
		emit itemClicked( selectedItems()[0] );
	}
}

void KTFramesTable::setRowCount(int rows)
{
	m_model->setRowCount(rows);
}


int KTFramesTable::rowCount() const
{
	return m_model->rowCount();
}


void KTFramesTable::setColumnCount(int columns)
{
	m_model->setColumnCount(columns);
}


int KTFramesTable::columnCount() const
{
	return m_model->columnCount();
}

int KTFramesTable::row(const KTFramesTableItem *item) const
{
	Q_ASSERT(item);
	return m_model->index(item).row();
}

int KTFramesTable::column(const KTFramesTableItem *item) const
{
	Q_ASSERT(item);
	return m_model->index(item).column();
}

KTFramesTableItem *KTFramesTable::item(int row, int column) const
{
	return m_model->item(row, column);
}

void KTFramesTable::setItem(int row, int column, KTFramesTableItem *item)
{
	Q_ASSERT(item);
	item->m_view = this;
	m_model->setItem(row, column, item);
}

KTFramesTableItem *KTFramesTable::takeItem(int row, int column)
{
	KTFramesTableItem *item = m_model->takeItem(row, column);
	item->m_view = 0;
	return item;
}

int KTFramesTable::currentRow() const
{
	return currentIndex().row();
}

int KTFramesTable::currentColumn() const
{
	return currentIndex().column();
}


KTFramesTableItem *KTFramesTable::currentItem() const
{
	return m_model->item(currentIndex());
}

void KTFramesTable::setCurrentItem(KTFramesTableItem *item)
{
	setCurrentIndex(m_model->index(item));
}

bool KTFramesTable::isItemSelected(const KTFramesTableItem *item) const
{
	QModelIndex index = m_model->index(item);
	return selectionModel()->isSelected(index) && !isIndexHidden(index);
}


void KTFramesTable::setItemSelected(const KTFramesTableItem *item, bool select)
{
	QModelIndex index = m_model->index(item);
	selectionModel()->select(index, select ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);
}

QList<KTFramesTableItem*> KTFramesTable::selectedItems()
{
	QModelIndexList indexes = selectedIndexes();
	QList<KTFramesTableItem*> items;
	for (int i = 0; i < indexes.count(); ++i) {
		QModelIndex index = indexes.at(i);
		KTFramesTableItem *item = m_model->item(index);
		if (item)
			items.append(item);
	}
	return items;
}


QList<KTFramesTableItem*> KTFramesTable::findItems(const QString &text, Qt::MatchFlags flags) const
{
	QModelIndexList indexes;
	for (int column = 0; column < columnCount(); ++column)
	{
		indexes += m_model->match(model()->index(0, column, QModelIndex()),
					  Qt::DisplayRole, text, -1, flags);
	}
	QList<KTFramesTableItem*> items;
	for (int i = 0; i < indexes.size(); ++i)
		items.append(m_model->item(indexes.at(i)));
	return items;
}

KTFramesTableItem *KTFramesTable::itemAt(const QPoint &p) const
{
	return m_model->item(indexAt(p));
}


QRect KTFramesTable::visualItemRect(const KTFramesTableItem *item) const
{
// 	D_FUNCINFO;
	Q_ASSERT(item);
	QModelIndex index = m_model->index(const_cast<KTFramesTableItem*>(item));
	Q_ASSERT(index.isValid());
	return visualRect(index);
}

int KTFramesTable::verticalOffset () const
{
	return m_rectHeight;
}

int KTFramesTable::horizontalOffset () const
{
	return m_rectWidth;
}

void KTFramesTable::scrollToItem(const KTFramesTableItem *item, ScrollHint hint)
{
	Q_ASSERT(item);
	QModelIndex index = m_model->index(const_cast<KTFramesTableItem*>(item));
	Q_ASSERT(index.isValid());
	scrollTo(index, hint);
}

void KTFramesTable::insertRow(int row)
{
	m_model->insertRows(row);
	
	fixSize();
}

void KTFramesTable::insertColumn(int column)
{
	m_model->insertColumns(column);
	horizontalHeader()->resizeSection(column, m_rectWidth);
}


void KTFramesTable::removeRow(int row)
{
	m_model->removeRows(row);
}


void KTFramesTable::removeColumn(int column)
{
	m_model->removeColumns(column);
}

void KTFramesTable::clear()
{
	selectionModel()->clear();
	m_model->clear();
}

QModelIndex KTFramesTable::indexFromItem(KTFramesTableItem *item) const
{
	Q_ASSERT(item);
	return m_model->index(item);
}

void KTFramesTable::selectCell(int row, int column)
{
	if (row >= 0 && row < model()->rowCount(rootIndex()) && column >= 0 && column < model()->columnCount(rootIndex()))
	{
		int logicalRow = verticalHeader()->logicalIndex(row);
		int logicalCol = horizontalHeader()->logicalIndex(column);
		
		QModelIndex index = model()->index(logicalRow, logicalCol, QModelIndex() );
		
		selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
		
		m_ruler->select( logicalCol );
	}
}

void KTFramesTable::selectColumn(int logicalIndex)
{
	int cRow = verticalHeader()->logicalIndex(currentRow());
	
	QModelIndex index = model()->index(cRow, logicalIndex, QModelIndex() );
	selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
	
	
	emit itemClicked( m_model->item(index) );
}

QStyleOptionViewItem KTFramesTable::viewOptions() const
{
	QStyleOptionViewItem option = QAbstractItemView::viewOptions();
	option.showDecorationSelected = true;
	option.decorationSize = QSize(22,22);
	option.decorationPosition = QStyleOptionViewItem::Right;
	
	return option;
}


void KTFramesTable::insertLayer(int pos, const QString &name)
{
	pos = verticalHeader()->logicalIndex(pos);
	insertRow( pos + 1);
	
	LayerItem layer;
	
	m_layers.insert(pos, layer);
	
	selectCell( pos, 0);
}

void KTFramesTable::removeCurrentLayer()
{
	int pos = verticalHeader()->logicalIndex(currentRow());
	removeLayer(pos);
}

void KTFramesTable::removeLayer(int pos)
{
	pos = verticalHeader()->logicalIndex(pos);
	removeRow( pos );
	m_layers.removeAt(pos);
}

void KTFramesTable::moveLayer(int position, int newPosition)
{
	if ( position < 0 || position >= rowCount() || newPosition < 0 || newPosition >= rowCount() ) return;
	
	blockSignals(true);
	
	verticalHeader()->moveSection(position, newPosition);
	
	blockSignals(false);
	
// 	KTFramesTableItem *item1 = takeItem(position, 0);
// 	
// 	bool up = true;
// 	if ( position > newPosition )
// 	{
// 		up = false; // down
// 	}
// 	
// 	if ( up )
// 	{
// 		for(int i = position+1; i <= newPosition; i++)
// 		{
// 			setItem(i-1, 0, takeItem(i, 0));
// 		}
// 	}
// 	else
// 	{
// 		for(int i = position-1; i >= newPosition; i-- )
// 		{
// 			setItem(i+1, 0, takeItem(i, 0));
// 		}
// 	}
// 	
// 	setItem(newPosition, 0, item1);
// 	
// 	setCurrentItem(item1);
}

int KTFramesTable::lastFrameByLayer(int layerPos)
{
	int pos = verticalHeader()->logicalIndex(layerPos);
	if ( pos < 0 || pos > m_layers.count() )
	{
		return -1;
	}
	return m_layers[pos].lastItem;
}

// FRAMES


void KTFramesTable::insertFrame(int layerPos, const QString &name)
{
	if ( layerPos < 0 || layerPos >= m_layers.count() ) return;
	
	layerPos = verticalHeader()->logicalIndex(layerPos);
	
	m_layers[layerPos].lastItem++;
	
	if ( m_layers[layerPos].lastItem >= columnCount() )
	{
		insertColumn( m_layers[layerPos].lastItem );
	}
	
	setAttribute( layerPos, m_layers[layerPos].lastItem, KTFramesTableItem::IsUsed, true);
	
	m_model->setData( m_model->index(layerPos, m_layers[layerPos].lastItem), name, Qt::ToolTipRole );
	
	viewport()->update();
}

void KTFramesTable::setCurrentFrame(KTFramesTableItem *item)
{
	setCurrentItem(item);
}

void KTFramesTable::setCurrentLayer(int layerPos)
{
	setCurrentItem(item(verticalHeader()->logicalIndex(layerPos), 0));
}

void KTFramesTable::selectFrame(int index)
{
	setCurrentItem( item( currentRow(), index ) );
}

void KTFramesTable::removeFrame(int layerPos, int position)
{
// 	for(int frameIndex = position; frameIndex < columnCount(); frameIndex++ )
// 	{
// 		setAttribute( layerPos, position, KTFramesTableItem::IsUsed, false);
// 	}
	
	if ( layerPos < 0 || layerPos >= m_layers.count() )
	{
		return;
	}
	
	layerPos = verticalHeader()->logicalIndex(layerPos);
	
	setAttribute( layerPos, m_layers[layerPos].lastItem, KTFramesTableItem::IsUsed, false );
	
	m_layers[layerPos].lastItem--;
	
// 	viewport()->update( visualRect(indexFromItem( item(layerPos, position) )) );
	viewport()->update();
}

void KTFramesTable::lockFrame(int layerPos, int position, bool lock)
{
	if ( layerPos < 0 || layerPos >= m_layers.count() )
	{
		return;
	}
	
	layerPos = verticalHeader()->logicalIndex(layerPos);
	
	setAttribute( layerPos, position, KTFramesTableItem::IsLocked, lock );
	
	viewport()->update();
}


void KTFramesTable::setAttribute(int row, int col, KTFramesTableItem::Attributes att, bool value)
{
	m_model->setData( m_model->index(row, col), value, att);
}

void KTFramesTable::fixSize()
{
	for(int column = 0; column < columnCount(); column++)
	{
		horizontalHeader()->resizeSection(column, m_rectWidth);
	}
	for( int row = 0; row < rowCount(); row++)
	{
		verticalHeader()->resizeSection(row, m_rectHeight);
	}
}

// void KTFramesTable::fixSectionMoved(int logical, int visual, int newVisual)
// {
// 	verticalHeader()->moveSection(newVisual, visual);
// }

