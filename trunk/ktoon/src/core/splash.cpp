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

#include "splash.h"
#include "images.h"
#include <qpixmap.h>
#include <qbitmap.h>
#include <qimage.h>

//------------------ CONSTRUCTOR -----------------

Splash::Splash() : QSplashScreen( QPixmap( splash_xpm ), Qt::WStyle_StaysOnTop )
{
	setFont( QFont( "helvetica", 10 ) );
	QPixmap pixmap(splash_xpm);
	
	QImage img = pixmap.convertToImage();
	
	if ( !pixmap.mask() )
	{
		if ( img.hasAlphaBuffer() )
		{
		QBitmap bitmap;
		bitmap = img.createAlphaMask();
		pixmap.setMask( bitmap );
		}
		else
		{
		QBitmap bitmap;
		bitmap = img.createHeuristicMask();
		pixmap.setMask( bitmap );
		}
	}
	if ( pixmap.mask() )
		this -> setMask( *pixmap.mask() );
}

Splash::~Splash()
{
}

