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
#ifndef KTLAYEREXPOSURE_H
#define KTLAYEREXPOSURE_H


#include <QList>
#include <QFrame>
#include <QLayout>
#include <QMouseEvent>
#include <QBoxLayout>
#include <QMenu>

#include "esframe.h"
#include "eslayer.h"



typedef QList<ESFrame*> ListOfFrames;

/**
@author Jorge Cuadrado
*/
class KTLayerExposure : public QFrame
{
	Q_OBJECT
	public:
		KTLayerExposure(const QString &initial_text, int id,int numFrame, QWidget *parent = 0);
		~KTLayerExposure();
		void insertFrame(int id, const QString &text );
		enum KTTAction{ RenameLayer = 0, RemoveThisLayer, InsertFrames, RemoveThisFrame};
		
		void addFrame(const QString &text );
		bool isSelected();
		void swapFrames(int id1, int id2);
		void setId(int id);
		void setName(const QString & name);
		int id();
		bool currentFrameIsUsed();
		int  numUsedFrame();
		
		void selectedFirstFrame();
		int currentFrame();
		ListOfFrames frames();
		
		int visualIndex(int logicalIndex);
		int logicalIndex(int visualIndex);
		
	private:
		bool m_selected;
		int m_id, m_currentFrame,  m_useFrame;
		ESLayer *m_header;
		ListOfFrames m_frames;
		QMenu *menuLayer;
		void createMenuRight();
		
	public slots:
		void emitRequestRenameFrame(int indexFrame, const QString& name);
		void emitRequestRenameLayer(const QString& name);
		
	public slots:
		void setSelected(bool selected = true, QMouseEvent *e = 0);
		void frameSelect(int id, int button =0 , int x = 0, int y = 0);
		void otherSelected(int id);
		void setUseFrames(const QString &name, bool addedToEnd);
		void setLayerVisible(bool);
		void removeFrame(int id);
		void moveCurrentFrameUp();
		void moveCurrentFrameDown();
		void lockFrame();
		void renameCurrentFrame();
		void removeCurrentFrame();
		void applyAction(int action);
		
		void insertFrames();
		
	signals:
		void requestInsertFrame(bool);
		void visibilityChanged( int idLayer, bool value);
		void selected(int id);
		void clicked( int row, int col, int button,int x, int y);
		void frameSelected(int id);
		void clickedMenuFrame(int action, int idFrame, int idLayer);
		void removed(int);
		void requestRenameLayer( int idLayer, const QString &name );
		void requestRenameFrame(  int indexLayer, int indexFrame, const QString &name );
		
	protected:
		QBoxLayout *m_layout;
};

#endif
