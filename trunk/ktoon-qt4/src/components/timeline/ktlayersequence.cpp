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
#include <qmenu.h>
#include <QResizeEvent>

#include "ktlayersequence.h"
#include "ktdebug.h"

KTLayerSequence::KTLayerSequence(QWidget *parent) : KTWidgetListView(parent), m_layerCount(0)
{
	KTINIT;
	
	setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	
}


KTLayerSequence::~KTLayerSequence()
{
	KTEND;
}


// void KTLayerSequence::resizeEvent(QResizeEvent *e)
// {
// 	QSize nSize = e->size();
// 	QWidget::resizeEvent( e );
// 	widget()->resize( nSize.width(), /*widget()->*/height() );
// }

ListOfTLLayers KTLayerSequence::layers()
{
	return m_layers;
}

void KTLayerSequence::setPalette(const QPalette &)
{
}

KTTimeLineLayer * KTLayerSequence::createNewLayer(const QString &name, bool toEnd)
{
	ktDebug() << "KTLayerSequence::createNewLayer(" << name << "," << toEnd << ")";
	KTTimeLineLayer *newLayer = new KTTimeLineLayer( name, this);
// 	newLayer->resize( width(), 24 );
	newLayer->setMinimumHeight(24);
	
	connect(newLayer, SIGNAL(rightClicked(KTTimeLineLayer *,const QPoint &)), this, SLOT(displayMenu(KTTimeLineLayer *,const QPoint &)));
	connect( newLayer, SIGNAL( selected(int) ), SLOT( selectLayer(int) ) );
// 	connect( newLayer, SIGNAL( draggedAbove( int ) ), SLOT( slotDragLayerAbove( int ) ) );
// 	connect( newLayer, SIGNAL( draggedBelow( int ) ), SLOT( slotDragLayerBelow( int ) ) );
// 	connect( newLayer, SIGNAL( releasedAbove( int ) ), SLOT( slotReleaseLayerAbove( int ) ) );
// 	connect( newLayer, SIGNAL( releasedBelow( int ) ), SLOT( slotReleaseLayerBelow( int ) ) );
// 	connect( newLayer, SIGNAL( renamed( const QString & ) ), SLOT( slotRenameLayer( const QString & ) ) );
// 	connect( newLayer, SIGNAL( visibilityChanged( bool ) ), SLOT( slotChangeVisibilityState( bool ) ) );
	
// 	newLayer->show();
	m_layers.append( newLayer );
	
	m_pLastLayer = newLayer;
	
	m_layerCount++;
	
// 	adjustSize();
	
	return newLayer;
}

void KTLayerSequence::removeLayer()
{
	ktDebug() << "Remove layer";
	removeItem(currentItem());
}

void KTLayerSequence::selectLayer(int id)
{
}

void KTLayerSequence::moveLayerUp()
{
}

void KTLayerSequence::moveLayerDown()
{
}

void KTLayerSequence::displayMenu(KTTimeLineLayer *ly, const QPoint &pos)
{
	if ( ly )
	{
		QMenu *menu = new QMenu( this );
		menu -> insertItem( tr( "Rename Layer" ), ly, SLOT( rename() ) );
		menu -> insertSeparator();
		menu -> insertItem( tr( "Insert Layer" ), this, SLOT( createNewLayer() ) );
		menu -> insertItem( tr( "Delete Layer" ), this, SLOT( removeLayer() ) );
		menu -> exec( pos );
	}
}

