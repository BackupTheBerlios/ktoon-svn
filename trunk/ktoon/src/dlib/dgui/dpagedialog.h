/*
 *  This file is part of the KDE Libraries
 *  Copyright (C) 1999-2001 Mirko Boehm (mirko@kde.org) and
 *                          Espen Sand (espen@kde.org)
 *                          Holger Freyther <freyther@kde.org>
 *                2005-2006 Olivier Goffart <ogoffart at kde.org>
 *                     2006 Tobias Koenig <tokoe@kde.org>
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
#ifndef KPAGEDIALOG_H
#define KPAGEDIALOG_H

#include <QDialog>
#include <dpagewidget.h>
#include <dcore/dglobal.h>

/**
 * @short A dialog base class which can handle multiple pages.
 *
 * This class provides a dialog base class which handles multiple
 * pages and allows the user to switch between these pages in
 * different ways.
 *
 * Currently, @p Auto, @p Plain, @p List, @p Tree and @p Tabbed face
 * types are available (@see DPageView).
 *
 * <b>Example:</b>\n
 *
 * \code
 * UrlDlg::UrlDlg( QWidget *parent )
 *   : DPageDialog( parent )
 * {
 *   setFaceType( List );
 *
 *   QLabel *label = new QLabel( "Test Page" );
 *   addPage( label, i18n( "My Test Page" );
 *
 *   label = new QLabel( "Second Test Page" );
 *   DPageWidgetItem *page = new DPageWidgetItem( label, i18n( "My Second Test Page" ) );
 *   page->setHeader( i18n( "My header string" ) );
 *   page->setIcon( KIcon( "file" ) );
 *
 *   addPage( page );
 * }
 * \endcode
 *
 * @author Tobias Koenig (tokoe@kde.org)
 */
class D_GUI_EXPORT DPageDialog : public QDialog
{
  Q_OBJECT

  public:

    /**
     *  @li @p Auto   - A dialog with a face based on the structure of the
     *                  available pages.
     *                  If only a single page is added, the dialog behaves like
     *                  in @p Plain mode, with multiple pages whithout sub pages
     *                  it behaves like in @p List mode and like in @p Tree mode
     *                  otherwise.
     *  @li @p Plain  - A normal dialog.
     *  @li @p List   - A dialog with an icon list on the left side and a
     *                  representation of the contents on the right side.
     *  @li @p Tree   - A dialog with a tree on the left side and a
     *                  representation of the contents on the right side.
     *  @li @p Tabbed - A dialog with a tab bar above the representation
     *                  of the contents.
     */
    enum FaceType
    {
      Auto   = DPageWidget::Auto,
      Plain  = DPageWidget::Plain,
      List   = DPageWidget::List,
      Tree   = DPageWidget::Tree,
      Tabbed = DPageWidget::Tabbed
    };

  public:
    /**
     * Creates a new page dialog.
     */
    DPageDialog( QWidget *parent = 0, Qt::WFlags flags = 0 );

    /**
     * Destroys the page dialog.
     */
    ~DPageDialog();

    /**
     * Sets the face type of the dialog.
     */
    void setFaceType( FaceType faceType );

    /**
     * Adds a new top level page to the dialog.
     *
     * @param widget The widget of the page.
     * @param name The name which is displayed in the navigation view.
     *
     * @returns The associated @see DPageWidgetItem.
     */
    DPageWidgetItem* addPage( QWidget *widget, const QString &name );

    /**
     * Adds a new top level page to the dialog.
     *
     * @param item The @see DPageWidgetItem which describes the page.
     */
    void addPage( DPageWidgetItem *item );

    /**
     * Inserts a new page in the dialog.
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
     * Inserts a new page in the dialog.
     *
     * @param before The new page will be insert before this @see DPageWidgetItem
     *               on the same level in hierarchy.
     *
     * @param item The @see DPageWidgetItem which describes the page.
     */
    void insertPage( DPageWidgetItem *before, DPageWidgetItem *item );

    /**
     * Inserts a new sub page in the dialog.
     *
     * @param parent The new page will be insert as child of this @see DPageWidgetItem.
     * @param widget The widget of the page.
     * @param name The name which is displayed in the navigation view.
     *
     * @returns The associated @see DPageWidgetItem.
     */
    DPageWidgetItem* addSubPage( DPageWidgetItem *parent, QWidget *widget, const QString &name );

    /**
     * Inserts a new sub page in the dialog.
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


  public Q_SLOTS:
    /**
     * Destruct the dialog delayed.
     *
     * You can call this function from slots like closeClicked() and hidden().
     * You should not use the dialog any more after calling this function.
     */
    void delayedDestruct();


  protected Q_SLOTS:
    /**
     * Deletes the dialog immediately. If you want to delete the dialog
     * delayed use delayedDestruct() or QObject::deleteLater().
     *
     * Attention: Do no use connect this slot to signals from user
     * actions!
     */
    void slotDelayedDestruct();

  private:
    class Private;
    Private* const d;
};

#endif
