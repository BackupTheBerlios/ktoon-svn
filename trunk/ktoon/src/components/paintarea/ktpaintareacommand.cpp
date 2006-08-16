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


KTPaintAreaCommand::KTPaintAreaCommand(KTPaintArea *area, const KTPaintAreaEvent *event) : QUndoCommand(), m_paintArea(area)
{
	m_event = event->clone();
}


KTPaintAreaCommand::~KTPaintAreaCommand()
{
}

void KTPaintAreaCommand::undo()
{
	switch(m_event->action() )
	{
		case KTPaintAreaEvent::ChangePen:
		{
			m_paintArea->brushManager()->setPen( qvariant_cast<QPen>(m_oldData));
		}
		break;
		case KTPaintAreaEvent::ChangePenBrush:
		{
			m_paintArea->brushManager()->setPenBrush( qvariant_cast<QBrush>(m_oldData));
		}
		break;
		case KTPaintAreaEvent::ChangeBrush:
		{
			m_paintArea->brushManager()->setBrush( qvariant_cast<QBrush>(m_oldData));
		}
		break;
		
		default: break;
	}
}

void KTPaintAreaCommand::redo()
{
	switch(m_event->action() )
	{
		case KTPaintAreaEvent::ChangePen:
		{
			m_oldData = m_paintArea->brushManager()->pen();
			
			m_paintArea->brushManager()->setPen( qvariant_cast<QPen>(m_event->data()));
		}
		break;
		case KTPaintAreaEvent::ChangePenBrush:
		{
			m_oldData = m_paintArea->brushManager()->pen().brush();
			
			
			m_paintArea->brushManager()->setPenBrush( qvariant_cast<QBrush>(m_event->data()));
			
		}
		break;
		case KTPaintAreaEvent::ChangeBrush:
		{
			m_oldData = m_paintArea->brushManager()->brush();
			
			
			m_paintArea->brushManager()->setBrush( qvariant_cast<QBrush>(m_event->data()));
		}
		break;
		
		default: break;
	}
}

