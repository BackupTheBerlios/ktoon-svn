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
// #include "status.h"
#include "ktdebug.h"
#include "layer.h"
#include <memory>

KTTableExposure::KTTableExposure(int rows, int cols, QWidget *parent)
	: QScrollArea(parent),m_numLayer(0), m_currentLayer(0), m_currentFrame(0)
{
	KTINIT;
	setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOn);
	viewport ()->setPaletteBackgroundColor ( paletteBackgroundColor() );
	
	m_port = new QWidget(this);
	m_layout = new QBoxLayout (  m_port, QBoxLayout::LeftToRight,4,1);
	m_layout->setSpacing(0);
	m_layout->setMargin(0);
	for(int i = 0; i < cols; i++)
	{
		insertLayer(rows);
	}
	
	setWidget(m_port);
	setWidgetResizable (true);
}


KTTableExposure::~KTTableExposure()
{
	KTEND;
}

void KTTableExposure::touchFirstFrame()
{
	m_layers.at(0)->frameSelect(0,0,0,0);
}

void KTTableExposure::clickedCell(int row,int col,int button,int gx,int gy)
{
	
	m_currentLayer = col;
	m_currentFrame = row;
	
	
	
	if ( m_layers.at(m_currentLayer)->currentFrameIsUsed() )
	{
		emit(cellSelected(col, row));
	}
	else
	{
// 		KTStatus -> setCurrentKeyFrame( NULL );
	}
	emit(clickedFrame());
}

void KTTableExposure::insertLayer(int rows, QString text)
{
	
	if(text == QString::null)
	{
		text = tr("Layer ")+ QString::number(m_numLayer);
	}
	
	KTLayerExposure *newLayer = new KTLayerExposure(text, m_numLayer, rows, m_port);

	m_layers.append(newLayer);
	connect(newLayer, SIGNAL(selected(int)), this, SLOT(changeCurrentLayer(int)));
	connect(this, SIGNAL(layerSelected(int)), newLayer, SLOT(otherSelected(int)));
	connect(newLayer, SIGNAL(clicked(int,int,int,int,int)), this, SLOT(clickedCell(int,int,int,int,int)));
	connect(newLayer, SIGNAL(setUsedFrame(const QString &)), this, SLOT(useFrame(const QString &)));
	connect(newLayer, SIGNAL(copyFrame()), this, SLOT(copyCurrentFrame()));
	connect(newLayer, SIGNAL(pasteFrame()), this, SLOT(pasteCurrentFrame()));
	connect(newLayer, SIGNAL(removed(int)), this, SLOT(removeLayer(int)));
	connect(newLayer, SIGNAL(removedFrame(int)), this, SLOT(removeKeyFrame(int)));
	connect(newLayer, SIGNAL(layerRenamed(int, const QString &)), this, SLOT(layerRename(int, const QString &)));
	connect(newLayer, SIGNAL(frameRenamed(int, int, const QString &)), this, SLOT(frameRename(int, int, const QString &)));
	
	connect(newLayer, SIGNAL(requestInsertLayer()), this, SIGNAL(requestInsertLayer()));
	
	connect(newLayer, SIGNAL(requestInsertFrame()), this, SIGNAL(requestInsertFrame()));
	
	
	m_layout->addWidget( newLayer, 0, /*Qt::AlignTop | */Qt::AlignLeft);
	
	m_numLayer++;
	newLayer->show();
	
	m_port->adjustSize();
// 	emit layerInserted();
	
	
}

void KTTableExposure::changeCurrentLayer(int idLayer)
{
	m_currentLayer = idLayer;
	m_currentFrame = m_layers.at(m_currentLayer)->frameUsed();
	emit layerSelected(idLayer);
}

void KTTableExposure::setUseFrame()
{
	m_layers.at(m_currentLayer)->setUseFrames();
}

void KTTableExposure::insertFrames()
{
	m_layers.at(m_currentLayer)->insertFrames();
}
		
void KTTableExposure::useFrame(const QString &newName)
{
// 	emit requestInsertFrame();
}

void KTTableExposure::removeFrame()
{
	m_layers.at(m_currentLayer)->removeCurrentFrame();
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
// 		emit requestMoveUpFrame();
	}
	else if(dir == Down)
	{
		m_layers.at(m_currentLayer)->moveCurrentFrameDown();
// 		emit requestMoveDownFrame();
	}
}

void KTTableExposure::lockCurrentFrame()
{
	m_layers.at(m_currentLayer)->lockFrame();
}

void KTTableExposure::removeCurrentLayer()
{
	if(m_numLayer > 1 && m_layers.at(m_currentLayer)->isSelected())
	{
		m_layout->remove( m_layers.at(m_currentLayer) );
		m_layers.removeAt(m_currentLayer);
		for( int i = m_currentLayer; i < m_layers.count(); i++)
		{
			m_layers.at(i)->setId(i);
		}
		m_numLayer--;
// 		emit (layerRemoved());
	}
}

void KTTableExposure::copyCurrentFrame()
{
// 	KTStatus->currentDrawingArea()->slotSelectAll();
// 	KTStatus->currentDrawingArea()->slotCopy();
}

void KTTableExposure::pasteCurrentFrame()
{
// 	KTStatus->currentDrawingArea()->clear();
// 	KTStatus->currentDrawingArea()->slotPasteInPlace();
}

void KTTableExposure::removeKeyFrame(int idLayer)
{
// 	emit requestRemoveFrame();
}

void KTTableExposure::removeLayer(int idLayer)
{
	m_layout->remove( m_layers.at(idLayer) );
	m_layers.removeAt(idLayer);
	for( int i = idLayer; i < m_layers.count(); i++)
	{
		m_layers.at(i)->setId(i);
	}
// 	emit (layerRemoved());
	m_numLayer--;
}

QStringList KTTableExposure::textHeaders()
{
	QStringList list;
	for( int i = 0; i < m_layers.count(); i++)
	{
		list << m_layers.at(i)->textHeader();
	}
	return list;
}


void KTTableExposure::loadLayers(QList<Layer*> layers)
{
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
}

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
