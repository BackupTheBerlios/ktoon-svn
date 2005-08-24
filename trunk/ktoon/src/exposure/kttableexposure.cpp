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
#include <qvbox.h> 
#include "ktdebug.h"

KTTableExposure::KTTableExposure(int rows, int cols, QWidget *parent, const char *name)
	: QScrollView (parent, name),m_numLayer(0), m_currentLayer(0), m_currentFrame(0)
{
	KTINIT;
	
	setDragAutoScroll ( true ) ;
	viewport ()->setPaletteBackgroundColor ( paletteBackgroundColor() );
	m_port = new QWidget(this);
	setHScrollBarMode ( Auto );
	QVBox *gridNumber = new QVBox( m_port);
	for(int i = 0; i < rows; i++)
	{
		QLabel *tmp = new QLabel(gridNumber);
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
// 	qDebug(QString::number(row) + " " + QString::number(col));
// 	ktDebug(1) << row << " "<< col << " " << endl ;
	m_currentLayer = col;
	m_currentFrame = row;
	if(button == Qt::RightButton)
	{
		//show pupo gx, gy
	}
}

void KTTableExposure::insertLayer(int rows)
{

	KTLayerExposure *newLayer = new KTLayerExposure("Layer"+ QString::number(m_numLayer), m_numLayer, 100, m_port);
	m_layers.append(newLayer);
	connect(newLayer, SIGNAL(selected(int)), this, SIGNAL(layerSelected(int)));
	connect(this, SIGNAL(layerSelected(int)), newLayer, SLOT(otherSelected(int)));
	connect(newLayer, SIGNAL(clicked(int,int,int,int,int)), this, SLOT(clickedCell(int,int,int,int,int)));
	m_layout->addWidget(newLayer);
	
	m_numLayer++;
	newLayer->show();
	
	
}

void KTTableExposure::setUseFrame()
{
	m_layers.at(m_currentLayer)->setUseFrames(m_currentFrame);
// 	m_layers[m_currentLayer]->setUseFrames(m_currentFrame);
}

void KTTableExposure::removeFrameSelected()
{
	m_layers.at(m_currentLayer)->removeFrame(m_currentFrame);
// 	m_layers[m_currentLayer]->removeFrame(m_currentFrame);
}

void KTTableExposure::moveCurrentFrame(Direction d)
{
	if(d == Up)
	{
		m_layers.at(m_currentLayer)->moveCurrentFrameUp();
// 		m_layers[m_currentLayer]->moveCurrentFrameUp();
	}
	else if(d == Down)
	{
		m_layers.at(m_currentLayer)->moveCurrentFrameDown();
// 		m_layers[m_currentLayer]->moveCurrentFrameDown();
	}
}

void KTTableExposure::lockCurrentFrame()
{
	m_layers.at(m_currentLayer)->lockFrame(m_currentFrame);
// 	m_layers[m_currentLayer]->lockCurrentFrame(m_currentFrame);
}

void KTTableExposure::removeCurrentLayer()
{
	if(m_numLayer > 1 && m_layers.at(m_currentLayer)->isSelected()) //m_layers[m_currentLayer]->isSelected())
	{
		m_layout->remove( m_layers.at(m_currentLayer) );//m_layers[m_currentLayer]);
// 		delete m_layers[m_currentLayer];
		m_layers.remove(m_currentLayer);
// 		QPtrListIterator<KTLayerExposure> it(m_layers);
// 
// 		for ( ; it.current(); ++it )
// 		{
// 			
// 		}
		for( int i = m_currentLayer; i < m_layers.count(); i++)
		{
			m_layers.at(i)->setId(i);
// 			m_layers[i]->setId(i);
		}
		m_numLayer--;
	}
	ktDebug(1) << m_layers.count();
}