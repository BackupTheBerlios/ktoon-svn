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
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "ktreewidgetsearchline.h"

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

class KTreeWidgetSearchLine::KTreeWidgetSearchLinePrivate
{
    public:
        KTreeWidgetSearchLinePrivate() :
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

KTreeWidgetSearchLine::KTreeWidgetSearchLine(QWidget *parent, QTreeWidget *treeWidget) :
        KClickLineEdit(tr("Search"), parent)
{
    k = new KTreeWidgetSearchLinePrivate;

    connect(this, SIGNAL(textChanged(const QString &)),
            this, SLOT(queueSearch(const QString &)));

    setTreeWidget(treeWidget);

    if (!treeWidget)
        setEnabled(false);
}

KTreeWidgetSearchLine::KTreeWidgetSearchLine(QWidget *parent,
                       const QList<QTreeWidget *> &treeWidgets) :
                       KClickLineEdit(tr("Search"),parent)
{
    k = new KTreeWidgetSearchLinePrivate;

    connect(this, SIGNAL(textChanged(const QString &)),
            this, SLOT(queueSearch(const QString &)));

    setTreeWidgets(treeWidgets);
}


KTreeWidgetSearchLine::~KTreeWidgetSearchLine()
{
    delete k;
}

Qt::CaseSensitivity KTreeWidgetSearchLine::caseSensitivity() const
{
    return k->caseSensitive;
}

QList<int> KTreeWidgetSearchLine::searchColumns() const
{
    if (k->canChooseColumns)
        return k->searchColumns;
    else
        return QList<int>();
}

bool KTreeWidgetSearchLine::keepParentsVisible() const
{
    return k->keepParentsVisible;
}

QTreeWidget *KTreeWidgetSearchLine::treeWidget() const
{
    if (k->treeWidgets.count() == 1)
        return k->treeWidgets.first();
    else
        return 0;
}

const QList<QTreeWidget *> &KTreeWidgetSearchLine::treeWidgets() const
{
    return k->treeWidgets;
}

////////////////////////////////////////////////////////////////////////////////
// public slots
////////////////////////////////////////////////////////////////////////////////

void KTreeWidgetSearchLine::addTreeWidget(QTreeWidget *tw)
{
    if (tw) {
        connectTreeWidget(tw);

        k->treeWidgets.append(tw);
        setEnabled(!k->treeWidgets.isEmpty());

        checkColumns();
    }
}

void KTreeWidgetSearchLine::removeTreeWidget(QTreeWidget *tw)
{
    if (tw) {
        int idx = k->treeWidgets.indexOf(tw);

        if (idx != -1) {
            k->treeWidgets.removeAt(idx);
            checkColumns();

            disconnectTreeWidget(tw);

            setEnabled(!k->treeWidgets.isEmpty());
        }
    }
}

void KTreeWidgetSearchLine::updateSearch(const QString &s)
{
    k->search = s.isNull() ? text() : s;

    foreach (QTreeWidget* treeWidget, k->treeWidgets)
             updateSearch(treeWidget);
}

void KTreeWidgetSearchLine::updateSearch(QTreeWidget *treeWidget)
{
    if (!treeWidget || !treeWidget->topLevelItemCount())
        return;


    // If there's a selected item that is visible, make sure that it's visible
    // when the search changes too (assuming that it still matches).

    QTreeWidgetItem *currentItem = treeWidget->currentItem();

    if (k->keepParentsVisible)
        for (int i = 0; i < treeWidget->topLevelItemCount(); ++i)
            checkItemParentsVisible(treeWidget->topLevelItem(i));
    else
        checkItemParentsNotVisible(treeWidget);

    if(currentItem)
        treeWidget->scrollToItem(currentItem);
}

void KTreeWidgetSearchLine::setCaseSensitivity(Qt::CaseSensitivity cs)
{
    if (k->caseSensitive != cs) {
        k->caseSensitive = cs;
        updateSearch();
    }
}

void KTreeWidgetSearchLine::setKeepParentsVisible(bool v)
{
    if (k->keepParentsVisible != v) {
        k->keepParentsVisible = v;
        updateSearch();
    }
}

void KTreeWidgetSearchLine::setSearchColumns(const QList<int> &columns)
{
    if (k->canChooseColumns)
        k->searchColumns = columns;
}

void KTreeWidgetSearchLine::setTreeWidget(QTreeWidget *tw)
{
    setTreeWidgets(QList<QTreeWidget *>());
    addTreeWidget(tw);
}

void KTreeWidgetSearchLine::setTreeWidgets(const QList<QTreeWidget *> &tw)
{
    foreach (QTreeWidget* treeWidget, k->treeWidgets)
             disconnectTreeWidget(treeWidget);

    k->treeWidgets = tw;

    foreach (QTreeWidget* treeWidget, k->treeWidgets)
             connectTreeWidget(treeWidget);

    checkColumns();
    setEnabled(!k->treeWidgets.isEmpty());
}

////////////////////////////////////////////////////////////////////////////////
// protected members
////////////////////////////////////////////////////////////////////////////////

bool KTreeWidgetSearchLine::itemMatches(const QTreeWidgetItem *item, const QString &s) const
{
    if (s.isEmpty())
        return true;

    // If the search column list is populated, search just the columns
    // specifified.  If it is empty default to searching all of the columns.

    if (!k->searchColumns.isEmpty()) {
        QList<int>::ConstIterator it = k->searchColumns.begin();
        for (; it != k->searchColumns.end(); ++it) {
             if (*it < item->treeWidget()->columnCount() &&
                 item->text(*it).indexOf(s, 0, k->caseSensitive) >= 0)
                 return true;
        }
    }
    else {
        for (int i = 0; i < item->treeWidget()->columnCount(); i++) {
             if (item->treeWidget()->columnWidth(i) > 0 &&
                 item->text(i).indexOf(s, 0, k->caseSensitive) >= 0) {
                 return true;
             }
        }
    }

    return false;
}

void KTreeWidgetSearchLine::contextMenuEvent( QContextMenuEvent*e )
{
    QMenu *popup = KClickLineEdit::createStandardContextMenu();

    if (k->canChooseColumns) {
        popup->addSeparator();
        QMenu *subMenu = popup->addMenu(tr("Search Columns"));

        QAction* allVisibleColumnsAction = subMenu->addAction(tr("All Visible Columns"), this, SLOT(slotAllVisibleColumns()));
        allVisibleColumnsAction->setCheckable(true);
        allVisibleColumnsAction->setChecked(!k->searchColumns.count());
        subMenu->addSeparator();

        bool allColumnsAreSearchColumns = true;

        QActionGroup* group = new QActionGroup(popup);
        group->setExclusive(false);
        connect(group, SIGNAL(triggered(QAction*)), SLOT(slotColumnActivated(QAction*)));

        QHeaderView* const header = k->treeWidgets.first()->header();

        for (int j = 0; j < header->count(); j++) {
            int i = header->logicalIndex(j);
            if (header->isSectionHidden(i))
                continue;

            QString columnText = k->treeWidgets.first()->headerItem()->text(i);
            QAction* columnAction = subMenu->addAction(k->treeWidgets.first()->headerItem()->icon(i), columnText);
            columnAction->setCheckable(true);
            columnAction->setChecked(k->searchColumns.isEmpty() || k->searchColumns.contains(i));
            columnAction->setData(i);
            columnAction->setActionGroup(group);

            if (k->searchColumns.isEmpty() || k->searchColumns.indexOf(i) != -1)
                columnAction->setChecked(true);
            else
                allColumnsAreSearchColumns = false;
        }

        allVisibleColumnsAction->setChecked(allColumnsAreSearchColumns);

        // searchColumnsMenuActivated() relies on one possible "all" representation
        if(allColumnsAreSearchColumns && !k->searchColumns.isEmpty())
            k->searchColumns.clear();
    }

    popup->exec(e->globalPos());
    delete popup;
}

void KTreeWidgetSearchLine::slotAllVisibleColumns()
{
    if (k->searchColumns.isEmpty())
        k->searchColumns.append(0);
    else
        k->searchColumns.clear();

    updateSearch();
}

void KTreeWidgetSearchLine::slotColumnActivated(QAction* action)
{
    if (!action)
        return;

    bool ok;
    int column = action->data().toInt(&ok);

    if (!ok)
        return;

    if (action->isChecked()) {
        if (!k->searchColumns.isEmpty()) {
            if (!k->searchColumns.contains(column))
                k->searchColumns.append(column);
            if (k->searchColumns.count() == k->treeWidgets.first()->header()->count() - k->treeWidgets.first()->header()->hiddenSectionCount())
                k->searchColumns.clear();

        } else {
            k->searchColumns.append(column);
        }

    } else {
        if (k->searchColumns.isEmpty()) {
            QHeaderView* const header = k->treeWidgets.first()->header();
            for (int i = 0; i < header->count(); i++) {
                 if (i != column && !header->isSectionHidden(i))
                    k->searchColumns.append(i);
            }

        } else if (k->searchColumns.contains(column)) {
                   k->searchColumns.removeAll(column);
        }
    }

    updateSearch();
}

void KTreeWidgetSearchLine::connectTreeWidget(QTreeWidget *tw)
{
    connect(tw, SIGNAL(destroyed( QObject * )),
            this, SLOT(treeWidgetDeleted( QObject *)));

    connect(tw->model(), SIGNAL(rowsInserted(const QModelIndex&, int, int)),
            this, SLOT(rowsInserted(const QModelIndex&, int, int)));
}

void KTreeWidgetSearchLine::disconnectTreeWidget(QTreeWidget *tw)
{
    disconnect(tw, SIGNAL(destroyed( QObject * )),
               this, SLOT(treeWidgetDeleted( QObject *)));

    disconnect(tw->model(), SIGNAL(rowsInserted(const QModelIndex&, int, int)),
               this, SLOT(rowsInserted(const QModelIndex&, int, int)));
}

bool KTreeWidgetSearchLine::canChooseColumnsCheck()
{
    // This is true if either of the following is true:

    // there are no listviews connected
    if (k->treeWidgets.isEmpty())
        return false;

    const QTreeWidget *first = k->treeWidgets.first();

    const unsigned int numcols = first->columnCount();

    // the listviews have only one column,
    if (numcols < 2)
        return false;

    QStringList headers;
    for (unsigned int i = 0; i < numcols; ++i)
         headers.append(first->headerItem()->text(i));

    QList<QTreeWidget *>::ConstIterator it = k->treeWidgets.constBegin();
    for (++it /* skip the first one */; it !=k->treeWidgets.constEnd(); ++it) {
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

void KTreeWidgetSearchLine::queueSearch(const QString &search)
{
    k->queuedSearches++;
    k->search = search;
    QTimer::singleShot(200, this, SLOT(activateSearch()));
}

void KTreeWidgetSearchLine::activateSearch()
{
    --(k->queuedSearches);

    if (k->queuedSearches == 0)
        updateSearch(k->search);
}

////////////////////////////////////////////////////////////////////////////////
// private slots
////////////////////////////////////////////////////////////////////////////////

// Hack to make a protected method public
class QTreeWidgetWorkaround : public QTreeWidget
{
    public:
        QTreeWidgetItem *itemFromIndex(const QModelIndex &index) const 
        {
            return QTreeWidget::itemFromIndex(index);
        }
};

void KTreeWidgetSearchLine::rowsInserted(const QModelIndex & parent, int start, int end) const
{
    QAbstractItemModel* model = qobject_cast<QAbstractItemModel*>(sender());

    if (!model) 
        return;

    QTreeWidget* widget = 0L;
    foreach (QTreeWidget* tree, k->treeWidgets)
             if (tree->model() == model) {
                 widget = tree;
                 break;
             }

    if (!widget) 
        return;

    QTreeWidgetWorkaround* widgetWorkaround = static_cast<QTreeWidgetWorkaround *>( widget );
    for (int i = start; i <= end; ++i) {
         if (QTreeWidgetItem* item = widgetWorkaround->itemFromIndex(model->index(i, 0, parent)))
             item->treeWidget()->setItemHidden(item, !itemMatches(item, text()));
    }
}

void KTreeWidgetSearchLine::treeWidgetDeleted(QObject *o)
{
    k->treeWidgets.removeAll(static_cast<QTreeWidget *>(o));
    setEnabled(k->treeWidgets.isEmpty());
}

////////////////////////////////////////////////////////////////////////////////
// private methods
////////////////////////////////////////////////////////////////////////////////

void KTreeWidgetSearchLine::checkColumns()
{
    k->canChooseColumns = canChooseColumnsCheck();
}

void KTreeWidgetSearchLine::checkItemParentsNotVisible(QTreeWidget *treeWidget)
{
    QTreeWidgetItemIterator it(treeWidget);

    for (; *it; ++it) {
         QTreeWidgetItem *item = *it;
         item->treeWidget()->setItemHidden(item, !itemMatches(item, k->search));
    }
}

/** Check whether \p item, its siblings and their descendents should be shown. 
 *  Show or hide the items as necessary.
 *
 *  \p item  The list view item to start showing / hiding items at. Typically, 
 *           this is the first child of another item, or the the first child 
 *           of the list view.
 *  \return \c true if an item which should be visible is found, \c false if 
 *           all items found should be hidden. If this function returns true 
 *           and \p highestHiddenParent was not 0, highestHiddenParent will 
 *           have been shown.
 */

bool KTreeWidgetSearchLine::checkItemParentsVisible(QTreeWidgetItem* item)
{
    QTreeWidget* treeWidget = item->treeWidget();

    bool childMatch = false;
    for (int i = 0; i < item->childCount(); ++i)
         childMatch |= checkItemParentsVisible(item->child(i));

    // Should this item be shown? It should if any children should be, or if it matches.
    if (childMatch || itemMatches(item, k->search)) {
        treeWidget->setItemHidden(item, false);
        return true;
    }

    treeWidget->setItemHidden(item, true);

    return false;
}

////////////////////////////////////////////////////////////////////////////////
// KTreeWidgetSearchLineWidget
////////////////////////////////////////////////////////////////////////////////

class KTreeWidgetSearchLineWidget::KTreeWidgetSearchLineWidgetPrivate
{
    public:
        KTreeWidgetSearchLineWidgetPrivate() : treeWidget(0), searchLine(0), clearButton(0) {}
        QTreeWidget *treeWidget;
        KTreeWidgetSearchLine *searchLine;
        QToolButton *clearButton;
};

KTreeWidgetSearchLineWidget::KTreeWidgetSearchLineWidget(QWidget *parent, QTreeWidget *treeWidget)
    : QWidget(parent)
{
    k = new KTreeWidgetSearchLineWidgetPrivate;
    k->treeWidget = treeWidget;

    QTimer::singleShot(0, this, SLOT(createWidgets()));
}

KTreeWidgetSearchLineWidget::~KTreeWidgetSearchLineWidget()
{
    delete k;
}

KTreeWidgetSearchLine *KTreeWidgetSearchLineWidget::createSearchLine(QTreeWidget *treeWidget) const
{
    return new KTreeWidgetSearchLine(const_cast<KTreeWidgetSearchLineWidget*>(this), treeWidget);
}

void KTreeWidgetSearchLineWidget::createWidgets()
{
    if (!k->clearButton) {
        k->clearButton = new QToolButton(this);
        k->clearButton->setText(tr("Clear"));
    }

    k->clearButton->show();

    searchLine()->show();

    connect(k->clearButton, SIGNAL(clicked()), k->searchLine, SLOT(clear()));

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(2);
    layout->setMargin(0);
    layout->addWidget(k->clearButton);
    layout->addWidget(k->searchLine);
}

KTreeWidgetSearchLine *KTreeWidgetSearchLineWidget::searchLine() const
{
    if (!k->searchLine)
        k->searchLine = createSearchLine(k->treeWidget);

    return k->searchLine;
}

void KTreeWidgetSearchLineWidget::setEraseIcon(const QIcon &icon)
{
    if (k->clearButton) {
        k->clearButton->setText("");
        k->clearButton->setIcon(icon);
    }
}
