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

#ifndef GLPEN_H
#define GLPEN_H

/**
 * @file glpen.h
 * @brief Include this file if you need the class GLPen
 */

#include "glgraphiccomponent.h"

/**
 * @brief Class that represents the drawing area's pen object
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This class contains relevant information about the pen such as its control points and its end point. It implements
 * the graphic component's virtual function buildList() for the creation of this object's OpenGL display list.
 */
class GLPen : public GLGraphicComponent
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
    GLPen( QGLWidget *parent, const QPoint & origin, const Color & color, const Brush & width, const QPoint & end );
    /**
     * @brief Copy constructor
     *
     * @param pen The pen to copy
     */
    GLPen( const GLPen & pen );
    /**
     * @brief Default destructor
     *
     * Destroys the GLPencil object
     */
    ~GLPen();

    //Set methods
    /**
     * @brief Sets a new end point to this pen
     *
     * @param end The new end point
     * @sa endPen()
     */
    void setEndPen( const QPoint & end );
    /**
     * @brief Sets the bezier curve order
     *
     * The order deals with the polynomial for the NURBS surface.
     * @param order_pen The new order
     */
    void setOrderPen( const int & order_pen );
    /**
     * @brief Sets the bezier curve stride
     *
     * The stride represents the number of floating point values between control points in each parametric direction.
     * @param stride_pen The new stride
     */
    void setStridePen( const int & stride_pen );
    /**
     * @brief Inserts the two control points for a bezier point
     *
     * @param control1 The first control point
     * @param control2 The second control point
     */
    void setControlPen( const QPoint & control1, const QPoint & control2 );
    /**
     * @brief Inserts only one control point.
     *
     * It is used for the clipboard.
     * @param point The control point
     */
    void setOneControlPen( const QPoint & point );
    /**
     * @brief Sets the number of control points
     *
     * @param control_points The number of control points
     */
    void setControlPointsPen( const int & control_points );
    /**
     * @brief Sets the number of knots
     *
     * @param knots_count The number of knots
     */
    void setKnotsCount( const int & knots_count );
    /**
     * @brief Replaces the last two control points
     *
     * It calculates the other control point from the argument point and the bezier point.
     * @param point One of the control points
     */
    void replaceControlPen( const QPoint & point );
    /**
     * @brief Replaces the last two control points
     *
     * This is an overloaded function provided only for convenience. It behaves essentially like the above function.
     * @param control1 The first control point
     * @param control2 The second control point
     */
    void replaceControlPen( const QPoint & control1, const QPoint & control2 );
    /**
     * @brief Allows to modify the last control point
     *
     * @param point The control point
     */
    void lastControlPen( const QPoint & point );
    /**
     * @brief Allows to modify the two last control points in order to they will have a separate angle of 45 degrees
     *
     * @param point The control point
     */
    void angleControlPen( const QPoint & point );
    /**
     * @brief Moves the last two control points
     *
     * @param point The control point
     */
    void moveControlPen( const QPoint & point );
    /**
     * @brief Moves the two control points in the index position an the next
     *
     * @param index The control point position
     * @param point The control point
     */
    void moveControlPen( int index, const QPoint & point );
    /**
     * @brief Moves and bezier points
     *
     * @param point The bezier point
     */
    void moveBezierPoint( const QPoint & point );
    /**
     * @brief Sets a bezier point for taking care of it when it is calculating operations for the last two control
     *        points
     *
     * @param bezier The control point
     */
    void setBezierPoint( const QPoint & bezier );
    /**
     * @brief Sets the points for this pen
     *
     * @param points The new pen points
     */
    void setPoints( QPtrList<QPoint> points );
    /**
     * @brief Sets the bezier points for this pen
     *
     * @param bezier The new pen bezier points
     */
    void setBezier( QPtrList<QPoint> bezier );
    /**
     * @brief Calculates the top left point of the rectangle that fits outside this pencil
     */
    void calculateTopLeft();
    /**
     * @brief Calculates the bottom right point of the rectangle that fits outside this line
     */
    void calculateBottomRight();
    /**
     * @brief clears the point list
     */
    void clearPoints();
    /**
     * @brief Activates or deactivates the show bezier points option
     *
     * @param show The boolean value of the option
     */
    void setShowBezierPoints( bool show );

    //Get methods
    /**
     * @brief Gets a this pencil's end point
     *
     * @return The current end point
     * @sa setEndPencil()
     */
    QPoint endPen() const;
    /**
     * @brief Gets the bezier curve order
     *
     * @return The current order
     */
    int orderPen() const;
    /**
     * @brief Gets the bezier curve stride
     *
     * @return The current stride
     */
    int stridePen() const;
    /**
     * @brief Gets the number of control points
     *
     * @return The number of control points
     */
    int controlPointsPen() const;
    /**
     * @brief Gets the number of knots
     *
     * @return The number of knots
     */
    int knotsCount() const;
    /**
     * @brief Gets the bezier point
     *
     * @return The bezier point
     */
    QPoint bezierPoint() const;
    /**
     * @brief Gets the clipboard representation of this pen
     *
     * @return A string that represents this pen's properties
     * @sa setClipboardGraphic()
     */
    QString clipboardGraphic();
    /**
     * @brief Gets this pen's points
     *
     * @return The current points
     */
    QPtrList<QPoint> getPoints() const;
    /**
     * @brief Gets this pen's bezier points
     *
     * @return The current bezier points
     */
    QPtrList<QPoint> getBezier() const;
    /**
     * @brief Gets this pen's show bezier points option value
     *
     * @return The current option value
     */
    bool showBezierPoints() const;

    /**
     * @brief Creates an XML tag for this object
     *
     * @param doc The DOM Document
     * @return The DOM Element associated
     */
    QDomElement createXML( QDomDocument &doc );

protected:
    QPoint end;
    QPoint bezier_point;
    QPtrList<QPoint> points;
    QPtrList<QPoint> bezier;

    /**
     * @brief Constructs an openGL display list with instructions for drawing this pen
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

private:
    int order_pen;
    int stride_pen;
    int knots_count;
    int control_points;
    bool show_bezier_point;


};

#endif
