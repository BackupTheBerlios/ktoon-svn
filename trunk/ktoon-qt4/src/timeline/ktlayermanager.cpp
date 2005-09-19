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

#include "ktlayermanager.h"

#include "ktapplication.h"
#include <qlabel.h>
#include <qlayout.h>
//Added by qt3to4:
#include <QPixmap>
#include <Q3Frame>

#include "kseparator.h"

#include "ktdebug.h"

KTLayerManager::KTLayerManager(QWidget *parent) : Q3VBox(parent, "KTLayerManager"), m_currentTime(0.0), m_totalTime(0.04), m_allSelected(false), m_allVisible(true), m_allLock(false)
{
	KTINIT;
	
	layout()->setAlignment(Qt::AlignTop);
	
	m_utilsInTop = new Q3HButtonGroup( this );
	connect(m_utilsInTop, SIGNAL(clicked(int)), this, SLOT(changeLayersState(int)));
	m_utilsInTop->setInsideSpacing(0); 
	m_utilsInTop->setInsideMargin(1);
	m_utilsInTop->layout()->setAlignment(Qt::AlignRight | Qt::AlignCenter );
	
// 	m_utilsInTop -> setFrameStyle( QFrame::Box | QFrame::Plain );
	
	m_eyeButton = new KTImageButton( QPixmap(KTOON_HOME+"/images/icons/show_hide_all_layers.xpm"), 20,  m_utilsInTop );
	m_utilsInTop->insert(m_eyeButton, ToggleLayerView);

	QToolTip::add( m_eyeButton, tr( "Show / Hide all Layers" ) );
	
	m_lockButton = new KTImageButton( QPixmap(KTOON_HOME+"/images/icons/kilit_pic.xpm"),  20, m_utilsInTop );
	m_utilsInTop->insert(m_lockButton, LockLayers);
	
	QToolTip::add( m_lockButton, tr( "Lock all Layers" ) );

	m_outlineButton = new KTImageButton( QPixmap(KTOON_HOME+"/images/icons/outline_pic.xpm"), 20, m_utilsInTop );
	m_utilsInTop->insert(m_outlineButton, ShowOutlines);

	QToolTip::add( m_outlineButton, tr( "Show only outlines" ) );
	
	m_utilsInTop->setMaximumHeight(m_eyeButton->height()+2);
	
	QWidget *spacer = new QWidget(m_utilsInTop);
	spacer->setMinimumWidth(20);
	
	
	//------------------------------------------------------
	
	m_sequence = new KTLayerSequence(this);
	
	//------------------------------------------------------
	
	m_utilsInBottom = new Q3HButtonGroup( this );
	connect(m_utilsInBottom, SIGNAL(clicked(int)), this, SLOT(selectLayerAction(int)));
	
	m_utilsInBottom->setInsideSpacing(0); 
	m_utilsInBottom->setInsideMargin(1);
	
// 	m_utilsInBottom -> setFrameStyle( Q3Frame::Box | Q3Frame::Plain );
	m_utilsInBottom -> resize( 192, 24 );
	m_utilsInBottom -> setMinimumSize( 192, 24 );
	m_utilsInBottom->layout()->setAlignment(Qt::AlignLeft | Qt::AlignCenter);

	m_insertButton = new KTImageButton( QPixmap(KTOON_HOME+"/images/icons/layer+.xpm") , 20,  m_utilsInBottom );
	m_utilsInBottom->insert(m_insertButton, InsertLayer);

	QToolTip::add( m_insertButton, tr( "Insert Layer" ) );

	m_removeButton = new KTImageButton( QPixmap(KTOON_HOME+"/images/icons/layer-.xpm"),  20, m_utilsInBottom );
	m_utilsInBottom->insert(m_removeButton, RemoveLayer);
	
	QToolTip::add( m_removeButton, tr( "Remove Layer" ) );

	m_moveUpButton = new KTImageButton( QPixmap(KTOON_HOME+"/images/icons/arrowup.xpm"),  20, m_utilsInBottom );
	m_utilsInBottom->insert(m_moveUpButton, MoveLayerUp);

	QToolTip::add( m_moveUpButton, tr( "Move Layer Up" ) );

	m_moveDownButton = new KTImageButton( QPixmap(KTOON_HOME+"/images/icons/arrowdown.xpm"), 20,  m_utilsInBottom );
	m_utilsInBottom->insert(m_moveDownButton, MoveLayerDown);

	QToolTip::add( m_moveDownButton, tr( "Move Layer Down" ) );
	
	new KSeparator( Qt::Vertical, m_utilsInBottom);
	
	m_time = new QLabel( QString::number( m_currentTime, 'f', 2 ) + " / " + QString::number( m_totalTime, 'f', 2 ), m_utilsInBottom );
	
	m_time -> setFont( QFont( font().family(), 7 ) );
	m_time -> setAlignment( Qt::AlignCenter );
	m_time -> resize( 80, 20 );

	new KSeparator( Qt::Vertical, m_utilsInBottom);
	
	m_utilsInBottom->setMaximumHeight(m_moveUpButton->height());
	
	show();
}


KTLayerManager::~KTLayerManager()
{
	KTEND;
}

QScrollBar *KTLayerManager::verticalScrollBar()
{
	return m_sequence->verticalScrollBar();
}

void KTLayerManager::changeLayersState(int opt)
{
	KTTimeLineLayer *iterator;
	ListOfTLLayers layerList = m_sequence->layers();

	for ( iterator = layerList.first(); iterator; iterator = layerList.next() )
	{
		if ( iterator )
		{
			switch(opt)
			{
				case ShowOutlines:
				{
					iterator->setOnlyOutlines(!m_allSelected);
				}
				break;
				case LockLayers:
				{
					iterator->setLock(m_allLock);
				}
				break;
				case ToggleLayerView:
				{
					iterator->setView(m_allVisible);
				}
				break;
				default:
				{
					ktDebug() << "Invalid action" << endl;
				}
				break;
			}
		}
		else
		{
			ktError() << "Invalid layer" << endl;
		}
	}
	
	// Toggle states
	switch(opt)
	{
		case ShowOutlines:
		{
			m_allSelected = !m_allSelected;
		}
		break;
		case LockLayers:
		{
			m_allLock = !m_allLock;
		}
		break;
		case ToggleLayerView:
		{
			m_allVisible = !m_allVisible;
		}
		break;
	}
}

void KTLayerManager::selectLayerAction(int opt)
{
	switch(opt)
	{
		case InsertLayer:
		{
			m_sequence->createNewLayer();
		}
		break;
		case RemoveLayer:
		{
			m_sequence->removeLayer();
		}
		break;
		case MoveLayerUp:
		{
			m_sequence->moveLayerUp();
		}
		break;
		case MoveLayerDown:
		{
			m_sequence->moveLayerDown();
		}
		break;
		case NoAction:
		{
			
		}
		break;
	}
	
	emit actionSelected(opt);
}



