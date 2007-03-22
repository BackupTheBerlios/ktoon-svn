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

#include <QApplication>
#include <QHeaderView>
#include <QPainter>
#include <QVBoxLayout>

#include "dpageview_p.h"

using namespace KDEPrivate;

/**
 * DPagePlainView
 */

DPagePlainView::DPagePlainView( QWidget *parent )
 : QAbstractItemView( parent )
{
  hide();
}

QModelIndex DPagePlainView::indexAt( const QPoint& ) const
{
  return QModelIndex();
}

void DPagePlainView::scrollTo( const QModelIndex&, ScrollHint )
{
}

QRect DPagePlainView::visualRect( const QModelIndex& ) const
{
  return QRect();
}

QModelIndex DPagePlainView::moveCursor(QAbstractItemView::CursorAction, Qt::KeyboardModifiers)
{
  return QModelIndex();
}

int DPagePlainView::horizontalOffset() const
{
  return 0;
}

int DPagePlainView::verticalOffset() const
{
  return 0;
}

bool DPagePlainView::isIndexHidden( const QModelIndex& ) const
{
  return false;
}

void DPagePlainView::setSelection( const QRect&, QFlags<QItemSelectionModel::SelectionFlag> )
{
}

QRegion DPagePlainView::visualRegionForSelection( const QItemSelection& ) const
{
  return QRegion();
}


/**
 * DPageListView
 */

DPageListView::DPageListView( QWidget *parent )
 : QListView( parent )
{
  setViewMode( QListView::ListMode );
  setMovement( QListView::Static );

  QFont boldFont( font() );
  boldFont.setBold( true );
  setFont( boldFont );

  setItemDelegate( new DPageListViewDelegate( this ) );
}

DPageListView::~DPageListView()
{
}

void DPageListView::setModel( QAbstractItemModel *model )
{
/*
  DPageListViewProxy *proxy = new DPageListViewProxy( this );
  proxy->setSourceModel( model );
  proxy->rebuildMap();

  connect( model, SIGNAL( layoutChanged() ), proxy, SLOT( rebuildMap() ) );
*/
  connect( model, SIGNAL( layoutChanged() ), this, SLOT( updateWidth() ) );

//  QListView::setModel( proxy );
  QListView::setModel( model );

  updateWidth();
}

void DPageListView::updateWidth()
{
  if ( !model() )
    return;

  int rows = model()->rowCount();

  int width = 0;
  for ( int i = 0; i < rows; ++i )
    width = qMax( width, sizeHintForIndex( model()->index( i, 0 ) ).width() );

  setFixedWidth( width + 20 );
}


/**
 * DPageTreeView
 */

DPageTreeView::DPageTreeView( QWidget *parent )
 : QTreeView( parent )
{
  header()->hide();
}

void DPageTreeView::setModel( QAbstractItemModel *model )
{
  connect( model, SIGNAL( layoutChanged() ), this, SLOT( updateWidth() ) );

  QTreeView::setModel( model );

  updateWidth();
}

void DPageTreeView::updateWidth()
{
  if ( !model() )
    return;

  int columns = model()->columnCount();

  int width = 0;
  for ( int i = 0; i < columns; ++i ) {
    resizeColumnToContents( i );
    width = qMax( width, sizeHintForColumn( i ) );
  }

  setFixedWidth( width + 20 );
}


/**
 * DPageTabbedView
 */

DPageTabbedView::DPageTabbedView( QWidget *parent )
  : QAbstractItemView( parent )
{
  // hide the viewport of the QAbstractScrollArea
  QList<QWidget*> list = findChildren<QWidget*>();
  for ( int i = 0; i < list.count(); ++i )
    list[ i ]->hide();

  setFrameShape( NoFrame );

  QVBoxLayout *layout = new QVBoxLayout( this );
  layout->setMargin( 0 );
  layout->setSpacing( 0 );

  mTabBar = new QTabBar( this );
  connect( mTabBar, SIGNAL( currentChanged( int ) ), this, SLOT( currentPageChanged( int ) ) );

  layout->addWidget( mTabBar, Qt::AlignBottom );
}

void DPageTabbedView::setModel( QAbstractItemModel *model )
{
  QAbstractItemView::setModel( model );

  connect( model, SIGNAL( layoutChanged() ), this, SLOT( layoutChanged() ) );

  layoutChanged();
}

QModelIndex DPageTabbedView::indexAt( const QPoint& ) const
{
  if ( model() )
    return model()->index( 0, 0 );
  else
    return QModelIndex();
}

void DPageTabbedView::scrollTo( const QModelIndex &index, ScrollHint )
{
  if ( !index.isValid() )
    return;

  mTabBar->setCurrentIndex( index.row() );
}

QRect DPageTabbedView::visualRect( const QModelIndex& ) const
{
  return QRect();
}

QModelIndex DPageTabbedView::moveCursor(QAbstractItemView::CursorAction, Qt::KeyboardModifiers)
{
  return QModelIndex();
}

int DPageTabbedView::horizontalOffset() const
{
  return 0;
}

int DPageTabbedView::verticalOffset() const
{
  return 0;
}

bool DPageTabbedView::isIndexHidden( const QModelIndex &index ) const
{
  return ( mTabBar->currentIndex() != index.row() );
}

void DPageTabbedView::setSelection( const QRect&, QFlags<QItemSelectionModel::SelectionFlag> )
{
}

QRegion DPageTabbedView::visualRegionForSelection( const QItemSelection& ) const
{
  return QRegion();
}

void DPageTabbedView::currentPageChanged( int index )
{
  if ( !model() )
    return;

  QModelIndex modelIndex = model()->index( index, 0 );

  selectionModel()->setCurrentIndex( modelIndex, QItemSelectionModel::Select );
}

void DPageTabbedView::layoutChanged()
{
  // save old position
  int pos = mTabBar->currentIndex();

  // clear tab bar
  int count = mTabBar->count();
  for ( int i = 0; i < count; ++i ) {
    mTabBar->removeTab( 0 );
  }

  if ( !model() )
    return;

  // add new tabs
  for ( int i = 0; i < model()->rowCount(); ++i )
    mTabBar->addTab( model()->data( model()->index( i, 0 ) ).toString() );

  mTabBar->setCurrentIndex( pos );

  setFixedHeight( mTabBar->minimumSizeHint().height() );
}

/**
 * DPageListViewDelegate
 */

DPageListViewDelegate::DPageListViewDelegate( QObject *parent )
 : QAbstractItemDelegate( parent )
{
}

void DPageListViewDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  if ( !index.isValid() )
    return;

  painter->setRenderHint( QPainter::Antialiasing );

  const QString text = index.model()->data( index, Qt::DisplayRole ).toString();
  const QIcon icon = index.model()->data( index, Qt::DecorationRole ).value<QIcon>();
  const QPixmap pixmap = icon.pixmap( 32, 32 );

  QFontMetrics fm = painter->fontMetrics();
  int ht = fm.boundingRect( 0, 0, 0, 0, Qt::AlignCenter, text ).height();
  int wt = fm.boundingRect( 0, 0, 0, 0, Qt::AlignCenter, text ).width();
  int wp = pixmap.width();
  int hp = pixmap.height();

  QPen pen = painter->pen();
  QPalette::ColorGroup cg = option.state & QStyle::State_Enabled
                            ? QPalette::Normal : QPalette::Disabled;
  if (cg == QPalette::Normal && !(option.state & QStyle::State_Active))
    cg = QPalette::Inactive;
  if (option.state & QStyle::State_Selected) {
    painter->fillRect(option.rect, option.palette.brush(cg, QPalette::Highlight));
    painter->setPen(option.palette.color(cg, QPalette::HighlightedText));
  } else {
    painter->setPen(option.palette.color(cg, QPalette::Text));
  }

  QFont font = painter->font();
  painter->setFont(option.font);

  painter->drawPixmap( option.rect.x() + (option.rect.width()/2)-(wp/2), option.rect.y() + 5, pixmap );
  if ( !text.isEmpty() )
    painter->drawText( option.rect.x() + (option.rect.width()/2)-(wt/2), option.rect.y() + hp+7, wt, ht, Qt::AlignCenter, text );

  painter->setFont(font);
  painter->setPen(pen);

  drawFocus( painter, option, option.rect );
}

QSize DPageListViewDelegate::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  if ( !index.isValid() )
    return QSize( 0, 0 );

  const QString text = index.model()->data( index, Qt::DisplayRole ).toString();
  const QIcon icon = index.model()->data( index, Qt::DecorationRole ).value<QIcon>();
  const QPixmap pixmap = icon.pixmap( 32, 32 );

  QFontMetrics fm = option.fontMetrics;
  int ht = fm.boundingRect( 0, 0, 0, 0, Qt::AlignCenter, text ).height();
  int wt = fm.boundingRect( 0, 0, 0, 0, Qt::AlignCenter, text ).width() + 10;
  int wp = pixmap.width() + 10;

  int width, height;
  if ( text.isEmpty() )
    height = pixmap.height();
  else
    height = pixmap.height() + ht + 10;

  width = qMax( wt, wp );

  return QSize( width, height );
}

void DPageListViewDelegate::drawFocus( QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect ) const
{
  if (option.state & QStyle::State_HasFocus) {
    QStyleOptionFocusRect o;
    o.QStyleOption::operator=(option);
    o.rect = rect;
    o.state |= QStyle::State_KeyboardFocusChange;
    QPalette::ColorGroup cg = (option.state & QStyle::State_Enabled)
                              ? QPalette::Normal : QPalette::Disabled;
    o.backgroundColor = option.palette.color(cg, (option.state & QStyle::State_Selected)
                                             ? QPalette::Highlight : QPalette::Background);
    QApplication::style()->drawPrimitive(QStyle::PE_FrameFocusRect, &o, painter);
  }
}


/**
 * DPageListViewProxy
 */

DPageListViewProxy::DPageListViewProxy( QObject *parent )
 : QAbstractProxyModel( parent )
{
}

DPageListViewProxy::~DPageListViewProxy()
{
}

int DPageListViewProxy::rowCount( const QModelIndex& ) const
{
  return mList.count();
}

int DPageListViewProxy::columnCount( const QModelIndex& ) const
{
  return 1;
}

QModelIndex DPageListViewProxy::index( int row, int column, const QModelIndex& ) const
{
  if ( column > 1 || row >= mList.count() )
    return QModelIndex();
  else
    return createIndex( row, column, mList[ row ].internalPointer() );
}

QModelIndex DPageListViewProxy::parent( const QModelIndex& ) const
{
  return QModelIndex();
}

QVariant DPageListViewProxy::data( const QModelIndex &index, int role ) const
{
  if ( !index.isValid() )
    return QVariant();

  if ( index.row() >= mList.count() )
    return QVariant();

  return sourceModel()->data( mList[ index.row() ], role );
}

QModelIndex DPageListViewProxy::mapFromSource( const QModelIndex &index ) const
{
  if ( !index.isValid() )
    return QModelIndex();

  for ( int i = 0; i < mList.count(); ++i ) {
    if ( mList[ i ] == index )
      return createIndex( i, 0, index.internalPointer() );
  }

  return QModelIndex();
}

QModelIndex DPageListViewProxy::mapToSource( const QModelIndex &index ) const
{
  if ( !index.isValid() )
    return QModelIndex();

  return mList[ index.row() ];
}

void DPageListViewProxy::rebuildMap()
{
  mList.clear();

  const QAbstractItemModel *model = sourceModel();
  if ( !model )
    return;

  for ( int i = 0; i < model->rowCount(); ++i ) {
    addMapEntry( model->index( i, 0 ) );
  }

  for ( int i = 0; i < mList.count(); ++i )
    qDebug( "%d:0 -> %d:%d", i, mList[ i ].row(), mList[ i ].column() );

  emit layoutChanged();
}

void DPageListViewProxy::addMapEntry( const QModelIndex &index )
{
  if ( sourceModel()->rowCount( index ) == 0 ) {
    mList.append( index );
  } else {
    const int count = sourceModel()->rowCount( index );
    for ( int i = 0; i < count; ++i )
      addMapEntry( sourceModel()->index( i, 0, index ) );
  }
}

