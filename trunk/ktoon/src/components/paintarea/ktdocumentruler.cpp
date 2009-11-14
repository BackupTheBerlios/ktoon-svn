/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by Jorge Cuadrado <kuadrosx@toonka.com>            *
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

#include <iostream>
using namespace std;

#include <QApplication>

struct KTDocumentRuler::Private
{
    QPointF oldPos;
    QPoint dragStartPosition;
};

KTDocumentRuler::KTDocumentRuler(Qt::Orientation orientation, QWidget *parent) : KRulerBase(orientation, parent), 
                                 k( new Private)
{
    k->oldPos = QPointF(0.0,0.0);
    setDrawPointer(true);
}


KTDocumentRuler::~KTDocumentRuler()
{
    delete k;
}

void KTDocumentRuler::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        k->dragStartPosition = event->pos();
}

void KTDocumentRuler::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;

    if ((event->pos() - k->dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    QString data;
    if (orientation() == Qt::Vertical)
        data = "verticalLine";
    else
        data = "horizontalLine";

    mimeData->setData("ktoon-ruler", data.toAscii());
    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->start(Qt::CopyAction | Qt::MoveAction);
    KRulerBase::mouseMoveEvent(event);
}

void KTDocumentRuler::movePointers(const QPointF &pos)
{
    // FIXME
    if (orientation() == Qt::Horizontal) {
        translateArrow(-k->oldPos.x(), 0);
        translateArrow(zero().x() + pos.x(), 0);
        cout << "Horizontal PosX: " << zero().x() + pos.x() << endl;
        cout << "Horizontal PosY: " << 0 << endl;
    } else if (orientation() == Qt::Vertical) {
               translateArrow(0, -k->oldPos.y());
               translateArrow(0, zero().y() + pos.y());
               cout << "Vertical PosX: " << 0 << endl;
               cout << "Vertical PosY: " << zero().y() + pos.y() << endl;
    }

    k->oldPos = zero() + pos;
    repaint();
}
