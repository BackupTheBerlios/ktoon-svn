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

#ifndef ITEM_H
#define ITEM_H

/*!
 * \file item.h
 * \brief Include this file if you need the class Item
 */

class QDomDocument;
class QDomElement;
class QString;

/*!
 * \brief Class that encapsulates the concept of a symbol item
 *
 * <b>Date of Creation: October 6 - 2004.</b>\n
 * This is an abstract class that manages the symbol item storing scheme. It provides the symbol name.
 */
class Item
{

public:
     //! Constants for defining the kind of item
     enum ItemKind
     {
         ITEM_SYMBOL = 1, /*!< An item that is a symbol */
	 ITEM_FOLDER = 2, /*!< An item that is a folder */
	 ITEM_UNDEFINED = 3, /*!< An undefined item */
     };

    /*!
     * \brief Default Constructor
     *
     * Constructs an Item object.
     */
    Item();
    /*!
     * \brief Default Destructor
     *
     * Destroys the Item object.
     */
    virtual ~Item();

    //Set methods
    /*!
     * \brief Sets a new name to this item
     *
     * \param new_name The new name
     * \sa getName()
     */
    void setName( const QString &new_name );

    //Get methods
    /*!
     * \brief Gets this item's name
     *
     * \return The current name
     * \sa setName()
     */
    QString getName() const;
    /*!
     * \brief Gets the kind of this item
     *
     * \return The item kind
     */
    int itemKind() const;

    /*!
     * \brief Creates an XML tag for this object
     *
     * Implemented in Folder and Symbol subclasses.
     * \param doc The DOM Document
     * \return The DOM Element associated
     */
    virtual QDomElement createXML( QDomDocument &doc ) = 0;

protected:
    QString name;
    int item_kind;

};

#endif
