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
#include <QDesktopWidget>
#include <QLinearGradient>

#include "dcore/ddebug.h"

DOsd *DOsd::s_osd = 0;

DOsd::DOsd( QWidget * parent ) : QWidget( parent), m_timer( 0 )
{
	setFocusPolicy( Qt::NoFocus );
	
	m_palette = palette();
// 	setBackgroundMode( Qt::NoBackground );
	
	move( 50, 50 );
	resize( 0, 0 );
	hide();
	
	m_animator = new Animation;
	connect(&m_animator->timer, SIGNAL(timeout()), this, SLOT(animate()));
	
	m_timer = new QTimer( this );
	connect( m_timer, SIGNAL( timeout() ), SLOT( hide() ) );
	
	
	setWindowFlags( Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::ToolTip );
	
	m_document = new QTextDocument(this);
}

DOsd::~DOsd()
{
	delete m_animator;
	delete m_timer;
}

void DOsd::display( const QString &message, Level level, int ms )
{
	if (message.isEmpty()) return;
	
	QString htmlMessage = message;
	
	htmlMessage.replace('\n', "<br/>");
	
	QBrush background = palette().background();
	QBrush foreground = palette().foreground();
	
	if ( level != None )
	{
		switch ( level )
		{
			case Info:
			{
				background = QColor(0x678eae);
				m_document->setHtml("<font size=+3 >"+tr("Information")+"</font><br><font size=+2>"+htmlMessage+"</font>");
			}
			break;
			case Warning:
			{
				m_document->setHtml("<font size=+3>"+tr("Attention")+"</font><br><font size=+2>"+htmlMessage+"</font>");
			}
			break;
			case Error:
			{
				background = Qt::red;
				m_document->setHtml("<font size=+3>"+tr("Error")+"</font><br><font size=+2>"+htmlMessage+"</font>");
			}
			break;
			case Fatal:
			{
				background = Qt::magenta;
				m_document->setHtml("<font size=+3>"+tr("Error")+"</font><br><font size=+2>"+htmlMessage+"</font>");
			}
			break;
			default:
			{
				m_document->setHtml(htmlMessage);
			}
			break;
		}
	}
	
	if ( ms < 0 )
	{
		ms = m_document->toPlainText().length() * 90;
	}
	
	m_animator->level = level;
	
	QSizeF textSize = m_document->size();
	
	int width = (int)textSize.width()+10;
	int height = (int)textSize.height()+10;
	
	QDesktopWidget desktop;
	move ( (desktop.screenGeometry().width() - textSize.width() ) - 50, 50 );
	
	
	QRect geometry( 0, 0, width + 10, height + 8 );
	QRect geometry2( 0, 0, width + 9, height + 7 );

	// resize pixmap, mask and widget
	/*static*/ QBitmap mask;
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
	
	maskPainter.end();
	
	drawPixmap( background, foreground );
	
    	// show widget and schedule a repaint
	show();
	update();

    	// close the message window after given mS
	if ( ms > 0 )
	{
		m_animator->timer.start(300);
		m_timer->start( ms );
	} 
	else if ( m_timer )
	{
		m_timer->stop();
	}
}

DOsd *DOsd::self()
{
	if ( ! s_osd )
	{
		s_osd = new DOsd(QApplication::desktop() );
	}
	
	return s_osd;
}

void DOsd::paintEvent( QPaintEvent * e )
{
	QPainter p( this );
	p.drawPixmap( e->rect().topLeft(), m_pixmap, e->rect() );
}

void DOsd::mousePressEvent( QMouseEvent *e )
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
	
	if ( m_animator->level == Info )
		return;
	
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
	else if ( m_animator->level == Fatal )
	{
		if ( m_animator->on )
		{
			background = Qt::magenta;
		}
		else
		{
			background = palette().background();
		}
	}
	
	m_animator->on = m_animator->on ? false : true;
	
	drawPixmap( background, palette().foreground() );
	
	repaint();
}

void DOsd::drawPixmap(const QBrush &background, const QBrush &foreground)
{
	QPixmap symbol;
	
	QRect textRect = QRect(QPoint(0, 0), m_document->size().toSize() );
	
	QSizeF textSize = m_document->size();
	
	int width = (int)textSize.width()+10;
	int height = (int)textSize.height()+10;
	
	int textXOffset = 0;
	int shadowOffset = QApplication::isRightToLeft() ? -1 : 1;
	
	QRect geometry( 0, 0, width + 10, height + 8 );
	QRect geometry2( 0, 0, width + 9, height + 7 );
	
	textXOffset = 2;
	
	width += textXOffset;
	height = qMax( height, symbol.height() );
	
	// draw background
	m_pixmap.fill( Qt::gray );
	QPainter bufferPainter( &m_pixmap );
	bufferPainter.setRenderHint(QPainter::Antialiasing);
	bufferPainter.setPen( QPen(QBrush(foreground), 3)  );
	
	
	
	QLinearGradient gradient(geometry.topLeft(), geometry.bottomLeft() );
	
	QColor color0 = background.color();
	color0.setAlpha(180);
	
	QColor color1 = palette().color( QPalette::Button);
	color1.setAlpha(180);
	
	gradient.setColorAt(0.0, color0 );
	gradient.setColorAt(1.0, color1 );
	gradient.setSpread(QGradient::ReflectSpread );
	
	bufferPainter.setBrush( gradient ); 
	bufferPainter.drawRoundRect( geometry2, 1600 / geometry2.width(), 1600 / geometry2.height() );
	 
	// draw shadow and text
// 	int yText = geometry.height() - height / 2;
	bufferPainter.setPen( palette().background().color().dark( 115 ) );
	
	
	bufferPainter.translate( 5 + textXOffset + shadowOffset, 1);
	
	m_document->drawContents(&bufferPainter,  QRect(0,0, textRect.width(), textRect.height() ));
	
	bufferPainter.translate( -shadowOffset, -1 );
	
	bufferPainter.setPen( palette().foreground().color() );
	m_document->drawContents(&bufferPainter,  QRect(0, 0, textRect.width(), textRect.height() ));
}

