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
	
	connect(newLayer, SIGNAL(requestRenameLayer(int, const QString &)), this, SIGNAL(requestRenameLayer(int, const QString &)));
	
	connect(newLayer, SIGNAL(requestRenameFrame(int, int, const QString &)), this, SIGNAL(requestRenameFrame(int, int, const QString &)));
	
	connect(newLayer, SIGNAL(requestInsertFrame(bool)), this, SIGNAL(requestInsertFrame(bool)));
	
	m_layout->addWidget( newLayer);
	
	m_numLayer++;
	newLayer->setSelected(true);
	
	newLayer->show();
	m_port->adjustSize();
}

void KTTableExposure::changeCurrentLayer(int idLayer)
{
	m_layers.at(m_currentLayer)->otherSelected( idLayer);
	emit layerSelected(idLayer);
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

void KTTableExposure::moveCurrentLayer(Direction dir)
{
	ktDebug() << "KTTableExposure::moveCurrentLayer(Direction" << dir << ")";
	int pos = m_layout->indexOf ( m_layers[m_currentLayer]);
	if(dir == Left)
	{
		if(pos > 0 )
		{
			m_layout->removeWidget(m_layers[m_currentFrame]);
			m_layout->insertWidget(pos-1, m_layers[m_currentFrame]);
			
			m_layers[m_currentFrame]->setId(m_currentLayer-1);
			m_layers[m_currentFrame-1]->setId(m_currentLayer);
			
			m_layers.swap ( m_currentLayer, m_currentLayer-1);
			m_currentLayer--;
		}
	}
	else if(dir == Right)
	{
		int idFrame = m_layers[pos]->id();
		if( pos+1 < m_layers.count() )
		{
			m_layout->removeWidget(m_layers[m_currentLayer]);
			m_layout->insertWidget(pos-1, m_layers[m_currentLayer]);
			int id1 = m_layers[m_currentLayer+1]->id();
			int id2 = m_layers[m_currentLayer]->id();
			m_layers[m_currentLayer]->setId(id1);
			m_layers[m_currentLayer+1]->setId(id2);
			m_layers.swap ( id1, id2);
			m_currentLayer++;
		}
		
	}
}

void KTTableExposure::lockCurrentFrame()
{
	m_layers.at(m_currentLayer)->lockFrame();
}

void KTTableExposure::removeCurrentLayer() 
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

void KTTableExposure::setLayer(int index)
{
	ktDebug() << "KTTableExposure::setLayer(int" << index << ")"  << m_currentLayer;
	if(index == m_currentLayer)
	{
		return;
	}
	m_currentLayer = index;
	m_layers[m_currentLayer]->setSelected(true);
}

void KTTableExposure::setLayerName(int indexLayer, const QString& name )
{
	m_layers[indexLayer]->setName(name);
}

void KTTableExposure::setFrameName(int indexLayer, int indexFrame, const QString& name )
{
	m_layers[indexLayer]->frames()[indexFrame]->setName(name);
}

void KTTableExposure::removeLayer(int idLayer)
{
	KT_FUNCINFO;
	SHOW_VAR(idLayer);
	
	if ( m_layers.count() > 0 && idLayer >= 0 )
	{
		KTLayerExposure * ly = m_layers.takeAt(idLayer);
		if ( ly ) 
		{
			delete ly; 
		}
		for( int i = idLayer; i < m_layers.count(); i++)
		{
			KTLayerExposure *layer = m_layers.at(i);
			if ( layer ) layer->setId(i);
		}
		m_numLayer--;
		m_port->adjustSize();
		
		emit cellSelected( m_currentLayer-1, m_layers[idLayer-1]->currentFrame());
	}
	
}



