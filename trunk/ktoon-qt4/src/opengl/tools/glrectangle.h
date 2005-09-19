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

#ifndef GLRECTANGLE_H
#define GLRECTANGLE_H

/**
 * @file glrectangle.h
 * @brief Include this file if you need the class GLRectangle
 */

#include "glgraphiccomponent.h"
//Added by qt3to4:
#include <QKeyEvent>

/**
 * @brief Class that represents the drawing area's rectangle object
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This class contains relevant information about the rectangle such as its end KTColor and its fill color. It implements
 * the graphic component's virtual function buildList() for the creation of this object's OpenGL display list.
 */
class GLRectangle : public GLGraphicComponent
{

public:
    /**
     * @brief Default constructor
     *
     * Constructs a GLRectangle object
     * @param parent The parent widget
     * @param origin The origin KTColor
     * @param outline_color The outline color
     * @param width The brush which this rectangle was drawn with (to calculate the outline width)
     * @param end The end KTColor
     * @param fill_color The fill color
     */
    GLRectangle( QGLWidget *parent, const QPoint & origin, const KTColor & outline_color, const Brush & width, const QPoint & end, KTColor & fill_color );
    /**
     * @brief Copy constructor
     *
     * @param rectangle The rectangle to copy
     */
    GLRectangle( const GLRectangle & rectangle );
    /**
     * @brief Default destructor
     *
     * Destroys the GLRectangle object
     */
    ~GLRectangle( );

    //Set methods
    /**
     * @brief Sets a new end KTColor to this rectangle
     *
     * @param end The new end KTColor
     * @sa endRectangle()
     */
    void setEndRectangle( const QPoint & end );
    /**
     * @brief Sets a new fill color to this rectangle
     *
     * @param fill_color The new fill color
     * @sa fillColor()
     */
    void setFillColor( const KTColor & fill_color );
    /**
     * @brief Sets to true or false the perfect_rectangle property
     *
     * A perfect rectangle is simply a square. All the sides are the same length.
     * @param perfect The boolean value of the perfect_rectangle property
     * @sa perfectRectangle()
     */
    void setPerfectRectangle( const bool & perfect );
    /**
     * @brief Calculates the top left KTColor of the rectangle that fits outside this rectangle
     */
    void calculateTopLeft();
    /**
     * @brief Calculates the bottom right KTColor of the rectangle that fits outside this rectangle
     */
    void calculateBottomRight();

    //Get methods
    /**
     * @brief Gets this rectangle's end KTColor
     *
     * @return The current end KTColor
     * @sa setEndRectangle()
     */
    QPoint endRectangle() const;
    /**
     * @brief Gets this rectangle's fill color
     *
     * @return The fill color. Note that it is of type KTColor, not QColor
     * @sa setFillColor()
     */
    KTColor fillColor() const;
    /**
     * @brief Returns the perfect_rectangle property
     *
     * @return <b>true</b> if it is a perfect rectangle, otherwise returns <b>false</b>
     * @sa setPerfectRectangle()
     */
    bool perfectRectangle() const;
    /**
     * @brief Gets the clipboard representation of this rectangle
     *
     * @return A string that represents this rectangle's properties
     * @sa setClipboardGraphic()
     */
    QString clipboardGraphic();

    /**
     * @brief Creates an XML tag for this object
     *
     * @param doc The DOM Document
     * @return The DOM Element associated
     */
    QDomElement createXML( QDomDocument &doc );

protected:
    QPoint end;
    KTColor fill_color;
    bool perfect_rectangle;

    /**
     * @brief Constructs an openGL display list with instructions for drawing this rectangle
     */
    void buildList();
    /**
     * @brief Event for graphic key press control
     *
     * Implemented from GLGraphicComponent.
     * @param key_event The input event
     */
    void keyPressEvent( QKeyEvent *key_event );
    /**
     * @brief Event for graphic key release control
     *
     * Implemented from GLGraphicComponent.
     * @param key_event The input event
     */
    void keyReleaseEvent( QKeyEvent *key_event );

};

#endif
