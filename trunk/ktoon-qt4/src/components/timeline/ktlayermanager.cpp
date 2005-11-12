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

#include <QPixmap>

#include "kseparator.h"

#include "ktdebug.h"

KTLayerManager::KTLayerManager(QWidget *parent) : KTVHBox(parent), m_currentTime(0.0), m_totalTime(0.04), m_allSelected(false), m_allVisible(true), m_allLock(false)
{
	KTINIT;
	
	m_layerState = new QButtonGroup(this);
	
	setMinimumHeight(80);
	layout()->setAlignment(Qt::AlignTop);
	layout()->setMargin(0);
	layout()->setSpacing(0);
	
	m_utilsInTop = new KTVHBox( this , false);
	m_utilsInTop->setMaximumHeight(20);
	m_utilsInTop->setMinimumHeight(20);
	
	connect(m_utilsInTop, SIGNAL(clicked(int)), this, SLOT(changeLayersState(int)));
	m_utilsInTop->setSpacing(0); 
	m_utilsInTop->layout()->setMargin(1);
	
	m_utilsInTop->layout()->setAlignment(Qt::AlignRight | Qt::AlignCenter );

	m_eyeButton = new KTImageButton( QPixmap(KTOON_THEME_DIR+"/icons/show_hide_all_layers.png"), 20,  m_utilsInTop );
	
	m_layerState->addButton(m_eyeButton);
	QToolTip::add( m_eyeButton, tr( "Show / Hide all Layers" ) );
	
	m_lockButton = new KTImageButton( QPixmap(KTOON_THEME_DIR+"/icons/kilit_pic.png"),  20, m_utilsInTop );
	m_layerState->addButton(m_lockButton);
	
	QToolTip::add( m_lockButton, tr( "Lock all Layers" ) );

	m_outlineButton = new KTImageButton( QPixmap(KTOON_THEME_DIR+"/icons/outline_pic.png"), 20, m_utilsInTop );
	m_layerState->addButton(m_outlineButton);

	QToolTip::add( m_outlineButton, tr( "Show only outlines" ) );
	
	m_utilsInTop->layout()->setSpacing(0);
	
	QWidget *spacer = new QWidget(m_utilsInTop);
	spacer->setMinimumWidth(10);
	
	
	//------------------------------------------------------
	
	m_sequence = new KTLayerSequence(this);
	
	//------------------------------------------------------
	
	m_layerGroup = new QButtonGroup(this);
	connect(m_layerGroup, SIGNAL(buttonClicked ( QAbstractButton *)), this, SLOT(selectLayerAction( QAbstractButton *)));
	
	m_utilsInBottom = new KTVHBox( this, false );
	m_utilsInBottom->setMaximumHeight(20);
	m_utilsInBottom->setMinimumHeight(20);
	
	connect(m_utilsInBottom, SIGNAL(clicked(int)), this, SLOT(selectLayerAction(int)));
	
	m_utilsInBottom->layout()->setSpacing(0); 
	m_utilsInBottom->layout()->setMargin(0);

	m_utilsInBottom->layout()->setAlignment(Qt::AlignLeft | Qt::AlignCenter);

	m_insertButton = new KTImageButton( QPixmap(KTOON_HOME+"/themes/default/icons/add_layer.png") , 20,  m_utilsInBottom );
	m_layerGroup->addButton(m_insertButton);

	QToolTip::add( m_insertButton, tr( "Insert Layer" ) );

	m_removeButton = new KTImageButton( QPixmap(KTOON_HOME+"/themes/default/icons/remove_layer.png"),  20, m_utilsInBottom );
	m_layerGroup->addButton(m_removeButton);
	
	QToolTip::add( m_removeButton, tr( "Remove Layer" ) );

	m_moveUpButton = new KTImageButton( QPixmap(KTOON_HOME+"/themes/default/icons/arrowup.png"),  20, m_utilsInBottom );
	m_layerGroup->addButton(m_moveUpButton);

	QToolTip::add( m_moveUpButton, tr( "Move Layer Up" ) );

	m_moveDownButton = new KTImageButton( QPixmap(KTOON_HOME+"/themes/default/icons/arrowdown.png"), 20,  m_utilsInBottom );
	m_layerGroup->addButton(m_moveDownButton);

	QToolTip::add( m_moveDownButton, tr( "Move Layer Down" ) );
	
	new KSeparator( Qt::Vertical, m_utilsInBottom);
	
	m_time = new QLabel( QString::number( m_currentTime, 'f', 2 ) + " / " + QString::number( m_totalTime, 'f', 2 ), m_utilsInBottom );
	
	m_time -> setFont( QFont( font().family(), 7 ) );
	m_time -> setAlignment( Qt::AlignCenter );
	m_time -> resize( 80, 20 );

	new KSeparator( Qt::Vertical, m_utilsInBottom);
	
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
// 	KTTimeLineLayer *iterator;
// 	ListOfTLLayers layerList = m_sequence->layers();
// 
// 	for ( iterator = layerList.first(); iterator; iterator = layerList.next() )
// 	{
// 		if ( iterator )
// 		{
// 			switch(opt)
// 			{
// 				case ShowOutlines:
// 				{
// 					iterator->setOnlyOutlines(!m_allSelected);
// 				}
// 				break;
// 				case LockLayers:
// 				{
// 					iterator->setLock(m_allLock);
// 				}
// 				break;
// 				case ToggleLayerView:
// 				{
// 					iterator->setView(m_allVisible);
// 				}
// 				break;
// 				default:
// 				{
// 					ktDebug() << "Invalid action" << endl;
// 				}
// 				break;
// 			}
// 		}
// 		else
// 		{
// 			ktError() << "Invalid layer" << endl;
// 		}
// 	}
// 	
// 	// Toggle states
// 	switch(opt)
// 	{
// 		case ShowOutlines:
// 		{
// 			m_allSelected = !m_allSelected;
// 		}
// 		break;
// 		case LockLayers:
// 		{
// 			m_allLock = !m_allLock;
// 		}
// 		break;
// 		case ToggleLayerView:
// 		{
// 			m_allVisible = !m_allVisible;
// 		}
// 		break;
// 	}
}

void KTLayerManager::selectLayerAction(QAbstractButton *but)
{
	if ( but )
	{
		if ( but == m_insertButton )
		{
			emit actionSelected(InsertLayer);
		}
		else if ( but == m_removeButton)
		{
			emit actionSelected(RemoveLayer);
		}
		else if ( but == m_removeButton)
		{
		}
		else if ( but == m_moveUpButton)
		{
		}
		else if ( but == m_moveDownButton)
		{
		}
		else if ( but == m_lockButton)
		{
		}
		else if ( but == m_eyeButton)
		{
		}
		else if ( but == m_outlineButton)
		{
		}
		
// 			case MoveLayerUp:
// 			{
// 	// 			m_sequence->moveLayerUp();
// 			}
// 			break;
// 			case MoveLayerDown:
// 			{
// 	// 			m_sequence->moveLayerDown();
// 			}
	}
}

KTLayerSequence *KTLayerManager::layerSequence()
{
	return m_sequence;
}

