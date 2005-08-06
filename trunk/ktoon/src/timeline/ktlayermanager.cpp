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

KTLayerManager::KTLayerManager(QWidget *parent) : QVBox(parent, "KTLayerManager")
{
	qDebug("[Initializing KTLayerManager]");
	
	utilsInTop = new QHBox( this );
	utilsInTop -> setFrameStyle( QFrame::Box | QFrame::Plain );
	
	outlineButton = new QPushButton( QPixmap(KTOON_HOME+"/images/icons/outline_pic.xpm"), "", utilsInTop );
	outlineButton->setFlat( true );
	outlineButton->setAutoDefault( false );
	outlineButton->resize( 20, 20 );
// 	outlineButton->move( utilsInTop -> width() - outlineButton -> width() - 22, 2 );
// 	connect( outlineButton, SIGNAL( clicked() ), SLOT( slotSquare() ) );
	QToolTip::add( outlineButton, tr( "Show only outlines" ) );
	
	lockButton = new QPushButton( QPixmap(KTOON_HOME+"/images/icons/kilit_pic.xpm"), "", utilsInTop );
	lockButton->setFlat( true );
	lockButton->setAutoDefault( false );
	lockButton->resize( 20, 20 );
	
// 	lockButton -> move( outlineButton -> x() - lock -> width() - 2, 2 );
// 	connect( lock, SIGNAL( clicked() ), SLOT( slotLock() ) );
	
	QToolTip::add( lockButton, tr( "Lock all Layers" ) );

	eyeButton = new QPushButton( QPixmap(KTOON_HOME+"/images/icons/show_hide_all_layers.xpm"), "", utilsInTop );
	eyeButton->setFlat( true );
	eyeButton->setAutoDefault( false );
	eyeButton->resize( 20, 20 );
	
// 	eyeButton -> move( lockButton->x() - eyeButton->width() - 2, 2 );
// 	connect( eye, SIGNAL( clicked() ), SLOT( slotEye() ) );
	
	QToolTip::add( eyeButton, tr( "Show / Hide all Layers" ) );
	
	show();
}


KTLayerManager::~KTLayerManager()
{
	qDebug("[Destroying KTLayerManager]");
}


