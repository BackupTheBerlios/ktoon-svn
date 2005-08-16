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
#ifndef KTTABLEEXPOSURE_H
#define KTTABLEEXPOSURE_H

#include <qscrollview.h> 
#include <qlayout.h> 
#include <qlabel.h>
#include "ktlayerexposure.h"

/**
@author Jorge Cuadrado
*/

typedef QPtrList<KTLayerExposure> listOfLayers;

class KTTableExposure : public QScrollView
{
	Q_OBJECT
	public:
		enum Direction {Up = 0, Down};
		KTTableExposure(int rows, int cols, QWidget *parent = 0, const char *name = 0);
		~KTTableExposure();
		void setUseFrame();
		void removeFrameSelected();
		void moveCurrentFrame(Direction d);
		void lockCurrentFrame();
		void removeCurrentLayer();
		
	private:
		QBoxLayout *m_layout;
		QWidget *m_port;
		int m_numLayer;
		int m_currentLayer, m_currentFrame;
		listOfLayers m_layers;
		
	public slots:
		void insertLayer(int rows);
		void clickedCell(int row, int col, int button, int gx, int gy);
		
	signals:
		void layerSelected(int id);
};

#endif
