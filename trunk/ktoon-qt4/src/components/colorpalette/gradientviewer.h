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

#ifndef GRADIENTVIEWER_H
#define GRADIENTVIEWER_H

/**
 * @file gradientviewer.h
 * @brief Include this file if you need the class GradientViewer
 */

#include <q3frame.h>
#include <qpainter.h>

/**
 * @brief Class that handles the color palette's gradient viewer
 *
 * <b>Date of Creation: July 15 - 2004.</b>\n
 * This is a widget where the user can edit a multicolor gradient with the help of gradient switches.
 */
class GradientViewer : public Q3Frame
{
    Q_OBJECT

public:
    //! Constants for defining the gradient type
    enum GradientType
    {
        LINEAR = 201, /**< The linear type */
        RADIAL = 202, /**< The radial type */
    };

    /**
     * @brief Default Constructor
     *
     * Constructs a GradientViewer object.
     * @param parent The parent widget
     */
    GradientViewer( QWidget *parent );
    /**
     * @brief Default Destructor
     *
     * Destroys the GradientViewer object.
     */
    ~GradientViewer();

private:
    QWidget *parent_widget;
    QColor linear_array[45], linear_array_d[45];
    QColor radial_array[23], radial_array_d[23];
    QColor dark_color, light_color;
    int type;

signals:

public slots:
    /**
     * @brief Sets the type of this gradient
     *
     * @param in_type The type. See GradientType
     */
    void slotSetType( int in_type );
    /**
     * @brief Updates the gradient visualization according to the parameters
     *
     * See the signal gradientChanged() from the class ColorGradientSelector.
     * @param color1 The first color
     * @param color2 The second color
     * @param pos1 The first color position as it comes from the gradient selector
     * @param pos2 The second color position as it comes from the gradient selector
     * @param alpha1 The first color alpha (0..100)
     * @param alpha2 The second color alpha (0..100)
     * @sa interpolateColors()
     */
    void slotUpdateGradient( const QColor &color1, const QColor &color2, int pos1, int pos2, int alpha1, int alpha2 );

protected:
    void drawContents( QPainter *painter );
    /**
     * @brief Computes the intermediate colors from \a color1 to \a color2 and updates the color arrays.
     *
     * This function generates \a pos_color1 - \a pos_color2 intermediate colors from \a color1 to \a color2,
     * that are combined in some proportion according to their position. The colors are set in the color array
     * from \a pos_color1 to \a pos_color2.
     * @param color1 The first color
     * @param color2 The second color
     * @param pos_color1 The first color position within this widget
     * @param pos_color2 The second color position within this widget
     * @param alpha1 The first color alpha (0..100)
     * @param alpha2 The second color alpha (0..100)
     */
    void interpolateColors( const QColor &color1, const QColor &color2, int pos_color1, int pos_color2, int alpha1, int alpha2 );
    /**
     * @brief Maps a position from the gradient selector to a position of this widget
     *
     * @param gs_pos The gradient selector position
     * @return The corresponding gradient viewer position
     */
    int mapGSPosition( int gs_pos );

};

#endif
