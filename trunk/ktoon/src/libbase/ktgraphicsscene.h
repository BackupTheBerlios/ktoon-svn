/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#ifndef KTGRAPHICSSCENE_H
#define KTGRAPHICSSCENE_H

#include <QGraphicsScene>

/**
 * @author David Cuadrado \<krawek@gmail.com\>
*/

class KTFrame;
class KTScene;
class KTToolPlugin;
class KTBrushManager;

class KTItemResponse;

class KTGraphicsScene : public QGraphicsScene
{
	public:
		KTGraphicsScene();
		~KTGraphicsScene();
		
		void setCurrentFrame(int layer, int frame);
		
		void setCurrentScene(KTScene *scene);
		void drawCurrentPhotogram();
		
		void drawItems(QPainter *painter, int numItems, QGraphicsItem *items[], const QStyleOptionGraphicsItem options[], QWidget *widget);
		
		void drawPhotogram(int photogram);
		void addFrame(KTFrame *frame, double opacity = 1.0);
		void clean();
		int currentFrameIndex() const;
		int currentLayerIndex() const;
		int currentSceneIndex() const;
		
		void setNextOnionSkinCount(int n);
		void setPreviousOnionSkinCount(int n);
		
		void setLayerVisible(int layerIndex, bool visible);
		
		KTScene *scene() const;
		
		KTFrame *currentFrame();
		
		void setTool(KTToolPlugin *tool);
		KTToolPlugin *currentTool() const;
		
		bool isDrawing() const;
		
		KTBrushManager *brushManager() const;
		
		
		void itemResponse(KTItemResponse *event);
		
	protected:
		virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		virtual void keyPressEvent(QKeyEvent *keyEvent);
		
		void dragLeaveEvent ( QGraphicsSceneDragDropEvent * event );
		void dragEnterEvent ( QGraphicsSceneDragDropEvent * event );
		void dragMoveEvent ( QGraphicsSceneDragDropEvent * event );
		void dropEvent ( QGraphicsSceneDragDropEvent * event );
		
		
		
		
	private:
		struct Private;
		Private *const d;
};

#endif
