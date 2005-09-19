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

#ifndef CUSTOMPALETTE_H
#define CUSTOMPALETTE_H

/**
 * @file custompalette.h
 * @brief Include this file if you need the class CustomPalette
 */

#include <qgridview.h>
#include <qcolor.h>
#include <qpainter.h>

class KTColor;

/**
 * @brief Class that handles the user custom palette
 *
 * <b>Date of Creation: July 9 - 2004.</b>\n
 * This is a widget where the user can add custom colors. It may have a maximum of 12 x 19 selectable colors.
 */
class CustomPalette: public QGridView
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     *
     * Constructs a CustomPalette object.
     * @param parent The parent widget
     */
    CustomPalette( QWidget *parent );
    /**
     * @brief Default Destructor
     *
     * Destroys the CustomPalette object.
     */
    ~CustomPalette();
    /**
     * @brief Loads colors into this custom palette
     *
     * @param colors The colors
     */
    void loadColors( QPtrList<KTColor> colors );
    /**
     * @brief Clears this custom palette
     */
    void clear();

private:
    QColor cell_border_color;
    QColor color_matrix[12][19];
    QString color_names[12][19];
    int alpha_matrix[12][19];
    QColor invalid_color;
    int cur_row, cur_col;
    int sel_row, sel_col;

signals:
    /**
     * This signal is emitted when a color within the grid has been selected.
     * @param color The selected color
     */
    void colorSelected( const QColor &color );
    /**
     * This signal is emitted when a color within the grid has been selected. It's used to pass the selected
     * color name.
     * @param name The name of the selected color
     */
    void colorNameChanged( const QString &name );
    /**
     * This signal is emitted when a color within the grid has been selected. It's used to pass the selected
     * color alpha.
     * @param alpha The selected color alpha component
     */
    void colorAlphaChanged( int alpha );

public slots:
    /**
     * @brief Adds a a new color to the custom palette
     *
     * @param new_color The new color
     * @param alpha The new color alpha component
     * @sa slotRemoveColor()
     */
    void slotAddColor( const QColor &new_color, int alpha );
    /**
     * @brief Removes the selected color from the custom palette
     *
     * @sa slotAddColor()
     */
    void slotRemoveColor();
    /**
     * @brief Modifies the name of the selected color
     *
     * @param new_name The new name
     */
    void slotChangeColorName( const QString &new_name );

protected:
    /**
     * @brief Event for widget mouse press control
     *
     * Reimplemented from QWidget.
     * @param mouse_event The input event
     */
    void mousePressEvent( QMouseEvent *mouse_event );
    /**
     * @brief Paints a single cell at (\a row, \a col) using painter \a painter.
     *
     * Reimplemented from QGridView.
     * @param painter The painter
     * @param row The cell row
     * @param col The cell col
     */
    void paintCell( QPainter *painter, int row, int col );
    /**
     * @brief Gets the row and column of the cell that is associated to a point.
     *
     * @param p The input point
     * @param out_row The output variable that will be set to the cell row
     * @param out_col The output variable that will be set to the cell column
     */
    void mapPointToCell( const QPoint &p, int *out_row, int *out_col );
    /**
     * @brief Gets the position of the cell (\a row , \a col ) in the color matrix, as it would be seen in an array
     *
     * This function returns the one dimensional representation of a cell in a bidimensional matrix.
     * @param row The cell row
     * @param col The cell column
     * @return The one dimensional position
     * @sa map1Dto2D()
     */
    int map2Dto1D( int row, int col );

};

#endif
