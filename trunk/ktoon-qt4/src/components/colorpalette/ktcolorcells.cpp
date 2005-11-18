/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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
 
#include "ktcolorcells.h"
#include <QPainter>
#include "ktdebug.h"

KTColorItemCells::KTColorItemCells(int row, int col, QColor c, QWidget *parent): QFrame(parent), m_row(row), m_col(col), m_color(c)
{

	QPalette p = palette();
	p.setColor( QPalette::Background, c);
	setPalette ( p );
// 	setMaximumSize(10,10);
}

KTColorItemCells::~KTColorItemCells()
{
}

void KTColorItemCells::mousePressEvent ( QMouseEvent * e )
{
	emit selectColor(m_row, m_col, m_color);
}

void KTColorItemCells::resizeEvent ( QResizeEvent * event )
{
// 	adjustSize();
}

// QSize  KTColorItemCells::sizeHint () const
// {
// 	int size = qMin ( height (), width() );
// 	return  QSize(size, size);
// }

KTColorCells::KTColorCells(QWidget *parent): 
		QFrame(parent), m_columns(0), m_rows(0), m_maxColumns(19), m_maxRows(12), m_currentItem(0)
{
	m_layout = new QGridLayout;
	m_layout->setOriginCorner ( Qt::TopLeftCorner );
	m_layout->setSpacing(2);
	m_layout->setMargin(3);

	setLayout(m_layout);
}


KTColorCells::~KTColorCells()
{
}

void KTColorCells::fillColorsDefault()
{
	int i, j;

    //First column, first 6 rows, a gray scale
	for ( i = 0; i <= 5; i++ )
		addColor(QColor( i * 51, i * 51, i * 51 ));

    //First column, last 6 rows, basic colors
	addColor(QColor( 255, 0, 0 ));
	addColor(QColor( 0, 255, 0 ));
	addColor(QColor( 0, 0, 255 ));
	addColor(QColor( 255, 255, 0 ));
	addColor(QColor( 0, 255, 255 ));
	addColor(QColor( 255, 0, 255 ));

    //Segment from column 1 to 6 and row 0 to 5
	for ( i = 0; i <= 5; i++ )
		for ( j = 1; j <= 6; j++ )
			addColor(QColor( 0, ( j - 1 ) * 51, i * 51 ));

    //Segment from column 1 to 6 and row 6 to 11
	for ( i = 6; i <= 11; i++ )
		for ( j = 1; j <= 6; j++ )
			addColor(QColor( 153, ( j - 1 ) * 51, ( i - 6 ) * 51 ));

    //Segment from column 7 to 12 and row 0 to 5
	for ( i = 0; i <= 5; i++ )
		for ( j = 7; j <= 12; j++ )
			addColor(QColor( 51, ( j - 7 ) * 51, i * 51 ));

    //Segment from column 7 to 12 and row 6 to 11
	for ( i = 6; i <= 11; i++ )
		for ( j = 7; j <= 12; j++ )
			addColor(QColor( 204, ( j - 7 ) * 51, ( i - 6 ) * 51 ));

    //Segment from column 13 to 18 and row 0 to 5
	for ( i = 0; i <= 5; i++ )
		for ( j = 13; j <= 18; j++ )
			addColor(QColor( 102, ( j - 13 ) * 51, i * 51 ));

    //Segment from column 13 to 18 and row 6 to 11
	for ( i = 6; i <= 11; i++ )
		for ( j = 13; j <= 18; j++ )
			addColor(QColor( 255, ( j - 13 ) * 51, ( i - 6 ) * 51 ));
}

void KTColorCells::addColor(QColor color )
{
	
	m_colors.append(color);
	
	if(m_rows == m_maxRows)
	{
		m_rows= 0;
		m_columns++;
	}
	
	KTColorItemCells *itemColor = new KTColorItemCells(m_rows, m_columns, color, this);
	m_layout->addWidget(itemColor, m_rows, m_columns, Qt::AlignLeft | Qt::AlignTop);
	itemColor->setMaximumSize(10,10);
	m_currentRow = m_rows;
	m_currentCol = m_columns;
	m_rows++;
	connect (itemColor, SIGNAL(selectColor(int, int , const QColor& )),this, SLOT(selectColor(int, int , const QColor &)));

}

void KTColorCells::addColors(QList<QColor> colors)
{
}

void KTColorCells::paintEvent(QPaintEvent *e)
{

	QPainter p;
	p.begin(this);
	p.setPen(QPen(palette().color(QPalette::Highlight), 2));
	p.drawRect( m_layout->cellRect(m_currentRow, m_currentCol).normalized().adjusted(-1, -1, +1, +1) );
	p.end();
	
}

void KTColorCells::selectColor(int row, int colm , const QColor& color)
{
	m_currentRow = row;
	m_currentCol = colm;
	ktDebug() << "Seleceted " << row << "," << colm;
	repaint();
	emit changeColor(color);
}


void KTColorCells::resizeEvent ( QResizeEvent * event )
{
// 	int size = qMin ( height (), width() );
// 	resize(size, size);
}


// QSize  KTColorCells::sizeHint () const
// {
// 	int size = qMin ( height (), width() );
// 	return  QSize(size, size);
// }
