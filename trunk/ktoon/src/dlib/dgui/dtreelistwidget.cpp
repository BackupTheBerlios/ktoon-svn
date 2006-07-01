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

#include "dtreelistwidget.h"
#include <QHeaderView>
#include <QItemDelegate>
#include <QEvent>
#include <QLineEdit>

#include "ddebug.h"

class DTreeListWidgetDelegate : public QItemDelegate
{
	public:
		DTreeListWidgetDelegate(QObject * parent = 0 );
		~DTreeListWidgetDelegate();
// 		virtual bool editorEvent ( QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index );
		
};

DTreeListWidgetDelegate::DTreeListWidgetDelegate(QObject * parent) : QItemDelegate(parent)
{
}

DTreeListWidgetDelegate::~DTreeListWidgetDelegate()
{
}

// bool DTreeListWidgetDelegate::editorEvent ( QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index )
// {
// // 	return QItemDelegate::editorEvent (event, model, option, index);
// 	return true;
// }



//////////////////

DTreeListWidget::DTreeListWidget(QWidget *parent) : QTreeWidget(parent)
{
	setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);
// 	setAcceptDrops(true);
// 	setDragEnabled(true);
	
	setHeaderLabels(QStringList() << "");
	
	header()->hide();
	setUniformRowHeights (true);
	
	setAlternatingRowColors(true);
	
	DTreeListWidgetDelegate *delegator = new DTreeListWidgetDelegate(this);
	
	setItemDelegate(delegator);
	
	connect(this, SIGNAL(itemDoubleClicked ( QTreeWidgetItem *, int )), this, SLOT(editDoubleClickedItem(QTreeWidgetItem *, int  )));
}


DTreeListWidget::~DTreeListWidget()
{
}

void DTreeListWidget::editDoubleClickedItem(QTreeWidgetItem *item, int col)
{
	if ( item && m_isEditable )
	{
		item->setFlags( item->flags() | Qt::ItemIsEditable );
		editItem(item, col);
	}
}

void DTreeListWidget::addItems(const QStringList &items)
{
	QStringList::ConstIterator it = items.begin();
	
	while(it != items.end())
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(this);
		item->setText(0, *it);
		++it;
	}
}

QList<QTreeWidgetItem *> DTreeListWidget::topLevelItems()
{
	QList<QTreeWidgetItem *> items;
	for ( uint i = 0; i < topLevelItemCount (); i++ )
	{
		items << topLevelItem(i);
	}
	
	return items;
}

void DTreeListWidget::setEditable(bool isEditable)
{
	m_isEditable = isEditable;
}

bool DTreeListWidget::isEditable() const
{
	return m_isEditable;
}

void DTreeListWidget::closeEditor ( QWidget * editor, QAbstractItemDelegate::EndEditHint hint )
{
	D_FUNCINFO;
	
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

void DTreeListWidget::removeAll()
{
	clear();
}


