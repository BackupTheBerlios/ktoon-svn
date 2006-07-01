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

#include "dpagewidget.h"



void DPageWidget::Private::slotCurrentPageChanged( const QModelIndex &current, const QModelIndex &before )
{
  DPageWidgetItem *currentItem = 0;
  if ( current.isValid() )
    currentItem = model->item( current );

  DPageWidgetItem *beforeItem = 0;
  if ( before.isValid() )
    beforeItem = model->item( before );

  emit parent->currentPageChanged( currentItem, beforeItem );
}

DPageWidget::DPageWidget( QWidget *parent )
  : DPageView( parent ), d( new Private( this ) )
{
  d->model = new DPageWidgetModel( this );

  connect( this, SIGNAL( currentPageChanged( const QModelIndex&, const QModelIndex& ) ),
           this, SLOT( slotCurrentPageChanged( const QModelIndex&, const QModelIndex& ) ) );

  setModel( d->model );
}

DPageWidget::~DPageWidget()
{
  delete d;
}

DPageWidgetItem* DPageWidget::addPage( QWidget *widget, const QString &name )
{
  return d->model->addPage( widget, name );
}

void DPageWidget::addPage( DPageWidgetItem *item )
{
  d->model->addPage( item );
}

DPageWidgetItem* DPageWidget::insertPage( DPageWidgetItem *before, QWidget *widget, const QString &name )
{
  return d->model->insertPage( before, widget, name );
}

void DPageWidget::insertPage( DPageWidgetItem *before, DPageWidgetItem *item )
{
  d->model->insertPage( before, item );
}

DPageWidgetItem* DPageWidget::addSubPage( DPageWidgetItem *parent, QWidget *widget, const QString &name )
{
  return d->model->addSubPage( parent, widget, name );
}

void DPageWidget::addSubPage( DPageWidgetItem *parent, DPageWidgetItem *item )
{
  d->model->addSubPage( parent, item );
}

void DPageWidget::removePage( DPageWidgetItem *item )
{
  d->model->removePage( item );
}

void DPageWidget::setCurrentPage( DPageWidgetItem *item )
{
  const QModelIndex index = d->model->index( item );
  if ( !index.isValid() )
    return;

  DPageView::setCurrentPage( index );
}

DPageWidgetItem* DPageWidget::currentPage() const
{
  const QModelIndex index = DPageView::currentPage();

  if ( !index.isValid() )
    return 0;

  return d->model->item( index );
}

