/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#include "ktpaintareacommand.h"
#include "ktpaintarea.h"
#include "ktpaintareaevent.h"

#include "ktbrushmanager.h"

#include <QVariant>

/**
 * This class defines the undo/redo functions for the paint area
 * @author David Cuadrado <krawek@toonka.com>
*/

struct KTPaintAreaCommand::Private
{
    KTPaintArea *paintArea;
    KTPaintAreaEvent *event;
    QVariant oldData;
};

KTPaintAreaCommand::KTPaintAreaCommand(KTPaintArea *area, const KTPaintAreaEvent *event) : QUndoCommand(), k(new Private)
{
    k->paintArea = area;
    k->event = event->clone();
}

KTPaintAreaCommand::~KTPaintAreaCommand()
{
    delete k;
}

void KTPaintAreaCommand::undo()
{
    switch(k->event->action()) {
           case KTPaintAreaEvent::ChangePen:
                k->paintArea->brushManager()->setPen( qvariant_cast<QPen>(k->oldData));
                break;
           case KTPaintAreaEvent::ChangePenBrush:
                k->paintArea->brushManager()->setPenBrush( qvariant_cast<QBrush>(k->oldData));
                break;
           case KTPaintAreaEvent::ChangeBrush:
                k->paintArea->brushManager()->setBrush( qvariant_cast<QBrush>(k->oldData));
                break;
           default: 
                break;
    }
}

void KTPaintAreaCommand::redo()
{
    switch (k->event->action()) {
            case KTPaintAreaEvent::ChangePen:
                 {
                   k->oldData = k->paintArea->brushManager()->pen();
                   QPen pen = qvariant_cast<QPen>(k->event->data());
                   if (!pen.color().isValid()) {
                       QPen old = k->paintArea->brushManager()->pen();
                       pen.setColor( old.color() );
                       pen.setBrush( old.brush() );
                   }

                   k->paintArea->brushManager()->setPen( pen );
                 }
                 break;

            case KTPaintAreaEvent::ChangePenBrush:
                 {
                   k->oldData = k->paintArea->brushManager()->pen().brush();
                   k->paintArea->brushManager()->setPenBrush( qvariant_cast<QBrush>(k->event->data()));
                 }
                 break;

            case KTPaintAreaEvent::ChangeBrush:
                 {
                   k->oldData = k->paintArea->brushManager()->brush();
                   k->paintArea->brushManager()->setBrush( qvariant_cast<QBrush>(k->event->data()));
                 }
                 break;

            default: 
                 break;
    }
}
