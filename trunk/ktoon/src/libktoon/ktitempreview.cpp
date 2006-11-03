/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#include "ktitempreview.h"

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include <ddebug.h>

KTItemPreview::KTItemPreview(QWidget *parent) : QWidget(parent), m_item(0)
{
}


KTItemPreview::~KTItemPreview()
{
}

QSize KTItemPreview::sizeHint() const
{
	if ( m_item )
	{
		return m_item->boundingRect().size().toSize() + QSize(10,10);
	}
	
	return QWidget::sizeHint().expandedTo(QSize(100,100));
}


void KTItemPreview::render(QGraphicsItem *item)
{
	Q_CHECK_PTR(item);
	
	m_item = item;
	update();
}

void KTItemPreview::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing, true);
	
	if ( m_item )
	{
		QStyleOptionGraphicsItem opt;
		opt.state = QStyle::State_None;
		
		if (m_item->isEnabled())
			opt.state |= QStyle::State_Enabled;
		
		opt.exposedRect = QRectF(QPointF(0,0), m_item->boundingRect().size());
		opt.levelOfDetail = 1;
		
		QMatrix matrix = m_item->sceneMatrix();
		
		QRect r(15,15, rect().width()-15 , rect().height()-15);
		p.drawRect(r);
		
		QRectF br = m_item->boundingRect();
		double offset = qMin(br.width(), br.height());
		
		matrix.translate((-m_item->pos().x()-br.center().x())+r.center().x(), (-m_item->pos().y()-br.center().y())+r.center().y());
		matrix.scale(r.width()/offset, r.height()/offset);
		
		opt.matrix = matrix;
		
		// TODO: escalar y centrar
		
		
		opt.palette = palette();
		
		p.setMatrix(matrix);
		m_item->paint ( &p, &opt, this ); // paint isn't const...
	}
}




