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

#include <qlayout.h>
 
#include "ktlayersequence.h"
#include "ktdebug.h"

KTLayerSequence::KTLayerSequence(QWidget *parent) : QScrollView(parent, "KTLayerSequence")
{
	KTINIT;

	enableClipper( true );
	setHScrollBarMode( QScrollView::AlwaysOff );

// 	viewport() -> setMouseTracking( true );

	setPaletteBackgroundColor(Qt::black);
	setPaletteForegroundColor(Qt::gray );
	
	m_layerContainer = new QVBox(viewport());
	addChild(m_layerContainer);
	m_layerContainer->layout()->setAlignment(Qt::AlignTop );
	
	m_defaultLayer = new KTTimeLineLayer (tr("Layer %1").arg("1"), 1, m_layerContainer);
	m_layers.append( m_defaultLayer );
	
	setMaximumHeight( sizeHint().height() ); // IMPORTANT
	
	// TEST CODE
// 	for(uint i = 0; i < 30; i++)
// 	{
// 		KTTimeLineLayer *tmp = new KTTimeLineLayer (tr("Layer %1").arg("1"), 1, m_layerContainer);
// 		m_layers.append(tmp);
// 	}
	////
	
// 	connect( default_layer, SIGNAL( selected() ), SLOT( slotSelectLayer() ) );
// 	connect( default_layer, SIGNAL( draggedAbove( int ) ), SLOT( slotDragLayerAbove( int ) ) );
// 	connect( default_layer, SIGNAL( draggedBelow( int ) ), SLOT( slotDragLayerBelow( int ) ) );
// 	connect( default_layer, SIGNAL( releasedAbove( int ) ), SLOT( slotReleaseLayerAbove( int ) ) );
// 	connect( default_layer, SIGNAL( releasedBelow( int ) ), SLOT( slotReleaseLayerBelow( int ) ) );
// 	connect( default_layer, SIGNAL( renamed( const QString & ) ), SLOT( slotRenameLayer( const QString & ) ) );
// 	connect( default_layer, SIGNAL( visibilityChanged( bool ) ), SLOT( slotChangeVisibilityState( bool ) ) );

// 	current_layer = default_layer;
// 	last_layer = current_layer;
// 	current_layer -> setSelected( true );
// 	current_layer -> setEdited( true );
}


KTLayerSequence::~KTLayerSequence()
{
	KTEND;
}


void KTLayerSequence::resizeEvent(QResizeEvent *e)
{
	QSize new_size = e->size();
	QWidget::resizeEvent( e );
	m_layerContainer->resize( new_size.width(), m_layerContainer->height() );
}


