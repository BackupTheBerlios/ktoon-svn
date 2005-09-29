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

#ifndef GLBRUSH_H
#define GLBRUSH_H

/**
 * @file glbrush.h
 * @brief Include this file if you need the class GLBrush
 */

#include <q3ptrlist.h>
//Added by qt3to4:
#include <QKeyEvent>

#include "glgraphiccomponent.h"

/**
 * @brief Class that represents the drawing area's brush object
 *
 * <b>Date of Creation: September 14 - 2004.</b>\n
 * This class contains relevant information about the brush such as its final point. It implements
 * the graphic component's virtual function buildList() for the creation of this object's OpenGL display list.
 */
class GLBrush : public GLGraphicComponent
{

public:
    /**
     * @brief Default constructor
     *
     * Constructs a GLBrush object
     * @param parent The parent widget
     * @param origin The origin point
     * @param color The line color
     * @param width The brush which this line was drawn with (to calculate the line width)
     * @param end The end point
     */
    GLBrush( QGLWidget *parent, const QPoint & origin, const KTColor & color, const Brush & width, const QPoint & end );
    /**
     * @brief Copy constructor
     *
     * @param brush The brush to copy
     */
    GLBrush( const GLBrush & brush );
    /**
     * @brief Default destructor
     *
     * Destroys the GLBrush object
     */
    ~GLBrush();

    //Set methods
    /**
     * @brief Sets a new end point to this brush
     *
     * @param end The new end point
     * @sa endBrush()
     */
    void setEndBrush( const QPoint & end );
    /**
     * @brief Calculates a 45 degree angle from a point relative to this brush's origin
     *
     * @param end The point that will be used for calculating the brush direction
     */
    void angleBrush( const QPoint & end );
    /**
     * @brief Do the same operation as the line tool
     *
     * @param end The point that will be used for calculating the brush direction
     */
    void lineBrush( const QPoint & end );
    /**
     * @brief Calculates the brush smoothness
     *
     * @param smooth The current brush smoothness
     */
    void smoothnessBrush( int smooth );
    /**
     * @brief Calculates the top left point of the rectangle that fits outside this brush
     */
    void calculateTopLeft();
    /**
     * @brief Calculates the bottom right point of the rectangle that fits outside this line
     */
    void calculateBottomRight();
    /**
     * @brief Sets the points for this brush
     *
     * @param _points The new brush points
     * @sa getPoints()
     */
    void setPoints( Q3PtrList<QPoint> _points );
    /**
     * @brief Set the pressure for the brush
     *
     * @param _pressure The new pressure
     * @sa pressureBrush()
     */
     void setPressureBrush( int _pressure );

    //Get methods
    /**
     * @brief Gets a this brush's end point
     *
     * @return The current end point
     * @sa setEndBrush()
     */
    QPoint endBrush() const;
    /**
     * @brief Gets the clipboard representation of this brush
     *
     * @return A string that represents this brush's properties
     * @sa setClipboardGraphic()
     */
    QString clipboardGraphic();
    /**
     * @brief Gets this brush's points
     *
     * @return The current points
     */
    Q3PtrList<QPoint> getPoints() const;
    /**
     * @brief Gets this brush's pressure values
     *
     * @return The list of pressure values
     * @sa setPressureBrush()
     */
    Q3PtrList<int> pressureBrush() const;

    /**
     * @brief Creates an XML tag for this object
     *
     * @param doc The DOM Document
     * @return The DOM Element associated
     */
    QDomElement createXML( QDomDocument &doc );

protected:
    QPoint end;
    Q3PtrList<QPoint> points;
    Q3PtrList<int> pressure;

    /**
     * @brief Constructs an openGL display list with instructions for drawing this brush
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
