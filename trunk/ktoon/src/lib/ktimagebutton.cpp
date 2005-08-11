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

KTImageButton::KTImageButton(const QPixmap &image, int size, QWidget *parent)
	: QPushButton(parent), m_imageSize(size)
{
	setup();
	
	setPixmap(image);
}


KTImageButton::~KTImageButton()
{
}

void KTImageButton::setup()
{
	setFlat( true );
	setAutoDefault( false );
	setMaximumSize(m_imageSize, m_imageSize);
}


// void KTImageButton::resizeEvent(QResizeEvent *e)
// {
// 	QPainter painter( &m_drawer );
// 
// 	painter.setBackgroundColor( paletteBackgroundColor() );
// 	painter.eraseRect( m_drawer.rect() );
// 
// 	painter.end();
// 	
// 
// 	QPixmap buffer = getPixmap();
// 
// 	bitBlt( &m_drawer, size().width(), size().height(), &buffer, 0, 0, buffer.width(), buffer.height(), Qt::CopyROP );
// 
// 	repaint( false );
// }


void KTImageButton::setPixmap ( const QPixmap & pix)
{
	QImage imgTmp(pix.convertToImage () );
	QPixmap newPixmap(imgTmp.smoothScale(m_imageSize,m_imageSize));
	
	QPushButton::setPixmap(newPixmap);
}


