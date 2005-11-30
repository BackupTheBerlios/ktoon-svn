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
#include "ktviewcolorcells.h"
#include "ktdebug.h"
#include <QScrollArea>


KTViewColorCells::KTViewColorCells(QWidget *parent)
	: QFrame(parent), m_numColorRecent(0), MAX_COLUMS(11)
{
	
	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);
	setFrameStyle ( QFrame::Box | QFrame::Raised);
	setupForm();
}


KTViewColorCells::~KTViewColorCells()
{
}

void KTViewColorCells::setupForm()
{
	m_chooserPalette = new QComboBox(this);
	
	m_chooserPalette->addItem(tr("Default Palette"));
	m_containerPalette = new QStackedWidget(this);
	layout()->addWidget(m_chooserPalette);
	layout()->addWidget(m_containerPalette);
	
	m_defaultPalette = new  KTCellView(11,18, m_containerPalette );
	fillDefaultColors();
	
	connect(m_defaultPalette, SIGNAL(itemPressed( KTCellViewItem* )), this, SLOT(changeColor(KTCellViewItem*)));
	
	m_containerPalette->addWidget(m_defaultPalette);
	
	m_chooserPalette->addItem(tr("Custom Color Palette"));
	m_customColorPalette = new  KTCellView(m_containerPalette);
	connect(m_customColorPalette, SIGNAL(itemPressed( KTCellViewItem* )), this, SLOT(changeColor(KTCellViewItem*)));
	m_containerPalette->addWidget(m_customColorPalette);
	

	
	connect(m_chooserPalette, SIGNAL(activated ( int  )), m_containerPalette, SLOT(setCurrentIndex ( int )));
}

void KTViewColorCells::addPalette(const QString & name)
{
// 	QScrollArea *scroll = new QScrollArea(m_containerPalette);

	KTCellView *palette = new  KTCellView(11,18, m_containerPalette);
	
// 	scroll->setWidget(palette);
	
	connect(palette, SIGNAL(itemPressed( KTCellViewItem* )), this, SLOT(changeColor(KTCellViewItem*)));
	m_chooserPalette->addItem(name);
	m_containerPalette->addWidget(palette);
	
}


void KTViewColorCells::changeColor(KTCellViewItem* item)
{
	if(item)
	{
		if(item->background().gradient ())
		{
			emit selectGradient( *item->background().gradient ());
		}
		else
		{
			emit selectColor(item->background());
		}
	}
}

void KTViewColorCells::fillDefaultColors()
{
	int i, j;
	j =0;
    //First column, first 6 rows, a gray scale
	for ( i = 0; i <= 5; i++ )
	{
		addDefaultColor(i,j,QColor( i * 51, i * 51, i * 51 ));
	}
    //First column, last 6 rows, basic colors
	addDefaultColor(i,j,QColor( 255, 0, 0 ));
	addDefaultColor(i,j,QColor( 0, 255, 0 ));
	addDefaultColor(i,j,QColor( 0, 0, 255 ));
	addDefaultColor(i,j,QColor( 255, 255, 0 ));
	addDefaultColor(i,j,QColor( 0, 255, 255 ));
	addDefaultColor(i,j,QColor( 255, 0, 255 ));

    //Segment from column 1 to 6 and row 0 to 5
	for ( i = 0; i <= 5; i++ )
	{
		for ( j = 1; j <= 6; j++ )
		{
			addDefaultColor(i,j,QColor( 0, ( j - 1 ) * 51, i * 51 ));
		}
	}

    //Segment from column 1 to 6 and row 6 to 11
	for ( i = 6; i <= 11; i++ )
	{
		for ( j = 1; j <= 6; j++ )
		{
			addDefaultColor(i,j,QColor( 153, ( j - 1 ) * 51, ( i - 6 ) * 51 ));
		}
	}

    //Segment from column 7 to 12 and row 0 to 5
	for ( i = 0; i <= 5; i++ )
		for ( j = 7; j <= 12; j++ )
			addDefaultColor(i,j,QColor( 51, ( j - 7 ) * 51, i * 51 ));

    //Segment from column 7 to 12 and row 6 to 11
	for ( i = 6; i <= 11; i++ )
		for ( j = 7; j <= 12; j++ )
			addDefaultColor(i,j,QColor( 204, ( j - 7 ) * 51, ( i - 6 ) * 51 ));

    //Segment from column 13 to 18 and row 0 to 5
	for ( i = 0; i <= 5; i++ )
		for ( j = 13; j <= 18; j++ )
			addDefaultColor(i,j,QColor( 102, ( j - 13 ) * 51, i * 51 ));

    //Segment from column 13 to 18 and row 6 to 11
	for ( i = 6; i <= 11; i++ )
		for ( j = 13; j <= 18; j++ )
			addDefaultColor(i,j,QColor( 255, ( j - 13 ) * 51, ( i - 6 ) * 51 ));
	
	
}

void KTViewColorCells::addDefaultColor(int i, int j, const QColor &c)
{
	KTCellViewItem *item = new KTCellViewItem;
	item->setBackground(c);
	m_defaultPalette->setItem(i-1,j-1, item);

}

void KTViewColorCells::addCustomColor(const QBrush& c)
{
	KTCellViewItem *item = new KTCellViewItem;
	item->setBackground(c);

	static int col = 0;
	static int row = 0;
	
	if( m_customColorPalette->columnCount() < MAX_COLUMS)
	{
		m_customColorPalette->insertRow(  m_customColorPalette->rowCount()+1);
	}
	if( m_numColorRecent % MAX_COLUMS == 0)
	{
		m_customColorPalette->insertColumn(  ( m_customColorPalette->columnCount()+1));
		row++;
		col = 0;
	}
	else
	{
		col++;
	}
	
	m_numColorRecent++;
	m_customColorPalette->setItem( col, row-1, item);
	m_customColorPalette->setCurrentItem( item);
}

void KTViewColorCells::removeCurrentColor()
{
	KTCellView *palette = qobject_cast<KTCellView *>(m_containerPalette->currentWidget());
	if(palette)
	{
		if( m_defaultPalette != palette)
		{
			//TODO: implementar en KTCellView removeItem
		}
	}
}
