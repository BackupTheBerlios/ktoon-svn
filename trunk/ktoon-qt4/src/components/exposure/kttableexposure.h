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
#include <QMenu>


#include "ktlayerexposure.h"

/**
 * @author Jorge Cuadrado
*/

typedef QList<KTLayerExposure*> ListOfLayers;

class KTTableExposure : public QScrollArea
{
	Q_OBJECT
	public:
		enum Direction {Up = 0, Down, Left, Right};
		
		enum KTTAction{ RenameFrame = 0, RemoveThisFrame, LockThisFrame, CopyThisFrame, PasteIntoFrame};
		KTTableExposure(int rows, int cols, QWidget *parent = 0);
		~KTTableExposure();
		
		void setUseFrame(int idLayer, const QString& name, bool addedToEnd );
		
		void moveCurrentFrame(Direction dir);
		void moveCurrentLayer(Direction dir);
		
		void removeCurrentLayer();
		void insertFrames();
		
		int currentLayer();
		
		KTLayerExposure *currentLayerExposure();
		
		void setCurrentCell(int idLayer, int idFrame);
		void setLayer(int index);
		void setFrameName(int indexLayer, int indexFrame, const QString& name );
		void setLayerName(int indexLayer, const QString& name );
		
	private:
		void createMenuRight();
		
	private:
		QBoxLayout *m_layout;
		QWidget *m_port;
		int m_numLayer;
		int m_currentLayer, m_currentFrame;
		ListOfLayers m_layers;
		QMenu *menuFrame;
		
	private slots:
		void emitRequestCopyCurrentFrame();
		void emitRequestPasteCurrentFrame();
		
	public slots:
		void insertLayer(int rows, const QString &text = QString::null);
		void clickedCell(int row, int col, int button, int gx, int gy);
		void changeCurrentLayer(int idLayer);
		void removeLayer(int idLayer);
		void lockCurrentFrame();
		void removeFrame();
		
	signals:
		void layerVisibilityChanged( int idLayer, bool value);
		void cellSelected(int idLayer, int idFrame);
		void clickedFrame();
		void layerSelected(int id);
		
		void requestInsertFrame(bool addedToEnd);
		void requestPasteFrame(int);
		void requestCopyFrame(int);
		
		void requestRenameLayer(int indexLayer, const QString &name );
		void requestRenameFrame(int indexLayer,  int indexFrame,const QString &name );
};

#endif
