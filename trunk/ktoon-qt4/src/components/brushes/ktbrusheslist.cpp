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

#include "ktbrusheslist.h"
#include <QPainter>
#include <QStringList>
#include "ktdebug.h"

KTBrushesList::KTBrushesList(QWidget *parent)
	: KTCellView(parent), MAX_COLUMNS(10), m_row(0), m_col(0)
{
}


KTBrushesList::~KTBrushesList()
{
}

void KTBrushesList::addBrush(const QPainterPath &form)
{
	KT_FUNCINFO;

	KTCellViewItem *newBrush = new KTCellViewItem();

	QImage tbrush(form.boundingRect().width()+2, form.boundingRect().height()+2, QImage::Format_RGB32);
	
	tbrush.fill(qRgba(255,255,255,0));
	QPainter p(&tbrush);
	p.setRenderHint(QPainter::Antialiasing);
	
	p.setPen(QPen(Qt::black,3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	
// 	p.setBrush( QBrush( foregroundColor (), Qt::SolidPattern));
	
	
	p.drawPath(form);
	
	newBrush->setImage( tbrush );
// 	newBrush->setBackground( QColor(34,34,234,60 ) );
	
	m_forms << form;
	
	/////
	
	if( columnCount() < MAX_COLUMNS)
	{
		insertColumn( columnCount()+1);
	}
	
	if( (m_forms.count()-1) % MAX_COLUMNS == 0)
	{
		insertRow( (rowCount()+1));
		m_row++;
		m_col = 0;
	}
	else
	{
		m_col++;
	}
	
	setItem( m_row-1 , m_col , newBrush);
}

QPainterPath KTBrushesList::path(int index)
{
	return m_forms[index];
}

int KTBrushesList::count() const
{
	return m_forms.count();
}
