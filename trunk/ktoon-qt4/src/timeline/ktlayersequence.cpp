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
#include <q3popupmenu.h>
//Added by qt3to4:
#include <QResizeEvent>

#include "ktlayersequence.h"
#include "ktdebug.h"

KTLayerSequence::KTLayerSequence(QWidget *parent) : QScrollArea(parent), m_layerCount(0)
{
	KTINIT;
	setWidgetResizable(true);
	setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);

// 	viewport() -> setMouseTracking( true );

// 	setPaletteBackgroundColor(Qt::black);
// 	setPaletteForegroundColor(Qt::gray );
	
	m_layerContainer = new KTVHBox(this);
// 	m_layerContainer->resize(590, m_layerContainer->height());
	m_layerContainer->setMinimumSize(590, 300);
	setWidget(m_layerContainer);
	
	m_layerContainer->layout()->setAlignment(Qt::AlignTop );
	
	m_defaultLayer = createNewLayer();
			
	/*new KTTimeLineLayer (tr("Layer %1").arg("1"), 1, m_layerContainer);
	m_layers.append( m_defaultLayer );*/
	
// 	setMaximumHeight( sizeHint().height() ); // IMPORTANT

	m_pCurrentLayer = m_defaultLayer;
	m_pLastLayer = m_pCurrentLayer;
	m_pCurrentLayer -> setSelected( true );
	m_pCurrentLayer -> setEdited( true );
}


KTLayerSequence::~KTLayerSequence()
{
	KTEND;
}


void KTLayerSequence::resizeEvent(QResizeEvent *e)
{
	QSize nSize = e->size();
	QWidget::resizeEvent( e );
	m_layerContainer->resize( nSize.width(), m_layerContainer->height() );
}

ListOfTLLayers KTLayerSequence::layers()
{
	return m_layers;
}

void KTLayerSequence::setPalette(const QPalette &)
{
}

KTTimeLineLayer * KTLayerSequence::createNewLayer()
{
	KTTimeLineLayer *newLayer = new KTTimeLineLayer( tr( "New Layer %1").arg( m_layerCount ), m_layers.count(), m_layerContainer);
	newLayer -> resize( width(), 24 );
	
	connect(newLayer, SIGNAL(rightClicked(KTTimeLineLayer *,const QPoint &)), this, SLOT(displayMenu(KTTimeLineLayer *,const QPoint &)));
	connect( newLayer, SIGNAL( selected(int) ), SLOT( selectLayer(int) ) );
// 	connect( newLayer, SIGNAL( draggedAbove( int ) ), SLOT( slotDragLayerAbove( int ) ) );
// 	connect( newLayer, SIGNAL( draggedBelow( int ) ), SLOT( slotDragLayerBelow( int ) ) );
// 	connect( newLayer, SIGNAL( releasedAbove( int ) ), SLOT( slotReleaseLayerAbove( int ) ) );
// 	connect( newLayer, SIGNAL( releasedBelow( int ) ), SLOT( slotReleaseLayerBelow( int ) ) );
// 	connect( newLayer, SIGNAL( renamed( const QString & ) ), SLOT( slotRenameLayer( const QString & ) ) );
// 	connect( newLayer, SIGNAL( visibilityChanged( bool ) ), SLOT( slotChangeVisibilityState( bool ) ) );
	
	newLayer->show();
	m_layers.append( newLayer );
	
	m_pLastLayer = newLayer;
	
	m_layerCount++;
	
	adjustSize();
	
	return newLayer;
}

void KTLayerSequence::removeLayer()
{
	if ( m_layers.count() > 1 )
	{
		KTTimeLineLayer *bridgeLayer;
		
		//Case 1: When the layer is the last within the list of layers
		if ( m_pCurrentLayer == m_pLastLayer )
		{
			m_layers.remove( m_pCurrentLayer );
			bridgeLayer = m_layers.getLast();
			delete m_pCurrentLayer;
			m_pCurrentLayer = bridgeLayer;
			m_pLastLayer = m_pCurrentLayer;
			m_pCurrentLayer -> setSelected( true );
			m_pCurrentLayer -> setEdited( true );
		}
		
		//Case 2: When the layer is any except the last
		else
		{
			bridgeLayer = m_layers.at( m_layers.find( m_pCurrentLayer ) + 1 );
			
			//Reaccomodate every layer next to the layer that is going to be deleted
			KTTimeLineLayer *iterator;
			for ( iterator = bridgeLayer; iterator; iterator = m_layers.next() )
			{
				iterator -> setPosition( iterator->position() - 1 );
			}

			m_layerContainer->layout()->remove(m_pCurrentLayer);
			m_layers.remove( m_pCurrentLayer );
			delete m_pCurrentLayer;
			m_pCurrentLayer = bridgeLayer;
			m_pCurrentLayer->setSelected( true );
			m_pCurrentLayer->setEdited( true );
		}
	}
}

void KTLayerSequence::selectLayer(int id)
{
	ktDebug() << "SELECT LAYER WITH ID: " << id << endl;
	KTTimeLineLayer *selected = m_layers.at(id);
	
	if ( !selected )
	{
		ktError(1) << "Layer not exists " << id << endl;
		return;
	}
	

	m_pCurrentLayer->clearEditFocus();

	m_pCurrentLayer->setSelected( false );
	m_pCurrentLayer->setEdited( false );
	
	m_pCurrentLayer = selected;
	m_pCurrentLayer->setSelected( true );
	m_pCurrentLayer->setEdited( true );

// 	emit layerSelectedToES( m_layers.find( m_pCurrentLayer ) );
// 	emit layerSelected( m_pCurrentLayer );
}

void KTLayerSequence::moveLayerUp()
{
	ktDebug() << "KTLayerSequence::moveLayerUp()" << endl;
	//If the current layer is the first, do nothing
	if ( m_pCurrentLayer == m_layers.getFirst() )
	{
		return;
	}
    
	//Find the layer above the current and reinsert it into the list in its new position
	KTTimeLineLayer *layerAbove = m_layers.take( m_layers.find( m_pCurrentLayer ) - 1 );
	
	m_pCurrentLayer -> setPosition(m_pCurrentLayer->position()-1);
	
	layerAbove -> setPosition(layerAbove->position()+1);
	
	m_layers.insert( layerAbove -> position() , layerAbove );
	
	if ( m_pCurrentLayer == m_pLastLayer )
	{
		m_pLastLayer = layerAbove;
	}
	
	//Swap both layers
	m_layerContainer->moveWidgetUp(m_pCurrentLayer);
	
// 	k_toon -> renderCameraPreview() -> updateGL();
}

void KTLayerSequence::moveLayerDown()
{
	//If the m_pCurrentLayer is the last, do nothing
	if ( m_pCurrentLayer == m_pLastLayer )
	{
		return;
	}
	
	//Find the layer above the current and reinsert it into the list in its new position
	KTTimeLineLayer *layerBelow = m_layers.take( m_layers.find( m_pCurrentLayer ) + 1 );
	m_pCurrentLayer -> setPosition( m_pCurrentLayer -> position() + 1 );
	layerBelow -> setPosition( layerBelow -> position() - 1 );
	m_layers.insert( layerBelow -> position(), layerBelow );
	if ( layerBelow == m_pLastLayer )
	{
		m_pLastLayer = m_pCurrentLayer;
	}

	m_layerContainer->moveWidgetDown(m_pCurrentLayer);
	
// 	k_toon -> renderCameraPreview() -> updateGL();
}

void KTLayerSequence::displayMenu(KTTimeLineLayer *ly, const QPoint &pos)
{
	if ( ly )
	{
		Q3PopupMenu *menu = new Q3PopupMenu( this );
		menu -> insertItem( tr( "Rename Layer" ), ly, SLOT( rename() ) );
		menu -> insertSeparator();
		menu -> insertItem( tr( "Insert Layer" ), this, SLOT( createNewLayer() ) );
		menu -> insertItem( tr( "Delete Layer" ), this, SLOT( removeLayer() ) );
		menu -> exec( pos );
	}
}

