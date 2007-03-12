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

#include "ktproxyitem.h"

#include <ddebug.h>

struct KTItemPreview::Private
{
	KTProxyItem *proxy;
};

KTItemPreview::KTItemPreview(QWidget *parent) : QWidget(parent), d(new Private)
{
	d->proxy = 0;
}


KTItemPreview::~KTItemPreview()
{
	delete d;
}

QSize KTItemPreview::sizeHint() const
{
	if ( d->proxy )
	{
		return d->proxy->boundingRect().size().toSize() + QSize(10,10);
	}
	
	return QWidget::sizeHint().expandedTo(QSize(100,100));
}


void KTItemPreview::render(QGraphicsItem *item)
{
	if ( !d->proxy )
		d->proxy = new KTProxyItem(item);
	else
		d->proxy->setItem(item);
	
	update();
}

void KTItemPreview::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing, true);
	
	if ( d->proxy )
	{
		QStyleOptionGraphicsItem opt;
		opt.state = QStyle::State_None;
		
		if (d->proxy->isEnabled())
			opt.state |= QStyle::State_Enabled;
		
		opt.exposedRect = QRectF(QPointF(0,0), d->proxy->boundingRect().size());
		opt.levelOfDetail = 1;
		
		QMatrix matrix = d->proxy->sceneMatrix();
		
		QRect r(15,15, rect().width()-15 , rect().height()-15);
// 		p.drawRect(r);
// 		QRectF br = d->proxy->boundingRect();
// 		double offset = qMin(br.width(), br.height());
		
// 		matrix.translate((-d->proxy->pos().x()-br.center().x())+r.center().x(), (-d->proxy->pos().y()-br.center().y())+r.center().y());
// 		scale(r.width()/offset, r.height()/offset);
		
// 		opt.matrix = matrix;
		
		opt.palette = palette();
		p.setMatrix(matrix);
		
		p.translate( (rect().width() - opt.exposedRect.width())/2, (rect().height() - opt.exposedRect.height())/2 );
		
		d->proxy->paint ( &p, &opt, this ); // paint isn't const...
	}
}




