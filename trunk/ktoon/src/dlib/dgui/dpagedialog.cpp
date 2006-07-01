/*
 *  This file is part of the KDE Libraries
 *  Copyright (C) 1999-2001 Mirko Boehm (mirko@kde.org) and
 *  Espen Sand (espen@kde.org)
 *  Holger Freyther <freyther@kde.org>
 *  2005-2006 Olivier Goffart <ogoffart at kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#include <QTimer>
#include <QVBoxLayout>

#include "dpagedialog.h"

class DPageDialog::Private
{
  public:
    Private()
      : mPageWidget( 0 )
    {
    }

    DPageWidget *mPageWidget;
};

DPageDialog::DPageDialog( QWidget *parent, Qt::WFlags flags )
  : QDialog( parent, flags ),
    d( new Private )
{
  d->mPageWidget = new DPageWidget( this );

  connect( d->mPageWidget, SIGNAL( currentPageChanged( DPageWidgetItem*, DPageWidgetItem* ) ),
           this, SIGNAL( currentPageChanged( DPageWidgetItem*, DPageWidgetItem* ) ) );

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget( d->mPageWidget );
}

DPageDialog::~DPageDialog()
{
  delete d;
}

void DPageDialog::setFaceType( FaceType faceType )
{
  d->mPageWidget->setFaceType( (DPageWidget::FaceType)faceType );
}

DPageWidgetItem* DPageDialog::addPage( QWidget *widget, const QString &name )
{
  return d->mPageWidget->addPage( widget, name );
}

void DPageDialog::addPage( DPageWidgetItem *item )
{
  d->mPageWidget->addPage( item );
}

DPageWidgetItem* DPageDialog::insertPage( DPageWidgetItem *before, QWidget *widget, const QString &name )
{
  return d->mPageWidget->insertPage( before, widget, name );
}

void DPageDialog::insertPage( DPageWidgetItem *before, DPageWidgetItem *item )
{
  d->mPageWidget->insertPage( before, item );
}

DPageWidgetItem* DPageDialog::addSubPage( DPageWidgetItem *parent, QWidget *widget, const QString &name )
{
  return d->mPageWidget->addSubPage( parent, widget, name );
}

void DPageDialog::addSubPage( DPageWidgetItem *parent, DPageWidgetItem *item )
{
  d->mPageWidget->addSubPage( parent, item );
}

void DPageDialog::removePage( DPageWidgetItem *item )
{
  d->mPageWidget->removePage( item );
}

void DPageDialog::setCurrentPage( DPageWidgetItem *item )
{
  d->mPageWidget->setCurrentPage( item );
}

DPageWidgetItem* DPageDialog::currentPage() const
{
  return d->mPageWidget->currentPage();
}

void DPageDialog::delayedDestruct()
{
  if ( isVisible() )
    hide();

  QTimer::singleShot( 0, this, SLOT( slotDelayedDestruct() ) );
}

void DPageDialog::slotDelayedDestruct()
{
  delete this;
}

