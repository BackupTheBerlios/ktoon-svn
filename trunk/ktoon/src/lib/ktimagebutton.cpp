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
 
#include "ktimagebutton.h"
#include <qpainter.h>
#include <iostream>

KTImageButton::KTImageButton(const QImage &image, QWidget *parent)
	: QPushButton(parent), m_image(image)
{
	m_drawer.setOptimization( QPixmap::BestOptim );
	m_drawer.resize( size().width(), size().height() );
	m_drawer.fill();
	
	
	setMaximumSize( 45, 45 );
}


KTImageButton::~KTImageButton()
{
}


void KTImageButton::resizeEvent(QResizeEvent *e)
{
	QPainter painter( &m_drawer );

	painter.setBackgroundColor( paletteBackgroundColor() );
	painter.eraseRect( m_drawer.rect() );

	painter.end();
	

	QPixmap buffer = getPixmap();

	bitBlt( &m_drawer, size().width(), size().height(), &buffer, 0, 0, buffer.width(), buffer.height(), Qt::CopyROP );

	repaint( false );
}

QPixmap KTImageButton::getPixmap()
{
	return QPixmap( m_image.smoothScale(44, 44) );
}

void KTImageButton::paintEvent(QPaintEvent *ev)
{
	QRegion reg = ev->region();

	QPainter painter( this );
	painter.setClipRegion( reg );
	painter.drawPixmap( 0, 0, m_drawer );

	painter.end();
	
	QPushButton::paintEvent(ev);
}

