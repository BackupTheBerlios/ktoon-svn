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

#include <dcore/ddebug.h>
#include "kttlruler.h"
#include "ktprojectrequest.h"


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
	
	QVariant value;

	QStyleOptionViewItem opt = option;
	
	// draw the background color
	value = index.data( Qt::BackgroundColorRole );
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
	
	KTFramesTable *table = qobject_cast<KTFramesTable *>(index.model()->parent());
	
	KTFramesTableItem *item = dynamic_cast<KTFramesTableItem *>(table->itemFromIndex(index));
	
	int offset = option.rect.width() - 2;
	
	if ( item )
	{
		if(item->isUsed() )
		{
			painter->save();
			painter->setBrush(Qt::black);
			
			if( item->isSound() )
			{
				painter->drawEllipse( option.rect.left(), option.rect.bottom() - offset, offset, offset);
			}
			else
			{
				painter->setBrush(Qt::blue);
				painter->drawEllipse( option.rect.left(), option.rect.bottom() - offset, offset, offset);
			}
			
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
{
}

KTFramesTableItem::~KTFramesTableItem()
{
}


bool KTFramesTableItem::isUsed()
{
	return data(IsUsed).toBool();
}

bool KTFramesTableItem::isLocked()
{
	return data(IsLocked).toBool();
}

bool KTFramesTableItem::isSound()
{
	return data(IsSound).toBool();
}

//// KTFramesTable

struct KTFramesTable::Private
{
	struct LayerItem
	{
		LayerItem() : lastItem(-1) {};
		int lastItem;
		bool sound;
	};
	
	int rectWidth, rectHeight;
	QList<LayerItem> layers;
	KTTLRuler *ruler;
};

KTFramesTable::KTFramesTable(QWidget *parent) : QTableWidget(0, 100, parent), d(new Private)
{
	d->ruler = new KTTLRuler;
	setup();
}

KTFramesTable::~KTFramesTable()
{
	delete d;
}

void KTFramesTable::setup()
{
	setItemDelegate( new KTFramesTableItemDelegate(this) );
		
	setSelectionBehavior(QAbstractItemView::SelectItems);
	setSelectionMode (QAbstractItemView::SingleSelection);
	
	setHorizontalHeader(d->ruler);
	connect(d->ruler, SIGNAL(logicalSectionSelected( int )), this, SLOT(emitFrameSelected( int )));
	connect(this, SIGNAL(currentItemChanged( QTableWidgetItem *, QTableWidgetItem *)), this, SLOT(emitFrameSelected(QTableWidgetItem *, QTableWidgetItem *)));
	verticalHeader()->hide();
	
	setItemSize( 10, 25 );
	
	horizontalHeader()->setResizeMode(QHeaderView::Custom);
	verticalHeader()->setResizeMode(QHeaderView::Custom);
	
	
}

void KTFramesTable::emitFrameSelected(int col)
{
	selectColumn(col);
	
	KTFramesTableItem *item = dynamic_cast<KTFramesTableItem *>(this->item(currentRow(), col));
	
	if( item )
	{
		if( item->isUsed())
		{
			emit frameRequest(KTProjectRequest::Select, this->column(item), verticalHeader()->visualIndex(this->row(item)),  -1);
		}
	}
}

void KTFramesTable::emitFrameSelected(QTableWidgetItem *curr, QTableWidgetItem *prev)
{
	KTFramesTableItem *item = dynamic_cast<KTFramesTableItem *>(curr);
	
	if( item )
	{
		if( item->isUsed())
		{
			emit frameRequest(KTProjectRequest::Select, this->column(item), verticalHeader()->visualIndex(this->row(item)),  -1);
		}
	}
}

void KTFramesTable::setItemSize(int w, int h)
{
	d->rectHeight = h;
	d->rectWidth = w;
	
	fixSize();
}

void KTFramesTable::insertLayer(int pos, const QString &name)
{
	insertRow( pos );
	
	Private::LayerItem layer;
	d->layers.insert(pos, layer);
	
// 	selectCell( pos, 0 );
	
	fixSize();
}

void KTFramesTable::insertSoundLayer(int layerPos, const QString &name)
{
	insertRow(layerPos);
	
	Private::LayerItem layer;
	layer.sound = true;
	d->layers.insert(layerPos, layer);
	
	fixSize();
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
	d->layers.removeAt(pos);
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
	if ( pos < 0 || pos > d->layers.count() )
	{
		return -1;
	}
	return d->layers[pos].lastItem;
}

// FRAMES


void KTFramesTable::insertFrame(int layerPos, const QString &name)
{
	if ( layerPos < 0 || layerPos >= d->layers.count() ) return;
	
	layerPos = verticalHeader()->logicalIndex(layerPos);
	
	d->layers[layerPos].lastItem++;
	
	if ( d->layers[layerPos].lastItem >= columnCount() )
	{
		insertColumn( d->layers[layerPos].lastItem );
	}
	
	setAttribute( layerPos, d->layers[layerPos].lastItem, KTFramesTableItem::IsUsed, true);
	if( d->layers[layerPos].sound )
	{
		setAttribute( layerPos, d->layers[layerPos].lastItem, KTFramesTableItem::IsSound, true);
	}
	
	
	
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
	
	if ( layerPos < 0 || layerPos >= d->layers.count() )
	{
		return;
	}
	
	layerPos = verticalHeader()->logicalIndex(layerPos);
	
	setAttribute( layerPos, d->layers[layerPos].lastItem, KTFramesTableItem::IsUsed, false );
	
	d->layers[layerPos].lastItem--;
	
// 	viewport()->update( visualRect(indexFromItem( item(layerPos, position) )) );
	viewport()->update();
}

void KTFramesTable::lockFrame(int layerPos, int position, bool lock)
{
	if ( layerPos < 0 || layerPos >= d->layers.count() )
	{
		return;
	}
	
	layerPos = verticalHeader()->logicalIndex(layerPos);
	
	setAttribute( layerPos, position, KTFramesTableItem::IsLocked, lock );
	
	viewport()->update();
}


void KTFramesTable::setAttribute(int row, int col, KTFramesTableItem::Attributes att, bool value)
{
	QTableWidgetItem *item = this->item(row, col);
	
	if( !item )
	{
		item = new KTFramesTableItem;
		setItem(row, col, item);
	}
	
	item->setData(att, value);
}

void KTFramesTable::fixSize()
{
	for(int column = 0; column < columnCount(); column++)
	{
		horizontalHeader()->resizeSection(column, d->rectWidth);
	}
	for( int row = 0; row < rowCount(); row++)
	{
		verticalHeader()->resizeSection(row, d->rectHeight);
	}
}

// void KTFramesTable::fixSectionMoved(int logical, int visual, int newVisual)
// {
// 	verticalHeader()->moveSection(newVisual, visual);
// }

