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

#ifndef QT_GRID
#define QT_GRID

/*!
 * \file qtgrid.h
 * \brief Include this file if you need the class QtGrid
 */

#include <qcanvas.h>

/*!
 * \brief Class that handles the drawing area grid for the non accelerated implementation
 *
 * <b>Date of Creation: Feb 28 - 2005.</b>\n
 * Equivalent to the GLGrid in the OpenGL implementation.
 */
class QtGrid : public QCanvasPolygonalItem
{

public:
    //! Constants for defining QtGrid properties
    enum QtGridProperties
    {
	RTTI = 1001, /*!< See QCanvasItem::rtti() */
	EMPTY_GRID = 2, /*!< Constant that defines a grid without lines */
	GRID12 = 12, /*!< Constant that defines a 12 field grid */
	GRID16 = 16, /*!< Constant that defines a 16 field grid */
	SUBGRID12 = 36, /*!< Constant that defines a 12 field grid with a 3 field subgrid */
	SUBGRID16 = 48 /*!< Constant that defines a 16 field grid with a 3 field subgrid */
    };
    
    /*!
     * \brief Default Constructor
     *
     * Constructs a QtGrid object
     * \param canvas The Canvas where this item will be drawn
     * \param grid_type The grid type which may be QtGrid::NO_GRID, QtGrid::GRID12, QtGrid::16 or QtGrid::SUBGRID
     */
    QtGrid( QCanvas *canvas, int grid_type );
    /*!
     * \brief Default Destructor
     *
     * Destroys the QtGrid object
     */
    ~QtGrid();
    
    /*!
     * \brief Gets the RTTI value (QtGrid::RTTI)
     *
     * Reimplemented from QCanvasPolygon.
     * \sa QCanvasItem::rtti()
     * \return 1001 (QtGrid::RTTI)
     */
    virtual int rtti() const;
    /*!
     * \brief Gets the points bounding this item
     *
     * Reimplemented from QCanvasPolygonalItem
     * \return A point array with the bounding points
     */
    virtual QPointArray areaPoints() const;
    /*!
     * \brief Gets this grid's width
     *
     * \return The width
     */
    int width() const;
    /*!
     * \brief Gets this grid's height
     *
     * \return The height
     */
    int height() const;
    /*!
     * \brief Resizes the Grid
     *
     * \param w The new width
     * \param h The new height
     */
    void resize( int w, int h );

private:
    QColor grid_color, subgrid_color, diagonal_color, ntsc_color;
    int grid_type;
    bool see_ntsc;
    int _width, _height;

protected:
    /*!
     * \brief Draws the grid using a painter
     *
     * Reimplemented from QCanvasPolygon.
     * \param painter The painter
     */
    virtual void drawShape( QPainter &painter );
    /*!
     * \brief Draws a 12 field grid
     *
     * \param painter The painter
     */
    void draw12FieldGrid( QPainter &painter );
    /*!
     * \brief Draws a 16 field grid
     *
     * \param painter The painter
     */
    void draw16FieldGrid( QPainter &painter );
    /*!
     * \brief Draws a 12 field grid with a 3 field subgrid
     *
     * \param painter The painter
     */
    void draw12FieldSubGrid( QPainter &painter );
    /*!
     * \brief Draws a 16 field grid with a 3 field subgrid
     *
     * \param painter The painter
     */
    void draw16FieldSubGrid( QPainter &painter );
    /*!
     * \brief Draws two diagonal lines that intersects themselves
     *
     * \param painter The painter
     */
    void drawDiagonals( QPainter &painter );
};

#endif
