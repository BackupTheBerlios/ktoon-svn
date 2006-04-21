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

#include "kttlruler.h"
#include "ddebug.h"

#include <QPainter>

KTTLRuler::KTTLRuler(QWidget *parent) : QHeaderView(Qt::Horizontal, parent)
{
	DINIT;
}


KTTLRuler::~KTTLRuler()
{
	DEND;
}

void KTTLRuler::paintSection ( QPainter * painter, const QRect & rect, int logicalIndex ) const
{
	painter->save();
	if ( logicalIndex % 5 == 0 )
	{
		QFontMetricsF fm(painter->font());
		
		QString number = QString::number(logicalIndex);
		
		painter->drawText(rect.center().x()-(fm.width(number)/2), rect.center().y() +(fm.height()/2) ,number);
	}
	
	painter->drawLine(rect.bottomLeft(), rect.bottomLeft()- QPointF(0, 4));
	painter->drawLine(rect.topLeft(), rect.topLeft()+ QPointF(0, 4));
	
	QPen pen = painter->pen();
	pen.setWidth(4);
	painter->setPen(pen); 
	
	painter->drawLine(rect.bottomLeft(), rect.bottomRight());
	
	painter->restore();
}


