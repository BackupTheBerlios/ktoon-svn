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

#ifndef COLORCELLS_H
#define COLORCELLS_H

/*!
 * \file colorcells.h
 * \brief Include this file if you need the class ColorCells
 */

#include <qgridview.h>
#include <qcolor.h>
#include <qpainter.h>

/*!
 * \brief Class that handles the selector of a color in a grid
 *
 * <b>Date of Creation: July 2 - 2004.</b>\n
 * This is a widget that shows a grid of 12 x 19 selectable colors.
 */
class ColorCells: public QGridView
{
    Q_OBJECT

public:
    /*!
     * \brief Default Constructor
     *
     * Constructs a ColorCells object.
     * \param parent The parent widget
     */
    ColorCells( QWidget *parent );
    /*!
     * \brief Default Destructor
     *
     * Destroys the ColorCells object.
     */
    ~ColorCells();

private:
    QWidget *parent_widget;
    QColor cell_border_color;
    QColor color_matrix[12][19];

signals:
    /*!
     * This signal is emitted when a color within the grid has been selected.
     * \param color The selected color
     */
    void colorSelected( const QColor &color );

public slots:

protected:
    /*!
     * \brief Event for widget mouse press control
     *
     * Reimplemented from QWidget.
     * \param mouse_event The input event
     */
    void mousePressEvent( QMouseEvent *mouse_event );
    /*!
     * \brief Paints a single cell at (\a row, \a col) using painter \a painter.
     *
     * Reimplemented from QGridView.
     * \param painter The painter
     * \param row The cell row
     * \param col The cell col
     */
    void paintCell( QPainter *painter, int row, int col );
    /*!
     * \brief Gets the row and column of the cell that is associated to a point.
     *
     * \param p The input point
     * \param out_row The output variable that will be set to the cell row
     * \param out_col The output variable that will be set to the cell column
     */
    void mapPointToCell( const QPoint &p, int *out_row, int *out_col );

};

#endif
