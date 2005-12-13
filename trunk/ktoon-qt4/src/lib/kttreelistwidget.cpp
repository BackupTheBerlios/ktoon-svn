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

#include "kttreelistwidget.h"
#include <QHeaderView>
#include <QItemDelegate>
#include <QEvent>
#include <QLineEdit>

#include "ktdebug.h"

class KTTreeListWidgetDelegate : public QItemDelegate
{
	public:
		KTTreeListWidgetDelegate(QObject * parent = 0 );
		~KTTreeListWidgetDelegate();
// 		virtual bool editorEvent ( QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index );
		
};

KTTreeListWidgetDelegate::KTTreeListWidgetDelegate(QObject * parent) : QItemDelegate(parent)
{
}

KTTreeListWidgetDelegate::~KTTreeListWidgetDelegate()
{
}

// bool KTTreeListWidgetDelegate::editorEvent ( QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index )
// {
// // 	return QItemDelegate::editorEvent (event, model, option, index);
// 	return true;
// }



//////////////////

KTTreeListWidget::KTTreeListWidget(QWidget *parent) : QTreeWidget(parent)
{
	setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);
	setAcceptDrops(true);
	setDragEnabled(true);
	
	setHeaderLabels(QStringList() << "");
	
	header()->hide();
	setUniformRowHeights (true);
	
	setAlternatingRowColors(true);
	
	KTTreeListWidgetDelegate *delegator = new KTTreeListWidgetDelegate(this);
	
	setItemDelegate(delegator);
	
	connect(this, SIGNAL(itemDoubleClicked ( QTreeWidgetItem *, int )), this, SLOT(editDoubleClickedItem(QTreeWidgetItem *, int  )));
}


KTTreeListWidget::~KTTreeListWidget()
{
}

void KTTreeListWidget::editDoubleClickedItem(QTreeWidgetItem *item, int col)
{
	if ( item )
	{
		item->setFlags( item->flags() | Qt::ItemIsEditable );
		editItem(item, col);
	}
}

void KTTreeListWidget::addItems(const QStringList &items)
{
	QStringList::ConstIterator it = items.begin();
	
	while(it != items.end())
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(this);
		item->setText(0, *it);
		++it;
	}
}

void KTTreeListWidget::closeEditor ( QWidget * editor, QAbstractItemDelegate::EndEditHint hint )
{
	KT_FUNCINFO;
	
	QLineEdit *edit = qobject_cast<QLineEdit *>(editor);

	if ( edit )
	{
		emit itemRenamed(edit->text());
	}
	
	QTreeWidget::closeEditor(editor, hint);
}

