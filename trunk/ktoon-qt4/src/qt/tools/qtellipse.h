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

#ifndef QT_ELLIPSE
#define QT_ELLIPSE

/**
 * @file qtellipse.h
 * @brief Include this file if you need the class QtEllipse
 */

#include <q3canvas.h>

/**
 * @brief Class that handles the ellipse graphic component for the non accelerated implementation
 *
 * <b>Date of Creation: Mar 2 - 2005.</b>\n
 * Equivalent to the GLEllipse in the OpenGL implementation. It is weird that it is as a subclass from
 * QCanvasRectangle, but the QCanvasEllipse is implemented in such a very different way than thi ellipse is.
 * Therefore, it was discarded from the begining.
 */
class QtEllipse : public Q3CanvasRectangle
{

public:
    //! Constants for defining QtEllipse properties
    enum QtEllipseProperties
    {
	RTTI = 1003, /**< See QCanvasItem::rtti() */
    };
    
    /**
     * @brief Default Constructor
     *
     * Constructs a QtEllipse object
     * @param canvas The Canvas where this item will be drawn
     */
    QtEllipse( Q3Canvas *canvas );
    /**
     * @brief Default Destructor
     *
     * Destroys the QtEllipse object
     */
    ~QtEllipse();

    /**
     * @brief Gets the RTTI value (QtEllipse::RTTI)
     *
     * Reimplemented from QCanvasEllipse.
     * @sa QCanvasItem::rtti()
     * @return 1003 (QtEllipse::RTTI)
     */
    virtual int rtti() const;
    /**
     * @brief Gets whether this ellipse is selected or not
     *
     * @return <b>true</b> if this ellipse is selected. <b>false</b>, otherwise.
     */
    bool isSelected() const;
    /**
     * @brief Sets the selection state of this ellipse
     *
     * @param select The new selection state
     */
    void setSelected( bool select );
    
private:
    bool selected;

protected:
    /**
     * @brief Draws the ellipse using a painter
     *
     * Reimplemented from QCanvasRectangle.
     * @param painter The painter
     */
    virtual void drawShape( QPainter &painter );
    /**
     * @brief Draws arrows bounding this object
     *
     * @param painter The painter
     */
    void drawSelectionArrows( QPainter &painter );

};

#endif
