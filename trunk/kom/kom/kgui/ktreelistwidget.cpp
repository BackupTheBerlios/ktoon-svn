/***************************************************************************
 *   Project KOM: KToon Open Media library                                 *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *                                                                         *
 *   License:                                                              *
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
        KTreeListWidgetDelegate(QObject * parent = 0);
        ~KTreeListWidgetDelegate();
};

KTreeListWidgetDelegate::KTreeListWidgetDelegate(QObject * parent) : QItemDelegate(parent)
{
}

KTreeListWidgetDelegate::~KTreeListWidgetDelegate()
{
}

//////////////////

KTreeListWidget::KTreeListWidget(QWidget *parent) : QTreeWidget(parent)
{
    setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);
    setHeaderLabels(QStringList() << "");
    
    header()->hide();
    setUniformRowHeights (true);
    
    setAlternatingRowColors(true);
    
    KTreeListWidgetDelegate *delegator = new KTreeListWidgetDelegate(this);
    
    setItemDelegate(delegator);
    
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(editDoubleClickedItem(QTreeWidgetItem *, int)));
}

KTreeListWidget::~KTreeListWidget()
{
}

void KTreeListWidget::editDoubleClickedItem(QTreeWidgetItem *item, int col)
{
    if (item && m_isEditable) {
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        editItem(item, col);
    }
}

void KTreeListWidget::addItems(const QStringList &items)
{
    QStringList::ConstIterator it = items.begin();
    
    while (it != items.end()) {
           QTreeWidgetItem *item = new QTreeWidgetItem(this);
           item->setText(0, *it);
           ++it;
    }
}

QList<QTreeWidgetItem *> KTreeListWidget::topLevelItems()
{
    QList<QTreeWidgetItem *> items;
    for (int i = 0; i < topLevelItemCount (); i++)
         items << topLevelItem(i);
    
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

void KTreeListWidget::closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint)
{
    K_FUNCINFO;
    
    QLineEdit *edit = qobject_cast<QLineEdit *>(editor);

    if (edit) {
        QTreeWidgetItem *item = currentItem();
        if (item)
            emit itemRenamed(item);
    }

    QTreeWidget::closeEditor(editor, hint);
}

void KTreeListWidget::removeAll()
{
    clear();
}
