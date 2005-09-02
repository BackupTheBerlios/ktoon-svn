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

#include "ktframesequencemanager.h"
#include "ktdebug.h"

#include <qlabel.h>
#include <qlayout.h>

KTFrameSequenceManager::KTFrameSequenceManager(QWidget *parent) : QVBox(parent, "KTFrameSequenceManager")
{
	KTINIT;
	
	m_sequenceLayout = new QScrollView(this);
	m_sequenceLayout->enableClipper( true );
// 	m_sequenceLayout->setHScrollBarMode( QScrollView::AlwaysOff );

	m_sequenceLayout->horizontalScrollBar()->setLineStep(1);
	m_sequenceLayout->horizontalScrollBar()->setPageStep (5);
// 	connect( m_sequenceLayout->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(moveRuler(int)));
	
	m_vBox = new QVBox(m_sequenceLayout->viewport());
	m_sequenceLayout->addChild(m_vBox);
	
	m_ruler = new KTTLRuler(m_vBox);
	m_ruler->setMargin(1);
	m_ruler->setMinimumSize(300, m_ruler->height());
	m_ruler->setBackgroundColor(Qt::yellow);
	
	setMaximumHeight( sizeHint().height() );
	
	KTFrameSequence *m_frameSequence = new KTFrameSequence(m_vBox);
	m_frameSequence->show();
	
	m_sequences.append(m_frameSequence);
	m_currentFrameSequence = m_frameSequence;
	m_lastSequence = m_frameSequence;
	
// 	m_utils = new QHBox(this);
// 	m_scroll = new QScrollBar(0, 3000, 1, 5, 0, Qt::Horizontal, m_utils);
// 	connect( m_scroll, SIGNAL(valueChanged(int)), this, SLOT(moveRuler(int)));
	
}


KTFrameSequenceManager::~KTFrameSequenceManager()
{
	KTEND;
}

void KTFrameSequenceManager::insertFrameSequence()
{
	KTFrameSequence *newFrameSequence = new KTFrameSequence(m_vBox );

// 	connect( newFrameSequence, SIGNAL( frameInserted() ), SLOT( slotUpdateMaxUsedFrames() ) );
// 	connect( newFrameSequence, SIGNAL( frameRemoved() ), SLOT( slotUpdateMaxUsedFrames() ) );
// 	connect( newFrameSequence, SIGNAL( keyframeRemoved( int ) ), SIGNAL( keyframeRemovedToES( int ) ) );
// 	connect( newFrameSequence, SIGNAL( motionTweenCreated( int ) ), SIGNAL( motionTweenCreatedToES( int ) ) );
// 	connect( newFrameSequence, SIGNAL( motionTweenRemoved( int ) ), SIGNAL( motionTweenRemovedToES( int ) ) );
	
	newFrameSequence -> show();
	m_sequences.append( newFrameSequence );
	m_lastSequence = newFrameSequence;
}

void KTFrameSequenceManager::removeFrameSequence()
{
	if ( m_sequences.count() > 1 )
	{
		KTFrameSequence *bridgeFrameSequence;

      		//Case 1: When the sequence of frames is the last within the list
		if ( m_currentFrameSequence == m_lastSequence )
		{
			m_sequences.remove( m_currentFrameSequence );
			bridgeFrameSequence = m_sequences.getLast();
			delete m_currentFrameSequence;
			m_currentFrameSequence = bridgeFrameSequence;
			m_lastSequence = m_currentFrameSequence;
// 			m_currentFrame= 0;
		}
      		//Case 2: When the sequence of frames is any except the last
		else
		{
			bridgeFrameSequence = m_sequences.at( m_sequences.find( m_currentFrameSequence ) + 1 );

	  		//Reaccomodate every frame_sequence next to the frame_sequence that is going to be deleted
// 			TLFrameSequence *frame_sequence_iterator;
// 			for ( frame_sequence_iterator = bridgeFrameSequence; frame_sequence_iterator; frame_sequence_iterator = m_sequences.next() )
// 			{
// 				moveChild( frame_sequence_iterator, childX( frame_sequence_iterator ), childY( frame_sequence_iterator ) - frame_sequence_iterator -> height() );
// 				frame_sequence_iterator -> setPosition( frame_sequence_iterator -> position() - 1 );
// 			}
			
			m_vBox->layout()->remove(m_currentFrameSequence);
			m_sequences.remove( m_currentFrameSequence );
			delete m_currentFrameSequence;
			m_currentFrameSequence = bridgeFrameSequence;
// 			m_currentFrame = 0
		}
	}
}

void KTFrameSequenceManager::moveRuler(int pos)
{
	m_ruler->resize(m_sequenceLayout->width(), m_ruler->height());
	m_ruler->move( pos*-1, m_ruler->y() );
}

QScrollBar *KTFrameSequenceManager::verticalScrollBar()
{
	return m_sequenceLayout->verticalScrollBar();
}


