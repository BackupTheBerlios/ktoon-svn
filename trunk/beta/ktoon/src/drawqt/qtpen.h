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

#ifndef QT_PEN
#define QT_PEN

/**
 * @file qtpen.h
 * @brief Include this file if you need the class QtPen
 */

#include <qcanvas.h>

/**
 * @brief Class that handles the pen graphic component for the non accelerated implementation
 *
 * <b>Date of Creation: Mar 8 - 2005.</b>\n
 * Equivalent to GLPen in the OpenGL implementation.
 */
class QtPen : public QCanvasPolygonalItem
{

public:
    //! Constants for defining QtPen properties
    enum QtPenProperties
    {
	RTTI = 1008, /**< See QCanvasItem::rtti() */
    };
    
    /**
     * @brief Default Constructor
     *
     * Constructs a QtPen object
     * @param canvas The Canvas where this item will be drawn
     */
    QtPen( QCanvas *canvas );
    /**
     * @brief Default Destructor
     *
     * Destroys the QtPen object
     */
    ~QtPen();

    /**
     * @brief Gets the RTTI value (QtPen::RTTI)
     *
     * Reimplemented from QCanvasPolygonalItem.
     * @sa QCanvasItem::rtti()
     * @return 1008 (QtPen::RTTI)
     */
    virtual int rtti() const;
    /**
     * @brief Gets the points bounding this item
     *
     * Reimplemented from QCanvasPolygonalItem.
     * @return A point array with the bounding points
     */
    virtual QPointArray areaPoints() const;
    /**
     * @brief Maps a canvas coordinate point to local coordinates
     *
     * @param point The point to map
     * @return A mapped point
     */
    QPoint mapToLocal( const QPoint &point ) const;
    /**
     * @brief Gets whether this pen was hit or not within the specified point
     *
     * @param point The point
     * @return <b>true</b> if this brush was hit, <b>false</b> otherwise.
     */
    virtual bool hit( const QPoint &point );
    /**
     * @brief Appends a point to this pen
     *
     * @param point The new point
     */
    void addPoint( const QPoint &point );
    /**
     * @brief Modifies this pen in a way that it makes a bezier curve
     *
     * The control points are the last and the last but one from the point array, and the argument point
     * and its reflection. If there is only one point in the point array, nothing happens.
     * @param point The main point. It will be reflexed in order to get one more control point
     * @param spin The spin point. It is used for calculating the reflection from \a point
     */
    void addBezier( const QPoint &point, const QPoint &spin );
    /**
     * @brief Modifies this pen in a way that it makes a bezier curve
     *
     * It replaces the last control points from the array by the new points.
     * @param point The main point. It will be reflexed in order to get one more control point
     * @param spin The spin point. It is used for calculating the reflection from \a point
     */
    void replaceBezier( const QPoint &point, const QPoint &spin );
    
private:
    QPointArray point_array;
    QPointArray control_points;
    QValueList<int> indexes;  //Indicates which points will take place as bezier points

protected:
    /**
     * @brief Draws the pen using a painter
     *
     * Reimplemented from QCanvasPolygonalItem.
     * @param painter The painter
     */
    virtual void drawShape( QPainter &painter );
    /**
     * @brief Gets wheter this index is into the list of bezier indexes
     *
     * @param index The index
     * @return <b>true</b> if the argument index belongs to the list of bezier indexes. <b>false</b>, otherwise.
     */
    bool isBezierIndex( unsigned int index ) const;

};

#endif
