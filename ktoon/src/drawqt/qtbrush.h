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

#ifndef QT_BRUSH
#define QT_BRUSH

/*!
 * \file qtbrush.h
 * \brief Include this file if you need the class QtBrush
 */

#include <qcanvas.h>

/*!
 * \brief Class that handles the brush graphic component for the non accelerated implementation
 *
 * <b>Date of Creation: Mar 4 - 2005.</b>\n
 * Equivalent to the GLBrush in the OpenGL implementation.
 */
class QtBrush : public QCanvasPolygonalItem
{

public:
    //! Constants for defining QtBrush properties
    enum QtBrushProperties
    {
	RTTI = 1006, /*!< See QCanvasItem::rtti() */
    };
    
    /*!
     * \brief Default Constructor
     *
     * Constructs a QtBrush object
     * \param canvas The Canvas where this item will be drawn
     */
    QtBrush( QCanvas *canvas );
    /*!
     * \brief Default Destructor
     *
     * Destroys the QtBrush object
     */
    ~QtBrush();

    /*!
     * \brief Gets the RTTI value (QtBrush::RTTI)
     *
     * Reimplemented from QCanvasPolygonalItem.
     * \sa QCanvasItem::rtti()
     * \return 1006 (QtBrush::RTTI)
     */
    virtual int rtti() const;
    /*!
     * \brief Gets the points bounding this item
     *
     * Reimplemented from QCanvasPolygonalItem.s
     * \return A point array with the bounding points
     */
    virtual QPointArray areaPoints() const;
    /*!
     * \brief Maps a canvas coordinate point to local coordinates
     *
     * \param point The point to map
     * \return A mapped point
     */
    QPoint mapToLocal( const QPoint &point ) const;
    /*!
     * \brief Gets whether this brush was hit or not within the specified point
     *
     * \param point The point
     * \return <b>true</b> if this brush was hit, <b>false</b> otherwise.
     */
    virtual bool hit( const QPoint &point );
    /*!
     * \brief Appends a point to this brush
     *
     * \param point The new point
     */
    void addPoint( const QPoint &point );
    
private:
    QPointArray point_array;

protected:
    /*!
     * \brief Draws the brush using a painter
     *
     * Reimplemented from QCanvasPolygonalItem.
     * \param painter The painter
     */
    virtual void drawShape( QPainter &painter );
     
};

#endif
