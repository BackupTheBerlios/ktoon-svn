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

#ifndef COLORMIXER_H
#define COLORMIXER_H

/**
 * @file colormixer.h
 * @brief Include this file if you need the class ColorMixer
 */

#include <qgridview.h>
#include <qcolor.h>
#include <qpainter.h>

/**
 * @brief Class that handles the color palette's color mixer
 *
 * <b>Date of Creation: July 2 - 2004.</b>\n
 * This is a widget where in the horizontal dimension hue changes, while in the vertical dimension saturation
 * changes. As a result, it is possible to found here any single color.
 */
class ColorMixer: public QGridView
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a ColorMixer object.
     * @param parent The parent widget
     */
    ColorMixer( QWidget *parent );
    /**
     * @brief Default Destructor
     *
     * Destroys the ColorMixer object.
     */
    ~ColorMixer();

    /**
     * @brief Updates the hue-saturation cross
     *
     * @param sat The new saturation
     * @param hue The new hue
     */
    void updateCross( int sat, int hue );

private:
    QColor color_matrix[85][90];
    QColor cross_color;
    int cur_row, cur_col;
    bool dragging;

signals:
    /**
     * This signal is emitted when a color in the grid is selected.
     * @param color the selected color
     */
    void colorSelected( const QColor &color );

public slots:

protected:
    /**
     * @brief Event for widget mouse press control
     *
     * Reimplemented from QWidget.
     * @param mouse_event The input event
     */
    void mousePressEvent( QMouseEvent *mouse_event );
    /**
     * @brief Event for widget mouse move control
     *
     * Reimplemented from QWidget.
     * @param mouse_event The input event
     */
    void mouseMoveEvent( QMouseEvent *mouse_event );
    /**
     * @brief Event for widget mouse release control
     *
     * Reimplemented from QWidget.
     * @param mouse_event The input event
     */
    void mouseReleaseEvent( QMouseEvent *mouse_event );
    /**
     * @brief Paints a single cell at (\a row, \a col) using painter \a painter.
     *
     * Remember that in this widget the cells have a size of 1 x 1 pixels.
     * Reimplemented from QGridView.
     * @param painter The painter
     * @param row The cell row
     * @param col The cell col
     */
    void paintCell( QPainter *painter, int row, int col );

};

#endif
