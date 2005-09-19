/***************************************************************************
 *   Copyright (C) 2004 by Simena Dinas                                    *
 *   dsimena@toonka.com                                                    *
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

#ifndef PALETTE_H
#define PALETTE_H

/**
 * @file palette.h
 * @brief Include this file if you need the class Palette
 */

#include <qdom.h>

#include "color.h"

/**
 * @brief Class that encapsulates the concept of color palette
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This is a class that manages the custom color palette storing scheme. It provides a list of custom colors.
 */
class Palette
{

public:
    /**
     * @brief Default Constructor
     *
     * Constructs an Palette object.
     */
    Palette();
    /**
     * @brief Default Destructor
     *
     * Destroys the Palette object.
     */
    ~Palette();

    /**
     * @brief Sets a new list of colors for this palette
     *
     * @param colors The new list of colors
     * @sa getColors()
     */
    void setColors( QPtrList<Color> colors );

    /**
     * @brief Gets the list of colors
     *
     * @return The current list of colors
     * @sa setColors()
     */
    QPtrList<Color> getColors() const;

    /**
     * @brief Creates an XML tag for this object
     *
     * @param doc The DOM Document
     * @return The DOM Element associated
     */
    QDomElement createXML( QDomDocument &doc );

private:
    QPtrList<Color> colors;

};

#endif
