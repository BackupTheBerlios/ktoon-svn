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

#include "ktdebug.h"

KTTimeLineLayer::KTTimeLineLayer(const QString &name, int position, QWidget *parent) : QHBox(parent, "KTTimeLineLayer"), m_isLocked(false), m_isVisible(true), m_onlySeeOutlines(false), m_isSelected(false), m_isEdited(false)
{
	setMinimumSize( 192, 24 );
	setFrameStyle( QFrame::Panel | QFrame::Raised );
	setLineWidth( 2 );
	
	m_position = position;

    	//Icon Initializations
	
	//----------- Main Component Initializations -------------
	
	QLabel *staticLayerImage = new QLabel( this );
	staticLayerImage -> setPixmap( QPixmap( KTOON_HOME+"/images/icons/layer_pic.xpm" ) );
	staticLayerImage->resize( 20, 20 );

	m_layerName = new KTSqueezeLabel( name, this );
	m_layerName->setMargin(1);
	m_layerName->resize( 50, 20 );
	m_layerName->setFont( QFont( font().family(), 9 ) );
	
	m_editionImage = new QLabel( this );
	m_editionImage -> setMinimumSize( 19, 19 );
	m_editionImage -> setMaximumSize( 19, 19 );

	m_nameEditor = new QLineEdit( m_layerName );
	
	m_nameEditor->setFont( QFont( font().family(), 9 ) );

	m_nameEditor->hide();
	connect( m_nameEditor, SIGNAL( lostFocus() ), SLOT( editName() ) );
	connect( m_nameEditor, SIGNAL( returnPressed() ), SLOT( editName() ) );
	
	m_utils = new QHBox(this);
	m_utils->layout()->setAlignment(Qt::AlignRight );
	
	m_utils->setSpacing(5);
	
	m_visibilityImage = new QLabel( m_utils );
	m_visibilityImage -> setPixmap( QPixmap( KTOON_HOME+"/images/icons/enable.xpm" ) );
	m_visibilityImage -> resize( 20, 20 );

	
	m_lockImage = new QLabel( m_utils );
	m_lockImage -> setPixmap( QPixmap( KTOON_HOME+"/images/icons/disable.xpm" ) );
	m_lockImage -> resize( 20, 20 );

	m_onlyOutlines = new QCheckBox( m_utils );
	m_onlyOutlines->setPaletteForegroundColor(Qt::black); // FIXME
	m_onlyOutlines -> resize( 20, 20 );

	connect( m_onlyOutlines, SIGNAL( clicked() ), this, SLOT( toggleOutlines() ) );
	
	QWidget *spacer = new QWidget(m_utils);
	spacer->setMinimumWidth(20);
	
	show();
}


KTTimeLineLayer::~KTTimeLineLayer()
{
}

void KTTimeLineLayer::setSelected( bool selected )
{
	m_isSelected = selected;

	if ( m_isSelected )
	{
		m_layerName->setPaletteForegroundColor( palette().color(QPalette::Active , QColorGroup::HighlightedText) );
		m_layerName->setPaletteBackgroundColor( palette().color(QPalette::Active , QColorGroup::Highlight) );
		setPaletteBackgroundColor( palette().color(QPalette::Active , QColorGroup::Highlight) );
	}
	else
	{
		m_layerName->setPaletteForegroundColor( parentWidget()->paletteForegroundColor() );
		m_layerName->setPaletteBackgroundColor( parentWidget()->paletteBackgroundColor() );
		setPaletteBackgroundColor( parentWidget()->paletteBackgroundColor() );
	}
}

void KTTimeLineLayer::setEdited( bool isEdited )
{
	m_isEdited = isEdited;
	
	if ( m_isEdited )
	{
		m_editionImage -> setPixmap( QPixmap( KTOON_HOME+"/images/icons/written_pic.xpm" ) );
	}
	else
	{
		m_editionImage->setPixmap( QString::null );
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
	
	setSelected( true );
	setEdited( true );

// 	emit selected();
}

void KTTimeLineLayer::setLock(bool yes)
{
	if ( !yes )
	{
		m_lockImage->setPixmap( QPixmap( KTOON_HOME+"/images/icons/enable.xpm" ) );
	}
	else
	{
		m_lockImage->setPixmap( QPixmap( KTOON_HOME+"/images/icons/disable.xpm" ) );
	}
	m_isLocked = yes;
}

void KTTimeLineLayer::toggleLock()
{
	if ( !m_isLocked )
	{
		m_lockImage->setPixmap( QPixmap( KTOON_HOME+"/images/icons/enable.xpm" ) );
	}
	else
	{
		m_lockImage->setPixmap( QPixmap( KTOON_HOME+"/images/icons/disable.xpm" ) );
	}
	m_isLocked = !m_isLocked;
}

void KTTimeLineLayer::setView(bool yes)
{
	if ( !yes )
	{
		m_visibilityImage -> setPixmap( QPixmap( KTOON_HOME+"/images/icons/enable.xpm" ) );
	}
	else
	{
		m_visibilityImage->setPixmap( QPixmap( KTOON_HOME+"/images/icons/disable.xpm" ) );
	}
	m_isVisible = yes;
}

void KTTimeLineLayer::toggleView()
{
	if ( !m_isVisible )
	{
		m_visibilityImage -> setPixmap( QPixmap( KTOON_HOME+"/images/icons/enable.xpm" ) );
	}
	else
	{
		m_visibilityImage->setPixmap( QPixmap( KTOON_HOME+"/images/icons/disable.xpm" ) );
	}
	m_isVisible = !m_isVisible;
}

void KTTimeLineLayer::editName()
{
	m_layerName->setText( m_nameEditor->text() );
	m_nameEditor-> hide();

// 	emit renamed( layer_name -> text() );
}

void KTTimeLineLayer::mouseDoubleClickEvent( QMouseEvent *e )
{
	if ( childAt( e->pos() ) == m_layerName && e->button() == Qt::LeftButton )
	{
		m_nameEditor->setText( m_layerName->completeText() );
		m_nameEditor->resize( m_layerName->size() );
		m_nameEditor->show();
		m_nameEditor->setFocus();
		e-> accept();
	}
	else
	{
		e->ignore();
	}
}

