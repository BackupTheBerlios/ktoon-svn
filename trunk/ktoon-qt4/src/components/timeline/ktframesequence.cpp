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

KTFrameSequence::KTFrameSequence( int ID, int count, KTWidgetListView *parent) : KTWidgetListItem(parent), m_ID(ID), m_selectedFrameIndex(0)
{
	hide();
	m_layout = new QHBoxLayout(this);
	m_layout->setMargin(1);
	m_layout->setSpacing(0);
	
	createFrames(count);
	
	setLayout(m_layout);
}


KTFrameSequence::~KTFrameSequence()
{
}


void KTFrameSequence::createFrames(int count)
{
	for(uint i = 0; i < count; i++)
	{
		TLFrame *nextFrame = new TLFrame( i, this );
		nextFrame->setMinimumHeight(height());
		
		m_layout->addWidget(nextFrame, 0, Qt::AlignTop | Qt::AlignLeft);
		
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
	
	adjustSize();
}

void KTFrameSequence::selectFrame(int position)
{
	TLFrame *tlFSelected = m_frames.at(position);
	
	if ( !tlFSelected )
	{
		ktError() << "Invalid TLFrame" << endl;
		return;
	}
	
	if ( position != m_selectedFrameIndex )
	{
		tlFSelected->setSelected( true );
		
		emit frameSelected(tlFSelected);
		emit selected();
		
		m_selectedFrameIndex = position;
	}
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


