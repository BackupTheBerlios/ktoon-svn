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

#include <QLabel>
#include <QLayout>

#include <QPushButton>

KTFrameSequenceManager::KTFrameSequenceManager(QWidget *parent) : KTWidgetListView(parent), m_currentFrame(0)
{
	KTINIT;
}


KTFrameSequenceManager::~KTFrameSequenceManager()
{
	KTEND;
}

void KTFrameSequenceManager::insertFrameSequence()
{
	KTFrameSequence *newFrameSequence = new KTFrameSequence(m_sequences.count(),100, this );
// 	newFrameSequence->setMinimumHeight(24);
	
	connect(newFrameSequence, SIGNAL(frameSelected(TLFrame *)), this, SLOT(setCurrentFrame(TLFrame *)));

// 	connect( newFrameSequence, SIGNAL( frameInserted() ), SLOT( slotUpdateMaxUsedFrames() ) );
// 	connect( newFrameSequence, SIGNAL( frameRemoved() ), SLOT( slotUpdateMaxUsedFrames() ) );
// 	connect( newFrameSequence, SIGNAL( keyframeRemoved( int ) ), SIGNAL( keyframeRemovedToES( int ) ) );
// 	connect( newFrameSequence, SIGNAL( motionTweenCreated( int ) ), SIGNAL( motionTweenCreatedToES( int ) ) );
// 	connect( newFrameSequence, SIGNAL( motionTweenRemoved( int ) ), SIGNAL( motionTweenRemovedToES( int ) ) );
	
	newFrameSequence->show();
	m_sequences.append( newFrameSequence );
	m_lastSequence = newFrameSequence;
	
	if ( m_currentFrameSequence == 0)
	{
		m_currentFrameSequence = m_lastSequence;
	}
}

void KTFrameSequenceManager::removeFrameSequence()
{
	ktDebug() << "Removing frame sequence";
	removeItem( currentItem() );
}

void KTFrameSequenceManager::setCurrentFrame(TLFrame *frame)
{
	if (!frame|| m_currentFrame == frame)
	{
		ktError() << "Invalid Frame" << endl;
		return;
	}
	
	if ( m_currentFrame )
	{
		m_currentFrame->setSelected(false);
	}
	
	m_currentFrame = frame;
	m_currentFrame->setSelected(true);
}

void KTFrameSequenceManager::selectFrame(int index)
{
	if ( m_currentFrameSequence )
	{
		m_currentFrameSequence->selectFrame(index);
	}
}
