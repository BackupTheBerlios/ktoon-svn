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

#ifndef COLOR
#define COLOR

/**
 * @file color.h
 * @brief Include this file if you need the class KTColor
 */

#include <qptrlist.h>
#include <qstring.h>
#include <qdom.h>

/**
 * @brief Class that encapsulates the concept of color
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This is a class that manages the custom color storing scheme. It provides the RGBA components and a color name.
 */
class Color
{

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a KTColor object.
     */
    Color();
    /**
     * @brief Parametric Constructor
     *
     * Constructs a KTColor object with the parameters specified.
     * @param r The color red component
     * @param g The color green component
     * @param b The color blue component
     * @param a The color alpha
     */
    Color( float r, float g, float b, float a );
    /**
     * @brief Default Destructor
     *
     * Destroys the KTColor object.
     */
    ~Color();

    //Set Methods
    /**
     * @brief Sets a new name for this color
     *
     * @param name The new name
     * @sa nameColor()
     */
    void setNameColor( const QString & name );
    /**
     * @brief Sets a new red component for this color
     *
     * @param r The new red component
     * @sa colorRed()
     */
    void setColorRed( const float & r );
    /**
     * @brief Sets a new green component for this color
     *
     * @param g The new green component
     * @sa colorGreen()
     */
    void setColorGreen( const float & g );
    /**
     * @brief Sets a new blue component for this color
     *
     * @param b The new blue component
     * @sa colorBlue()
     */
    void setColorBlue( const float & b );
    /**
     * @brief Sets a new alpha for this color
     *
     * @param a The new alpha
     * @sa colorAlpha()
     */
    void setColorAlpha( const float & a );

    //Get Methods
    /**
     * @brief Gets the current color name
     *
     * @return The current name
     * @sa setNameColor()
     */
    QString nameColor( ) const;
    /**
     * @brief Gets the current color red component
     *
     * @return The current red component
     * @sa setColorRed()
     */
    float colorRed( ) const;
    /**
     * @brief Gets the current color green component
     *
     * @return The current green component
     * @sa setColorGreen()
     */
    float colorGreen( ) const;
    /**
     * @brief Gets the current color blue component
     *
     * @return The current blue component
     * @sa setColorBlue()
     */
    float colorBlue( ) const;
    /**
     * @brief Gets the current color alpha
     *
     * @return The current alpha
     * @sa setColorAlpha()
     */
    float colorAlpha( ) const;
    /**
     * @brief Gets the current color id
     *
     * @return The current id
     */
    int idColor() const;

    /**
     * @brief Creates an XML tag for this object
     *
     * @param doc The DOM Document
     * @return The DOM Element associated
     */
    QDomElement createXML( QDomDocument &doc );
    
private:
    QString name;
    float red, green, blue, alpha;
    int id_color;

};

#endif
