/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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

#include <QStringList>
#include <QScrollArea>
#include <QLayout>
#include <QLabel>
#include <QList>
#include <QBoxLayout>

#include "ktlayerexposure.h"
#include "gldrawing.h"
#include "keyframe.h"
#include "layer.h"

/**
 * @author Jorge Cuadrado
*/

typedef QList<KTLayerExposure*> ListOfLayers;

class KTTableExposure : public QScrollArea //Q3ScrollView
{
	Q_OBJECT
	public:
		enum Direction {Up = 0, Down};
		KTTableExposure(int rows, int cols, QWidget *parent = 0);
		~KTTableExposure();
		void setUseFrame();
		void removeFrame();
		void moveCurrentFrame(Direction dir);
		void lockCurrentFrame();
		void removeCurrentLayer();
		
		void loadLayers(QList<Layer*> layers);
		void updateLayers();
		QStringList textHeaders();
		
	private:
		QBoxLayout *m_layout;
		QWidget *m_port;
		int m_numLayer;
		int m_currentLayer, m_currentFrame;
		ListOfLayers m_layers;
		
	public slots:
		void insertLayer(int rows, QString text = QString::null);
		void clickedCell(int row, int col, int button, int gx, int gy);
		void changeCurrentLayer(int idLayer);
		void useFrame( const QString &newName);
		void removeKeyFrame(int id);
		void touchFirstFrame();
		void copyCurrentFrame();
		void pasteCurrentFrame();
		void removeLayer(int idLayer);
// 		void changedName(const QString &);
		void layerRename(int, const QString &);
		void frameRename(int idFrame, int idLayer, const QString  &newName);
		
	signals:
		void layerSelected(int id);
		void clickedFrame();
		void cellSelected(int idLayer, int idFrame);
		void activateCursor();
		void layerInserted();
		void frameRemoved(int idFrame);
		
		void layerRemoved();
		void layerRenamed( int , const QString & );
		void frameMovedUp( int pos );
		void frameMovedDown( int pos );
		
		
};

#endif
