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

#ifndef KPAGEWIDGETMODEL_H
#define KPAGEWIDGETMODEL_H

#include <QtGui/QIcon>
#include <dcore/dglobal.h>


#include "dpagemodel.h"

class QWidget;

class D_GUI_EXPORT DPageWidgetItem
{
  public:
    DPageWidgetItem( QWidget *widget, const QString &name );
    ~DPageWidgetItem();

    QWidget* widget() const;

    QString name() const;

    void setHeader( const QString &header );
    QString header() const;

    void setIcon( const QIcon &icon );
    QIcon icon() const;

    void setIsCheckable( bool checkable );
    bool isCheckable() const;

  private:
    class Private;
    Private* const d;
};

class Q_GUI_EXPORT DPageWidgetModel : public DPageModel
{
  public:
    DPageWidgetModel( QObject *parent = 0 );
    ~DPageWidgetModel();

    /**
     * Adds a new top level page to the model.
     *
     * @param widget The widget of the page.
     * @param name The name which is displayed in the navigation view.
     *
     * @returns The associated @see DPageWidgetItem.
     */
    DPageWidgetItem* addPage( QWidget *widget, const QString &name );

    /**
     * Adds a new top level page to the model.
     *
     * @param item The @see DPageWidgetItem which describes the page.
     */
    void addPage( DPageWidgetItem *item );

    /**
     * Inserts a new page in the model.
     *
     * @param before The new page will be insert before this @see DPageWidgetItem
     *               on the same level in hierarchy.
     * @param widget The widget of the page.
     * @param name The name which is displayed in the navigation view.
     *
     * @returns The associated @see DPageWidgetItem.
     */
    DPageWidgetItem* insertPage( DPageWidgetItem *before, QWidget *widget, const QString &name );

    /**
     * Inserts a new page in the model.
     *
     * @param before The new page will be insert before this @see DPageWidgetItem
     *               on the same level in hierarchy.
     *
     * @param item The @see DPageWidgetItem which describes the page.
     */
    void insertPage( DPageWidgetItem *before, DPageWidgetItem *item );

    /**
     * Inserts a new sub page in the model.
     *
     * @param parent The new page will be insert as child of this @see DPageWidgetItem.
     * @param widget The widget of the page.
     * @param name The name which is displayed in the navigation view.
     *
     * @returns The associated @see DPageWidgetItem.
     */
    DPageWidgetItem* addSubPage( DPageWidgetItem *parent, QWidget *widget, const QString &name );

    /**
     * Inserts a new sub page in the model.
     *
     * @param parent The new page will be insert as child of this @see DPageWidgetItem.
     *
     * @param item The @see DPageWidgetItem which describes the page.
     */
    void addSubPage( DPageWidgetItem *parent, DPageWidgetItem *item );

    /**
     * Removes the page associated with the given @see DPageWidgetItem.
     */
    void removePage( DPageWidgetItem *item );

    /**
     * These methods are reimplemented from QAsbtractItemModel.
     */
    virtual int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    virtual Qt::ItemFlags flags( const QModelIndex &index ) const;
    virtual QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex &index ) const;
    virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const;

    /**
     * Returns the @see DPageWidgetItem for a given index or 0 if the index is invalid.
     */
    DPageWidgetItem* item( const QModelIndex &index );

    /**
     * Returns the index for a given @see DPageWidgetItem. The index is invalid if the
     * item can't be found in the model.
     */
    QModelIndex index( const DPageWidgetItem *item ) const;

  private:
    class Private;
    Private* const d;
};

#endif
