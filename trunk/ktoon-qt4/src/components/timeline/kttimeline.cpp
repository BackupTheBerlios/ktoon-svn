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

#include <QList>

#include "kttimeline.h"
#include "ktapplication.h"
#include "ktdebug.h"

KTTimeLine::KTTimeLine(QWidget *parent) : KTModuleWidgetBase(parent, "KTTimeLine")
{
	KTINIT;

	setCaption(tr("The Time line"));
	
	m_container = new KTVHBox(this, false);
	addChild(m_container);
	
	m_splitter = new QSplitter( m_container );
	m_splitter->setMinimumHeight(130);
// 	m_container->addWidget(m_splitter);
	
	m_layerManager = new KTLayerManager( m_splitter );
	m_splitter->addWidget(m_layerManager);
	m_layerManager->resize( 590, m_layerManager->height() );
	
	m_sequenceManager = new KTFrameSequenceManager(m_splitter);
	m_splitter->addWidget(m_sequenceManager);
	
	connect(m_layerManager, SIGNAL(actionSelected(int)), this, SLOT(execAction(int)));
	
	// Mover scrolls simetricamente
	connect( m_sequenceManager->verticalScrollBar(), SIGNAL( valueChanged( int ) ), m_layerManager->verticalScrollBar(), SLOT( setValue( int ) ) );
	connect( m_layerManager->verticalScrollBar(), SIGNAL( valueChanged( int ) ), m_sequenceManager->verticalScrollBar(), SLOT( setValue( int ) ) );
	
	show();
// 	hide();
	
// 	setResizeEnabled(true);

	m_container->setMinimumHeight( m_container->sizeHint().height() );

	m_splitter->setSizes( QList<int>() << 190 << 590 );
}


KTTimeLine::~KTTimeLine()
{
	KTEND;
}

void KTTimeLine::execAction(int action)
{
	switch(action)
	{
		case KTLayerManager::InsertLayer:
		{
			m_sequenceManager->insertFrameSequence();
			emit layerInserted();
		}
		break;
		case KTLayerManager::RemoveLayer:
		{
			m_sequenceManager->removeFrameSequence();
			emit layerRemoved();
		}
		break;
		
	}
}



