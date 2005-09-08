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

#include "esframe.h"
#include "eslayer.h"
// #include <qvaluelist.h>
#include <qptrlist.h> 
#include <qframe.h> 
#include <qlayout.h>
#include <qpopupmenu.h>

typedef QPtrList<ESFrame> ListOfFrames;

/**
@author Jorge Cuadrado
*/
class KTLayerExposure : public QFrame
{
	Q_OBJECT
	public:
		
		KTLayerExposure(const QString &initial_text, int id,int numFrame, QWidget *parent = 0, const char *name = 0);
		~KTLayerExposure();
		QString textHeader();
		void insertFrame(int id );
		bool isSelected();
		void invertFrames(int id1, int id2);
		void setId(int id);
		bool currentFrameIsUsed();
		int  useFrame();
		
		
	private:
		enum KTLActions { RenameFrame = 0, RemoveThisFrame, LockThisFrame, InsertFrames, CopyThisFrame, PasteIntoFrame, RenameLayer, RemoveThisLayer};
		bool m_selected;
		uint m_currentFrame, m_id, m_useFrame;
		ESLayer *m_header;
		ListOfFrames m_frames;
		QPopupMenu *menuFrame, *menuLayer;
		void createMenuRight();
		
	public slots:
		void setSelected(bool selected = true, QMouseEvent *e = 0);
		void frameSelect(int id, int button =0 , int x = 0, int y = 0);
		void otherSelected(int id);
		void setUseFrames(int id);
		void removeFrame(int id);
		void moveCurrentFrameUp();
		void moveCurrentFrameDown();
		void lockFrame(int id);
		
		void renameCurrentFrame();
		void removeCurrentFrame();
		void lockCurrentFrame();
		
		void applyAction(int action);
		
		
	signals:
		void selected(int id);
		void clicked( int row, int col, int button,int x, int y);
		void frameSelected(int id);
		void setUsedFrame(int id);
		void clickedMenuFrame(int action, int idFrame, int idLayer);
		void copyFrame();
		void pasteFrame();
		void removed(int);
		void removedFrame(int id);
		
	protected:
		QBoxLayout *m_layout;
};

#endif
