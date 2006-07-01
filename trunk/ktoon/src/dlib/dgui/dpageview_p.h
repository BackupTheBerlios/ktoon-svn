/*
    This file is part of the KDE Libraries

    Copyright (C) 2006 Tobias Koenig (tokoe@kde.org)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB. If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef KPAGEVIEW_P_H
#define KPAGEVIEW_P_H

#include <QAbstractItemDelegate>
#include <QAbstractProxyModel>
#include <QListView>
#include <QTabBar>
#include <QTreeView>

namespace KDEPrivate {

class DPageListViewDelegate;
class DPageListViewProxy;

class DPagePlainView : public QAbstractItemView
{
  public:
    DPagePlainView( QWidget *parent = 0 );

    virtual QModelIndex indexAt( const QPoint &point ) const;
    virtual void scrollTo( const QModelIndex &index, ScrollHint hint = EnsureVisible );
    virtual QRect visualRect( const QModelIndex & index ) const;

  protected:
    virtual QModelIndex moveCursor(QAbstractItemView::CursorAction, Qt::KeyboardModifiers);
    virtual int horizontalOffset() const;
    virtual int verticalOffset() const;
    virtual bool isIndexHidden( const QModelIndex& ) const;
    virtual void setSelection( const QRect&, QFlags<QItemSelectionModel::SelectionFlag> );
    virtual QRegion visualRegionForSelection( const QItemSelection& ) const;
};

class DPageListView : public QListView
{
  Q_OBJECT

  public:
    DPageListView( QWidget *parent = 0 );
    virtual ~DPageListView();

    virtual void setModel( QAbstractItemModel *model );

  private Q_SLOTS:
    void updateWidth();
};

class DPageTreeView : public QTreeView
{
  Q_OBJECT

  public:
    DPageTreeView( QWidget *parent = 0 );

    virtual void setModel( QAbstractItemModel *model );

  private Q_SLOTS:
    void updateWidth();
};

class DPageTabbedView : public QAbstractItemView
{
  Q_OBJECT

  public:
    DPageTabbedView( QWidget *parent = 0 );

    virtual void setModel( QAbstractItemModel *model );

    virtual QModelIndex indexAt( const QPoint &point ) const;
    virtual void scrollTo( const QModelIndex &index, ScrollHint hint = EnsureVisible );
    virtual QRect visualRect( const QModelIndex & index ) const;

  protected:
    virtual QModelIndex moveCursor(QAbstractItemView::CursorAction, Qt::KeyboardModifiers);
    virtual int horizontalOffset() const;
    virtual int verticalOffset() const;
    virtual bool isIndexHidden( const QModelIndex& ) const;
    virtual void setSelection( const QRect&, QFlags<QItemSelectionModel::SelectionFlag> );
    virtual QRegion visualRegionForSelection( const QItemSelection& ) const;

  private Q_SLOTS:
    void currentPageChanged( int );
    void layoutChanged();

  private:
    QTabBar *mTabBar;
};

class DPageListViewDelegate : public QAbstractItemDelegate
{
  public:
    DPageListViewDelegate( QObject *parent = 0 );

    virtual void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
    virtual QSize sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const;

  private:
    void drawFocus( QPainter*, const QStyleOptionViewItem&, const QRect& ) const;
};

/**
 * We need this proxy model to map the leaves of a tree-like model
 * to a one-level list model.
 */
class DPageListViewProxy : public QAbstractProxyModel
{
  Q_OBJECT

  public:
    DPageListViewProxy( QObject *parent = 0 );
    virtual ~DPageListViewProxy();

    virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    virtual QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& ) const;
    virtual QVariant data( const QModelIndex &index, int role ) const;
    virtual QModelIndex mapFromSource( const QModelIndex &index ) const;
    virtual QModelIndex mapToSource( const QModelIndex &index ) const;

  public Q_SLOTS:
    void rebuildMap();

  private:
    void addMapEntry( const QModelIndex& );

    QList<QModelIndex> mList;
};

}

#endif
