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

#ifndef SYMBOL_ITEM_H
#define SYMBOL_ITEM_H

/**
 * @file symbolitem.h
 * @brief Include this file if you need the class SymbolItem
 */

#include <qlistview.h>
#include <qpixmap.h>

class SymbolTable;
class GLGraphicComponent;

/**
 * @brief Class that handles an item within the table of symbols
 *
 * It may be a folder or a graphic item.
 * <b>Date of Creation: September 16 - 2004.</b>\n
 */
class SymbolItem : public QListViewItem
{

public:
    //! Constants for defining the kind of item
    enum SymbolItemKind
    {
        FOLDER = 1, /**< Declares a folder item */
        GRAPHIC = 2, /**< Declares a graphic item */
	UNDEFINED = 3 /**< Declares an undefined item */
    };

    /**
     * @brief First Constructor
     *
     * Constructs a SymbolItem object.
     * @param parent The table of symbols which this item will belong to
     * @param text The text that will be displayed
     */
    SymbolItem( SymbolTable *parent, const QString &text );
    /**
     * @brief Second Constructor
     *
     * Constructs a SymbolItem object.
     * @param parent The symbol item (it must be a folder) which this item will belong to
     * @param text The text that will be displayed
     */
    SymbolItem( SymbolItem *parent, const QString &text );
    /**
     * @brief Default Destructor
     *
     * Destroys the SymbolItem object.
     */
    ~SymbolItem();

    /**
     * @brief Gets the kind of this item
     *
     * @return SymbolItem::FOLDER if this item is a folder, SymbolItem::GRAPHIC if this item is a graphic,
     *         or SymbolItem::UNDEFINED if its kind is undefined
     */
    int getKind() const;
    /**
     * @brief Sets the kind of this item
     *
     * @param kind The kind
     */
    void setKind( int kind );
    /**
     * @brief Gets the graphic related to this item
     *
     * @return This item's graphic
     */
    GLGraphicComponent *getGraphic() const;
    /**
     * @brief Sets a graphic to this item
     *
     * @param graphic_ The new graphic
     */
    void setGraphic( GLGraphicComponent *graphic_ );
    /**
     * @brief Asks if this symbol item has symbol items of the graphic kind as childs
     *
     * @return <b>true</b> if this symbol item is a folder and has one or more symbol items of the graphic kind
     *         as direct or indirect childs. Otherwise, returns <b>false</b>
     */
    bool folderHasSymbols() const;

private:
    int kind;
    GLGraphicComponent *graphic;
    QPixmap i_folder, i_graphic;

protected:

};

#endif
