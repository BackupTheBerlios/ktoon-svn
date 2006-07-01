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

#include "dosd.h"

#include <QApplication>
#include <QBitmap>
#include <QTimer>
#include <QPainter>

#include "dcore/ddebug.h"

DOsd::DOsd( QWidget * parent )
	: QWidget( parent), m_timer( 0 )
{
	setFocusPolicy( Qt::NoFocus );
	
	m_palette = palette();
// 	setBackgroundMode( Qt::NoBackground );
	
	move( 10, 10 );
	resize( 0, 0 );
	hide();
	
	m_animator = new Animation;
	connect(&m_animator->timer, SIGNAL(timeout()), this, SLOT(animate()));
}

DOsd::~DOsd()
{
	delete m_animator;
}

void DOsd::display( const QString & message, Level level, int ms )
{
	if ( ms < 0 )
	{
		ms = message.length() * 80;
	}
	
	m_animator->level = level;
	
	QBrush background = palette().background();
	QBrush foreground = palette().foreground();
	
	// determine text rectangle
	QRect textRect = fontMetrics().boundingRect( message );
	textRect.translate( -textRect.left(), -textRect.top() );
	textRect.adjust( 0, 0, 2, 2 );
	int width = textRect.width();
	int height = textRect.height();

	if ( level != None )
	{
		switch ( level )
		{
			case Info:
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
	}
	QRect geometry( 0, 0, width + 10, height + 8 );
	QRect geometry2( 0, 0, width + 9, height + 7 );

    	// resize pixmap, mask and widget
	static QBitmap mask;
	mask = QBitmap( geometry.size() );
	m_pixmap = QPixmap( geometry.size() );
	resize( geometry.size() );

    	// create and set transparency mask
	QPainter maskPainter(&mask);
	maskPainter.setRenderHint(QPainter::Antialiasing);
	mask.fill( Qt::white );
	maskPainter.setBrush( Qt::black );
	maskPainter.drawRoundRect( geometry2, 1600 / geometry2.width(), 1600 / geometry2.height() );
	setMask( mask );
	
	
	drawPixmap( message, background, foreground);
	
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
		
		m_animator->timer.start(300);
		m_timer->start( ms );
	} else if ( m_timer )
	{
		m_timer->stop();
	}
	
	m_lastMessage = message;
}

void DOsd::paintEvent( QPaintEvent * e )
{
	QPainter p( this );
	p.drawPixmap( e->rect().topLeft(), m_pixmap, e->rect() );
}

void DOsd::mousePressEvent( QMouseEvent * /*e*/ )
{
	if ( m_timer )
		m_timer->stop();
	hide();
}

void DOsd::animate()
{
	if ( !isVisible() )
	{
		m_animator->timer.stop();
	}
	
	QBrush background;
	
	if ( m_animator->level == Error )
	{
		if ( m_animator->on )
		{
			background = Qt::red;
		}
		else
		{
			background = palette().background();
		}
	}
	else if ( m_animator->level == Warning )
	{
		if ( m_animator->on )
		{
			background = QColor("orange");
		}
		else
		{
			background = palette().background();
		}
	}
	
	m_animator->on = !m_animator->on;
	drawPixmap( m_lastMessage, background, palette().foreground());
	
	update();
}

void DOsd::drawPixmap(const QString &message, const QBrush &background, const QBrush &foreground)
{
	QPixmap symbol;
	
	QRect textRect = fontMetrics().boundingRect( message );
	textRect.translate( -textRect.left(), -textRect.top() );
	textRect.adjust( 0, 0, 2, 2 );
	int width = textRect.width();
	int height = textRect.height();
	int textXOffset = 0;
	int shadowOffset = message.isRightToLeft() ? -1 : 1;
	
	QRect geometry( 0, 0, width + 10, height + 8 );
	QRect geometry2( 0, 0, width + 9, height + 7 );
	
	textXOffset = 2;
	width += textXOffset;
	height = qMax( height, symbol.height() );
	
	// draw background
	QPainter bufferPainter( &m_pixmap );
	bufferPainter.setRenderHint(QPainter::Antialiasing);
	bufferPainter.setPen( QPen(QBrush(foreground), 3)  );
	bufferPainter.setBrush( background ); 
	bufferPainter.drawRoundRect( geometry2, 1600 / geometry2.width(), 1600 / geometry2.height() );
	 
	// draw shadow and text
	int yText = geometry.height() - height / 2;
	bufferPainter.setPen( palette().background().color().dark( 115 ) );
	bufferPainter.drawText( 5 + textXOffset + shadowOffset, yText + 1, message );
	bufferPainter.setPen( palette().foreground().color() );
	bufferPainter.drawText( 5 + textXOffset, yText, message );
}

