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

#ifndef GLPENCIL_H
#define GLPENCIL_H

/**
 * @file glpencil.h
 * @brief Include this file if you need the class GLPencil
 */

#include <qptrlist.h>

#include "glgraphiccomponent.h"

/**
 * @brief Class that represents the drawing area's pencil object
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This class contains relevant information about the pencil such as its final point. It implements
 * the graphic component's virtual function buildList() for the creation of this object's OpenGL display list.
 */
class GLPencil : public GLGraphicComponent
{

public:
    /**
     * @brief Default constructor
     *
     * Constructs a GLPencil object
     * @param parent The parent widget
     * @param origin The origin point
     * @param color The line color
     * @param width The brush which this line was drawn with (to calculate the line width)
     * @param end The end point
     */
    GLPencil( QGLWidget *parent, const QPoint & origin, const Color & color, const Brush & width, const QPoint & end );
    /**
     * @brief Copy constructor
     *
     * @param pencil The pencil to copy
     */
    GLPencil( const GLPencil & pencil );
    /**
     * @brief Default destructor
     *
     * Destroys the GLPencil object
     */
    ~GLPencil();

    //Set methods
    /**
     * @brief Sets a new end point to this pencil
     *
     * @param end The new end point
     * @sa endPencil()
     */
    void setEndPencil( const QPoint & end );
    /**
     * @brief Calculates a 45 degree angle from a point relative to this pencil's origin
     *
     * @param end The point that will be used for calculating the pencil direction
     */
    void anglePencil( const QPoint & end );
    /**
     * @brief Do the same operation as the line tool
     *
     * @param end The point that will be used for calculating the pencil direction
     */
    void linePencil( const QPoint & end );
    /**
     * @brief Calculates the pencil smoothness
     *
     * @param smooth The current brush smoothness
     */
    void smoothnessPencil( int smooth );
    /**
     * @brief Calculates the top left point of the rectangle that fits outside this pencil
     */
    void calculateTopLeft();
    /**
     * @brief Calculates the bottom right point of the rectangle that fits outside this line
     */
    void calculateBottomRight();
    /**
     * @brief Sets the points for this pencil
     *
     * @param _points The new pencil points
     */
    void setPoints( QPtrList<QPoint> _points );

    //Get methods
    /**
     * @brief Gets a this pencil's end point
     *
     * @return The current end point
     * @sa setEndPencil()
     */
    QPoint endPencil() const;
    /**
     * @brief Gets the clipboard representation of this pencil
     *
     * @return A string that represents this pencil's properties
     * @sa setClipboardGraphic()
     */
    QString clipboardGraphic();
    /**
     * @brief Gets this pencil's points
     *
     * @return The current points
     */
    QPtrList<QPoint> getPoints() const;

    /**
     * @brief Creates an XML tag for this object
     *
     * @param doc The DOM Document
     * @return The DOM Element associated
     */
    QDomElement createXML( QDomDocument &doc );

protected:
    QPoint end;
    QPtrList<QPoint> points;

    /**
     * @brief Constructs an openGL display list with instructions for drawing this pencil
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
