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
#include <qtooltip.h>

#include <QMouseEvent>
#include <QLabel>

#include "ktdebug.h"

KTTimeLineLayer::KTTimeLineLayer(const QString &name) : QFrame(), m_isLocked(false), m_isVisible(true), m_onlySeeOutlines(false), m_isSelected(false), m_isEdited(false)
{
	KTINIT;
	
	QPalette pal = palette();
	pal.setBrush( QPalette::Base, pal.button());
	
	
	setPalette(pal);
	setAutoFillBackground(true);
	
	
	
	setFrameStyle( QFrame::Panel | QFrame::Raised );
	
	QHBoxLayout *m_layout = new QHBoxLayout;
	m_layout->setMargin(1);
	m_layout->setSpacing(1);
	
	setLineWidth( 2 );

    	//Icon Initializations
	
	//----------- Main Component Initializations -------------
	
	QLabel *staticLayerImage = new QLabel(this);
	m_layout->addWidget(staticLayerImage);
	staticLayerImage->show();
	
	staticLayerImage -> setPixmap( QPixmap( KTOON_HOME+"/themes/default/icons/layer_pic.png" ) );
	staticLayerImage->setMinimumSize( 20, 20 );
// 	layout()->setAlignment(staticLayerImage, Qt::AlignLeft);

	m_layerName = new KTELabel( name, this );
	m_layout->addWidget(m_layerName);
	
	m_layerName->show();
// 	layout()->setAlignment(m_layerName, Qt::AlignLeft);
	connect(m_layerName, SIGNAL(edited(const QString &)), this, SIGNAL(renamed(const QString &)));

	m_layerName->setMargin(1);
	m_layerName->resize( 70, 20 );
	m_layerName->setFont( QFont( font().family(), 9 ) );
	
// 	m_editionImage -> setMinimumSize( 19, 19 );
// 	m_editionImage -> setMaximumSize( 19, 19 );
	
	m_utils = new KTVHBox(this, false);
	m_layout->addWidget(m_utils);
	m_utils->show();
// 	layout()->setAlignment(m_utils, Qt::AlignRight);
	m_utils->layout()->setAlignment(Qt::AlignRight );
	
	m_editionImage = new QLabel( m_utils );
	
// 	QPalette pal = palette();
// 	pal.setColor(QPalette::Background, Qt::red);
// 	m_layerName->setPalette(pal);
	
	m_utils->layout()->setSpacing(6);
	
	m_visibilityImage = new QLabel( m_utils );
// 	m_layout->addWidget(m_visibilityImage);
	m_visibilityImage -> setPixmap( QPixmap( KTOON_HOME+"/themes/default/icons/enable.png" ) );
	m_visibilityImage -> resize( 20, 20 );

	
	m_lockImage = new QLabel( m_utils );
// 	m_layout->addWidget(m_lockImage);
	m_lockImage -> setPixmap( QPixmap( KTOON_HOME+"/themes/default/icons/disable.png" ) );
	m_lockImage -> resize( 20, 20 );

	m_onlyOutlines = new QCheckBox( m_utils );
	
// 	m_layout->addWidget(m_onlyOutlines);
	m_onlyOutlines->resize( 20, 20 );

	connect( m_onlyOutlines, SIGNAL( clicked() ), this, SLOT( toggleOutlines() ) );

	setLayout(m_layout);
	
	setMaximumHeight(26);
}


KTTimeLineLayer::~KTTimeLineLayer()
{
	KTEND;
}

QSize KTTimeLineLayer::sizeHint() const
{
	QSize size = QFrame::sizeHint();
	size.setHeight(24);
	
	return size;
}

void KTTimeLineLayer::setEdited( bool isEdited )
{
	m_isEdited = isEdited;
	
	if ( m_isEdited )
	{
		m_editionImage -> setPixmap( QPixmap( KTOON_HOME+"/themes/default/icons/written_pic.png" ) );
	}
	else
	{
		m_editionImage->setPixmap( QPixmap() );
	}
}

void KTTimeLineLayer::setOnlyOutlines( bool yes )
{
	m_onlyOutlines->setChecked(yes);
}

void KTTimeLineLayer::toggleOutlines()
{
// 	m_onlyOutlines->toggle();
	
	m_onlySeeOutlines = !m_onlySeeOutlines;	
	m_onlyOutlines->setChecked(m_onlySeeOutlines);
	
// 	setSelected( true );
	setEdited( true );

	emit selected(this);
}

void KTTimeLineLayer::setLock(bool yes)
{
	if ( !yes )
	{
		m_lockImage->setPixmap( QPixmap( KTOON_HOME+"/themes/default/icons/enable.png" ) );
	}
	else
	{
		m_lockImage->setPixmap( QPixmap( KTOON_HOME+"/themes/default/icons/disable.png" ) );
	}
	m_isLocked = yes;
}

void KTTimeLineLayer::toggleLock()
{
	if ( !m_isLocked )
	{
		m_lockImage->setPixmap( QPixmap( KTOON_HOME+"/themes/default/icons/enable.png" ) );
	}
	else
	{
		m_lockImage->setPixmap( QPixmap( KTOON_HOME+"/themes/default/icons/disable.png" ) );
	}
	m_isLocked = !m_isLocked;
}

void KTTimeLineLayer::setView(bool yes)
{
	if ( !yes )
	{
		m_visibilityImage -> setPixmap( QPixmap( KTOON_HOME+"/themes/default/icons/enable.png" ) );
	}
	else
	{
		m_visibilityImage->setPixmap( QPixmap( KTOON_HOME+"/themes/default/icons/disable.png" ) );
	}
	m_isVisible = yes;
}

void KTTimeLineLayer::toggleView()
{
	if ( !m_isVisible )
	{
		m_visibilityImage -> setPixmap( QPixmap( KTOON_HOME+"/themes/default/icons/enable.png" ) );
	}
	else
	{
		m_visibilityImage->setPixmap( QPixmap( KTOON_HOME+"/themes/default/icons/disable.png" ) );
	}
	m_isVisible = !m_isVisible;
}

void KTTimeLineLayer::mousePressEvent( QMouseEvent *me )
{
	setEdited( true );

	if ( childAt( me -> pos() ) == m_visibilityImage )
	{
		setView( !m_isVisible );
	}
	else if ( childAt( me -> pos() ) == m_lockImage )
	{
		setLock( !m_isLocked );
	}
	
	if ( me -> button() == Qt::LeftButton )
	{
// 		dragging = true;
// 		TLLayerSequence *ls_parent = ( TLLayerSequence * )grandparent_widget;
// 		ls_parent -> setDragging( true );
	}
	
	emit selected(this);

	if ( me -> button() == Qt::RightButton )
	{
		emit rightClicked(this, me->globalPos());
// 		TLLayerManager *lm = ( TLLayerManager * )( grandparent_widget -> parentWidget() );
// 		if ( right_click_menu )
// 		{
// 			delete right_click_menu;
// 		}
// 		right_click_menu = new QPopupMenu( this );
// 		// 	right_click_menu -> setFont( QFont( "helvetica", 10 ) );
// 		right_click_menu -> insertItem( tr( "Rename Layer" ), this, SLOT( slotSendDoubleClickEvent() ) );
// 		right_click_menu -> insertSeparator();
// 		right_click_menu -> insertItem( tr( "Insert Layer" ), lm -> insertLayerButton(), SLOT( animateClick() ) );
// 		right_click_menu -> insertItem( tr( "Delete Layer" ), lm -> removeLayerButton(), SLOT( animateClick() ) );
// 		right_click_menu -> exec( QCursor::pos() );
	}
	me -> accept();
}

void KTTimeLineLayer::rename()
{
	m_layerName->edit();
}

void KTTimeLineLayer::clearEditFocus()
{
	m_layerName->clearFocus();
}

