/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#include "ktreelistwidget.h"
#include <QHeaderView>
#include <QItemDelegate>
#include <QEvent>
#include <QLineEdit>

#include "kdebug.h"

class KTreeListWidgetDelegate : public QItemDelegate
{
	public:
		KTreeListWidgetDelegate(QObject * parent = 0 );
		~KTreeListWidgetDelegate();
// 		virtual bool editorEvent ( QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index );
		
};

KTreeListWidgetDelegate::KTreeListWidgetDelegate(QObject * parent) : QItemDelegate(parent)
{
}

KTreeListWidgetDelegate::~KTreeListWidgetDelegate()
{
}

// bool KTreeListWidgetDelegate::editorEvent ( QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index )
// {
// // 	return QItemDelegate::editorEvent (event, model, option, index);
// 	return true;
// }



//////////////////

KTreeListWidget::KTreeListWidget(QWidget *parent) : QTreeWidget(parent)
{
	setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);
// 	setAcceptDrops(true);
// 	setDragEnabled(true);
	
	setHeaderLabels(QStringList() << "");
	
	header()->hide();
	setUniformRowHeights (true);
	
	setAlternatingRowColors(true);
	
	KTreeListWidgetDelegate *delegator = new KTreeListWidgetDelegate(this);
	
	setItemDelegate(delegator);
	
	connect(this, SIGNAL(itemDoubleClicked ( QTreeWidgetItem *, int )), this, SLOT(editDoubleClickedItem(QTreeWidgetItem *, int  )));
}


KTreeListWidget::~KTreeListWidget()
{
}

void KTreeListWidget::editDoubleClickedItem(QTreeWidgetItem *item, int col)
{
	if ( item && m_isEditable )
	{
		item->setFlags( item->flags() | Qt::ItemIsEditable );
		editItem(item, col);
	}
}

void KTreeListWidget::addItems(const QStringList &items)
{
	QStringList::ConstIterator it = items.begin();
	
	while(it != items.end())
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(this);
		item->setText(0, *it);
		++it;
	}
}

QList<QTreeWidgetItem *> KTreeListWidget::topLevelItems()
{
	QList<QTreeWidgetItem *> items;
	for ( int i = 0; i < topLevelItemCount (); i++ )
	{
		items << topLevelItem(i);
	}
	
	return items;
}

void KTreeListWidget::setEditable(bool isEditable)
{
	m_isEditable = isEditable;
}

bool KTreeListWidget::isEditable() const
{
	return m_isEditable;
}

void KTreeListWidget::closeEditor ( QWidget * editor, QAbstractItemDelegate::EndEditHint hint )
{
	K_FUNCINFO;
	
	QLineEdit *edit = qobject_cast<QLineEdit *>(editor);
	if ( edit )
	{
		QTreeWidgetItem *item = currentItem();
		if ( item )
		{
			emit itemRenamed(item);
		}
	}
	QTreeWidget::closeEditor(editor, hint);
}

void KTreeListWidget::removeAll()
{
	clear();
}


