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

#ifndef KPAGEWIDGET_H
#define KPAGEWIDGET_H

#include "dpageview.h"
#include "dpagewidgetmodel.h"
#include <dcore/dglobal.h>


class D_GUI_EXPORT DPageWidget : public DPageView
{
  Q_OBJECT

  public:
    DPageWidget( QWidget *parent = 0 );
    ~DPageWidget();

    /**
     * Adds a new top level page to the widget.
     *
     * @param widget The widget of the page.
     * @param name The name which is displayed in the navigation view.
     *
     * @returns The associated @see DPageWidgetItem.
     */
    DPageWidgetItem* addPage( QWidget *widget, const QString &name );

    /**
     * Adds a new top level page to the widget.
     *
     * @param item The @see DPageWidgetItem which describes the page.
     */
    void addPage( DPageWidgetItem *item );

    /**
     * Inserts a new page in the widget.
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
     * Inserts a new page in the widget.
     *
     * @param before The new page will be insert before this @see DPageWidgetItem
     *               on the same level in hierarchy.
     *
     * @param item The @see DPageWidgetItem which describes the page.
     */
    void insertPage( DPageWidgetItem *before, DPageWidgetItem *item );

    /**
     * Inserts a new sub page in the widget.
     *
     * @param parent The new page will be insert as child of this @see DPageWidgetItem.
     * @param widget The widget of the page.
     * @param name The name which is displayed in the navigation view.
     *
     * @returns The associated @see DPageWidgetItem.
     */
    DPageWidgetItem* addSubPage( DPageWidgetItem *parent, QWidget *widget, const QString &name );

    /**
     * Inserts a new sub page in the widget.
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
     * Sets the page which is associated with the given @see DPageWidgetItem to
     * be the current page and emits the currentPageChanged() signal.
     */
    void setCurrentPage( DPageWidgetItem *item );

    /**
     * Returns the @see DPageWidgetItem for the current page or 0 if there is no
     * current page.
     */
    DPageWidgetItem* currentPage() const;

  Q_SIGNALS:
    /**
     * This signal is emitted whenever the current page has changed.
     *
     * @param item The new current page or 0 if no current page is available.
     */
    void currentPageChanged( DPageWidgetItem *current, DPageWidgetItem *before );

  private:
    class Private;
    Private* const d;

    Q_PRIVATE_SLOT( d, void slotCurrentPageChanged( const QModelIndex&, const QModelIndex& ) )
};

class DPageWidget::Private
{
	public:
		Private( DPageWidget *_parent )
	: parent( _parent )
		{
		}

		DPageWidgetModel *model;

		void slotCurrentPageChanged( const QModelIndex&, const QModelIndex& );

		DPageWidget *parent;
};

#endif
