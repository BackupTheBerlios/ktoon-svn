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

#ifndef COLORDISPLAY_H
#define COLORDISPLAY_H

/*!
 * \file colordisplay.h
 * \brief Include this file if you need the class ColorDisplay
 */

#include <qframe.h>
#include <qcolor.h>
#include <qpainter.h>

/*!
 * \brief Class that handles the color palette's color display
 *
 * <b>Date of Creation: July 6 - 2004.</b>\n
 * This is a widget that shows the color that is currently used.
 */
class ColorDisplay : public QFrame
{
    Q_OBJECT

public:
    /*!
     * \brief Default Constructor
     *
     * Constructs a ColorDisplay object.
     * \param parent The parent widget
     */
    ColorDisplay( QWidget *parent );
    /*!
     * \brief Default Destructor
     *
     * Destroys the ColorDisplay object.
     */
    ~ColorDisplay();

    /*!
     * \brief Gets the current color
     *
     * \return The current color
     * \sa slotSetColor()
     */
    QColor currentColor();

private:
    QWidget *parent_widget;
    QColor current_color;
    QColor dark_block_color, light_block_color, intermediate_dark_color, intermediate_light_color;
    float alpha;

signals:
    /*!
     * This signal is emitted when the current color has changed.
     * \param color The color which the ColorDisplay changed to
     */
    void colorChanged( const QColor &color );

public slots:
    /*!
     * \brief Sets a new color and displays it
     *
     * \param new_color The new color
     * \sa currentColor()
     */
    void slotSetColor( const QColor &new_color );
    /*!
     * \brief Sets a new value to the red component of the current color and updates the visualization
     *
     * \param value The red component value (0..255)
     * \sa slotChangeG(), slotChangeB()
     */
    void slotChangeR( int value );
    /*!
     * \brief Sets a new value to the green component of the current color and updates the visualization
     *
     * \param value The green component value (0..255)
     * \sa slotChangeR(), slotChangeB()
     */
    void slotChangeG( int value );
    /*!
     * \brief Sets a new value to the blue component of the current color and updates the visualization
     *
     * \param value The blue component value (0..255)
     * \sa slotChangeR(), slotChangeG()
     */
    void slotChangeB( int value );
    /*!
     * \brief Sets a new hue to the current color and updates the visualization
     *
     * \param value The hue (0..360)
     * \sa slotChangeS(), slotChangeV()
     */
    void slotChangeH( int value );
    /*!
     * \brief Sets a new saturation to the current color and updates the visualization
     *
     * \param value The saturation (0..255)
     * \sa slotChangeH(), slotChangeV()
     */
    void slotChangeS( int value );
    /*!
     * \brief Sets a new value (The "V" from "HSV") to the current color and updates the visualization
     *
     * \param value The value (0..255)
     * \sa slotChangeH(), slotChangeS()
     */
    void slotChangeV( int value );
    /*!
     * \brief Sets a new alpha value to the current color and updates the visualization
     *
     * \param value The alpha value (0..100)
     */
    void slotChangeA( int value );

protected:
    /*!
     * \brief Draw the contents of the ColorDisplay
     *
     * Reimplemented from QFrame.
     * \param painter The frame painter
     */
    void drawContents( QPainter *painter );

};

#endif
