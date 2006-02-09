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
	
	m_buttonGroup = new QButtonGroup(this);
	
	connect(m_buttonGroup, SIGNAL(buttonClicked (int)), this, SIGNAL(actionSelected(int)));
	
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
	
	m_buttonGroup->addButton(m_eyeButton, ToggleLayerView);
	m_eyeButton->setToolTip(tr( "Show / Hide all Layers" ) );
	
	m_lockButton = new KTImageButton( QPixmap(KTOON_THEME_DIR+"/icons/kilit_pic.png"),  20, m_utilsInTop );
	m_buttonGroup->addButton(m_lockButton, LockLayers);
	
	m_lockButton->setToolTip(tr( "Lock all Layers" ) );

	m_outlineButton = new KTImageButton( QPixmap(KTOON_THEME_DIR+"/icons/outline_pic.png"), 20, m_utilsInTop );
	m_buttonGroup->addButton(m_outlineButton,ShowOutlines);

	m_outlineButton->setToolTip(tr( "Show only outlines" ) );
	
	m_utilsInTop->layout()->setSpacing(0);
	
	QWidget *spacer = new QWidget(m_utilsInTop);
	spacer->setMinimumWidth(10);
	
	
	//------------------------------------------------------
	
	m_sequence = new KTLayerSequence(this);
	
	//------------------------------------------------------

	m_utilsInBottom = new KTVHBox( this, false );
	m_utilsInBottom->setMaximumHeight(16);
	m_utilsInBottom->setMinimumHeight(16);
	
	connect(m_utilsInBottom, SIGNAL(clicked(int)), this, SLOT(selectLayerAction(int)));
	
	m_utilsInBottom->layout()->setSpacing(0); 
	m_utilsInBottom->layout()->setMargin(0);

	m_utilsInBottom->layout()->setAlignment(Qt::AlignLeft | Qt::AlignCenter);

	m_insertButton = new KTImageButton( QPixmap(KTOON_HOME+"/themes/default/icons/add_layer.png") , 20,  m_utilsInBottom );
	m_buttonGroup->addButton(m_insertButton, InsertLayer);

	m_insertButton->setToolTip(tr( "Insert Layer" ) );

	m_removeButton = new KTImageButton( QPixmap(KTOON_HOME+"/themes/default/icons/remove_layer.png"),  20, m_utilsInBottom );
	m_buttonGroup->addButton(m_removeButton, RemoveLayer);
	
	m_removeButton->setToolTip(tr( "Remove Layer" ) );

	m_moveUpButton = new KTImageButton( QPixmap(KTOON_HOME+"/themes/default/icons/arrowup.png"),  20, m_utilsInBottom );
	m_buttonGroup->addButton(m_moveUpButton,MoveLayerUp);

	m_moveUpButton->setToolTip(tr( "Move Layer Up" ) );

	m_moveDownButton = new KTImageButton( QPixmap(KTOON_HOME+"/themes/default/icons/arrowdown.png"), 20,  m_utilsInBottom );
	m_buttonGroup->addButton(m_moveDownButton,MoveLayerDown);

	m_moveDownButton->setToolTip(tr( "Move Layer Down" ) );
	
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

KTLayerSequence const *KTLayerManager::layerSequence()
{
	return m_sequence;
}

void KTLayerManager::removeLayer()
{
	m_sequence->removeLayer();
}

void KTLayerManager::removeLayer(int pos)
{
	m_sequence->removeRow(pos);
}

void KTLayerManager::createNewLayer(const QString &name, bool toEnd)
{
	m_sequence->createNewLayer( name, toEnd);
}

void KTLayerManager::selectLayer(int layerPos)
{
// 	SHOW_VAR(layerPos);
	
	m_sequence->selectionModel()->clear();
	m_sequence->setCurrentCell(layerPos, 0);
	m_sequence->selectRow(layerPos);
}

void KTLayerManager::moveCurrentLayer(bool up)
{
	if ( up)
	{
		m_sequence->moveLayerUp();
	}
	else
	{
		m_sequence->moveLayerDown();
	}
}

