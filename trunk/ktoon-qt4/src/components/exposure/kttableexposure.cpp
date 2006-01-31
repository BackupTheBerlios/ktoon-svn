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
#include <QGroupBox>

#include <QLayout>
#include <QLabel>
#include <QBoxLayout>
#include "ktdebug.h"

KTTableExposure::KTTableExposure(int rows, int cols, QWidget *parent)
	: QScrollArea(parent),m_numLayer(0), m_currentLayer(0), m_currentFrame(0)
{
	KTINIT;
	setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOn);
	
	m_port = new QWidget(this);
	m_layout = new QBoxLayout ( QBoxLayout::LeftToRight, m_port );
	m_layout->setSpacing(0);
	m_layout->setMargin(0);
	m_port->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	setWidget(m_port);
	createMenuRight();
}


KTTableExposure::~KTTableExposure()
{
	KTEND;
}

void KTTableExposure::createMenuRight()
{
	menuFrame = new QMenu(this); 
	//FIXME implemtentar renameCurrentFrame removeCurrentFrame
	menuFrame->addAction( tr( "Rename Frame" ), this,  SLOT(renameCurrentFrame()));
	menuFrame->addAction( tr( "Remove this Frame" ), this, SLOT(removeCurrentFrame()));
	
	menuFrame->addAction( tr( "Copy this Frame" ), this,  SLOT(emitRequestCopyCurrentFrame()));
	
	menuFrame->addAction( tr( "Paste into this Frame" ), this, SLOT(emitRequestPasteCurrentFrame()));
	
	menuFrame->addAction( tr( "Lock this Frame" ), this, SLOT(lockCurrentFrame()));
}

void KTTableExposure::emitRequestCopyCurrentFrame()
{
	emit requestCopyFrame(m_currentFrame);
}

void KTTableExposure::emitRequestPasteCurrentFrame()
{
	emit requestPasteFrame(m_layers[m_currentLayer]->currentFrame());
	m_layers[m_currentLayer]->frameSelect(m_layers[m_currentLayer]->currentFrame());
}




void KTTableExposure::clickedCell(int row, int col,int button,int gx,int gy)
{	
	m_currentLayer = col;
	m_currentFrame = row;
	
	if(button == Qt::RightButton)
	{
		menuFrame->exec(QPoint(gx,gy));
	}
	else if ( m_layers.at(m_currentLayer)->currentFrameIsUsed() )
	{
		emit(cellSelected(col, row));
	}
	else
	{
		emit(layerSelected(col));
	}
	
}


int KTTableExposure::currentLayer()
{
	return m_currentLayer;
}

void KTTableExposure::insertLayer(int rows, const QString &text)
{
	KTLayerExposure *newLayer = new KTLayerExposure(text, m_numLayer, rows, m_port);
	m_layers.append(newLayer);
	
	connect(newLayer, SIGNAL(selected(int)), this, SLOT(changeCurrentLayer(int)));
	
	connect(this, SIGNAL(layerSelected(int)), newLayer, SLOT(otherSelected(int)));
	
	connect(newLayer, SIGNAL(visibilityChanged( int, bool)), this, SIGNAL(layerVisibilityChanged( int, bool)));
	
	connect(newLayer, SIGNAL(clicked(int,int,int,int,int)), this, SLOT(clickedCell(int,int,int,int,int)));
	
	connect(newLayer, SIGNAL(removed(int)), this, SLOT(removeLayer(int)));
	
	connect(newLayer, SIGNAL(layerRenamed(int, const QString &)), this, SLOT(layerRename(int, const QString &)));
	
	connect(newLayer, SIGNAL(frameRenamed(int, int, const QString &)), this, SLOT(frameRename(int, int, const QString &)));
	
	connect(newLayer, SIGNAL(requestInsertFrame(bool)), this, SIGNAL(requestInsertFrame(bool)));
	
	m_layout->addWidget( newLayer/*, 0, Qt::AlignLeft*/);
	
	m_numLayer++;
	newLayer->setSelected(true);
// 	m_currentLayer = m_numLayer;
	
	newLayer->show();
	m_port->adjustSize();
}

void KTTableExposure::changeCurrentLayer(int idLayer)
{
	if(m_currentLayer != idLayer || m_currentFrame != m_layers.at(m_currentLayer)->numUsedFrame())
	{
		m_currentLayer = idLayer;
		m_currentFrame = m_layers.at(m_currentLayer)->numUsedFrame();
		emit layerSelected(idLayer);
		emit cellSelected(m_currentLayer, m_currentFrame);
	}
}

void KTTableExposure::setUseFrame(int idLayer, const QString& name, bool addedToEnd)
{
	ktDebug() << "ID LAYER: " << idLayer << endl;
	m_layers.at(idLayer)->setUseFrames(name, addedToEnd);
}

void KTTableExposure::insertFrames()
{
	KTLayerExposure *layer = m_layers.at(m_currentLayer);
	if ( layer )
	{
		layer->insertFrames();
	}
}
		
void KTTableExposure::removeFrame()
{
	if ( m_layers.count() > 0)
	{
		KTLayerExposure *layer = m_layers.at(m_currentLayer);
		if(layer)
		{
			layer->removeCurrentFrame();
		}
	}
}

KTLayerExposure *KTTableExposure::currentLayerExposure()
{
	return m_layers.at(m_currentLayer);
}

void KTTableExposure::moveCurrentFrame(Direction dir)
{
	if(dir == Up)
	{
		m_layers.at(m_currentLayer)->moveCurrentFrameUp();
	}
	else if(dir == Down)
	{
		m_layers.at(m_currentLayer)->moveCurrentFrameDown();
	}
}

void KTTableExposure::lockCurrentFrame()
{
	m_layers.at(m_currentLayer)->lockFrame();
}

void KTTableExposure::removeCurrentLayer() // FIXME SIGSEGV
{
	KT_FUNCINFO;
	if ( m_layers.count() > 0 && m_currentLayer >= 0 )
	{
		KTLayerExposure * ly = m_layers.takeAt(m_currentLayer);
		if ( ly ) 
		{
			delete ly; 
		}
		for( int i = m_currentLayer; i < m_layers.count(); i++)
		{
			KTLayerExposure *layer = m_layers.at(i);
			if ( layer ) layer->setId(i);
		}
		m_numLayer--;
		m_port->adjustSize();
		
		emit cellSelected( m_currentLayer-1, m_layers[m_currentLayer-1]->currentFrame());
// 		m_currentLayer--;
	}
}


void KTTableExposure::setCurrentCell(int idLayer, int idFrame)
{
	
	if( idLayer < 0 || idFrame < 0  || (idLayer == m_currentLayer && m_layers[m_currentLayer]->currentFrame() == idFrame ))
	{
		return;
	}
	m_currentLayer = idLayer;
	m_currentFrame = idFrame;
	m_layers[m_currentLayer]->setSelected(true);
	
	
}

void KTTableExposure::removeLayer(int idLayer)
{
// 	KT_FUNCINFO;
	SHOW_VAR(idLayer);
	m_layout->removeWidget( m_layers.at(idLayer) );
	m_layers.removeAt(idLayer);
	for( int i = idLayer; i < m_layers.count(); i++)
	{
		m_layers.at(i)->setId(i);
	}
	m_numLayer--;
}



// void KTTableExposure::loadLayers(QList<Layer*> layers)
// {
// 	ktDebug() << "KTTableExposure::loadLayers init" <<  layers.count();
// 	m_layers.clear();
// 	KTStatus -> currentScene() -> getLayers();
// 	QList<Layer*> ly = KTStatus->currentScene()->getLayers();
// 	ly.setAutoDelete( true );
// 	ly.clear();
// 	ly.setAutoDelete( false );
// 	KTStatus->currentScene()->setLayers(layers);
// 	m_numLayer = 0;
	
// // 	for (int i = 0; i < layers.count(); i++ )
// 	{
// 		insertLayer(100, layers.at(i)->nameLayer());
// 		m_layers.at(i)->loadFrames(layers.at(i));
// 		m_numLayer++;
// 	}
	
// 	ktDebug() << "KTTableExposure::loadLayers finish load " <<  m_numLayer << "layers" << endl;
// }

void KTTableExposure::updateLayers()
{
// 	ktDebug() << "KTTableExposure::updateLayers" << endl;
// 	m_layers.clear();
// 	m_numLayer = 0;
// 	QList<Layer*> ly = KTStatus->currentScene()->getLayers();
/*	if(ly.count() == 0)
	{
		insertLayer(100);
	}
	else
	{
		for (int i = 0; i < ly.count(); i++ )
		{
			insertLayer(100, ly.at(i)->nameLayer());
			m_layers.at(i)->loadFrames(ly.at(i));
			m_numLayer++;
		}
}*/
// 	KTStatus->currentScene()->setLayers(ly);
// 	ktDebug() << "KTTableExposure::updateLayers" << ly.count() << endl;
}

void KTTableExposure::layerRename(int idLayer, const QString  &newName)
{
// 	ktDebug() << "KTTableExposure::layerRename (" << idLayer << " , " << newName << ") " << endl;
// 	QList<Layer*> ly = KTStatus -> currentScene() -> getLayers();
	/*
	ly.at(idLayer)->setNameLayer(newName);
	emit layerRenamed( idLayer , newName);*/
}

void KTTableExposure::frameRename(int idFrame, int idLayer, const QString  &newName)
{
// 	ktDebug() << " KTTableExposure::frameRename " << newName << endl;
// 	QList<Layer*> ly = KTStatus -> currentScene() -> getLayers();
// 	ly.at(idLayer)->keyFrames().at(idFrame)->setNameKeyFrame(newName);
}
