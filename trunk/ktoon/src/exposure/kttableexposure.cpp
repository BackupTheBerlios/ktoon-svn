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


KTTableExposure::KTTableExposure(int rows, int cols, QWidget *parent, const char *name)
	: QScrollView (parent, name),m_numLayer(0), m_currentLayer(0), m_currentFrame(0)
{
	qDebug("[Initializing KTTableExposure]");
	
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
	
	m_layout = new QBoxLayout (  m_port, QBoxLayout::LeftToRight);
	m_layout->addWidget ( gridNumber );
	
	for(int i = 0; i < cols; i++)
	{
		insertLayer(rows);
	}
	
	addChild(m_port);
	adjustSize();
}


KTTableExposure::~KTTableExposure()
{
	qDebug("[Destroying KTTableExposure]");
	
}

void KTTableExposure::clickedCell(int row,int col,int button,int gx,int gy)
{
	qDebug(QString::number(row) + " " + QString::number(col));
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
	connect(newLayer, SIGNAL(clicked(int,int,int,int,int)), this, SLOT(clickedCell(int,int,int,int,int)));	m_layout->addWidget(newLayer);
	m_numLayer++;
}

void KTTableExposure::setUseFrame()
{
	qDebug("KTTableExposure::setUseFrame()");
	m_layers[m_currentLayer]->setUseFrames(m_currentFrame);
}