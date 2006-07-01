/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#include "dcirclebuttonbar.h"

#include <QApplication>

DCircleButtonBar::DCircleButtonBar(int radio, QWidget *parent) : QFrame(parent), m_radio(radio), m_buttonCount(0), m_offset(30)
{	
	m_layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
	m_layout->setSpacing(0);
	m_layout->setMargin(0);
	
	m_mask = QPixmap( (m_buttonCount+1)*m_radio+m_offset*2+m_offset/2, m_radio+10);
	m_mask.fill(Qt::transparent);
	
	setLayout(m_layout);
}


DCircleButtonBar::~DCircleButtonBar()
{
}

void DCircleButtonBar::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	
	setMinimumSize( m_mask.size() );
	
	painter.setPen(QPen(palette().color(QPalette::Foreground ),5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	
	painter.drawPath(m_border);
	
	QPalette pal = palette();
	
	pal.setColor(QPalette::Background, pal.color(QPalette::Button ));
	setPalette(pal);
	
}

DCircleButton *DCircleButtonBar::addButton(const QPixmap &pix)
{
	m_buttonCount++;
	
	m_mask = QPixmap( (m_buttonCount+1)*m_radio+m_offset*3, m_radio+10);
	m_mask.fill(Qt::transparent);
	
	DCircleButton *but = new DCircleButton(m_radio,true, this);
	
	
	m_layout->addWidget(but);
	but->setIcon(pix);
	
	return but;
}

void DCircleButtonBar::resizeEvent(QResizeEvent *)
{
	m_border = QPainterPath();
	m_border.moveTo(m_offset, 0);
	m_border.cubicTo(
			m_offset, 0,
	0, m_mask.height()/2,
	m_offset, m_mask.height()
			);
	
	m_border.lineTo(m_mask.width()-m_offset,  m_mask.height());
	
	m_border.cubicTo(
			m_mask.width()-m_offset,  m_mask.height(),
	m_mask.width(), m_mask.height()/2,
	m_mask.width()-m_offset, 0
			);
	m_border.lineTo(m_offset, 0);
	
	QPainter p(&m_mask);
	p.setPen(QPen(Qt::black,1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	p.setBrush(Qt::red);
	p.drawPath(m_border);
	
	setMask(m_mask.mask());
}

