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



KTColorItemCells::KTColorItemCells(QColor c, QWidget *parent): QFrame(parent), m_color(c)
{
	QPalette p = palette();
	p.setColor( QPalette::QPalette::Background, c);
	setPalette ( p );
	setMaximumSize(10,10);
}

KTColorItemCells::~KTColorItemCells()
{
}

void KTColorItemCells::mousePressEvent ( QMouseEvent * e )
{
	emit selectColor(m_color);
}

KTColorCells::KTColorCells(QWidget *parent): QFrame(parent)
{
	m_layout = new QGridLayout;
	setLayout(m_layout);
}


KTColorCells::~KTColorCells()
{
	
}

void KTColorCells::addColor(QColor color )
{
	m_colors.append(color);
	KTColorItemCells *itemColor = new KTColorItemCells(color, this);
	m_layout->addWidget(itemColor);
	
}

void KTColorCells::addColors(QList<QColor> colors)
{
	
}
