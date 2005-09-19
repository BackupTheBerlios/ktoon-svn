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

#include "colorcells.h"
#include <qsizepolicy.h> 
//Added by qt3to4:
#include <QMouseEvent>
//-------------- CONSTRUCTOR -----------------

ColorCells::ColorCells( QWidget *parent ) : Q3GridView( parent )
{
    Q_CHECK_PTR( parent );
    
    parent_widget = parent;
    setCellWidth( 9 );
    setCellHeight( 9 );
    setNumRows( 12 );
    setNumCols( 19 );
    cell_border_color = QColor( 0, 0, 0 );

    //------------- Set up the color matrix -----------------

    int i, j;

    //First column, first 6 rows, a gray scale
    for ( i = 0; i <= 5; i++ )
	color_matrix[i][0] = QColor( i * 51, i * 51, i * 51 );

    //First column, last 6 rows, basic colors
    color_matrix[6][0] = QColor( 255, 0, 0 );
    color_matrix[7][0] = QColor( 0, 255, 0 );
    color_matrix[8][0] = QColor( 0, 0, 255 );
    color_matrix[9][0] = QColor( 255, 255, 0 );
    color_matrix[10][0] = QColor( 0, 255, 255 );
    color_matrix[11][0] = QColor( 255, 0, 255 );

    //Segment from column 1 to 6 and row 0 to 5
    for ( i = 0; i <= 5; i++ )
        for ( j = 1; j <= 6; j++ )
	    color_matrix[i][j] = QColor( 0, ( j - 1 ) * 51, i * 51 );

    //Segment from column 1 to 6 and row 6 to 11
    for ( i = 6; i <= 11; i++ )
        for ( j = 1; j <= 6; j++ )
	    color_matrix[i][j] = QColor( 153, ( j - 1 ) * 51, ( i - 6 ) * 51 );

    //Segment from column 7 to 12 and row 0 to 5
    for ( i = 0; i <= 5; i++ )
        for ( j = 7; j <= 12; j++ )
	    color_matrix[i][j] = QColor( 51, ( j - 7 ) * 51, i * 51 );

    //Segment from column 7 to 12 and row 6 to 11
    for ( i = 6; i <= 11; i++ )
        for ( j = 7; j <= 12; j++ )
	    color_matrix[i][j] = QColor( 204, ( j - 7 ) * 51, ( i - 6 ) * 51 );

    //Segment from column 13 to 18 and row 0 to 5
    for ( i = 0; i <= 5; i++ )
        for ( j = 13; j <= 18; j++ )
	    color_matrix[i][j] = QColor( 102, ( j - 13 ) * 51, i * 51 );

    //Segment from column 13 to 18 and row 6 to 11
    for ( i = 6; i <= 11; i++ )
        for ( j = 13; j <= 18; j++ )
	    color_matrix[i][j] = QColor( 255, ( j - 13 ) * 51, ( i - 6 ) * 51 );
    
    this->setSizePolicy(QSizePolicy::Expanding ,QSizePolicy::Expanding, false);
    setMaximumSize(175, 113);
    
    adjustSize();
}

//-------------- DESTRUCTOR -----------------

ColorCells::~ColorCells()
{

}

//-------------- PUBLIC MEMBERS ----------------

//-------------- SLOTS ----------------

//------------- EVENTS AND PROTECTED MEMBERS ------------

void ColorCells::mousePressEvent( QMouseEvent *mouse_event )
{
    Q_CHECK_PTR( mouse_event );

    int row_clicked, col_clicked;
    mapPointToCell( mouse_event -> pos(), &row_clicked, &col_clicked );

    //Out of bounds
    if ( row_clicked > 11 || col_clicked > 18 || row_clicked < 0 || col_clicked < 0 )
    {
	mouse_event -> ignore();
	return;
    }

    QColor color_selected = color_matrix[row_clicked][col_clicked];
    emit colorSelected( color_selected );
    mouse_event -> accept();
}

void ColorCells::paintCell( QPainter *painter, int row, int col )
{
    painter -> setPen( cell_border_color );
    painter -> setBrush( color_matrix[row][col] );
    painter -> drawRect( 0, 0, cellWidth(), cellHeight() );
}

void ColorCells::mapPointToCell( const QPoint &p, int *out_row, int *out_col )
{
    *out_row = p.y() / 9;
    *out_col = p.x() / 9;
}
