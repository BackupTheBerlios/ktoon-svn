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

#include "custompalette.h"
#include "ktoon.h"


//-------------- CONSTRUCTOR -----------------

CustomPalette::CustomPalette( QWidget *parent ) : QGridView( parent )
{
    parent_widget = parent;
    setCellWidth( 9 );
    setCellHeight( 9 );
    setNumRows( 12 );
    setNumCols( 19 );
    cell_border_color = QColor( 0, 0, 0 );
    k_toon = ( Ktoon * )( parent_widget -> parentWidget() );

    clear();
}

//-------------- DESTRUCTOR -----------------

CustomPalette::~CustomPalette()
{

}

//-------------- PUBLIC MEMBERS ----------------

void CustomPalette::loadColors( QPtrList<Color> colors )
{
    clear();
    QPtrList<Color> empty;
    k_toon -> document() -> getPalette() -> setColors( empty );

    Color *c_it;
    for ( c_it = colors.first(); c_it; c_it = colors.next() )
    {
        sel_row = cur_row;
	sel_col = cur_col;
        QColor nc = QColor( ( int )( c_it -> colorRed()  * 255.0 ),
	                    ( int )( c_it -> colorGreen() * 255.0 ),
			    ( int )( c_it -> colorBlue() * 255.0 ) );
	int a = ( int )( c_it -> colorAlpha() * 100.0 );
	slotAddColor( nc, a );
	slotChangeColorName( c_it -> nameColor() );
    }
}

void CustomPalette::clear()
{
    cur_row = 0;
    cur_col = 0;
    sel_row = -1;
    sel_col = -1;

    for ( int i = 0; i < numRows(); i++ )
        for ( int j = 0; j < numCols(); j++ )
	{
	    color_matrix[i][j] = invalid_color;
	    updateCell( i, j );
	}

    for ( int i = 0; i < numRows(); i++ )
        for ( int j = 0; j < numCols(); j++ )
	    color_names[i][j] = "";

    for ( int i = 0; i < numRows(); i++ )
        for ( int j = 0; j < numCols(); j++ )
	    alpha_matrix[i][j] = -1;
}

//-------------- SLOTS ----------------

void CustomPalette::slotAddColor( const QColor &new_color, int alpha )
{
    if ( cur_row == numRows() - 1 && cur_col == numCols() - 1 )
    {
	//do nothing
    }
    else if ( cur_col == numCols() - 1 )
    {
        color_matrix[cur_row][cur_col] = new_color;
	alpha_matrix[cur_row][cur_col] = alpha;
	int old_sel_row = sel_row;
	int old_sel_col = sel_col;
	sel_row = cur_row;
	sel_col = cur_col;
	if ( old_sel_row != -1 && old_sel_col != -1 )
	    updateCell( old_sel_row, old_sel_col );
	updateCell( cur_row, cur_col );
	cur_col = 0;
	cur_row++;

	QPtrList<Color> cl = k_toon -> document() -> getPalette() -> getColors();
	Color *n_color = new Color( ( float )new_color.red() / 255.0, ( float )new_color.green() / 255.0,
	                            ( float )new_color.blue() / 255.0, ( float )alpha / 100.0 );
	try {
	  cl.append( n_color );
	  k_toon -> document() -> getPalette() -> setColors( cl );
	  }
	catch(...)
	   {
	   delete n_color;
	   throw;
	   }
    }
    else
    {
        color_matrix[cur_row][cur_col] = new_color;
	alpha_matrix[cur_row][cur_col] = alpha;
	int old_sel_row = sel_row;
	int old_sel_col = sel_col;
	sel_row = cur_row;
	sel_col = cur_col;
	if ( old_sel_row != -1 && old_sel_col != -1 )
	    updateCell( old_sel_row, old_sel_col );
	updateCell( cur_row, cur_col );
	cur_col++;

	QPtrList<Color> cl = k_toon -> document() -> getPalette() -> getColors();
	Color *n_color = new Color( ( float )new_color.red() / 255.0, ( float )new_color.green() / 255.0,
	                            ( float )new_color.blue() / 255.0, ( float )alpha / 100.0 );
	try {
	  cl.append( n_color );
	  k_toon -> document() -> getPalette() -> setColors( cl );
	  }
	catch(...)
	   {
	   delete n_color;
	   throw;
	   }
    }
}

void CustomPalette::slotRemoveColor()
{
    int i, j;
    if ( sel_row == -1 || sel_col == -1 )
    {
	//do nothing
    }
    else
    {
        if ( cur_col == 0 )
	{
	    cur_col = numCols() - 1;
	    cur_row--;
	}
	else
	    cur_col--;

        color_matrix[sel_row][sel_col] = invalid_color;
	color_names[sel_row][sel_col] = "";
	alpha_matrix[sel_row][sel_col] = -1;
	updateCell( sel_row, sel_col );

	QPtrList<Color> cl = k_toon -> document() -> getPalette() -> getColors();
	cl.setAutoDelete( true );
	cl.remove( map2Dto1D( sel_row, sel_col ) );
	cl.setAutoDelete( false );
	k_toon -> document() -> getPalette() -> setColors( cl );

	for ( i = sel_row; i < numRows(); i++ )
	{
	    if ( i == sel_row )
	    {
	        for ( j = sel_col; j < numCols(); j++ )
	        {
		    if ( j == numCols() - 1 && i < numRows() - 1 )
		    {
		        color_matrix[i][j] = color_matrix[i + 1][0];
		    	color_names[i][j] = color_names[i + 1][0];
			alpha_matrix[i][j] = alpha_matrix[i + 1][0];
		    	updateCell( i, j );
		    }
		    else if ( j < numCols() - 1 )
		    {
		    	color_matrix[i][j] = color_matrix[i][j + 1];
		    	color_names[i][j] = color_names[i][j + 1];
			alpha_matrix[i][j] = alpha_matrix[i][j + 1];
		    	updateCell( i, j );
		    }
		    else if ( j == numCols() - 1 && i < numRows() - 1 )
		    {
		    	color_matrix[i][j] = invalid_color;
		    	color_names[i][j] = "";
			alpha_matrix[i][j] = -1;
		    	updateCell( i, j );
		    }
	    	}
	    }
	    else
	    {
	        for ( j = 0; j < numCols(); j++ )
	        {
		    if ( j == numCols() - 1 && i < numRows() - 1 )
		    {
		        color_matrix[i][j] = color_matrix[i + 1][0];
		    	color_names[i][j] = color_names[i + 1][0];
			alpha_matrix[i][j] = alpha_matrix[i + 1][0];
		    	updateCell( i, j );
		    }
		    else if ( j < numCols() - 1 )
		    {
		    	color_matrix[i][j] = color_matrix[i][j + 1];
		    	color_names[i][j] = color_names[i][j + 1];
			alpha_matrix[i][j] = alpha_matrix[i][j + 1];
		    	updateCell( i, j );
		    }
		    else if ( j == numCols() - 1 && i < numRows() - 1 )
		    {
		    	color_matrix[i][j] = invalid_color;
		    	color_names[i][j] = "";
			alpha_matrix[i][j] = -1;
		    	updateCell( i, j );
		    }
	    	}
	    }
	}
	if ( cur_col == sel_col && cur_row == sel_row )
	{
	    sel_row = -1;
	    sel_col = -1;
	    updateCell( cur_row, cur_col );
	}
    }
}

void CustomPalette::slotChangeColorName( const QString &new_name )
{
    if ( sel_row != -1 && sel_col != -1 )
    {
        color_names[sel_row][sel_col] = new_name;

	QPtrList<Color> cl = k_toon -> document() -> getPalette() -> getColors();
	Color *s_color = cl.at( map2Dto1D( sel_row, sel_col ) );
	s_color -> setNameColor( new_name );
    }
}

//------------- EVENTS AND PROTECTED MEMBERS ------------

void CustomPalette::mousePressEvent( QMouseEvent *mouse_event )
{
    int row_clicked, col_clicked;
    mapPointToCell( mouse_event -> pos(), &row_clicked, &col_clicked );

    //Out of bounds
    if ( row_clicked > 11 || col_clicked > 18 || row_clicked < 0 || col_clicked < 0 )
    {
	mouse_event -> ignore();
	return;
    }

    int old_sel_row, old_sel_col;
    old_sel_row = sel_row;
    old_sel_col = sel_col;
    QColor color_selected = color_matrix[row_clicked][col_clicked];
    if ( color_selected.isValid() )
    {
        sel_row = row_clicked;
	sel_col = col_clicked;
	if ( old_sel_row != -1 && old_sel_col != -1 )
	    updateCell( old_sel_row, old_sel_col );
	updateCell( sel_row, sel_col );
	emit colorNameChanged( color_names[sel_row][sel_col] );
        emit colorSelected( color_selected );
	emit colorAlphaChanged( alpha_matrix[sel_row][sel_col] );
        mouse_event -> accept();
    }
}

void CustomPalette::paintCell( QPainter *painter, int row, int col )
{
    QColor cell_color = color_matrix[row][col];

    if ( cell_color.isValid() )
    {
        if ( sel_row == row && sel_col == col )
	    painter -> setPen( Qt::white );
	else
            painter -> setPen( cell_border_color );
        painter -> setBrush( cell_color );
        painter -> drawRect( 0, 0, cellWidth(), cellHeight() );
    }
    else
    {
        painter -> setPen( Qt::white );
        painter -> setBrush( Qt::white );
        painter -> drawRect( 0, 0, cellWidth(), cellHeight() );
    }
}

void CustomPalette::mapPointToCell( const QPoint &p, int *out_row, int *out_col )
{
    *out_row = p.y() / 9;
    *out_col = p.x() / 9;
}

int CustomPalette::map2Dto1D( int row, int col )
{
    return row * 19 + col;
}
