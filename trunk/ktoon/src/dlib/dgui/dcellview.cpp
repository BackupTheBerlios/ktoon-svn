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

#include "dcellview.h"

#include <QPainter>
#include <QPaintEvent>
#include <QItemSelectionModel>
#include <QPainterPath>
#include <QScrollBar>
#include <QHeaderView>

#include "ddebug.h"
#include "dgradientadjuster.h"

////////// DCellViewItemDelegate ///////////

class DCellViewItemDelegate : public QAbstractItemDelegate
{
	public:
		DCellViewItemDelegate(QObject * parent = 0 );
		~DCellViewItemDelegate();
		virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
		virtual QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

DCellViewItemDelegate::DCellViewItemDelegate(QObject * parent) :  QAbstractItemDelegate(parent)
{
}

DCellViewItemDelegate::~DCellViewItemDelegate()
{
}

void DCellViewItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	Q_ASSERT(index.isValid());
	const QAbstractItemModel *model = index.model();
	Q_ASSERT(model);
	
	QVariant value;

	QStyleOptionViewItem opt = option;

    	// do layout
	QImage img = qvariant_cast<QImage>(model->data(index,Qt::DisplayRole));
	
	if( ! img.isNull() )
	{
		painter->drawImage(opt.rect, img);
	}
	
	// draw the background color
	value = model->data(index, Qt::BackgroundColorRole);
	if (value.isValid()/* && qvariant_cast<QBrush>(value).isValid()*/)
	{
		QBrush brush = qvariant_cast<QBrush>(value);
		
		if ( brush.gradient() )
		{
			QGradient newGradient = DGradientAdjuster::adjustGradient( brush.gradient(), option.rect);
			painter->fillRect(option.rect, QBrush(newGradient));
		}
		else
		{
			painter->fillRect(option.rect, brush);
		}
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
}

QSize DCellViewItemDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
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

//////////// DCellViewModel

class DCellViewModel : public QAbstractTableModel
{
	public:
		DCellViewModel(int rows, int columns, DCellView *parent);
		~DCellViewModel();
		
		inline DCellViewItem *createItem() const { return new DCellViewItem(); }
		
		
		bool insertRows(int row, int count = 1, const QModelIndex &parent = QModelIndex());
		bool insertColumns(int column, int count = 1, const QModelIndex &parent = QModelIndex());

		bool removeRows(int row, int count = 1, const QModelIndex &parent = QModelIndex());
		bool removeColumns(int column, int count = 1, const QModelIndex &parent = QModelIndex());

		void setItem(int row, int column, DCellViewItem *item);
		DCellViewItem *takeItem(int row, int column);
		DCellViewItem *item(int row, int column) const;
		DCellViewItem *item(const QModelIndex &index) const;
		void removeItem(DCellViewItem *item);

		void setHorizontalHeaderItem(int section, DCellViewItem *item);
		void setVerticalHeaderItem(int section, DCellViewItem *item);
		DCellViewItem *horizontalHeaderItem(int section);
		DCellViewItem *verticalHeaderItem(int section);

		QModelIndex index(const DCellViewItem *item) const;
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
		void itemChanged(DCellViewItem *item);
		
		virtual QModelIndex parent ( const QModelIndex & ) const
		{
			return QModelIndex();
		}
		

	private:
		QVector<DCellViewItem*> m_table;
		QVector<DCellViewItem*> m_vertical;
		QVector<DCellViewItem*> m_horizontal;
};

DCellViewModel::DCellViewModel(int rows, int columns, DCellView *parent)
	: QAbstractTableModel(parent), m_table(rows * columns), m_vertical(rows), m_horizontal(columns)
{}

DCellViewModel::~DCellViewModel()
{
	clear();
}

bool DCellViewModel::insertRows(int row, int count, const QModelIndex &)
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

bool DCellViewModel::insertColumns(int column, int count, const QModelIndex &)
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

bool DCellViewModel::removeRows(int row, int count, const QModelIndex &)
{
	if (row >= 0 && row < m_vertical.count()) 
	{
		beginRemoveRows(QModelIndex(), row, row + count - 1);
		int i = tableIndex(row, 0);
		int n = count * columnCount();
		DCellViewItem *oldItem = 0;
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

bool DCellViewModel::removeColumns(int column, int count, const QModelIndex &)
{
	if (column >= 0 && column < m_horizontal.count()) 
	{
		beginRemoveColumns(QModelIndex(), column, column + count - 1);
		DCellViewItem *oldItem = 0;
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

void DCellViewModel::setItem(int row, int column, DCellViewItem *item)
{
	int i = tableIndex(row, column);
	if (i < 0 || i >= m_table.count())
		return;
	DCellViewItem *oldItem = m_table.at(i);
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

DCellViewItem *DCellViewModel::takeItem(int row, int column)
{
	long i = tableIndex(row, column);
	DCellViewItem *itm = m_table.value(i);
	if (itm) {
		itm->m_model = 0;
		m_table[i] = 0;
	}
	return itm;
}

DCellViewItem *DCellViewModel::item(int row, int column) const
{
	return m_table.value(tableIndex(row, column));
}

DCellViewItem *DCellViewModel::item(const QModelIndex &index) const
{
	if (!isValid(index))
		return 0;
	return m_table.at(tableIndex(index.row(), index.column()));
}

void DCellViewModel::removeItem(DCellViewItem *item)
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

void DCellViewModel::setHorizontalHeaderItem(int section, DCellViewItem *item)
{
	if (section < 0 || section >= m_horizontal.count())
		return;
	DCellViewItem *oldItem = m_horizontal.at(section);
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

void DCellViewModel::setVerticalHeaderItem(int section, DCellViewItem *item)
{
	if (section < 0 || section >= m_vertical.count())
		return;
	DCellViewItem *oldItem = m_vertical.at(section);
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

DCellViewItem *DCellViewModel::horizontalHeaderItem(int section)
{
	return m_horizontal.value(section);
}

DCellViewItem *DCellViewModel::verticalHeaderItem(int section)
{
	return m_vertical.value(section);
}

QModelIndex DCellViewModel::index(const DCellViewItem *item) const
{
	int i = m_table.indexOf(const_cast<DCellViewItem*>(item));
	int row = i / columnCount();
	int col = i % columnCount();
	return index(row, col);
}

QModelIndex DCellViewModel::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
	{
		DCellViewItem *item = m_table.at(tableIndex(row, column));
		return createIndex(row, column, item);
	}
	return QModelIndex();
}

void DCellViewModel::setRowCount(int rows)
{
	int rc = m_vertical.count();
	if (rc == rows)
		return;
	if (rc < rows)
		insertRows(qMax(rc, 0), rows - rc);
	else
		removeRows(qMax(rows, 0), rc - rows);
}

void DCellViewModel::setColumnCount(int columns)
{
	int cc = m_horizontal.count();
	if (cc == columns)
		return;
	if (cc < columns)
		insertColumns(qMax(cc, 0), columns - cc);
	else
		removeColumns(qMax(columns, 0), cc - columns);
}

int DCellViewModel::rowCount(const QModelIndex &) const
{
	return m_vertical.count();
}

int DCellViewModel::columnCount(const QModelIndex &) const
{
	return m_horizontal.count();
}

QVariant DCellViewModel::data(const QModelIndex &index, int role) const
{
	DCellViewItem *itm = item(index);
	if (itm)
	{
		return itm->data(role);
	}
	return QVariant();
}

bool DCellViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	DCellViewItem *itm = item(index);

	if (itm)
	{
		itm->setData(role, value);
		return true;
	}

	DCellView *view = qobject_cast<DCellView*>(QObject::parent());
	if (!view)
		return false;

	itm = createItem();
	itm->setData(role, value);
	view->setItem(index.row(), index.column(), itm);
	return true;
}

Qt::ItemFlags DCellViewModel::flags(const QModelIndex &index) const
{
	DCellViewItem *itm = item(index);
	if (itm)
		return itm->flags();
	return Qt::ItemIsEditable
			|Qt::ItemIsSelectable
			|Qt::ItemIsUserCheckable
			|Qt::ItemIsEnabled;
}

bool DCellViewModel::isValid(const QModelIndex &index) const
{
	return index.isValid() && index.row() < m_vertical.count() && index.column() < m_horizontal.count();
}

void DCellViewModel::clear()
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

void DCellViewModel::itemChanged(DCellViewItem *item)
{
	QModelIndex idx = index(item);
	emit dataChanged(idx, idx);
}


////////// DCellViewItem ////////
DCellViewItem::DCellViewItem()
	:  m_view(0), m_model(0), m_itemFlags(Qt::ItemIsEditable
		      |Qt::ItemIsSelectable
		      |Qt::ItemIsUserCheckable
		      |Qt::ItemIsEnabled
		      |Qt::ItemIsDragEnabled
		      |Qt::ItemIsDropEnabled)
{
}

DCellViewItem::~DCellViewItem()
{
	if (m_model)
	{
		m_model->removeItem(this);
	}
}


DCellViewItem *DCellViewItem::clone() const
{
	DCellViewItem *item = new DCellViewItem();
	*item = *this;
	return item;
}


void DCellViewItem::setData(int r, const QVariant &value)
{
	m_values.insert(r, value);
	
// 	if (m_model)
// 		m_model->itemChanged(this);
}

QVariant DCellViewItem::data(int role) const
{
	return m_values[role];
}

void DCellViewItem::setImage(const QImage &img)
{
	m_values.insert(Qt::DisplayRole, img);
}

QImage DCellViewItem::image() const
{
	return qvariant_cast<QImage>(m_values[Qt::DisplayRole]);
}

void DCellViewItem::setBackground(const QBrush &c)
{
	m_values.insert(Qt::BackgroundColorRole, c);
}

QBrush DCellViewItem::background() const
{
	return qvariant_cast<QBrush>(m_values[Qt::BackgroundColorRole]);
}
	
////////// DCellView  ///////////
DCellView::DCellView(QWidget *parent)
	: QTableView(parent)
{
	m_model = new DCellViewModel(0, 0, this);
	setModel( m_model );
	setup();
}

DCellView::DCellView(int rows, int columns, QWidget *parent)
	: QTableView(parent)
{
	m_model = new DCellViewModel(rows, columns, this);
	setModel( m_model );
	setup();
}

void DCellView::setup()
{
	setItemDelegate( new DCellViewItemDelegate(this));
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
	
	setItemSize( 18, 18 );
	
	horizontalHeader()->setResizeMode(QHeaderView::Custom);
	verticalHeader()->setResizeMode(QHeaderView::Custom);
	
#if QT_VERSION >= 0x040100
	setAutoFillBackground(true);
#endif
}

void DCellView::setItemSize(int w, int h)
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

void DCellView::emitItemPressed(const QModelIndex &index)
{
	emit itemPressed( m_model->item(index) );
}

void DCellView::emitItemClicked(const QModelIndex &index)
{
	setCurrentItem( m_model->item(index) ); // FIXME: it's necessary?
	emit itemClicked(m_model->item(index));
}

void DCellView::emitItemDoubleClicked(const QModelIndex &index)
{
	emit itemDoubleClicked(m_model->item(index));
}

void DCellView::emitItemActivated(const QModelIndex &index)
{
	emit itemActivated(m_model->item(index));
}

void DCellView::emitItemEntered(const QModelIndex &index)
{
	emit itemEntered(m_model->item(index));
}

void DCellView::emitItemChanged(const QModelIndex &index)
{
	emit itemChanged(m_model->item(index));
}

void DCellView::emitCurrentItemChanged(const QModelIndex &previous, const QModelIndex &current)
{
	emit currentItemChanged(m_model->item(current), m_model->item(previous));
}

DCellView::~DCellView()
{
}

// void DCellView::emitItemPressed()
// {
// }

void DCellView::setRowCount(int rows)
{
	m_model->setRowCount(rows);
}


int DCellView::rowCount() const
{
	return m_model->rowCount();
}


void DCellView::setColumnCount(int columns)
{
	m_model->setColumnCount(columns);
}


int DCellView::columnCount() const
{
	return m_model->columnCount();
}

int DCellView::row(const DCellViewItem *item) const
{
	Q_ASSERT(item);
	return m_model->index(item).row();
}

int DCellView::column(const DCellViewItem *item) const
{
	Q_ASSERT(item);
	return m_model->index(item).column();
}

DCellViewItem *DCellView::item(int row, int column) const
{
	return m_model->item(row, column);
}

void DCellView::setItem(int row, int column, DCellViewItem *item)
{
	Q_ASSERT(item);
	item->m_view = this;
	m_model->setItem(row, column, item);
}

DCellViewItem *DCellView::takeItem(int row, int column)
{
	DCellViewItem *item = m_model->takeItem(row, column);
	item->m_view = 0;
	return item;
}

int DCellView::currentRow() const
{
	return currentIndex().row();
}

int DCellView::currentColumn() const
{
	return currentIndex().column();
}


DCellViewItem *DCellView::currentItem() const
{
	return m_model->item(currentIndex());
}

void DCellView::setCurrentItem(DCellViewItem *item)
{
	setCurrentIndex(m_model->index(item));
}

bool DCellView::isItemSelected(const DCellViewItem *item) const
{
	QModelIndex index = m_model->index(item);
	return selectionModel()->isSelected(index) && !isIndexHidden(index);
}


void DCellView::setItemSelected(const DCellViewItem *item, bool select)
{
	QModelIndex index = m_model->index(item);
	selectionModel()->select(index, select ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);
}

QList<DCellViewItem*> DCellView::selectedItems()
{
	QModelIndexList indexes = selectedIndexes();
	QList<DCellViewItem*> items;
	for (int i = 0; i < indexes.count(); ++i) {
		QModelIndex index = indexes.at(i);
		DCellViewItem *item = m_model->item(index);
		if (item)
			items.append(item);
	}
	return items;
}


QList<DCellViewItem*> DCellView::findItems(const QString &text, Qt::MatchFlags flags) const
{
	QModelIndexList indexes;
	for (int column = 0; column < columnCount(); ++column)
	{
		indexes += m_model->match(model()->index(0, column, QModelIndex()),
		Qt::DisplayRole, text, -1, flags);
	}
	QList<DCellViewItem*> items;
	for (int i = 0; i < indexes.size(); ++i)
		items.append(m_model->item(indexes.at(i)));
	return items;
}

DCellViewItem *DCellView::itemAt(const QPoint &p) const
{
	return m_model->item(indexAt(p));
}


QRect DCellView::visualItemRect(const DCellViewItem *item) const
{
// 	D_FUNCINFO;
	Q_ASSERT(item);
	QModelIndex index = m_model->index(const_cast<DCellViewItem*>(item));
	Q_ASSERT(index.isValid());
	return visualRect(index);
}

int DCellView::verticalOffset () const
{
	return m_rectHeight;
}

int DCellView::horizontalOffset () const
{
	return m_rectWidth;
}

void DCellView::scrollToItem(const DCellViewItem *item, ScrollHint hint)
{
	Q_ASSERT(item);
	QModelIndex index = m_model->index(const_cast<DCellViewItem*>(item));
	Q_ASSERT(index.isValid());
	scrollTo(index, hint);
}

void DCellView::insertRow(int row)
{
	m_model->insertRows(row);
	
	verticalHeader()->resizeSection(row-1, m_rectHeight);
}

void DCellView::insertColumn(int column)
{
	m_model->insertColumns(column);
	horizontalHeader()->resizeSection(column-1, m_rectWidth);
}


void DCellView::removeRow(int row)
{
	m_model->removeRows(row);
}


void DCellView::removeColumn(int column)
{
	m_model->removeColumns(column);
}

void DCellView::clear()
{
	selectionModel()->clear();
	m_model->clear();
}

QModelIndex DCellView::indexFromItem(DCellViewItem *item) const
{
	Q_ASSERT(item);
	return m_model->index(item);
}

void DCellView::selectCell(int row, int column)
{
	D_FUNCINFO;
	if (row >= 0 && row < model()->rowCount(rootIndex()) && column >= 0 && column < model()->columnCount(rootIndex()))
	{
		QItemSelectionModel::SelectionFlags command = selectionCommand(QModelIndex());
		QModelIndex index = model()->index(row, column, rootIndex());
		
		selectionModel()->setCurrentIndex(index, command);
	}
}

QStyleOptionViewItem DCellView::viewOptions() const
{
	QStyleOptionViewItem option = QAbstractItemView::viewOptions();
	option.showDecorationSelected = true;
	option.decorationSize = QSize(22,22);
	option.decorationPosition = QStyleOptionViewItem::Right;
	
	return option;
}

