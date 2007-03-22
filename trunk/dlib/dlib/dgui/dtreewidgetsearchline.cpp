/* This file is part of the KDE libraries
   Copyright (c) 2003 Scott Wheeler <wheeler@kde.org>
   Copyright (c) 2005 Rafal Rzepecki <divide@users.sourceforge.net>
   Copyright (c) 2006 Hamish Rodda <rodda@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "dtreewidgetsearchline.h"

#include <QApplication>
#include <QTimer>
#include <QMenu>
#include <QLabel>
#include <QContextMenuEvent>
#include <QList>
#include <QToolButton>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QTreeWidget>

class DTreeWidgetSearchLine::DTreeWidgetSearchLinePrivate
{
public:
    DTreeWidgetSearchLinePrivate() :
        caseSensitive(Qt::CaseInsensitive),
        activeSearch(false),
        keepParentsVisible(true),
        canChooseColumns(true),
        queuedSearches(0) {}

    QList<QTreeWidget *> treeWidgets;
    Qt::CaseSensitivity caseSensitive;
    bool activeSearch;
    bool keepParentsVisible;
    bool canChooseColumns;
    QString search;
    int queuedSearches;
    QList<int> searchColumns;
};

////////////////////////////////////////////////////////////////////////////////
// public methods
////////////////////////////////////////////////////////////////////////////////

DTreeWidgetSearchLine::DTreeWidgetSearchLine(QWidget *parent, QTreeWidget *treeWidget) :
		DClickLineEdit(tr("Search"), parent)
{
    d = new DTreeWidgetSearchLinePrivate;

    connect(this, SIGNAL(textChanged(const QString &)),
            this, SLOT(queueSearch(const QString &)));

    setTreeWidget( treeWidget );
	if( !treeWidget)
			setEnabled(false);
}

DTreeWidgetSearchLine::DTreeWidgetSearchLine(QWidget *parent,
                                       const QList<QTreeWidget *> &treeWidgets) :
		DClickLineEdit(tr("Search"),parent)
{
    d = new DTreeWidgetSearchLinePrivate;

    connect(this, SIGNAL(textChanged(const QString &)),
            this, SLOT(queueSearch(const QString &)));

    setTreeWidgets( treeWidgets );
}


DTreeWidgetSearchLine::~DTreeWidgetSearchLine()
{
    delete d;
}

Qt::CaseSensitivity DTreeWidgetSearchLine::caseSensitivity() const
{
    return d->caseSensitive;
}

QList<int> DTreeWidgetSearchLine::searchColumns() const
{
    if (d->canChooseColumns)
        return d->searchColumns;
    else
        return QList<int>();
}

bool DTreeWidgetSearchLine::keepParentsVisible() const
{
    return d->keepParentsVisible;
}

QTreeWidget *DTreeWidgetSearchLine::treeWidget() const
{
    if ( d->treeWidgets.count() == 1 )
        return d->treeWidgets.first();
    else
        return 0;
}

const QList<QTreeWidget *> &DTreeWidgetSearchLine::treeWidgets() const
{
    return d->treeWidgets;
}


////////////////////////////////////////////////////////////////////////////////
// public slots
////////////////////////////////////////////////////////////////////////////////

void DTreeWidgetSearchLine::addTreeWidget(QTreeWidget *tw)
{
    if (tw) {
        connectTreeWidget(tw);

        d->treeWidgets.append(tw);
        setEnabled(!d->treeWidgets.isEmpty());

        checkColumns();
    }
}

void DTreeWidgetSearchLine::removeTreeWidget(QTreeWidget *tw)
{
    if (tw) {
        int idx = d->treeWidgets.indexOf(tw);

        if ( idx != -1 ) {
            d->treeWidgets.removeAt( idx );
            checkColumns();

            disconnectTreeWidget(tw);

            setEnabled(!d->treeWidgets.isEmpty());
        }
    }
}

void DTreeWidgetSearchLine::updateSearch(const QString &s)
{
    d->search = s.isNull() ? text() : s;

    foreach (QTreeWidget* treeWidget, d->treeWidgets)
        updateSearch( treeWidget );
}

void DTreeWidgetSearchLine::updateSearch(QTreeWidget *treeWidget)
{
    if(!treeWidget || !treeWidget->topLevelItemCount())
        return;


    // If there's a selected item that is visible, make sure that it's visible
    // when the search changes too (assuming that it still matches).

    QTreeWidgetItem *currentItem = treeWidget->currentItem();

    if (d->keepParentsVisible)
        for (int i = 0; i < treeWidget->topLevelItemCount(); ++i)
            checkItemParentsVisible(treeWidget->topLevelItem(i));
    else
        checkItemParentsNotVisible(treeWidget);

    if(currentItem)
        treeWidget->scrollToItem(currentItem);
}

void DTreeWidgetSearchLine::setCaseSensitivity(Qt::CaseSensitivity cs)
{
    if (d->caseSensitive != cs) {
        d->caseSensitive = cs;
        updateSearch();
    }
}

void DTreeWidgetSearchLine::setKeepParentsVisible(bool v)
{
    if (d->keepParentsVisible != v) {
        d->keepParentsVisible = v;
        updateSearch();
    }
}

void DTreeWidgetSearchLine::setSearchColumns(const QList<int> &columns)
{
    if (d->canChooseColumns)
        d->searchColumns = columns;
}

void DTreeWidgetSearchLine::setTreeWidget(QTreeWidget *tw)
{
    setTreeWidgets(QList<QTreeWidget *>());
    addTreeWidget(tw);
}

void DTreeWidgetSearchLine::setTreeWidgets(const QList<QTreeWidget *> &tw)
{
    foreach (QTreeWidget* treeWidget, d->treeWidgets)
        disconnectTreeWidget(treeWidget);

    d->treeWidgets = tw;

    foreach (QTreeWidget* treeWidget, d->treeWidgets)
        connectTreeWidget(treeWidget);

    checkColumns();
    setEnabled(!d->treeWidgets.isEmpty());
}

////////////////////////////////////////////////////////////////////////////////
// protected members
////////////////////////////////////////////////////////////////////////////////

bool DTreeWidgetSearchLine::itemMatches(const QTreeWidgetItem *item, const QString &s) const
{
    if(s.isEmpty())
        return true;

    // If the search column list is populated, search just the columns
    // specifified.  If it is empty default to searching all of the columns.

    if(!d->searchColumns.isEmpty()) {
        QList<int>::ConstIterator it = d->searchColumns.begin();
        for(; it != d->searchColumns.end(); ++it) {
            if(*it < item->treeWidget()->columnCount() &&
               item->text(*it).indexOf(s, 0, d->caseSensitive) >= 0)
                return true;
        }
    }
    else {
        for(int i = 0; i < item->treeWidget()->columnCount(); i++) {
            if(item->treeWidget()->columnWidth(i) > 0 &&
               item->text(i).indexOf(s, 0, d->caseSensitive) >= 0)
            {
                return true;
            }
        }
    }

    return false;
}

void DTreeWidgetSearchLine::contextMenuEvent( QContextMenuEvent*e )
{
	QMenu *popup = DClickLineEdit::createStandardContextMenu();

    if (d->canChooseColumns) {
        popup->addSeparator();
        QMenu *subMenu = popup->addMenu(tr("Search Columns"));

        QAction* allVisibleColumnsAction = subMenu->addAction(tr("All Visible Columns"), this, SLOT(slotAllVisibleColumns()));
        allVisibleColumnsAction->setCheckable(true);
        allVisibleColumnsAction->setChecked(!d->searchColumns.count());
        subMenu->addSeparator();

        bool allColumnsAreSearchColumns = true;

        QActionGroup* group = new QActionGroup(popup);
        group->setExclusive(false);
        connect(group, SIGNAL(triggered(QAction*)), SLOT(slotColumnActivated(QAction*)));

        QHeaderView* const header = d->treeWidgets.first()->header();
        for (int j = 0; j < header->count(); j++) {
            int i = header->logicalIndex(j);
            if (header->isSectionHidden(i))
                continue;

            QString columnText = d->treeWidgets.first()->headerItem()->text(i);
            QAction* columnAction = subMenu->addAction(d->treeWidgets.first()->headerItem()->icon(i), columnText);
            columnAction->setCheckable(true);
            columnAction->setChecked(d->searchColumns.isEmpty() || d->searchColumns.contains(i));
            columnAction->setData(i);
            columnAction->setActionGroup(group);

            if (d->searchColumns.isEmpty() || d->searchColumns.indexOf(i) != -1)
                columnAction->setChecked(true);
            else
                allColumnsAreSearchColumns = false;
        }
        allVisibleColumnsAction->setChecked(allColumnsAreSearchColumns);

        // searchColumnsMenuActivated() relies on one possible "all" representation
        if(allColumnsAreSearchColumns && !d->searchColumns.isEmpty())
            d->searchColumns.clear();
    }

    popup->exec( e->globalPos() );
    delete popup;
}

void DTreeWidgetSearchLine::slotAllVisibleColumns()
{
    if(d->searchColumns.isEmpty())
        d->searchColumns.append(0);
    else
        d->searchColumns.clear();

    updateSearch();
}

void DTreeWidgetSearchLine::slotColumnActivated(QAction* action)
{
    if (!action)
        return;

    bool ok;
    int column = action->data().toInt(&ok);
    if (!ok)
        return;

    if (action->isChecked()) {
        if (!d->searchColumns.isEmpty()) {
            if (!d->searchColumns.contains(column))
                d->searchColumns.append(column);
            if (d->searchColumns.count() == d->treeWidgets.first()->header()->count() - d->treeWidgets.first()->header()->hiddenSectionCount())
                d->searchColumns.clear();

        } else {
            d->searchColumns.append(column);
        }

    } else {
        if (d->searchColumns.isEmpty()) {
            QHeaderView* const header = d->treeWidgets.first()->header();
            for (int i = 0; i < header->count(); i++) {
                if (i != column && !header->isSectionHidden(i))
                    d->searchColumns.append(i);
            }

        } else if (d->searchColumns.contains(column)) {
            d->searchColumns.removeAll(column);
        }
    }

    updateSearch();
}

void DTreeWidgetSearchLine::connectTreeWidget(QTreeWidget *tw)
{
    connect(tw, SIGNAL(destroyed( QObject * )),
            this, SLOT(treeWidgetDeleted( QObject *)));
    connect(tw->model(), SIGNAL(rowsInserted(const QModelIndex&, int, int)),
            this, SLOT(rowsInserted(const QModelIndex&, int, int)));
}

void DTreeWidgetSearchLine::disconnectTreeWidget(QTreeWidget *tw)
{
    disconnect(tw, SIGNAL(destroyed( QObject * )),
            this, SLOT(treeWidgetDeleted( QObject *)));
    disconnect(tw->model(), SIGNAL(rowsInserted(const QModelIndex&, int, int)),
            this, SLOT(rowsInserted(const QModelIndex&, int, int)));
}

bool DTreeWidgetSearchLine::canChooseColumnsCheck()
{
    // This is true if either of the following is true:

    // there are no listviews connected
    if (d->treeWidgets.isEmpty())
        return false;

    const QTreeWidget *first = d->treeWidgets.first();

    const unsigned int numcols = first->columnCount();
    // the listviews have only one column,
    if (numcols < 2)
        return false;

    QStringList headers;
    for (unsigned int i = 0; i < numcols; ++i)
        headers.append(first->headerItem()->text(i));

    QList<QTreeWidget *>::ConstIterator it = d->treeWidgets.constBegin();
    for (++it /* skip the first one */; it !=d->treeWidgets.constEnd(); ++it) {
        // the listviews have different numbers of columns,
        if ((unsigned int) (*it)->columnCount() != numcols)
            return false;

        // the listviews differ in column labels.
        QStringList::ConstIterator jt;
        unsigned int i;
        for (i = 0, jt = headers.constBegin(); i < numcols; ++i, ++jt) {
                Q_ASSERT(jt != headers.constEnd());
                if ((*it)->headerItem()->text(i) != *jt)
                    return false;
            }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
// protected slots
////////////////////////////////////////////////////////////////////////////////

void DTreeWidgetSearchLine::queueSearch(const QString &search)
{
    d->queuedSearches++;
    d->search = search;
    QTimer::singleShot(200, this, SLOT(activateSearch()));
}

void DTreeWidgetSearchLine::activateSearch()
{
    --(d->queuedSearches);

    if(d->queuedSearches == 0)
        updateSearch(d->search);
}

////////////////////////////////////////////////////////////////////////////////
// private slots
////////////////////////////////////////////////////////////////////////////////

// Hack to make a protected method public
class QTreeWidgetWorkaround : public QTreeWidget
{
public:
    QTreeWidgetItem *itemFromIndex( const QModelIndex &index ) const {
        return QTreeWidget::itemFromIndex( index );
    }
};

void DTreeWidgetSearchLine::rowsInserted(const QModelIndex & parent, int start, int end) const
{
    QAbstractItemModel* model = qobject_cast<QAbstractItemModel*>(sender());
    if (!model) return;

    QTreeWidget* widget = 0L;
    foreach (QTreeWidget* tree, d->treeWidgets)
        if (tree->model() == model) {
            widget = tree;
            break;
        }

    if (!widget) return;

    QTreeWidgetWorkaround* widgetWorkaround = static_cast<QTreeWidgetWorkaround *>( widget );
    for (int i = start; i <= end; ++i) {
        if (QTreeWidgetItem* item = widgetWorkaround->itemFromIndex(model->index(i, 0, parent)))
            item->treeWidget()->setItemHidden(item, !itemMatches(item, text()));
    }
}

void DTreeWidgetSearchLine::treeWidgetDeleted(QObject *o)
{
    d->treeWidgets.removeAll(static_cast<QTreeWidget *>(o));
    setEnabled(d->treeWidgets.isEmpty());
}

////////////////////////////////////////////////////////////////////////////////
// private methods
////////////////////////////////////////////////////////////////////////////////

void DTreeWidgetSearchLine::checkColumns()
{
    d->canChooseColumns = canChooseColumnsCheck();
}

void DTreeWidgetSearchLine::checkItemParentsNotVisible(QTreeWidget *treeWidget)
{
    QTreeWidgetItemIterator it(treeWidget);
    for(; *it; ++it)
    {
        QTreeWidgetItem *item = *it;
        item->treeWidget()->setItemHidden(item, !itemMatches(item, d->search));
    }
}

/** Check whether \p item, its siblings and their descendents should be shown. Show or hide the items as necessary.
 *
 *  \p item  The list view item to start showing / hiding items at. Typically, this is the first child of another item, or the
 *              the first child of the list view.
 *  \return \c true if an item which should be visible is found, \c false if all items found should be hidden. If this function
 *             returns true and \p highestHiddenParent was not 0, highestHiddenParent will have been shown.
 */
bool DTreeWidgetSearchLine::checkItemParentsVisible(QTreeWidgetItem* item)
{
    QTreeWidget* treeWidget = item->treeWidget();

    bool childMatch = false;
    for (int i = 0; i < item->childCount(); ++i)
        childMatch |= checkItemParentsVisible(item->child(i));

    // Should this item be shown? It should if any children should be, or if it matches.
    if (childMatch || itemMatches(item, d->search)) {
        treeWidget->setItemHidden(item, false);
        return true;
    }

    treeWidget->setItemHidden(item, true);
    return false;
}

////////////////////////////////////////////////////////////////////////////////
// DTreeWidgetSearchLineWidget
////////////////////////////////////////////////////////////////////////////////

class DTreeWidgetSearchLineWidget::DTreeWidgetSearchLineWidgetPrivate
{
public:
    DTreeWidgetSearchLineWidgetPrivate() : treeWidget(0), searchLine(0), clearButton(0) {}
    QTreeWidget *treeWidget;
    DTreeWidgetSearchLine *searchLine;
    QToolButton *clearButton;
};

DTreeWidgetSearchLineWidget::DTreeWidgetSearchLineWidget(QWidget *parent, QTreeWidget *treeWidget)
    : QWidget(parent)
{
    d = new DTreeWidgetSearchLineWidgetPrivate;
    d->treeWidget = treeWidget;

    QTimer::singleShot(0, this, SLOT(createWidgets()));
}

DTreeWidgetSearchLineWidget::~DTreeWidgetSearchLineWidget()
{
    delete d;
}

DTreeWidgetSearchLine *DTreeWidgetSearchLineWidget::createSearchLine(QTreeWidget *treeWidget) const
{
    return new DTreeWidgetSearchLine(const_cast<DTreeWidgetSearchLineWidget*>(this), treeWidget);
}

void DTreeWidgetSearchLineWidget::createWidgets()
{
    if(!d->clearButton) 
    {
	   d->clearButton = new QToolButton(this);
	   d->clearButton->setText(tr("Clear"));
    }

    d->clearButton->show();

    searchLine()->show();

    connect(d->clearButton, SIGNAL(clicked()), d->searchLine, SLOT(clear()));

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(2);
    layout->setMargin(0);
    layout->addWidget(d->clearButton);
    layout->addWidget(d->searchLine);
}

DTreeWidgetSearchLine *DTreeWidgetSearchLineWidget::searchLine() const
{
    if (!d->searchLine)
        d->searchLine = createSearchLine(d->treeWidget);
    return d->searchLine;
}

void DTreeWidgetSearchLineWidget::setEraseIcon(const QIcon &icon)
{
	if ( d->clearButton )
	{
		d->clearButton->setText("");
		d->clearButton->setIcon(icon);
	}
}




