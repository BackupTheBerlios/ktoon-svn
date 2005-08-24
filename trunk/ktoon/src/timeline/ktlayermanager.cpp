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

#include "kseparator.h"

#include "ktdebug.h"

KTLayerManager::KTLayerManager(QWidget *parent) : QVBox(parent, "KTLayerManager"), m_currentTime(0.0), m_totalTime(0.04)
{
	KTINIT;
	
	m_utilsInTop = new QHBox( this );
	
	m_utilsInTop->layout()->setAlignment(Qt::AlignRight | Qt::AlignTop | Qt::AlignVCenter );
	
	m_utilsInTop -> setFrameStyle( QFrame::Box | QFrame::Plain );
	
	m_outlineButton = new KTImageButton( QPixmap(KTOON_HOME+"/images/icons/outline_pic.xpm"), 20, m_utilsInTop );
// 	m_outlineButton->setFlat( true );
// 	m_outlineButton->setAutoDefault( false );

// 	outlineButton->move( utilsInTop -> width() - outlineButton -> width() - 22, 2 );
// 	connect( outlineButton, SIGNAL( clicked() ), SLOT( slotSquare() ) );
	QToolTip::add( m_outlineButton, tr( "Show only outlines" ) );
	
	m_lockButton = new KTImageButton( QPixmap(KTOON_HOME+"/images/icons/kilit_pic.xpm"),  20, m_utilsInTop );
// 	m_lockButton->setFlat( true );
// 	m_lockButton->setAutoDefault( false );
// 	m_lockButton->resize( 20, 20 );
	
// 	lockButton -> move( outlineButton -> x() - lock -> width() - 2, 2 );
// 	connect( lock, SIGNAL( clicked() ), SLOT( slotLock() ) );
	
	QToolTip::add( m_lockButton, tr( "Lock all Layers" ) );

	m_eyeButton = new KTImageButton( QPixmap(KTOON_HOME+"/images/icons/show_hide_all_layers.xpm"), 20,  m_utilsInTop );
// 	m_eyeButton->setFlat( true );
// 	m_eyeButton->setAutoDefault( false );
// 	m_eyeButton->resize( 20, 20 );
	
// 	eyeButton -> move( lockButton->x() - eyeButton->width() - 2, 2 );
// 	connect( eye, SIGNAL( clicked() ), SLOT( slotEye() ) );
	
	QToolTip::add( m_eyeButton, tr( "Show / Hide all Layers" ) );
	
	m_utilsInTop->setMaximumHeight( m_eyeButton->height()+2);
	
	//------------------------------------------------------
	
	QScrollView *scroll = new QScrollView(this);
	scroll->enableClipper( true );
	m_sequence = new KTLayerSequence(scroll->viewport());
	scroll->addChild(m_sequence);
	scroll->setHScrollBarMode( QScrollView::AlwaysOff );
// 	scroll->viewport()->setMouseTracking( true );
	scroll->setMaximumWidth(m_sequence->width());
	
	//------------------------------------------------------
	
	m_utilsInBottom = new QHBox( this );
	m_utilsInBottom -> setFrameStyle( QFrame::Box | QFrame::Plain );
	m_utilsInBottom -> resize( 192, 24 );
	m_utilsInBottom -> setMinimumSize( 192, 24 );
	m_utilsInBottom->layout()->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
	
// 	m_utilsInBottom -> move( 0, layer_sequence -> height() + layer_sequence -> y() );

	m_insertButton = new KTImageButton( QPixmap(KTOON_HOME+"/images/icons/layer+.xpm") , 20,  m_utilsInBottom );
	m_insertButton -> setFlat( true );
	m_insertButton -> setAutoDefault( false );
	m_insertButton -> resize( 20, 20 );
	m_insertButton -> move( 2, 2 );
// 	connect( m_insertButton, SIGNAL( clicked() ), layer_sequence, SLOT( slotInsertLayer() ) );
// 	connect( m_insertButton, SIGNAL( clicked() ), grandparent, SIGNAL( insertLayerClicked() ) );
	QToolTip::add( m_insertButton, tr( "Insert Layer" ) );

	m_removeButton = new KTImageButton( QPixmap(KTOON_HOME+"/images/icons/layer-.xpm"),  20, m_utilsInBottom );

// 	m_removeButton -> move( m_insertButton -> x() + m_insertButton -> width() + 2, 2 );
// 	connect( m_removeButton, SIGNAL( clicked() ), layer_sequence, SLOT( slotRemoveLayer() ) );
// 	connect( m_removeButton, SIGNAL( clicked() ), grandparent, SIGNAL( removeLayerClicked() ) );
	QToolTip::add( m_removeButton, tr( "Remove Layer" ) );

	m_moveUpButton = new KTImageButton( QPixmap(KTOON_HOME+"/images/icons/arrowup.xpm"),  20, m_utilsInBottom );
// 	m_moveUpButton -> setFlat( true );
// 	m_moveUpButton -> setAutoDefault( false );
// 	m_moveUpButton -> resize( 20, 20 );
// 	m_moveUpButton -> move( m_removeButton -> x() + m_removeButton -> width() + 2, 2 );
// 	connect( m_moveUpButton, SIGNAL( clicked() ), layer_sequence, SLOT( slotMoveLayerUp() ) );
// 	connect( m_moveUpButton, SIGNAL( clicked() ), grandparent, SIGNAL( layerMovedUp() ) );
	QToolTip::add( m_moveUpButton, tr( "Move Layer Up" ) );

	m_moveDownButton = new KTImageButton( QPixmap(KTOON_HOME+"/images/icons/arrowdown.xpm"), 20,  m_utilsInBottom );
// 	m_moveDownButton -> setFlat( true );
// 	m_moveDownButton -> setAutoDefault( false );
// 	m_moveDownButton -> resize( 20, 20 );
// 	m_moveDownButton -> move( m_moveUpButton -> x() + m_moveUpButton -> width() + 2, 2 );
// 	connect( m_moveDownButton, SIGNAL( clicked() ), layer_sequence, SLOT( slotMoveLayerDown() ) );
// 	connect( m_moveDownButton, SIGNAL( clicked() ), grandparent, SIGNAL( layerMovedDown() ) );
	QToolTip::add( m_moveDownButton, tr( "Move Layer Down" ) );
	
	new KSeparator( Qt::Vertical, m_utilsInBottom);
	
	m_time = new QLabel( QString::number( m_currentTime, 'f', 2 ) + " / " + QString::number( m_totalTime, 'f', 2 ), m_utilsInBottom );
	
	
// 	static_cast<QBoxLayout *>(m_utilsInBottom->layout())->insertWidget(4, m_time, 0, Qt::AlignRight);
	
	m_time -> setFont( QFont( font().family(), 7 ) );
	m_time -> setAlignment( Qt::AlignCenter );
	m_time -> resize( 80, 20 );
// 	time -> move( m_utilsInBottom -> width() - time -> width(), 2 );
	new KSeparator( Qt::Vertical, m_utilsInBottom);
	
	m_utilsInBottom->setMaximumHeight(m_moveUpButton->height());
	
	show();
}


KTLayerManager::~KTLayerManager()
{
	KTEND;
}


