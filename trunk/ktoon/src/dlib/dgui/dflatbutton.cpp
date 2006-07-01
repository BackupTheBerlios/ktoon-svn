/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#include "dflatbutton.h"
#include <QPainter>

DFlatButton::DFlatButton(QWidget *parent) : QAbstractButton(parent)
{
	setCheckable(true);
}

DFlatButton::DFlatButton(const QString &text, QWidget *parent ) : QAbstractButton(parent)
{
	setText(text);
	setCheckable(true);
}


DFlatButton::~DFlatButton()
{
}

void DFlatButton::paintEvent(QPaintEvent *e)
{
	QRect rect = contentsRect();
	
	QPainter painter(this);
	
	QPoint start = rect.topLeft();
	start.setY(rect.center().y());
	QLinearGradient gradient(start, rect.bottomLeft());
	gradient.setSpread(QGradient::ReflectSpread);
	gradient.setColorAt(0, palette().button().color());
	gradient.setColorAt(1, palette().background().color());
	
	painter.fillRect(rect, QBrush(gradient));
	
	painter.save();
	painter.setPen(QPen(palette().dark(), 3));
	painter.drawRect(rect.normalized().adjusted(0,0,-1,-1));
	
	if( isChecked () )
	{
		QColor fillColor = palette().highlight().color();
		fillColor.setAlpha(70);
		painter.setPen(QPen(palette().highlight(), 2) );
		painter.fillRect(rect.normalized().adjusted(2,2,-4,-4), fillColor);
		painter.drawRect(rect.normalized().adjusted(2,2,-2,-2));
	}
	
	painter.restore();
	
	painter.translate( rect.left(), rect.top() );
	
	QRect textRect = fontMetrics().boundingRect( text() );
	int textX = qMax( 0, ( rect.width() - textRect.width() ) / 2 );
	int textY = textRect.height();
	
	QPixmap pixmap = icon().pixmap(64,64);
	
	if( !pixmap.isNull() ) 
	{
		textX = qMax( textX, (pixmap.width() - textRect.width()) / 2 );
		textY += 5 + pixmap.height();

		int pixX = qMax( qMax( 0, (textRect.width() - pixmap.width()) / 2 ), ( rect.width() - pixmap.width() ) / 2 );
		painter.drawPixmap( pixX, textY-pixmap.height()-textRect.height(), pixmap );
	}
	
	painter.setFont(QFont("Times", 12, QFont::Bold) );
	painter.drawText( textX, textY, text() );
	
}


