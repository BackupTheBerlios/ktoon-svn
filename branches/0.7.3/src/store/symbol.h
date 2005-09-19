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

#ifndef SYMBOL_H
#define SYMBOL_H

/**
 * @file symbol.h
 * @brief Include this file if you need the class Symbol
 */

#include "item.h"

class GLGraphicComponent;
class QDomDocument;
class QDomElement;

/**
 * @brief Class that encapsulates the concept of symbol
 *
 * <b>Date of Creation: October 6 - 2004.</b>\n
 * This is class that manages the library symbol storing scheme. It provides a graphic component.
 */
class Symbol : public Item
{

public:
    /**
     * @brief Default Constructor
     *
     * Constructs an Symbol object.
     */
    Symbol();
    /**
     * @brief Default Destructor
     *
     * Destroys the Symbol object.
     */
    ~Symbol();

    //Set methods
    /**
     * @brief Sets a new graphic to this symbol
     *
     * @param graphic_ The new graphic
     * @sa getGraphic();
     */
    void setGraphic( GLGraphicComponent *graphic_ );

    //Get methods
    /**
     * @brief Gets this symbol's graphic
     *
     * @return The current graphic
     * @sa setGraphic()
     */
    GLGraphicComponent *getGraphic() const;

    /**
     * @brief Creates an XML tag for this object
     *
     * @param doc The DOM Document
     * @return The DOM Element associated
     */
    QDomElement createXML( QDomDocument &doc );

private:
    GLGraphicComponent *graphic;

};

#endif
