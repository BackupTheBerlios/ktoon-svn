/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include "kttimelineframe.h"
#include <QPainter>

KTTimeLineFrame::KTTimeLineFrame(int id, QWidget *parent) : QWidget(parent), m_ID(id)
{
	m_isUsed = m_isSpecial = m_isSelected = m_isOffset = m_isKey = false;
	
// 	setMinimumSize(10,18);
	setMinimumWidth(10);

}


KTTimeLineFrame::~KTTimeLineFrame()
{
}

void KTTimeLineFrame::setSelected(bool opt)
{
	m_isSelected = opt;
	repaint();
}

void KTTimeLineFrame::setSpecial(bool opt)
{
	m_isSpecial = opt;
}

void KTTimeLineFrame::setKey(bool opt)
{
	m_isKey = opt;
}

void KTTimeLineFrame::setUsed(bool opt)
{
	m_isUsed = opt;
}

void KTTimeLineFrame::setOffset(bool opt)
{
}

void KTTimeLineFrame::setId(int id)
{
	m_ID = id;
}
	
void KTTimeLineFrame::mousePressEvent( QMouseEvent *mEv )
{
	setSelected( true );
	mEv->accept();
	
	emit selected(m_ID);
	if ( mEv->button() == Qt::RightButton )
	{
// 		right_click_menu -> exec( QCursor::pos() );
	}
}

void KTTimeLineFrame::paintEvent( QPaintEvent *pev )
{
	QPainter p( this );
	
	//Default paint settings
	p.setPen( QColor( 180, 180, 180 ) );
	p.setBrush( QColor( 255, 255, 255 ) );
	p.drawRect( 0, 0, width(), height() );

	if ( m_isSelected )
	{
		paintSelected(&p);
	}
	else
	{
		paintNotSelected(&p);
	}
	
	//Draw the offset line if it applies
// 	if ( is_offset )
// 	{
// 		p.setPen( QColor( 0, 0, 0 ) );
// 		p.drawLine( width() / 2, 0, width() / 2, height()-1 );
// 	}

	//Draw the drag offset line if it applies
// 	if ( is_drag_offset )
// 	{
// 		p.setPen( QColor( 0, 0, 0 ) );
// 		p.drawLine( width() / 2, 0, width() / 2, height()-1 );
// 	}

	p.end();
}

void KTTimeLineFrame::paintSelected(QPainter *p)
{
	QPalette pal = palette();
	if ( !m_isUsed )
	{
		p->setPen( QColor( 180, 180, 180 ) );
		p->setBrush( pal.dark() );
		p->drawRect( 0, 0, width(), height() );
	}
	else
	{
	}
}

void KTTimeLineFrame::paintNotSelected(QPainter *p)
{
	if ( !m_isUsed )
	{
		if ( m_isSpecial )
		{
			p->setPen( QColor( 180, 180, 180 ) );
			p->setBrush( QColor( 200, 200, 200 ) );
			p->drawRect( 0, 0, 10, height() );
		}
	}
	else
	{
		
	}
}
