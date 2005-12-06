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

#include "ktcellview.h"

#include <QPainter>
#include <QPaintEvent>
#include <QItemSelectionModel>
#include <QPainterPath>
#include <QScrollBar>

#include "ktdebug.h"

////////// KTCellViewItemDelegate ///////////

class KTCellViewItemDelegate : public QAbstractItemDelegate
{
	public:
		KTCellViewItemDelegate(QObject * parent = 0 );
		~KTCellViewItemDelegate();
		virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
		virtual QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

KTCellViewItemDelegate::KTCellViewItemDelegate(QObject * parent) :  QAbstractItemDelegate(parent)
{
}

KTCellViewItemDelegate::~KTCellViewItemDelegate()
{
}

void KTCellViewItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
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
			const QGradient *gradient = brush.gradient();
			
			QGradient newGradient;
			switch( gradient->type() )
			{
				case  QGradient::LinearGradient:
				{
					newGradient = QLinearGradient(option.rect.topLeft(), option.rect.topRight());
					break;
				}
				case QGradient::RadialGradient:
				{
					
					QPointF focal= static_cast<const QRadialGradient*>(gradient)->focalPoint ();
// 					focal = focal * (QMatrix().scale( rect.width()/ 100 ,   rect.height()/100  ));
// 					ktDebug() << focal.x() << ", " << focal.y();
					newGradient = QRadialGradient(option.rect.center(), (option.rect.left()-option.rect.right())/2, option.rect.center() );
					break;
				}
				case QGradient::ConicalGradient:
				{
					double angle = static_cast<const QConicalGradient*>(gradient)->angle();
					ktDebug() << angle;
					newGradient = QConicalGradient(option.rect.center(), angle);
					break;
				}
			}
			newGradient.setStops(gradient->stops());
			
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
		painter->drawRect(option.rect.adjusted(2,2,-2,-2));
		painter->restore();
	}
}

QSize KTCellViewItemDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
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

//////////// KTCellViewModel

class KTCellViewModel : public QAbstractTableModel
{
	public:
		KTCellViewModel(int rows, int columns, KTCellView *parent);
		~KTCellViewModel();
		
		inline KTCellViewItem *createItem() const { return new KTCellViewItem(); }
		
		
		bool insertRows(int row, int count = 1, const QModelIndex &parent = QModelIndex());
		bool insertColumns(int column, int count = 1, const QModelIndex &parent = QModelIndex());

		bool removeRows(int row, int count = 1, const QModelIndex &parent = QModelIndex());
		bool removeColumns(int column, int count = 1, const QModelIndex &parent = QModelIndex());

		void setItem(int row, int column, KTCellViewItem *item);
		KTCellViewItem *takeItem(int row, int column);
		KTCellViewItem *item(int row, int column) const;
		KTCellViewItem *item(const QModelIndex &index) const;
		void removeItem(KTCellViewItem *item);

		void setHorizontalHeaderItem(int section, KTCellViewItem *item);
		void setVerticalHeaderItem(int section, KTCellViewItem *item);
		KTCellViewItem *horizontalHeaderItem(int section);
		KTCellViewItem *verticalHeaderItem(int section);

		QModelIndex index(const KTCellViewItem *item) const;
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
		void itemChanged(KTCellViewItem *item);
		
		virtual QModelIndex parent ( const QModelIndex & index ) const
		{
			return QModelIndex();
		}
		

	private:
		QVector<KTCellViewItem*> m_table;
		QVector<KTCellViewItem*> m_vertical;
		QVector<KTCellViewItem*> m_horizontal;
};

KTCellViewModel::KTCellViewModel(int rows, int columns, KTCellView *parent)
	: QAbstractTableModel(parent), m_table(rows * columns), m_vertical(rows), m_horizontal(columns)
{}

KTCellViewModel::~KTCellViewModel()
{
	clear();
}

bool KTCellViewModel::insertRows(int row, int count, const QModelIndex &)
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

bool KTCellViewModel::insertColumns(int column, int count, const QModelIndex &)
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

bool KTCellViewModel::removeRows(int row, int count, const QModelIndex &)
{
	if (row >= 0 && row < m_vertical.count()) 
	{
		beginRemoveRows(QModelIndex(), row, row + count - 1);
		int i = tableIndex(row, 0);
		int n = count * columnCount();
		KTCellViewItem *oldItem = 0;
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

bool KTCellViewModel::removeColumns(int column, int count, const QModelIndex &)
{
	if (column >= 0 && column < m_horizontal.count()) 
	{
		beginRemoveColumns(QModelIndex(), column, column + count - 1);
		KTCellViewItem *oldItem = 0;
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

void KTCellViewModel::setItem(int row, int column, KTCellViewItem *item)
{
	int i = tableIndex(row, column);
	if (i < 0 || i >= m_table.count())
		return;
	KTCellViewItem *oldItem = m_table.at(i);
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

KTCellViewItem *KTCellViewModel::takeItem(int row, int column)
{
	long i = tableIndex(row, column);
	KTCellViewItem *itm = m_table.value(i);
	if (itm) {
		itm->m_model = 0;
		m_table[i] = 0;
	}
	return itm;
}

KTCellViewItem *KTCellViewModel::item(int row, int column) const
{
	return m_table.value(tableIndex(row, column));
}

KTCellViewItem *KTCellViewModel::item(const QModelIndex &index) const
{
	if (!isValid(index))
		return 0;
	return m_table.at(tableIndex(index.row(), index.column()));
}

void KTCellViewModel::removeItem(KTCellViewItem *item)
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

void KTCellViewModel::setHorizontalHeaderItem(int section, KTCellViewItem *item)
{
	if (section < 0 || section >= m_horizontal.count())
		return;
	KTCellViewItem *oldItem = m_horizontal.at(section);
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

void KTCellViewModel::setVerticalHeaderItem(int section, KTCellViewItem *item)
{
	if (section < 0 || section >= m_vertical.count())
		return;
	KTCellViewItem *oldItem = m_vertical.at(section);
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

KTCellViewItem *KTCellViewModel::horizontalHeaderItem(int section)
{
	return m_horizontal.value(section);
}

KTCellViewItem *KTCellViewModel::verticalHeaderItem(int section)
{
	return m_vertical.value(section);
}

QModelIndex KTCellViewModel::index(const KTCellViewItem *item) const
{
	int i = m_table.indexOf(const_cast<KTCellViewItem*>(item));
	int row = i / columnCount();
	int col = i % columnCount();
	return index(row, col);
}

QModelIndex KTCellViewModel::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
	{
		KTCellViewItem *item = m_table.at(tableIndex(row, column));
		return createIndex(row, column, item);
	}
	return QModelIndex();
}

void KTCellViewModel::setRowCount(int rows)
{
	int rc = m_vertical.count();
	if (rc == rows)
		return;
	if (rc < rows)
		insertRows(qMax(rc, 0), rows - rc);
	else
		removeRows(qMax(rows, 0), rc - rows);
}

void KTCellViewModel::setColumnCount(int columns)
{
	int cc = m_horizontal.count();
	if (cc == columns)
		return;
	if (cc < columns)
		insertColumns(qMax(cc, 0), columns - cc);
	else
		removeColumns(qMax(columns, 0), cc - columns);
}

int KTCellViewModel::rowCount(const QModelIndex &) const
{
	return m_vertical.count();
}

int KTCellViewModel::columnCount(const QModelIndex &) const
{
	return m_horizontal.count();
}

QVariant KTCellViewModel::data(const QModelIndex &index, int role) const
{
	KTCellViewItem *itm = item(index);
	if (itm)
	{
		return itm->data(role);
	}
	return QVariant();
}

bool KTCellViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	KTCellViewItem *itm = item(index);

	if (itm) 
	{
		itm->setData(role, value);
		return true;
	}

	KTCellView *view = qobject_cast<KTCellView*>(QObject::parent());
	if (!view)
		return false;

	itm = createItem();
	itm->setData(role, value);
	view->setItem(index.row(), index.column(), itm);
	return true;
}

Qt::ItemFlags KTCellViewModel::flags(const QModelIndex &index) const
{
	KTCellViewItem *itm = item(index);
	if (itm)
		return itm->flags();
	return Qt::ItemIsEditable
			|Qt::ItemIsSelectable
			|Qt::ItemIsUserCheckable
			|Qt::ItemIsEnabled;
}

bool KTCellViewModel::isValid(const QModelIndex &index) const
{
	return index.isValid() && index.row() < m_vertical.count() && index.column() < m_horizontal.count();
}

void KTCellViewModel::clear()
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

void KTCellViewModel::itemChanged(KTCellViewItem *item)
{
	QModelIndex idx = index(item);
	emit dataChanged(idx, idx);
}


////////// KTCellViewItem ////////
KTCellViewItem::KTCellViewItem()
	:  m_view(0), m_model(0), m_itemFlags(Qt::ItemIsEditable
		      |Qt::ItemIsSelectable
		      |Qt::ItemIsUserCheckable
		      |Qt::ItemIsEnabled
		      |Qt::ItemIsDragEnabled
		      |Qt::ItemIsDropEnabled)
{
}

KTCellViewItem::~KTCellViewItem()
{
	if (m_model)
	{
		m_model->removeItem(this);
	}
}


KTCellViewItem *KTCellViewItem::clone() const
{
	KTCellViewItem *item = new KTCellViewItem();
	*item = *this;
	return item;
}


void KTCellViewItem::setData(int r, const QVariant &value)
{
	m_values.insert(r, value);
	
// 	if (m_model)
// 		m_model->itemChanged(this);
}

QVariant KTCellViewItem::data(int role) const
{
	return m_values[role];
}

void KTCellViewItem::setImage(const QImage &img)
{
	m_values.insert(Qt::DisplayRole, img);
}

QImage KTCellViewItem::image() const
{
	return qvariant_cast<QImage>(m_values[Qt::DisplayRole]);
}

void KTCellViewItem::setBackground(const QBrush &c)
{
	m_values.insert(Qt::BackgroundColorRole, c);
}

QBrush KTCellViewItem::background() const
{
	return qvariant_cast<QBrush>(m_values[Qt::BackgroundColorRole]);
}
	
////////// KTCellView  ///////////
KTCellView::KTCellView(QWidget *parent)
	: QAbstractItemView(parent)
{
	m_model = new KTCellViewModel(0, 0, this);
	setModel( m_model );
	setup();
}

KTCellView::KTCellView(int rows, int columns, QWidget *parent)
	: QAbstractItemView(parent)
{
	m_model = new KTCellViewModel(rows, columns, this);
	setModel( m_model );
	setup();
}

void KTCellView::setup()
{
	setItemDelegate( new KTCellViewItemDelegate(this));
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
	
	updateHorizontalScrollbar();
	updateVerticalScrollbar();
}

void KTCellView::emitItemPressed(const QModelIndex &index)
{
	emit itemPressed( m_model->item(index) );
}

void KTCellView::emitItemClicked(const QModelIndex &index)
{
	emit itemClicked(m_model->item(index));
}

void KTCellView::emitItemDoubleClicked(const QModelIndex &index)
{
	emit itemDoubleClicked(m_model->item(index));
}

void KTCellView::emitItemActivated(const QModelIndex &index)
{
	emit itemActivated(m_model->item(index));
}

void KTCellView::emitItemEntered(const QModelIndex &index)
{
	emit itemEntered(m_model->item(index));
}

void KTCellView::emitItemChanged(const QModelIndex &index)
{
	emit itemChanged(m_model->item(index));
}

void KTCellView::emitCurrentItemChanged(const QModelIndex &previous, const QModelIndex &current)
{
	emit currentItemChanged(m_model->item(current), m_model->item(previous));
}

KTCellView::~KTCellView()
{
}

// void KTCellView::emitItemPressed()
// {
// }

void KTCellView::setRowCount(int rows)
{
	m_model->setRowCount(rows);
}


int KTCellView::rowCount() const
{
	return m_model->rowCount();
}


void KTCellView::setColumnCount(int columns)
{
	m_model->setColumnCount(columns);
}


int KTCellView::columnCount() const
{
	return m_model->columnCount();
}

int KTCellView::row(const KTCellViewItem *item) const
{
	Q_ASSERT(item);
	return m_model->index(item).row();
}

int KTCellView::column(const KTCellViewItem *item) const
{
	Q_ASSERT(item);
	return m_model->index(item).column();
}

KTCellViewItem *KTCellView::item(int row, int column) const
{
	return m_model->item(row, column);
}

void KTCellView::setItem(int row, int column, KTCellViewItem *item)
{
	Q_ASSERT(item);
	item->m_view = this;
	m_model->setItem(row, column, item);
}

KTCellViewItem *KTCellView::takeItem(int row, int column)
{
	KTCellViewItem *item = m_model->takeItem(row, column);
	item->m_view = 0;
	return item;
}

int KTCellView::currentRow() const
{
	return currentIndex().row();
}

int KTCellView::currentColumn() const
{
	return currentIndex().column();
}


KTCellViewItem *KTCellView::currentItem() const
{
	return m_model->item(currentIndex());
}

void KTCellView::setCurrentItem(KTCellViewItem *item)
{
	setCurrentIndex(m_model->index(item));
}

bool KTCellView::isItemSelected(const KTCellViewItem *item) const
{
	QModelIndex index = m_model->index(item);
	return selectionModel()->isSelected(index) && !isIndexHidden(index);
}


void KTCellView::setItemSelected(const KTCellViewItem *item, bool select)
{
	QModelIndex index = m_model->index(item);
	selectionModel()->select(index, select ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);
}

QList<KTCellViewItem*> KTCellView::selectedItems()
{
	QModelIndexList indexes = selectedIndexes();
	QList<KTCellViewItem*> items;
	for (int i = 0; i < indexes.count(); ++i) {
		QModelIndex index = indexes.at(i);
		KTCellViewItem *item = m_model->item(index);
		if (item)
			items.append(item);
	}
	return items;
}


QList<KTCellViewItem*> KTCellView::findItems(const QString &text, Qt::MatchFlags flags) const
{
	QModelIndexList indexes;
	for (int column = 0; column < columnCount(); ++column)
	{
		indexes += m_model->match(model()->index(0, column, QModelIndex()),
		Qt::DisplayRole, text, -1, flags);
	}
	QList<KTCellViewItem*> items;
	for (int i = 0; i < indexes.size(); ++i)
		items.append(m_model->item(indexes.at(i)));
	return items;
}

KTCellViewItem *KTCellView::itemAt(const QPoint &p) const
{
	return m_model->item(indexAt(p));
}


QRect KTCellView::visualItemRect(const KTCellViewItem *item) const
{
// 	KT_FUNCINFO;
	Q_ASSERT(item);
	QModelIndex index = m_model->index(const_cast<KTCellViewItem*>(item));
	Q_ASSERT(index.isValid());
	return visualRect(index);
}

QRect KTCellView::visualRect ( const QModelIndex & index ) const
{
	if (!index.isValid() || index.parent() != rootIndex())
		return QRect();

	return viewport()->rect();
}

bool KTCellView::isIndexHidden ( const QModelIndex & index ) const
{
	return false;
}

int KTCellView::verticalOffset () const
{
// 	KT_FUNCINFO;
	return m_rectHeight;
}

int KTCellView::horizontalOffset () const
{
// 	KT_FUNCINFO;
	return m_rectWidth;
}

QModelIndex KTCellView::indexAt ( const QPoint & p ) const
{
	int r = rowAt(p.y());
	int c = columnAt(p.x());
	if (r >= 0 && c >= 0)
	{
		return m_model->index(c, r, rootIndex());
	}
	return QModelIndex();
}

int KTCellView::rowAt(int X) const
{
	return X / m_rectWidth;
}

int KTCellView::columnAt(int Y) const
{
	return Y / m_rectHeight;
}

void KTCellView::scrollToItem(const KTCellViewItem *item, ScrollHint hint)
{
	Q_ASSERT(item);
	QModelIndex index = m_model->index(const_cast<KTCellViewItem*>(item));
	Q_ASSERT(index.isValid());
	scrollTo(index, hint);
}

void KTCellView::scrollTo ( const QModelIndex & index, ScrollHint hint )
{
// 	KT_FUNCINFO;
	// check if we really need to do anything
	if (!index.isValid() || !model() /*|| (model()->parent(index) != rootIndex()) */|| isIndexHidden(index))
		return;

	QRect area = viewport()->rect();
	QRect rect = visualRect(index);
	if (hint == EnsureVisible && area.contains(rect)) 
	{
#if 0
		setDirtyRegion(rect);
#endif
		return;
	}

    	// vertical
	int verticalSteps = verticalStepsPerItem();
	bool above = (hint == EnsureVisible && rect.top() < area.top());
	bool below = (hint == EnsureVisible && rect.bottom() > area.bottom());
	
	if (hint == PositionAtTop || above) 
	{
		verticalScrollBar()->setValue(index.row() * verticalSteps);
	} 
	else if (hint == PositionAtBottom || below)
	{
		int r = index.row();
		int y = area.height();
// 		while (y > 0 && r > 0)
// 		{
// 			y -= rowHeight(r--);
// 		}
		
		y -= r * m_rectHeight;
		
// 		int h = rowHeight(r);
		int h = m_rectHeight;
		int a = (-y * verticalSteps) / (h ? h : 1);
		verticalScrollBar()->setValue(++r * verticalSteps + a);
	}

    	// horizontal
	int horizontalSteps = horizontalStepsPerItem();
	bool leftOf = isRightToLeft()
			? rect.right() > area.right()
	: rect.left() < area.left();
	bool rightOf = isRightToLeft()
			? rect.left() < area.left()
	: rect.right() > area.right();
	if (leftOf) 
	{
		horizontalScrollBar()->setValue(index.column() * horizontalSteps);
	} else if (rightOf) 
	{
		int c = index.column();
		int x = area.width();
// 		while (x > 0 && c > 0)
// 		{
// 			x -= columnWidth(c--);
// 		}
		x -= x* m_rectWidth;
// 		int w = columnWidth(c);
		int w = m_rectWidth;
		int a = (-x * horizontalSteps) / (w ? w : 1);
		horizontalScrollBar()->setValue(++c * horizontalSteps + a);
	}
#if 0
	setDirtyRegion(visualRect(index));
#endif
}

void KTCellView::updateVerticalScrollbar()
{
// 	KT_FUNCINFO;
	int height = viewport()->height();
	int top = rowCount();
	
	if (!m_model || top <= 0 || height <= 0) 
	{
		verticalScrollBar()->setRange(0, 0);
		return;
	}
	
	verticalScrollBar()->setPageStep( verticalStepsPerItem() );

	int max = top * verticalStepsPerItem();
	verticalScrollBar()->setRange(0, max);
}

void KTCellView::updateHorizontalScrollbar()
{
// 	KT_FUNCINFO;
	int width = viewport()->width();

	if (!m_model || width <= 0 ) {
		horizontalScrollBar()->setRange(0, 0);
		return;
	}

	int left = columnCount();

	horizontalScrollBar()->setPageStep(1 * horizontalStepsPerItem());

	int max = left * horizontalStepsPerItem();

	horizontalScrollBar()->setRange(0, max);
}

// void KTCellView::scrollContentsBy(int dx, int dy)
// {
// 	if (dx) 
// 	{ // horizontal
// 		int value = horizontalScrollBar()->value();
// 		int section = (value / horizontalStepsPerItem()) / m_rectWidth;
// 		
// 		int steps = horizontalStepsPerItem();
// 		int left = (value % steps) * m_rectWidth;
// 		int offset = (left / steps) + d->horizontalHeader->sectionPosition(section);
// 		if (isRightToLeft())
// 			dx = /*offset - */d->horizontalHeader->offset();
// 		else
// 			dx = d->horizontalHeader->offset() - offset;
// 		d->horizontalHeader->setOffset(offset);
// 	}
// 
// 	if (dy) { // vertical
// 		int value = verticalScrollBar()->value();
// 		int section = d->verticalHeader->logicalIndex(value / verticalStepsPerItem());
// 		while (d->verticalHeader->isSectionHidden(section))
// 			++section;
// 		int steps = verticalStepsPerItem();
// 		int above = (value % steps) * d->verticalHeader->sectionSize(section);
// 		int offset = (above / steps) + d->verticalHeader->sectionPosition(section);
// 		dy = d->verticalHeader->offset() - offset;
// 		d->verticalHeader->setOffset(offset);
// 	}
// 
// 	d->scrollContentsBy(dx, dy);
// }

void KTCellView::insertRow(int row)
{
	m_model->insertRows(row);
}

void KTCellView::insertColumn(int column)
{
	m_model->insertColumns(column);
}


void KTCellView::removeRow(int row)
{
	m_model->removeRows(row);
}


void KTCellView::removeColumn(int column)
{
	m_model->removeColumns(column);
}

void KTCellView::clear()
{
	selectionModel()->clear();
	m_model->clear();
}

QModelIndex KTCellView::indexFromItem(KTCellViewItem *item) const
{
	Q_ASSERT(item);
	return m_model->index(item);
}

KTCellViewItem *KTCellView::itemFromIndex(const QModelIndex &index) const
{
	Q_ASSERT(index.isValid());
	return m_model->item(index);
}

void KTCellView::setModel(QAbstractItemModel *model)
{
	QAbstractItemView::setModel(model);
}

QModelIndex KTCellView::moveCursor ( CursorAction cursorAction, Qt::KeyboardModifiers modifiers )
{
// 	KT_FUNCINFO;
	return QModelIndex();
}

QRegion KTCellView::visualRegionForSelection ( const QItemSelection & selection ) const
{
// 	KT_FUNCINFO;
	if (selection.isEmpty())
	{
		return QRegion();
	}
	
	QRegion selectionRegion;
	
	for (int i = 0; i < selection.count(); ++i) 
	{
		QItemSelectionRange range = selection.at(i);
		if (range.parent() != rootIndex() || !range.isValid())
			continue;
		QRect tl = visualRect(range.topLeft());
		QRect br = visualRect(range.bottomRight());
		selectionRegion += QRegion(tl|br);
	}
	
	return selectionRegion;
}

void KTCellView::setSelection ( const QRect & rect, QItemSelectionModel::SelectionFlags flags )
{
// 	KT_FUNCINFO;
	
	if (!selectionModel())
	{
		return;
	}
	
	QModelIndex tl = indexAt( QPoint(rect.right(), rect.top() ));
	QModelIndex br = indexAt(QPoint(rect.left(), rect.bottom()));

	selectionModel()->clear();
	selectionModel()->select(QItemSelection(tl, br), flags | QItemSelectionModel::ClearAndSelect);
}

void KTCellView::paintEvent(QPaintEvent *ev)
{
// 	KT_FUNCINFO;
	
	QStyleOptionViewItem option = viewOptions();
	
	const QPoint offset = QPoint(0,0);
	const QModelIndex current = currentIndex();
	const QItemSelectionModel *m_selectionModel = selectionModel();
	const QStyle::State state = option.state;
	const QPalette palette = this->palette();
	
	
	QPainter p(viewport());
	p.fillRect(viewport()->rect(), palette.color(QPalette::Base));
	
	const int rows = m_model->rowCount();
	const int columns = m_model->columnCount();
	
	if ( rows == 0 || columns == 0 )
	{
		return;
	}
	
	
#if 1
// 	m_rectWidth = viewport()->width() / columns;
// 	m_rectHeight = viewport()->height() / rows;
	
	m_rectWidth = 25/*viewport()->width() / columns*/;
	m_rectHeight = 25/*viewport()->height() / rows*/;
	
	QVector<QRect> rects = ev->region().rects();
	
	for (int i = 0; i < rects.size(); ++i)
	{
		QRect area = rects.at(i);
		area.translate(offset);

// 		int left = d->horizontalHeader->visualIndexAt(area.left());
// 		int right = d->horizontalHeader->visualIndexAt(area.right());;
		
		int left = (horizontalScrollBar()->value() - area.left())  / m_rectWidth;
		int right = (horizontalScrollBar()->value () - area.right()) / m_rectHeight;
		
		if (isRightToLeft())
		{
			left = (left <= -1 ? model()->columnCount(rootIndex()) - 1 : left);
			right = (right <= -1 ? 0 : right);
		} else 
		{
			left = (left <= -1 ? 0 : left);
			right = (right <= -1 ? model()->columnCount(rootIndex()) - 1 : right);
		}

		int tmp = left;
		left = qMin(left, right);
		right = qMax(tmp, right);

        	// get the vertical start and end sections (visual indexes)
// 		int top = d->verticalHeader->visualIndexAt(area.top());
// 		int bottom = d->verticalHeader->visualIndexAt(area.bottom());
		
		int top = (verticalScrollBar()->value() - area.top())  / m_rectWidth;
		int bottom = (verticalScrollBar()->value ()-area.bottom() ) / m_rectHeight;
		
// 		SHOW_VAR(left << " " << right << " " << top << " " << bottom << " ");
		
		top = (top <= -1 ? 0 : top);
		bottom = (bottom <= -1 ? m_model->rowCount(rootIndex()) - 1 : bottom);

		tmp = top;
		top = qMin(top, bottom);
		bottom = qMax(tmp, bottom);
		
        	// do the actual painting
// 		SHOW_VAR(left << " " << right << " " << top << " " << bottom << " ");
		
		for (int v = top; v <= bottom; ++v)
		{
			int row = v;

// 			int rowp = rowViewportPosition(row) + offset.y();
// 			int rowh = rowHeight(row) - gridSize;
			int rowp = v * m_rectWidth;
			
			for (int h = left; h <= right; ++h)
			{
				int col = h;

// 				int colp = columnViewportPosition(col) + offset.x();
// 				int colw = columnWidth(col) - gridSize;
				int colp = h * m_rectHeight;
				
				QModelIndex index = model()->index(row, col, rootIndex());
				if (index.isValid())
				{
					option.rect = QRect(rowp, colp, m_rectWidth, m_rectHeight);
					
					option.state = state;
					if (m_selectionModel && m_selectionModel->isSelected(index))
					{
						option.state |= QStyle::State_Selected;
					}
					QPalette::ColorGroup cg;
					if ((model()->flags(index) & Qt::ItemIsEnabled) == 0)
					{
						option.state &= ~QStyle::State_Enabled;
						cg = QPalette::Disabled;
					} else 
					{
						cg = QPalette::Normal;
					}
					
// 					if (focus && index == current)
// 					{
// 						option.state |= QStyle::State_HasFocus;
// 					}
					
// 					if (alternate && v & 1)
// 					{	painter.fillRect(option.rect,option.palette.brush(cg,QPalette::AlternateBase));
// 					}
// 					else
// 					{
// 					p.fillRect(option.rect,option.palette.brush(QPalette::Base));
// 					}
					
					itemDelegate()->paint(&p, option, index);
					
					p.drawRect(option.rect);
				}
			}
		}
	}
#else
	m_rectWidth = viewport()->width() / columnCount();
	m_rectHeight = viewport()->height() / rowCount();
	
	for(int v = 0; v < rows; ++v)
	{
		int row = v;
		
		for (int h = 0; h < columns; ++h)
		{
			int col = h;
			
			QModelIndex index = model()->index(row, col, rootIndex());
			if (index.isValid())
			{
				QRect trect = QRect((row*m_rectHeight), (col*m_rectWidth), m_rectHeight, m_rectWidth);
				
				option.rect = trect;
				option.state = state; // State is reset
				
				// TODO: pintarl el seleccionado
				if (m_selectionModel && m_selectionModel->isSelected(index))
				{
					option.state |= QStyle::State_Selected;
				}
				itemDelegate()->paint(&p, option, index);
				
				p.drawRect(trect);
			}
			else
			{
				ktDebug() << "INVALID ITEM";
			}
		}
	}
#endif
}

int KTCellView::firstVisualIndex(int y) const
{
	int top = verticalScrollBar()->value() / verticalStepsPerItem();

	return top+y;
}

void KTCellView::selectCell(int row, int column)
{
	if (row >= 0 && row < model()->rowCount(rootIndex()) && column >= 0 && column < model()->columnCount(rootIndex()))
	{
		QItemSelectionModel::SelectionFlags command = selectionCommand(QModelIndex());
		QModelIndex index = model()->index(row, column, rootIndex());
		
		selectionModel()->setCurrentIndex(index, command);
	}
}

QStyleOptionViewItem KTCellView::viewOptions() const
{
	QStyleOptionViewItem option = QAbstractItemView::viewOptions();
	option.showDecorationSelected = true;
	option.decorationSize = QSize(22,22);
	option.decorationPosition = QStyleOptionViewItem::Right;
	
	return option;
}


