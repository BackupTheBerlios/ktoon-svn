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

#ifndef LIBRARY_H
#define LIBRARY_H

/**
 * @file library.h
 * @brief Include this file if you need the class Library
 */

#include <qdialog.h>
#include <q3popupmenu.h>
#include <qgl.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qpixmap.h>
#include <qtoolbutton.h>
//Added by qt3to4:
#include <QKeyEvent>
#include <Q3PtrList>

#include "drawingarea.h"
#include "symbolview.h"
#include "symboltable.h"
#include "ktdialogbase.h"

class KToon;
class Folder;
class Item;

/**
 * @brief Class that handles the dialog box for management of the graphic component library
 *
 * <b>Date of Creation: September 14 - 2004.</b>\n
 * This is an useful dialog box to keep a set of the most used graphic components or groups of graphic components.
 * It provides a drag and drop operation where the user can drag the current displayed symbol and drop it into the
 * drawing area.
 */
class Library : public KTDialogBase
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a Library dialog box.
     * @param parent The parent widget
     * @param style Flags passed to the QDialog constructor
     * @param in_assigned_menu The assigned menu bar popup menu
     * @param id_assigned_item The assigned menu item from the assigned menu
     * @param share The QGLWidget which the SymbolView is going to share display lists with
     * @param assig_tb_button The assigned tool bar button
     */
			      Library( QWidget *parent,QGLWidget *share);//, WFlags style);//, QPopupMenu *in_assigned_menu, int id_assigned_item, QGLWidget *share, QToolButton *assig_tb_button );
    /**
     * @brief Default Destructor
     *
     * Destroys the Library dialog box.
     */
    ~Library();

    /**
     * @brief Adds a graphic to the symbol library
     *
     * @param graphic The graphic component that will be added
     * @param name A string to name the graphic as a symbol
     */
    void addSymbol( GLGraphicComponent *graphic, const QString &name );
    /**
     * @brief Gets the symbol view widget
     *
     * @return A pointer to the symbol view
     */
    SymbolView *getSymbolView() const;
    /**
     * @brief Loads the specified items into the library
     *
     * @param items The library items
     */
    void loadItems( Q3PtrList<Item> items );
    /**
     * @brief Loads the specified image as a symbol into the library
     *
     * @param file_name The image file name
     */
    void loadImageSymbol( const QString &file_name );

private:
	int image_count ,number_of_items;
    KToon *k_toon;

    //Icons
    QPixmap i_add_symbol, i_delete_symbol, i_insert_into_drawing_area, i_add_folder;

    //Interface Components
    QLabel *text_number_of_items;
    SymbolTable *table_symbols;
    QPushButton *add_symbol, *delete_symbol, *insert_into_drawing_area, *add_folder;
    SymbolView *symbol_view;

signals:

public slots:
    /**
     * @brief Adds the current graphic to Library
     *
     * @sa slotDeleteSymbol()
     */
    void slotAddSymbol();
    /**
     * @brief Removes the selected Symbol from Library
     *
     * @sa slotAddSymbol()
     */
    void slotDeleteSymbol();
    /**
     * @brief Performs operations when a symbol is selected
     */
    void slotSelectSymbol();
    /**
     * @brief Inserts the selected symbol into the drawing area
     */
    void slotInsertIntoDrawingArea();
    /**
     * @brief Adds a folder to the symbol list
     *
     * The folder is utilized to classify items
     */
    void slotAddFolder();
    /**
     * @brief Starts an item rename operation
     *
     * @param to_rename The item that is going to be renamed. It may be a graphic or a folder
     * @param point The point where the user performed a double click
     * @param col The column to rename (In this case is always 0 )
     */
    void slotStartRename( Q3ListViewItem *to_rename, const QPoint &point, int col );
    /**
     * @brief Updates the library data after an item deletion, drag or rename
     */
    void slotUpdateLibraryData();

protected:

    /**
     * @brief Updates the number of items label after an item deletion
     */
    void updateNumberOfItems();
    /**
     * @brief Creates a folder object from a SymbolItem
     *
     * @param item The SymbolItem
     * @return A new folder with the SymbolItem properties
     */
    Folder *createFolder( SymbolItem *item );
    /**
     * @brief Creates recursively an item folder hierarchy
     *
     * @param symbol_item The item in the symbol table that is going to be the top level or root item
     * @param folder The object that contains the hierarchy information
     */
    void createSubItems( SymbolItem *symbol_item, Folder *folder );
    /**
     * @brief Event for dialog box key pressing control
     *
     * Reimplemented from QWidget.
     * @param key_event The input event
     */
    void keyPressEvent( QKeyEvent *key_event );

};

#endif
