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

#include "colormixer.h"

//-------------- CONSTRUCTOR -----------------

ColorMixer::ColorMixer( QWidget *parent ) : QGridView( parent )
{
    Q_CHECK_PTR( parent );

    parent_widget = parent;
    resize( 94, 89 );
    setCellWidth( 1 );
    setCellHeight( 1 );
    setNumRows( 85 );
    setNumCols( 90 );
    cur_row = 0;
    cur_col = 0;
    cross_color = QColor( 0, 0, 0 );
    dragging = false;

    //------------- Set up the color matrix -----------------

    int i, j;
    QColor tmp_color = QColor();

    //Set the hue-saturation for all colors
    for ( i = 0; i < numRows(); i++ )
        for ( j = 0; j < numCols(); j++ )
        {
	    tmp_color.setHsv( 360 - j * 4, 255 - i * 3, 255 );
            color_matrix[i][j] = tmp_color;
        }
}

//-------------- DESTRUCTOR -----------------

ColorMixer::~ColorMixer()
{

}

//-------------- PUBLIC MEMBERS ----------------

void ColorMixer::updateCross( int sat, int hue )
{
    //Erase the old cross
    updateCell( cur_row, cur_col - 1 );
    updateCell( cur_row, cur_col - 2 );
    updateCell( cur_row, cur_col - 3 );
    updateCell( cur_row, cur_col + 1 );
    updateCell( cur_row, cur_col + 2 );
    updateCell( cur_row, cur_col + 3 );
    updateCell( cur_row - 1, cur_col );
    updateCell( cur_row - 2, cur_col );
    updateCell( cur_row - 3, cur_col );
    updateCell( cur_row + 1, cur_col );
    updateCell( cur_row + 2, cur_col );
    updateCell( cur_row + 3, cur_col );

    //Set the current selection and paint the new cross
    cur_row = ( 255 - sat ) / 3;
    cur_col = ( 360 - hue ) / 4;
    updateCell( cur_row, cur_col - 1 );
    updateCell( cur_row, cur_col - 2 );
    updateCell( cur_row, cur_col - 3 );
    updateCell( cur_row, cur_col + 1 );
    updateCell( cur_row, cur_col + 2 );
    updateCell( cur_row, cur_col + 3 );
    updateCell( cur_row - 1, cur_col );
    updateCell( cur_row - 2, cur_col );
    updateCell( cur_row - 3, cur_col );
    updateCell( cur_row + 1, cur_col );
    updateCell( cur_row + 2, cur_col );
    updateCell( cur_row + 3, cur_col );
}

//-------------- SLOTS ----------------

//------------- EVENTS AND PROTECTED MEMBERS ------------

void ColorMixer::mousePressEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );

    int row_clicked, col_clicked;
    row_clicked = mouse_event -> y();
    col_clicked = mouse_event -> x();

    //Out of bounds
    if ( row_clicked > 84 || col_clicked > 89 || row_clicked < 0 || col_clicked < 0 )
    {
	mouse_event -> ignore();
	return;
    }

    dragging = true;
    QColor color_selected = color_matrix[row_clicked][col_clicked];
    emit colorSelected( color_selected );
    mouse_event -> accept();
}

void ColorMixer::mouseMoveEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );

    int row_dragged, col_dragged;
    row_dragged = mouse_event -> y();
    col_dragged = mouse_event -> x();

    //Out of bounds
    if ( row_dragged > 84 || col_dragged > 89 || row_dragged < 0 || col_dragged < 0 )
    {
	mouse_event -> ignore();
	return;
    }

    if ( dragging )
    {
    	QColor color_selected = color_matrix[row_dragged][col_dragged];
    	emit colorSelected( color_selected );
    	mouse_event -> accept();
    }
}

void ColorMixer::mouseReleaseEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );

    dragging = false;
    mouse_event -> accept();
}

void ColorMixer::paintCell( QPainter *painter, int row, int col )
{
    Q_CHECK_PTR( painter );

    painter -> setPen( color_matrix[row][col] );
    painter -> drawPoint( 0, 0 );

    //Draw the selection cross
    painter -> setPen( cross_color );
    if ( cur_row == row && cur_col == col - 1 )
        painter -> drawPoint( 0, 0 );
    else if ( cur_row == row && cur_col == col - 2 )
        painter -> drawPoint( 0, 0 );
    else if ( cur_row == row && cur_col == col - 3 )
        painter -> drawPoint( 0, 0 );
    else if ( cur_row == row && cur_col == col + 1 )
        painter -> drawPoint( 0, 0 );
    else if ( cur_row == row && cur_col == col + 2 )
        painter -> drawPoint( 0, 0 );
    else if ( cur_row == row && cur_col == col + 3 )
        painter -> drawPoint( 0, 0 );
    else if ( cur_row == row - 1 && cur_col == col )
        painter -> drawPoint( 0, 0 );
    else if ( cur_row == row - 2 && cur_col == col )
        painter -> drawPoint( 0, 0 );
    else if ( cur_row == row - 3 && cur_col == col )
        painter -> drawPoint( 0, 0 );
    else if ( cur_row == row + 1 && cur_col == col )
        painter -> drawPoint( 0, 0 );
    else if ( cur_row == row + 2 && cur_col == col )
        painter -> drawPoint( 0, 0 );
    else if ( cur_row == row + 3 && cur_col == col )
        painter -> drawPoint( 0, 0 );
}
