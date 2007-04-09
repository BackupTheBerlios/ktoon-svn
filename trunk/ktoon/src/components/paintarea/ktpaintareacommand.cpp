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

#include "ktpaintareacommand.h"
#include "ktpaintarea.h"
#include "ktpaintareaevent.h"

#include "ktbrushmanager.h"

#include <QVariant>

struct KTPaintAreaCommand::Private
{
	KTPaintArea *paintArea;
	KTPaintAreaEvent *event;
	QVariant oldData;
};

KTPaintAreaCommand::KTPaintAreaCommand(KTPaintArea *area, const KTPaintAreaEvent *event) : QUndoCommand(), d(new Private)
{
	d->paintArea = area;
	d->event = event->clone();
}


KTPaintAreaCommand::~KTPaintAreaCommand()
{
	delete d;
}

void KTPaintAreaCommand::undo()
{
	switch(d->event->action() )
	{
		case KTPaintAreaEvent::ChangePen:
		{
			d->paintArea->brushManager()->setPen( qvariant_cast<QPen>(d->oldData));
		}
		break;
		case KTPaintAreaEvent::ChangePenBrush:
		{
			d->paintArea->brushManager()->setPenBrush( qvariant_cast<QBrush>(d->oldData));
		}
		break;
		case KTPaintAreaEvent::ChangeBrush:
		{
			d->paintArea->brushManager()->setBrush( qvariant_cast<QBrush>(d->oldData));
		}
		break;
		
		default: break;
	}
}

void KTPaintAreaCommand::redo()
{
	switch(d->event->action() )
	{
		case KTPaintAreaEvent::ChangePen:
		{
			d->oldData = d->paintArea->brushManager()->pen();
			
			QPen pen = qvariant_cast<QPen>(d->event->data());
			if ( !pen.color().isValid() )
			{
				QPen old = d->paintArea->brushManager()->pen();
				pen.setColor( old.color() );
				pen.setBrush( old.brush() );
			}
			
			d->paintArea->brushManager()->setPen( pen );
		}
		break;
		case KTPaintAreaEvent::ChangePenBrush:
		{
			d->oldData = d->paintArea->brushManager()->pen().brush();
			
			
			d->paintArea->brushManager()->setPenBrush( qvariant_cast<QBrush>(d->event->data()));
			
		}
		break;
		case KTPaintAreaEvent::ChangeBrush:
		{
			d->oldData = d->paintArea->brushManager()->brush();
			
			
			d->paintArea->brushManager()->setBrush( qvariant_cast<QBrush>(d->event->data()));
		}
		break;
		
		default: break;
	}
}

