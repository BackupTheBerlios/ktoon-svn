/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado   *
 *   kuadrosx@toonka.com   *
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

#include "kttableexposure.h"
#include <qvgroupbox.h>
#include <qlayout.h>

#include "status.h"
#include "ktdebug.h"
#include "layer.h"
#include <memory>

KTTableExposure::KTTableExposure(int rows, int cols, QWidget *parent, const char *name)
	: QScrollView (parent, name),m_numLayer(0), m_currentLayer(0), m_currentFrame(0)
{
	KTINIT;
	
	setDragAutoScroll ( true ) ;
	viewport ()->setPaletteBackgroundColor ( paletteBackgroundColor() );
	
	m_port = new QWidget(this);
	
	setHScrollBarMode ( Auto );
	
	QVGroupBox *gridNumber = new QVGroupBox( m_port );
// 	gridNumber->layout()->setAlignment( Qt::AlignBottom | Qt::AlignHCenter);
// 	gridNumber->setInsideSpacing( 2 );
	gridNumber->setInsideSpacing( 4);
	gridNumber->setInsideMargin ( 4 );
	
	QLabel *tmp = new QLabel(gridNumber);
	tmp->setText(" ");
	for(int i = 0; i < rows; i++)
	{
		tmp = new QLabel(gridNumber);
		tmp->setText(QString::number(i));
	}
	
	m_layout = new QBoxLayout (  m_port, QBoxLayout::LeftToRight,4,1);
	m_layout->addWidget ( gridNumber );
	
	m_layers.setAutoDelete( true );
	for(int i = 0; i < cols; i++)
	{
		insertLayer(rows);
	}
	
	addChild(m_port);
	
	adjustSize();
	setMinimumHeight(sizeHint().height());
}


KTTableExposure::~KTTableExposure()
{
	KTEND;
}

void KTTableExposure::clickedCell(int row,int col,int button,int gx,int gy)
{
	m_currentLayer = col;
	m_currentFrame = row;
	emit(cellSelected(col, row));
	
	
	QPtrList<Layer> ly = KTStatus -> currentScene() -> getLayers();
	Layer *sl = ly.at( m_currentLayer );

// 	ktDebug() << sl;
	KTStatus -> setCurrentLayer( sl );
// 	QPtrList<Layer> ly = KTStatus -> currentScene() -> getLayers();
// 	KTStatus -> setCurrentLayer( ly.at( list_of_layers.find( layer_iterator ) ) );

	ktDebug() << "id_m_currentLayer " << m_currentLayer << " " << "id_m_currentFrame" << m_currentFrame ;
	if ( m_layers.at(m_currentLayer)->currentFrameIsUsed() )
	{
		QPtrList<KeyFrame> kf = KTStatus -> currentLayer() -> keyFrames();
		KTStatus -> setCurrentKeyFrame( kf.at(  m_currentFrame) );
	}
	else
		KTStatus -> setCurrentKeyFrame( NULL );
	
	emit(frameSelected());
	
	if(button == Qt::RightButton)
	{
		//show pupo gx, gy
	}
}

void KTTableExposure::insertLayer(int rows)
{
	QString text = "Layer"+ QString::number(m_numLayer);
	
	KTLayerExposure *newLayer = new KTLayerExposure(text, m_numLayer, 100, m_port);
	m_layers.append(newLayer);
	connect(newLayer, SIGNAL(selected(int)), this, SLOT(changeCurrentLayer(int)));
	connect(this, SIGNAL(layerSelected(int)), newLayer, SLOT(otherSelected(int)));
	connect(newLayer, SIGNAL(clicked(int,int,int,int,int)), this, SLOT(clickedCell(int,int,int,int,int)));
	
	m_layout->addWidget( newLayer, 0, Qt::AlignTop | Qt::AlignCenter);
	
	m_numLayer++;
	newLayer->show();
	
	std::auto_ptr<Layer> ap_n_layer(new Layer);
	Layer* n_layer = ap_n_layer.get();
    
	n_layer->setIndexLayer( m_layers.count() );
	n_layer->setNameLayer( tr( "Layer" ) + m_numLayer );
	QPtrList<KeyFrame> kf = n_layer -> keyFrames();
	kf.first() -> setNameKeyFrame( text);
	QPtrList<Layer> ly = KTStatus -> currentScene() -> getLayers();
	ly.append( n_layer );
	KTStatus -> currentScene() -> setLayers( ly );
	ap_n_layer.release();
    
	KTStatus->currentDrawingArea() -> modifyDocument( true );

// 	if ( ( Timeline * )sender() != k_toon -> timeline() )
		emit layerInserted();
	
	
}

void KTTableExposure::changeCurrentLayer(int idLayer)
{
	m_currentLayer = idLayer;
	m_currentFrame = m_layers.at(m_currentLayer)->useFrame();
	emit layerSelected(idLayer);
}

void KTTableExposure::setUseFrame()
{
	m_layers.at(m_currentLayer)->setUseFrames(m_currentFrame);
	emit(activateCursor());
// 	KTStatus->currentDrawingArea()->modifyDocument( true );
}

void KTTableExposure::removeFrameSelected()
{
	m_layers.at(m_currentLayer)->removeFrame(m_currentFrame);
	QPtrList<KeyFrame> kf = KTStatus -> currentLayer() -> keyFrames();
	KeyFrame *lkf = kf.at(m_currentFrame);
// 		KeyFrame *pkf = kf.prev();
// 		pkf -> setLengthKeyFrame( pkf -> lengthKeyFrame() + lkf -> lengthKeyFrame() );
		emit frameRemoved( m_currentFrame );
	kf.setAutoDelete( true );
	kf.remove( lkf );
	kf.setAutoDelete( false );
	KTStatus -> currentLayer() -> setKeyFrames( kf );
	
}

void KTTableExposure::moveCurrentFrame(Direction d)
{
	if(d == Up)
	{
		m_layers.at(m_currentLayer)->moveCurrentFrameUp();
	}
	else if(d == Down)
	{
		m_layers.at(m_currentLayer)->moveCurrentFrameDown();
	}
}

void KTTableExposure::lockCurrentFrame()
{
	m_layers.at(m_currentLayer)->lockFrame(m_currentFrame);
}

void KTTableExposure::removeCurrentLayer()
{
	if(m_numLayer > 1 && m_layers.at(m_currentLayer)->isSelected())
	{
		m_layout->remove( m_layers.at(m_currentLayer) );
		m_layers.remove(m_currentLayer);
		for( int i = m_currentLayer; i < m_layers.count(); i++)
		{
			m_layers.at(i)->setId(i);
		}
		m_numLayer--;
	}
// 	ktDebug(1) << m_layers.count();
}

