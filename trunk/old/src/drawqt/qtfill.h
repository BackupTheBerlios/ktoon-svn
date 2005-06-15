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

#ifndef QT_FILL
#define QT_FILL

/*!
 * \file qtfill.h
 * \brief Include this file if you need the class QtFill
 */

#include <qcanvas.h>

/*!
 * \brief Class that handles the fill graphic component for the non accelerated implementation
 *
 * <b>Date of Creation: Mar 7 - 2005.</b>\n
 */
class QtFill : public QCanvasPolygon
{

public:
    //! Constants for defining QtFill properties
    enum QtFillProperties
    {
	RTTI = 1007, /*!< See QCanvasItem::rtti() */
    };
    
    /*!
     * \brief Default Constructor
     *
     * Constructs a QtFill object
     * \param canvas The Canvas where this item will be drawn
     */
    QtFill( QCanvas *canvas );
    /*!
     * \brief Default Destructor
     *
     * Destroys the QtFill object
     */
    ~QtFill();

    /*!
     * \brief Gets the RTTI value (QtFill::RTTI)
     *
     * Reimplemented from QCanvasPolygon.
     * \sa QCanvasItem::rtti()
     * \return 1007 (QtFill::RTTI)
     */
    virtual int rtti() const;
    
private:

protected:

};

#endif
