/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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

#include "ktdocumentruler.h"

#include <QApplication>

struct KTDocumentRuler::Private
{
	QPointF oldPos;
	QPoint dragStartPosition;
};

KTDocumentRuler::KTDocumentRuler(Qt::Orientation orientation, QWidget *parent) : DRulerBase(orientation, parent), d( new Private)
{
	d->oldPos = QPointF(0.0,0.0);
	setDrawPointer(true);
}


KTDocumentRuler::~KTDocumentRuler()
{
	delete d;
}

void KTDocumentRuler::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
		d->dragStartPosition = event->pos();
}

void KTDocumentRuler::mouseMoveEvent(QMouseEvent *event)
{
	if (!(event->buttons() & Qt::LeftButton))
	{
		return;
	}
	
	if ((event->pos() - d->dragStartPosition).manhattanLength()	< QApplication::startDragDistance())
	{
		return;
	}
	
	QDrag *drag = new QDrag(this);
	QMimeData *mimeData = new QMimeData;
	
	QString data;
	if(orientation() == Qt::Vertical)
	{
		data = "verticalLine";
	}
	else
	{
		data = "horizontalLine";
	}
	
	mimeData->setData("ktoon-ruler", data.toAscii () );
	drag->setMimeData(mimeData);

	Qt::DropAction dropAction = drag->start(Qt::CopyAction | Qt::MoveAction);
	DRulerBase::mouseMoveEvent(event);
	
}

void KTDocumentRuler::movePointers(const QPointF &pos)
{
	// FIXME
	if(orientation() == Qt::Horizontal)
	{
		translateArrow(-d->oldPos.x(), 0);
		translateArrow(zero().x() + pos.x(), 0);
	}
	else if(orientation() == Qt::Vertical)
	{
		translateArrow(0, -d->oldPos.y());
		translateArrow(0,zero().y() + pos.y());
	}
	
	d->oldPos = zero() + pos;
	repaint();
}


