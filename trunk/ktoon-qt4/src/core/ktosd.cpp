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

#include "ktosd.h"

#include <QApplication>
#include <QBitmap>
#include <QTimer>
#include <QPainter>

KTOsd::KTOsd( QWidget * parent )
	: QWidget( parent, "pageViewMessage" ), m_timer( 0 )
{
	setFocusPolicy( Qt::NoFocus );
	
	QPalette pal = palette();
// 	setBackgroundMode( Qt::NoBackground );
// 	setPaletteBackgroundColor(qApp->palette().color(QPalette::Active, QColorGroup::Background));
	
// 	pal.setColor(QPalette::Background, palette.color(QPalette::Background) );
	
	
	move( 10, 10 );
	resize( 0, 0 );
	hide();
}

KTOsd::~KTOsd()
{
}

void KTOsd::display( const QString & message, Level level, int ms )
{
	QBrush background = palette().background();
	QBrush foreground = palette().foreground();
	
	// determine text rectangle
	QRect textRect = fontMetrics().boundingRect( message );
	textRect.moveBy( -textRect.left(), -textRect.top() );
	textRect.addCoords( 0, 0, 2, 2 );
	int width = textRect.width(),
	height = textRect.height(),
	textXOffset = 0,
	shadowOffset = message.isRightToLeft() ? -1 : 1;

	QPixmap symbol;
	if ( level != None )
	{
		switch ( level )
		{
			case Info:
// 				background = QColor(0xd2ccff);
				background = palette().color(QPalette::Highlight);
				foreground = palette().color(QPalette::HighlightedText);
				break;
			case Warning:
				break;
			case Error:
				background = Qt::red;
				break;
			case Fatal:
				background = Qt::magenta;
				break;
			default:
				break;
		}
		textXOffset = 2 + symbol.width();
		width += textXOffset;
		height = qMax( height, symbol.height() );
	}
	QRect geometry( 0, 0, width + 10, height + 8 );
	QRect geometry2( 0, 0, width + 9, height + 7 );

    	// resize pixmap, mask and widget
	static QBitmap mask;
	mask.resize( geometry.size() );
	m_pixmap.resize( geometry.size() );
	resize( geometry.size() );

    	// create and set transparency mask
	QPainter maskPainter(&mask);
	maskPainter.setRenderHint(QPainter::Antialiasing);
	mask.fill( Qt::white );
	maskPainter.setBrush( Qt::black );
	maskPainter.drawRoundRect( geometry2, 1600 / geometry2.width(), 1600 / geometry2.height() );
	setMask( mask );

    	// draw background
	QPainter bufferPainter( &m_pixmap );
	bufferPainter.setRenderHint(QPainter::Antialiasing);
	bufferPainter.setPen( foreground  );
	bufferPainter.setBrush( background ); 
	bufferPainter.drawRoundRect( geometry2, 1600 / geometry2.width(), 1600 / geometry2.height() );

    	// draw icon if present
	if ( !symbol.isNull() )
	{
		bufferPainter.drawPixmap( 5, 4, symbol, 0, 0, symbol.width(), symbol.height() );
	}
	
    	// draw shadow and text
	int yText = geometry.height() - height / 2;
	bufferPainter.setPen( paletteBackgroundColor().dark( 115 ) );
	bufferPainter.drawText( 5 + textXOffset + shadowOffset, yText + 1, message );
	bufferPainter.setPen( foregroundColor() );
	bufferPainter.drawText( 5 + textXOffset, yText, message );

    	// show widget and schedule a repaint
	show();
	update();

    	// close the message window after given mS
	if ( ms > 0 )
	{
		if ( !m_timer )
		{
			m_timer = new QTimer( this );
			connect( m_timer, SIGNAL( timeout() ), SLOT( hide() ) );
		}
		m_timer->start( ms, true );
	} else if ( m_timer )
	{
		m_timer->stop();
	}
}

void KTOsd::paintEvent( QPaintEvent * e )
{
	QPainter p( this );
	p.drawPixmap( e->rect().topLeft(), m_pixmap, e->rect() );
}

void KTOsd::mousePressEvent( QMouseEvent * /*e*/ )
{
	if ( m_timer )
		m_timer->stop();
	hide();
}

