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

#include "ktframesequence.h"
#include "ktdebug.h"

#include <qlayout.h>
#include <qlabel.h>

KTFrameSequence::KTFrameSequence( int ID, int count, QWidget *parent) : Q3HBox(parent, "KTFrameSequence"), m_ID(ID)
{
	createFrames(count);
}


KTFrameSequence::~KTFrameSequence()
{
}


void KTFrameSequence::createFrames(int count)
{
	for(uint i = 0; i < count; i++)
	{
		TLFrame *nextFrame = new TLFrame( i, this );

		if ( i % 5 == 0 )
		{
			nextFrame->setSpecial( true );
		}
		if ( i == 0 )
		{
			nextFrame->setKey( true );
			nextFrame->setLast( true );
		}
		
		m_frames.append(nextFrame);
		
		connect(nextFrame, SIGNAL(selected(int)), this, SLOT(selectFrame(int))); // FIXME
	}
}

void KTFrameSequence::selectFrame(int position)
{
	TLFrame *selected = m_frames.at(position);
	
	if ( !selected )
	{
		ktError() << "Invalid TLFrame" << endl;
		return;
	}
	selected->setSelected( true );

// 	TLFrameSequenceManager *fsm = ( TLFrameSequenceManager * )( parent_widget );
// 	fsm -> getRuler() -> slotSetOffset( current_frame_sequence -> framePosition( selected_frame ) );
	
	emit frameSelected(selected);
}

void KTFrameSequence::setPosition(int pos)
{
	m_ID = pos;
}

int KTFrameSequence::position()
{
	return m_ID;
}

TLFrame *KTFrameSequence::frameAt(int pos)
{
	return m_frames.at(pos);
}


