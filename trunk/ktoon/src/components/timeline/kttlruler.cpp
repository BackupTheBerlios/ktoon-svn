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
#include <kcore/kdebug.h>

#include <QPainter>
#include <QMouseEvent>

KTTLRuler::KTTLRuler(QWidget *parent) : QHeaderView(Qt::Horizontal, parent)
{
	DINIT;
	
	setHighlightSections ( true );
// 	setSelectionMode ( QAbstractItemView::SingleSelection );
// 	setSelectionBehavior ( QAbstractItemView::SelectColumns );
	
	setClickable (true);
	
	connect(this, SIGNAL(sectionClicked(int)), this, SLOT(updateSelected(int)));
}

KTTLRuler::~KTTLRuler()
{
	DEND;
}

void KTTLRuler::paintSection ( QPainter * painter, const QRect & rect, int logicalIndex ) const
{
	if ( !model() || !rect.isValid() )
	{
		return;
	}
	
	painter->save();
	
	if ( selectionModel()->isSelected( model()->index(0, logicalIndex ) ) )
	{
		QBrush brush(Qt::red);
		brush.setStyle(Qt::Dense5Pattern );
		painter->fillRect(rect, brush);
	}
	
	if ( logicalIndex % 5 == 0 )
	{
		QFontMetricsF fm(painter->font());
		
		QString number = QString::number(logicalIndex+1);
		
		painter->drawText( (int)(rect.center().x()-(fm.width(number)/2)), (int)(rect.center().y() +(fm.height()/2)) ,number);
	}
	
	painter->drawLine(rect.bottomLeft(), rect.bottomLeft()- QPointF(0, 4));
	painter->drawLine(rect.topLeft(), rect.topLeft()+ QPointF(0, 4));
	
	QPen pen = painter->pen();
	pen.setWidth(4);
	painter->setPen(pen); 
	
	painter->drawLine(rect.bottomLeft(), rect.bottomRight());
	
	painter->restore();
}

void KTTLRuler::updateSelected(int logical)
{
	select(logical);
	
	emit logicalSectionSelected(logical);
}


void KTTLRuler::select(int logical)
{
	selectionModel()->select( model()->index(0, logical), QItemSelectionModel::ClearAndSelect);
	
	viewport()->update(QRect(sectionViewportPosition(logical), 0, sectionSize(logical),viewport()->height()) );
}


void KTTLRuler::mouseMoveEvent(QMouseEvent *e)
{
	if ( e->buttons() & Qt::LeftButton )
	{
		updateSelected(logicalIndexAt(e->pos()));
	}
	
	QHeaderView::mouseMoveEvent(e);
}


