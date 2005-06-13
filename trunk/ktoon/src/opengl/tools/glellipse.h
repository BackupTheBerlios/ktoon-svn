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

#ifndef GLELLIPSE_H
#define GLELLIPSE_H
  
/**
 * @file glellipse.h
 * @brief Include this file if you need the class GLEllipse
 */

#include "glgraphiccomponent.h"

/**
 * @brief Class that represents the drawing area's ellipse object
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This class contains relevant information about the ellipse such as its radius and its fill color. It implements
 * the graphic component's virtual function buildList() for the creation of this object's OpenGL display list.
 */
class GLEllipse : public GLGraphicComponent
{

public:
    /**
     * @brief Default constructor
     *
     * Constructs a GLEllipse object
     * @param parent The parent widget
     * @param origin The origin point
     * @param outline_color The outline color
     * @param width The brush which this ellipse was drawn with (to calculate the outline width)
     * @param end The end point
     * @param fill_color The fill color
     */
    GLEllipse( QGLWidget *parent, const QPoint & origin, const KTColor & outline_color, const Brush & width, const QPoint & end, KTColor & fill_color );
    /**
     * @brief Copy constructor
     *
     * @param ellipse The ellipse to copy
     */
    GLEllipse( const GLEllipse & ellipse );
    /**
     * @brief Default destructor
     *
     * Destroys the GLEllipse object
     */
    ~GLEllipse( );

    //Set methods
    /**
     * @brief Sets a new radius property to this ellipse
     *
     * You (a new software developer of this project) may be asking to me, ¿Why the hell the ellipse radius
     * is represented by a point and not by two integer components that denote the x radius and the y radius?
     * The answer is, for convenience. Since the ellipse is drawn because of a mouse event, it is more practical
     * to pass the point returned by the mouse event and draw the ellipse by means of OpenGL instructions, leaving
     * the calculations for them.
     * @param radius A point that denotes a mouse event position to draw the ellipse respect to its origin
     * @sa radiusEllipse()
     */
    void setRadiusEllipse( const QPoint & radius );
    /**
     * @brief Sets a new fill color to this ellipse
     *
     * @param fill_color The new fill color
     * @sa fillColor()
     */
    void setFillColor( const KTColor & fill_color );
    /**
     * @brief Sets to true or false the perfect_ellipse property
     *
     * A perfect ellipse is simply a circle. The x radius and the y radius are the same.
     * @param perfect The boolean value of the perfect_ellipse property
     * @sa perfectEllipse()
     */
    void setPerfectEllipse( const bool & perfect );
    /**
     * @brief Calculates the top left point of the rectangle that fits outside this ellipse
     */
    void calculateTopLeft();
    /**
     * @brief Calculates the bottom right point of the rectangle that fits outside this ellipse
     */
    void calculateBottomRight();

    //Get methods
    /**
     * @brief Gets this ellipse's radius property
     *
     * @return A point that denotes a mouse event position to draw the ellipse respect to its origin
     * @sa setRadiusEllipse()
     */
    QPoint radiusEllipse() const;
    /**
     * @brief Gets this ellipse's fill color
     *
     * @return The fill color. Note that it is of type KTColor, not QColor
     * @sa setFillColor()
     */
    KTColor fillColor() const;
    /**
     * @brief Returns the perfect_ellipse property
     *
     * @return <b>true</b> if it is a perfect ellipse, otherwise returns <b>false</b>
     * @sa setPerfectEllipse()
     */
    bool perfectEllipse() const;
    /**
     * @brief Gets the clipboard representation of this ellipse
     *
     * @return A string that represents this ellipse's properties
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
    QPoint radius;
    KTColor fill_color;
    bool perfect_ellipse;

    /**
     * @brief Constructs an openGL display list with instructions for drawing this ellipse
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
