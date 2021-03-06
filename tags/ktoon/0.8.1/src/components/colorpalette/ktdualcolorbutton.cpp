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

#include "ktdualcolorbutton.h"

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

#include "ddebug.h"
#include "dbrushadjuster.h"

KTDualColorButton::KTDualColorButton( QWidget *parent ) : QWidget( parent )
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

KTDualColorButton::KTDualColorButton(const QBrush &fgColor, const QBrush &bgColor, QWidget *parent) : QWidget(parent)
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

KTDualColorButton::~KTDualColorButton()
{
	delete arrowBitmap;
	delete resetPixmap;
	DEND;
}

QBrush KTDualColorButton::foreground() const
{
	return fg;
}

QBrush KTDualColorButton::background() const
{
	return bg;
}

KTDualColorButton::DualColor KTDualColorButton::current() const
{
	return curColor;
}

QBrush KTDualColorButton::currentColor() const
{
	return (curColor == Background ? bg : fg);
}

QSize KTDualColorButton::sizeHint() const
{
	return QSize(34, 34);
}

void KTDualColorButton::setForeground(const QBrush &c)
{
	fg = c;
	update();

	emit fgChanged(fg.color());
}

void KTDualColorButton::setBackground(const QBrush &c)
{
	bg = c;
	update();

	emit bgChanged(bg.color());
}

void KTDualColorButton::setCurrentColor(const QBrush &c)
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

void KTDualColorButton::setCurrent(DualColor s)
{
	curColor = s;
	update();
}

void KTDualColorButton::metrics(QRect &fgRect, QRect &bgRect)
{
	fgRect = QRect(0, 0, width()-14, height()-14);
	bgRect = QRect(14, 14, width()-14, height()-14);
}

void KTDualColorButton::paintEvent(QPaintEvent *)
{
	QRect fgRect, bgRect;
	QPainter p(this);

	metrics(fgRect, bgRect);
	QBrush defBrush = palette().color(QPalette::Button);

	
	qDrawShadeRect(&p, bgRect, palette(), curColor == Background, 2, 0,
			isEnabled() ? &DBrushAdjuster::adjustBrush(bg, bgRect) : &defBrush);
	qDrawShadeRect(&p, fgRect,  palette(), curColor == Foreground, 2, 0,
			isEnabled() ? &DBrushAdjuster::adjustBrush(fg, fgRect) : &defBrush);
	p.setPen(QPen(palette().shadow().color()));
	
	p.drawPixmap(fgRect.right()+2, 0, *arrowBitmap);
	p.drawPixmap(0, fgRect.bottom()+2, *resetPixmap);

}

void KTDualColorButton::dragEnterEvent(QDragEnterEvent *ev)
{
// 	ev->accept(isEnabled() && KColorDrag::canDecode(ev));
}

void KTDualColorButton::dropEvent(QDropEvent *ev)
{
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

void KTDualColorButton::mousePressEvent(QMouseEvent *ev)
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


void KTDualColorButton::mouseMoveEvent(QMouseEvent *ev)
{
// 	if(!miniCtlFlag)
// 	{
// 		int delay = KGlobalSettings::dndEventDelay();
// 		if(ev->x() >= mPos.x()+delay || ev->x() <= mPos.x()-delay ||
// 				 ev->y() >= mPos.y()+delay || ev->y() <= mPos.y()-delay)
// 		{
// 			KColorDrag *d = new KColorDrag( curColor == Foreground ?
// 					fg.color() : bg.color(),
// 			this);
// 			d->dragCopy();
// 			dragFlag = true;
// 		}
// 	}
}

void KTDualColorButton::mouseReleaseEvent(QMouseEvent *ev)
{
// 	if(!miniCtlFlag)
// 	{
// 		QRect fgRect, bgRect;
// 
// 		metrics(fgRect, bgRect);
// 		if(dragFlag)
// 			curColor = tmpColor;
// 		else if(fgRect.contains(ev->pos()) && curColor == Foreground)
// 		{
// 			if(tmpColor == Background)
// 			{
// 				curColor = Foreground;
// 				emit currentChanged(Foreground);
// 			}
// // 			else
// // 			{
// // 				QColor newColor = fg.color();
// // 				if(KColorDialog::getColor(newColor, d->dialogParent) != QDialog::Rejected)
// // 				{
// // 					fg.setColor(newColor);
// // 					emit fgChanged(newColor);
// // 				}
// // 			}
// 		}
// 		else if(bgRect.contains(ev->pos()) && curColor == Background)
// 		{
// 			if(tmpColor == Foreground)
// 			{
// 				curColor = Background;
// 				emit currentChanged(Background);
// 			}
// // 			else
// // 			{
// // 				QColor newColor = bg.color();
// // 				if(KColorDialog::getColor(newColor, d->dialogParent) != QDialog::Rejected)
// // 				{
// // 					bg.setColor(newColor);
// // 					emit bgChanged(newColor);
// // 				}
// // 			}
// 		}
// 		update();
// 		dragFlag = false;
// 	}
// 	else
// 		miniCtlFlag = false;
}

