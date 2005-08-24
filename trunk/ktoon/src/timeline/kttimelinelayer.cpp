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

#include "kttimelinelayer.h"
#include "ktapplication.h"
#include <qpixmap.h>
#include <qlayout.h>

KTTimeLineLayer::KTTimeLineLayer(const QString &name, int position, QWidget *parent) : QHBox(parent, "KTTimeLineLayer")
{
;
	setMinimumSize( 192, 24 );
	setFrameStyle( QFrame::Panel | QFrame::Raised );
	setLineWidth( 2 );
	
	m_position = position;

    	//Icon Initializations
	
	//----------- Main Component Initializations -------------
	
	QLabel *static_layer_image = new QLabel( this );
	static_layer_image -> setPixmap( QPixmap( KTOON_HOME+"/images/icons/layer_pic.xpm" ) );
	static_layer_image->resize( 20, 20 );
// 	static_layer_image->move( 2, 2 );

	m_layerName = new QLabel( name, this );
	m_layerName -> resize( 50, 20 );
	m_layerName -> setFont( QFont( font().family(), 9 ) );
// 	m_layerName -> move( static_layer_image -> x() + static_layer_image -> width(), 2 );
	
	m_editionImage = new QLabel( this );
	m_editionImage -> resize( 20, 20 );
// 	edition_image -> move( visibility_image -> x() - edition_image -> width() - 2, 2 );


	m_nameEditor = new QLineEdit( this );
	m_nameEditor->resize( 50, 20 );
	m_nameEditor->setFont( QFont( font().family(), 9 ) );
// 	m_nameEditor->move( layer_name -> x(), 2 );
	m_nameEditor->hide();
// 	connect( edit_name, SIGNAL( lostFocus() ), SLOT( slotEditName() ) );
// 	connect( edit_name, SIGNAL( returnPressed() ), SLOT( slotEditName() ) );
	
	m_utils = new QHBox(this);
	m_utils->layout()->setAlignment(Qt::AlignRight );
	
	m_utils->setSpacing(5);
	
	m_visibilityImage = new QLabel( m_utils );
	m_visibilityImage -> setPixmap( QPixmap( KTOON_HOME+"/images/icons/enable.xpm" ) );
	m_visibilityImage -> resize( 20, 20 );
// 	visibility_image -> move( lock_image -> x() - visibility_image -> width() - 2, 2 );
	
	m_lockImage = new QLabel( m_utils );
	m_lockImage -> setPixmap( QPixmap( KTOON_HOME+"/images/icons/disable.xpm" ) );
	m_lockImage -> resize( 20, 20 );
// 	lock_image -> move( m_onlyOutlines -> x() - lock_image -> width() - 2, 2 );
	
	m_onlyOutlines = new QCheckBox( m_utils );
	m_onlyOutlines -> resize( 20, 20 );
// 	m_onlyOutlines -> move( width() - m_onlyOutlines -> width() - 22, 2 );
// 	connect( m_onlyOutlines, SIGNAL( clicked() ), SLOT( slotOnlyOutlines() ) );

	show();
}


KTTimeLineLayer::~KTTimeLineLayer()
{
}


