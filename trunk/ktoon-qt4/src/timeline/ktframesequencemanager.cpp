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
//Added by qt3to4:
#include <Q3Frame>

KTFrameSequenceManager::KTFrameSequenceManager(QWidget *parent) : KTVHBox(parent, "KTFrameSequenceManager"), m_currentFrame(0)
{
	KTINIT;
	
	
	setMinimumHeight(80);
	
// 	KTVHBox *spacer = new KTVHBox(this);// FIXME
// 	spacer->setMinimumHeight(25); 
	
	m_sequenceLayout = new QScrollArea(this);
	m_sequenceLayout->setWidgetResizable(true);
// 	addWidget(m_sequenceLayout);
	
// 	m_sequenceLayout->enableClipper( true );

// 	m_sequenceLayout->horizontalScrollBar()->setLineStep(1);
// 	m_sequenceLayout->horizontalScrollBar()->setPageStep (5);
	
	m_vBox = new KTVHBox(m_sequenceLayout);
	m_sequenceLayout->setWidget(m_vBox);
	
	m_ruler = new KTTLRuler(m_vBox);
// 	m_ruler->layout()->setMargin(1);
	m_ruler->setMinimumSize(100, m_ruler->height());
	
// 	setMaximumHeight( sizeHint().height() );
	
	insertFrameSequence();
	
	m_currentFrameSequence = m_lastSequence;
	
	m_currentFrame = m_lastSequence->frameAt(0);
	
// 	m_utils = new QHBox(this);
// 	m_scroll = new QScrollBar(0, 3000, 1, 5, 0, Qt::Horizontal, m_utils);
// 	connect( m_scroll, SIGNAL(valueChanged(int)), this, SLOT(moveRuler(int)));
	
	KTVHBox *spacer2 = new KTVHBox(this);// FIXME
	spacer2->setMinimumHeight(25); 
	
}


KTFrameSequenceManager::~KTFrameSequenceManager()
{
	KTEND;
}

void KTFrameSequenceManager::insertFrameSequence()
{
	KTFrameSequence *newFrameSequence = new KTFrameSequence(m_sequences.count(),100, m_vBox );
	
	connect(newFrameSequence, SIGNAL(frameSelected(TLFrame *)), this, SLOT(setCurrentFrame(TLFrame *)));

// 	connect( newFrameSequence, SIGNAL( frameInserted() ), SLOT( slotUpdateMaxUsedFrames() ) );
// 	connect( newFrameSequence, SIGNAL( frameRemoved() ), SLOT( slotUpdateMaxUsedFrames() ) );
// 	connect( newFrameSequence, SIGNAL( keyframeRemoved( int ) ), SIGNAL( keyframeRemovedToES( int ) ) );
// 	connect( newFrameSequence, SIGNAL( motionTweenCreated( int ) ), SIGNAL( motionTweenCreatedToES( int ) ) );
// 	connect( newFrameSequence, SIGNAL( motionTweenRemoved( int ) ), SIGNAL( motionTweenRemovedToES( int ) ) );
	
	newFrameSequence -> show();
	m_sequences.append( newFrameSequence );
	m_lastSequence = newFrameSequence;
	
	adjustSize();
}

void KTFrameSequenceManager::removeFrameSequence()
{
	if ( m_sequences.count() > 1 )
	{
		KTFrameSequence *bridgeFrameSequence;

      		//Case 1: When the sequence of frames is the last within the list
		if ( m_currentFrameSequence == m_lastSequence )
		{
			QListIterator<KTFrameSequence*> iterator(m_sequences);
			
			
			m_sequences.removeAt(m_currentFrameSequence->position());
			
			bridgeFrameSequence = m_sequences.last();
			delete m_currentFrameSequence;
			m_currentFrameSequence = bridgeFrameSequence;
			m_lastSequence = m_currentFrameSequence;
// 			m_currentFrame= 0;
		}
      		//Case 2: When the sequence of frames is any except the last
		else
		{
			bridgeFrameSequence = m_sequences.at( m_currentFrameSequence->position() );

	  		//Reaccomodate every frame_sequence next to the frame_sequence that is going to be deleted
			
			
			QListIterator<KTFrameSequence*> iterator(m_sequences);
			
			while ( iterator.hasNext() )
			{
				KTFrameSequence *next = iterator.next();
				next->setPosition( next->position() - 1 );
			}

			m_vBox->layout()->remove(m_currentFrameSequence);
			m_sequences.removeAt( m_currentFrameSequence->position()+1 );
			delete m_currentFrameSequence;
			m_currentFrameSequence = bridgeFrameSequence;
		}
	}
}

void KTFrameSequenceManager::setCurrentFrame(TLFrame *frame)
{
	if (!frame|| m_currentFrame == frame)
	{
		ktError() << "Invalid Frame" << endl;
		return;
	}
	
	m_currentFrame->setSelected(false);
	m_currentFrame = frame;
	m_currentFrame->setSelected(true);
}


QScrollBar *KTFrameSequenceManager::verticalScrollBar()
{
	return m_sequenceLayout->verticalScrollBar();
}

QSize KTFrameSequenceManager::sizeHint() const
{
	const QSize sh(499, KTVHBox::sizeHint().height() );
	
	return sh;
}
