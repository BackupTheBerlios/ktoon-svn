/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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
#include "ktdisplaybrush.h"
#include <QPainter>
#include "ktdebug.h"

KTDisplayBrush::KTDisplayBrush(int minThickness, int maxThickness, QWidget *parent)
	: QWidget(parent), m_minThickness(minThickness), m_maxThickness(maxThickness)
{
	show();
}


KTDisplayBrush::~KTDisplayBrush()
{
}

void KTDisplayBrush::paintEvent ( QPaintEvent * event )
{
	QPainter p(this);
	p.setPen(foregroundColor ());
	
	p.save();
	p.setBrush( QBrush( foregroundColor (), Qt::SolidPattern));
	p.drawEllipse ( width()/2 - m_minThickness/2, height()/2 - m_minThickness/2 , m_minThickness, m_minThickness);
	p.setBrush(QBrush( foregroundColor (),Qt::Dense3Pattern));
	p.drawEllipse ( width()/2 - m_maxThickness/2,height()/2 - m_maxThickness/2, m_maxThickness, m_maxThickness);
	p.restore();
	p.drawRect( (width()/2- minimumSize().width()/2)-1, (height()/2 - minimumSize().height()/2), minimumSize().width(), minimumSize().height()-1);
	p.end();
}

void KTDisplayBrush::changeMinThickness(int minThickness)
{
	m_minThickness = minThickness;
	repaint();
}

void KTDisplayBrush::changeMaxThickness(int maxThickness)
{
	m_maxThickness = maxThickness;
	repaint();
}
