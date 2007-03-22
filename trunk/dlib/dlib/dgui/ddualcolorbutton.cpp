/* This file is part of the KDE libraries
   Copyright (C) 1999 Daniel M. Duley <mosfet@kde.org>
   David Cuadrado 2005 <krawek@gmail.com>
 
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.
 
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.
 
   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include "ddualcolorbutton.h"

#include "dcolorarrow.xpm"
#include "dcolorreset.xpm"

#include <qpainter.h>
#include <qbitmap.h>

#include <qdrawutil.h>

#include <QDragEnterEvent>
#include <QPixmap>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QDropEvent>
#include <QApplication>

#include "ddebug.h"
#include "dbrushadjuster.h"

DDualColorButton::DDualColorButton( QWidget *parent ) : QWidget( parent )
{
	arrowBitmap = new QPixmap((const char **)dcolorarrow_bits);
	
	arrowBitmap->setMask(*arrowBitmap); // heh
	resetPixmap = new QPixmap((const char **)dcolorreset_xpm);
	fg = QBrush(Qt::black, Qt::SolidPattern);
	bg = QBrush( QColor(0,0,0,0), Qt::SolidPattern);
	
	curColor = Foreground;
	dragFlag = false;
	miniCtlFlag = false;
	if(sizeHint().isValid())
	{
		setMinimumSize(sizeHint());
// 		setMaximumSize(sizeHint()); // FIXME: Remove if is necessary
	}
	
	setAcceptDrops(true);
}

DDualColorButton::DDualColorButton(const QBrush &fgColor, const QBrush &bgColor, QWidget *parent) : QWidget(parent)
{
	arrowBitmap = new QPixmap((const char **)dcolorarrow_bits);
	arrowBitmap->setMask(*arrowBitmap);
	resetPixmap = new QPixmap((const char **)dcolorreset_xpm);
	fg = fgColor;
	bg = bgColor;
	curColor = Foreground;
	dragFlag = false;
	miniCtlFlag = false;
	if(sizeHint().isValid())
	{
		setMinimumSize(sizeHint());
// 		setMaximumSize(sizeHint());
	}
	setAcceptDrops(true);
}

DDualColorButton::~DDualColorButton()
{
	delete arrowBitmap;
	delete resetPixmap;
	DEND;
}

QBrush DDualColorButton::foreground() const
{
	return fg;
}

QBrush DDualColorButton::background() const
{
	return bg;
}

DDualColorButton::DualColor DDualColorButton::current() const
{
	return curColor;
}

QBrush DDualColorButton::currentColor() const
{
	return (curColor == Background ? bg : fg);
}

QSize DDualColorButton::sizeHint() const
{
	return QSize(34, 34);
}

void DDualColorButton::setForeground(const QBrush &c)
{
	fg = c;
	update();

	emit fgChanged(fg.color());
}

void DDualColorButton::setBackground(const QBrush &c)
{
	bg = c;
	update();

	emit bgChanged(bg.color());
}

void DDualColorButton::setCurrentColor(const QBrush &c)
{
	if(curColor == Background)
	{
		bg = c;
	}
	else
	{
		fg = c;
	}
	update();
}

void DDualColorButton::setCurrent(DualColor s)
{
	curColor = s;
	update();
}

void DDualColorButton::metrics(QRect &fgRect, QRect &bgRect)
{
	fgRect = QRect(0, 0, width()-14, height()-14);
	bgRect = QRect(14, 14, width()-14, height()-14);
}

void DDualColorButton::paintEvent(QPaintEvent *)
{
	QRect fgRect, bgRect;
	
	QPalette pal = palette();
	QPainter p(this);
	
	metrics(fgRect, bgRect);
	QBrush defBrush = pal.color(QPalette::Button);
	
	
	qDrawShadeRect(&p, bgRect, pal, curColor == Background, 2, 0, isEnabled() ? &DBrushAdjuster::adjustBrush(bg, bgRect) : &defBrush);
	qDrawShadeRect(&p, fgRect,  pal, curColor == Foreground, 2, 0, isEnabled() ? &DBrushAdjuster::adjustBrush(fg, fgRect) : &defBrush);
	p.setPen(QPen(palette().shadow().color()));
	
	p.drawPixmap(fgRect.right()+2, 0, *arrowBitmap);
	p.drawPixmap(0, fgRect.bottom()+2, *resetPixmap);

}

void DDualColorButton::dragEnterEvent(QDragEnterEvent *ev)
{
// 	ev->accept(isEnabled() && KColorDrag::canDecode(ev));
	Q_UNUSED(ev);
}

void DDualColorButton::dropEvent(QDropEvent *ev)
{
	Q_UNUSED(ev);
// 	QColor c;
// 	if(KColorDrag::decode(ev, c))
// 	{
// 		if(curColor == Foreground)
// 		{
// 			fg.setColor(c);
// 			emit fgChanged(c);
// 		}
// 		else
// 		{
// 			bg.setColor(c);
// 			emit(bgChanged(c));
// 		}
// 		update();
// 	}
}

void DDualColorButton::mousePressEvent(QMouseEvent *ev)
{
	QRect fgRect, bgRect;
	metrics(fgRect, bgRect);
	mPos = ev->pos();
	tmpColor = curColor;
	dragFlag = false;
	if(fgRect.contains(mPos))
	{
		curColor = Foreground;
		emit currentChanged(Foreground);
		miniCtlFlag = false;
	}
	else if(bgRect.contains(mPos))
	{
		curColor = Background;
		emit currentChanged(Background);
		
		miniCtlFlag = false;
	}
	else if(ev->pos().x() > fgRect.width())
	{
    		// We handle the swap and reset controls as soon as the mouse is
    		// is pressed and ignore further events on this click (mosfet).
		QBrush c = fg;
		fg = bg;
		bg = c;
		emit fgChanged(fg);
// 		emit currentChanged(Foreground );
		emit bgChanged(bg);
// 		emit currentChanged(Background );
		miniCtlFlag = true;
	}
	else if(ev->pos().x() < bgRect.x())
	{
		fg.setColor(Qt::black);
		bg.setColor( QColor(0,0,0,0));
		emit fgChanged(fg);
		emit bgChanged(bg);
// 		emit currentChanged(Foreground );
// 		emit currentChanged(Background );
		miniCtlFlag = true;
	}
	update();
}

void DDualColorButton::mouseMoveEvent(QMouseEvent* ev)
{
	QWidget::mouseMoveEvent( ev );

	if ((ev->pos() - mPos).manhattanLength() <  QApplication::startDragDistance())
		return;

	QDrag *drag = new QDrag( this );
	QPixmap pix( 25, 25 );
	
	QRect fgRect, bgRect;
	metrics(fgRect, bgRect);
	QPainter painter( &pix );
	
	QBrush curr;
	if ( fgRect.contains( mPos ) )
	{
		curr = foreground();
	}
	else if ( bgRect.contains( mPos ) )
	{
		curr = background();
	}
	else
	{
		return;
	}
	
	
	painter.fillRect( pix.rect(), curr );
	painter.drawRect( 0, 0, pix.width(), pix.height() );
	painter.end();
	
	QMimeData *mimeData = new QMimeData;
	mimeData->setColorData( curr.color() );
	
	drag->setMimeData(mimeData);
	drag->setPixmap( pix );
	
	Qt::DropAction dropAction = drag->start(Qt::MoveAction);
}


