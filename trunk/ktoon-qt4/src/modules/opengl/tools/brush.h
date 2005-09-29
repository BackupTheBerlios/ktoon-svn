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

#ifndef BRUSH_H
#define BRUSH_H

/**
 * @file brush.h
 * @brief Include this file if you need the class Brush
 */

#include <qstring.h>
#include <qdom.h>

/**
 * @brief Class that encapsulates the concept of brush
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This is a class that manages the brush storing scheme. It provides the attributes of minimum
 * thickness, maximum thickness, smoothness and the brush name.
 */
class Brush
{

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a Brush object.
     */
    Brush();
    /**
     * @brief Parametric Constructor
     *
     * Constructs a Brush object with the parameters specified.
     * @param t_min The minimum thickness
     * @param t_max The maximum thickness
     * @param smooth The smoothness
     */
    Brush( int t_min, int t_max, int smooth );
    /**
     * @brief Default Destructor
     *
     * Destroys the Brush object.
     */
    ~Brush();

    // Set Methods
    /**
     * @brief Sets a new name for this brush
     *
     * @param name The new name
     * @sa nameBrush()
     */
    void setNameBrush( const QString & name );
    /**
     * @brief Sets a new minimum thickness for this brush
     *
     * @param thickness_min The new minimum thickness
     * @sa thicknessMinBrush()
     */
    void setThicknessMinBrush( const int & thickness_min );
    /**
     * @brief Sets a new maximum thickness for this brush
     *
     * @param thickness_max The new maximum thickness
     * @sa thicknessMaxBrush()
     */
    void setThicknessMaxBrush( const int & thickness_max );
    /**
     * @brief Sets a new smoothness for this brush
     *
     * @param smoothness The new smoothness
     * @sa smoothnessBrush()
     */
    void setSmoothnessBrush( const int & smoothness );

    // Get Methods
    /**
     * @brief Gets the current brush name
     *
     * @return The current name
     * @sa setNameBrush()
     */
    QString nameBrush() const;
    /**
     * @brief Gets the current brush minimum thickness
     *
     * @return The current minimum thickness
     * @sa setThicknessMinBrush()
     */
    int thicknessMinBrush() const;
    /**
     * @brief Gets the current brush maximum thickness
     *
     * @return The current maximum thickness
     * @sa setThicknessMaxBrush()
     */
    int thicknessMaxBrush() const;
    /**
     * @brief Gets the current brush smoothness
     *
     * @return The current smoothness
     * @sa setSmoothnessBrush()
     */
    int smoothnessBrush() const;
    /**
     * @brief Gets the brush id
     *
     * @return The brush id
     */
    int idBrush() const;

    /**
     * @brief Creates an XML tag for this object
     *
     * @param doc The DOM Document
     * @return The DOM Element associated
     */
    QDomElement createXML( QDomDocument &doc );
    
private:
    QString name;
    int thickness_min;
    int thickness_max;
    int smoothness;
    int id_brush;

};

#endif
