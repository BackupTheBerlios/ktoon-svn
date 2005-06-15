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

#ifndef FOLDER_H
#define FOLDER_H

/*!
 * \file folder.h
 * \brief Include this file if you need the class Folder
 */

#include <qptrlist.h>

#include "item.h"

class QDomDocument;
class QDomElement;

/*!
 * \brief Class that encapsulates the concept of folder
 *
 * <b>Date of Creation: October 6 - 2004.</b>\n
 * This is class that manages the library folder storing scheme. It provides a list of items.
 */
class Folder : public Item
{

public:
    /*!
     * \brief Default Constructor
     *
     * Constructs an Folder object.
     */
    Folder();
    /*!
     * \brief Default Destructor
     *
     * Destroys the Folder object.
     */
    ~Folder();

    //Set methods
    /*!
     * \brief Sets a new list of items to this folder
     *
     * \param new_items The new list of items
     * \sa getItems();
     */
    void setItems( QPtrList<Item> new_items );

    //Get methods
    /*!
     * \brief Gets this folder's list of items
     *
     * \return The current list of items
     * \sa setItems()
     */
    QPtrList<Item> getItems() const;

    /*!
     * \brief Creates an XML tag for this object
     *
     * \param doc The DOM Document
     * \return The DOM Element associated
     */
    QDomElement createXML( QDomDocument &doc );

private:
    QPtrList<Item> items;

};

#endif
