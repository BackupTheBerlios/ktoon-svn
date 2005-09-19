/***************************************************************************
 *   Copyright (C) 2004 by Fernando Jose Roldan Correa                     *
 *   froldan@toonka.com                                                    *
 *                                                                         *
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

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

/**
 * @file symboltable.h
 * @brief Include this file if you need the class SymbolTable
 */

#include "symbolitem.h"

/**
 * @brief Class that handles the table of symbols into the library dialog box
 *
 * <b>Date of Creation: September 15 - 2004.</b>\n
 */
class SymbolTable : public QListView
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a SymbolTable object.
     * @param parent The parent widget
     */
    SymbolTable( QWidget *parent );
    /**
     * @brief Default Destructor
     *
     * Destroys the SymbolTable object.
     */
    ~SymbolTable();

private:
    QWidget *parent_widget;
    bool dragging;
    bool valid_drag;
    SymbolItem *dragged_item;

signals:
    /**
     * This signal is emitted when an item has been released into a folder
     * @param item The released item
     */
    void released( SymbolItem *item );

public slots:

protected:
    /**
     * @brief Event for widget mouse press control
     *
     * Reimplemented from QListView.
     * @param mouse_event The input event
     */
    void contentsMousePressEvent( QMouseEvent *mouse_event );
    /**
     * @brief Event for widget mouse move control
     *
     * Reimplemented from QListView.
     * @param mouse_event The input event
     */
    void contentsMouseMoveEvent( QMouseEvent *mouse_event );
    /**
     * @brief Event for widget mouse release control
     *
     * Reimplemented from QListView.
     * @param mouse_event The input event
     */
    void contentsMouseReleaseEvent( QMouseEvent *mouse_event );
    /**
     * @brief Event for dialog box key pressing control
     *
     * Reimplemented from QWidget.
     * @param key_event The input event
     */
    void keyPressEvent( QKeyEvent *key_event );

};

#endif
